#include "pch.h"
#include "HttpSyncFetcher.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <cstdlib>
#include <iostream>
#include <string>
#include "Url.h"
#include "HttpResponse.h"

//#pragma comment(lib, "crypt32.lib")
//#pragma comment (lib, "cryptui.lib")

using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

namespace
{
	constexpr int HTTP_1_1 = 11;			// constant for beast HTTP 1.1 
	const char * const CONTENT_TYPE_HTML = "text/html";	// supported content type

	using RequestType = http::request<http::string_body>;
	using RequestPtr = std::unique_ptr<RequestType>;
	using ResponseType = http::response<http::string_body>;
	//	http::response<http::dynamic_body> res;					// concider for streaming large results

	RequestPtr CreateRequest(const ::WebCrawler::Url &url)
	{
		using namespace std;

		RequestPtr req{ make_unique<RequestType>(http::verb::get, url.GetPath(), HTTP_1_1) };
		req->set(http::field::host, url.GetHost());
		req->set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
		req->set(http::field::accept, CONTENT_TYPE_HTML);
		return req;
	}

	// Extract the HTTP response
	// Note that we are only interested in html documents - others types we do not return the body
	std::shared_ptr<::WebCrawler::HttpResponse> ExtractDocument(ResponseType &res)
	{
		using namespace std;
		using namespace ::WebCrawler;

		//		cout << res << "\n\n\n" << endl;

		auto responseCode{ res.result_int() };
		switch (responseCode)
		{
		case 200:
		case 201:
		case 206:
		{
			// results (possibly partial) returned from server
			auto typeIt{ res.find("Content-Type") };
			if (typeIt != res.end())
			{
				string type{ typeIt->value() };
				transform(type.begin(), type.end(), type.begin(), ::tolower);
				if (type.find(CONTENT_TYPE_HTML) != string::npos)
				{
					//		string body{ boost::beast::buffers_to_string(res.body().data()) };
					string body{ res.body().data() };
					return make_shared<HttpDocumentResponse>(responseCode, std::move(type), std::move(body));
				}
				else
					throw HttpResponseException("Web document has unsupported Content-Type (" + type + ")");
			}
			else
				throw HttpResponseException("Web document has no Content-Type in header.");
		}
		break;

		case 301:
		case 302:
		case 303:
		case 307:
		case 208:
		{
			// redirest required
//			cout << res << "\n\n\n" << endl;
			auto locationIt{ res.find("Location") };
			if (locationIt != res.end())
			{
				string location{ locationIt->value() };
				return make_shared<HttpRedirectResponse>(responseCode, std::move(location));
			}
			else
				throw HttpResponseException("Redirect requested with no Location supplied.");
		}
		}
		if (responseCode >= 400)
			return make_shared<HttpErrorResponse>(responseCode, responseCode < 500);
		throw HttpResponseException("Unsupported response code - ignoring (" + to_string(responseCode) + ")");
	}
}

namespace WebCrawler
{

	HttpSyncFetcher::HttpSyncFetcher(IResponseHandler *handler) : handler_(handler)
	{
	}

	HttpSyncFetcher::~HttpSyncFetcher()
	{

	}

	void HttpSyncFetcher::Fetch(const std::string & urlStr)
	{
		using namespace std;

		try
		{
			Url  url(urlStr);
			auto  response { url.GetProtocol() == Url::HTTPS ? LoadDocumentSSL(url) : LoadDocument(url) };
			if (!response)
			{
				std::cerr << "Unknown response returned for : " << urlStr << std::endl;	
			}
			else if (response->IsRedirect())
			{
				auto redirectResponse{ dynamic_pointer_cast<HttpRedirectResponse>(response) };
				if (redirectResponse && handler_ != nullptr)
					handler_->OnRedirect(urlStr, redirectResponse);
			}
			else if (response->HasDocument())
			{
				auto docResponse{ dynamic_pointer_cast<HttpDocumentResponse>(response) };
				if (docResponse && handler_ != nullptr)
					handler_->OnSuccess(urlStr, docResponse);
			}
			else if (response->IsClientError() || response->IsServerError())
			{
				auto errorResponse{ dynamic_pointer_cast<HttpErrorResponse>(response) };
				if (errorResponse && handler_ != nullptr)
					handler_->OnError(urlStr, errorResponse);
			}
		}
		catch (UrlException &e)
		{
			std::cerr << "Attempt to fetch invalid URL: " << e.what() << std::endl;
		}
		catch (boost::system::system_error &e)
		{
			std::cerr << "Error loading document: " << e.what() << std::endl;
		}
		catch (HttpResponseException &e)
		{
			std::cerr << "Invalid document loaded (" << urlStr << ") : " << e.what() << std::endl;
		}
		catch (std::exception& e)
		{
			std::cerr << "Error processing Document (" << urlStr << "): " << e.what() << std::endl;
		}
	}

	std::shared_ptr<HttpResponse> HttpSyncFetcher::LoadDocument(const Url &url)
	{
		using namespace std;
//		using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
//		namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

		// Create an io_context (TODO - should be able to re-use this)
		boost::asio::io_context ioc;

		// dns lookup
		tcp::resolver	dnsResolver{ ioc };
		auto results = dnsResolver.resolve(url.GetHost(), url.GetPort());

		// create session
		tcp::socket		socket{ ioc };
		boost::asio::connect(socket, results.begin(), results.end());

		// Set up and send a HTTP GET request message
		RequestPtr request { CreateRequest(url) };
		http::write(socket, *request);

		// Declare a container a retrieve http response
		boost::beast::flat_buffer buffer;
		ResponseType response;
		http::read(socket, buffer, response);

		// Gracefully close the socket
		boost::system::error_code ec;
		socket.shutdown(tcp::socket::shutdown_both, ec);

		// not_connected happens sometimes
		// so don't bother reporting it.
		if (ec && ec != boost::system::errc::not_connected)
			throw boost::system::system_error{ ec };

		return ExtractDocument(response);
	}

	std::shared_ptr<HttpResponse> HttpSyncFetcher::LoadDocumentSSL(const Url & url)
	{
		using namespace std;

		// The io_context is required for all I/O
		boost::asio::io_context ioc;

		// The SSL context is required, and holds certificates
		ssl::context ctx{ ssl::context::sslv23_client };

		// These objects perform our I/O
		tcp::resolver resolver{ ioc };
		ssl::stream<tcp::socket> stream{ ioc, ctx };

		// Set SNI Hostname (many hosts need this to handshake successfully)
		if (!SSL_set_tlsext_host_name(stream.native_handle(), url.GetHost().c_str()))
		{
			boost::system::error_code ec{ static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category() };
			throw boost::system::system_error{ ec };
		}

		// Look up the domain name
		auto const results = resolver.resolve(url.GetHost(), url.GetPort());

		// Make the connection on the IP address we get from a lookup
		boost::asio::connect(stream.next_layer(), results.begin(), results.end());

		// Perform the SSL handshake
		stream.handshake(ssl::stream_base::client);

		// Create and sent http GET
		RequestPtr request { CreateRequest(url) };
		http::write(stream, *request);

		// Receive the http response
		boost::beast::flat_buffer buffer;
		ResponseType res;
		http::read(stream, buffer, res);

		// Gracefully close the stream
		boost::system::error_code ec;
		stream.shutdown(ec);
		// http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
		if (ec.category() == boost::asio::error::get_ssl_category())
			throw boost::system::system_error{ ec };

		return ExtractDocument(res);
	}
}








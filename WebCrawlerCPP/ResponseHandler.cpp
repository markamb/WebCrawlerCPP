#include "pch.h"
#include "ResponseHandler.h"
#include "HttpSyncFetcher.h"
#include <iostream>
#include <regex>

namespace WebCrawler
{
	using namespace std;

	ResponseHandler::ResponseHandler(IWebCrawler *wc) :
			webCrawler_(wc), successCount_(0), redirectCount_(0), errorCount_(0), totalSize_(0)
	{

	}

	ResponseHandler::~ResponseHandler()
	{
	}

	void ResponseHandler::OnSuccess(std::string urlStr, std::shared_ptr<HttpDocumentResponse> document)
	{		
		using namespace std;


		size_t numLinks = 0;
		// get the links
		try
		{
			std::vector<std::string> links;

			const std::string &doc{ document->GetDocument() };
			regex  aRegEx(R"(<a.*?>.*?</a>)");
			regex  linkRegEx(R"(<a\s+(?:[^>]*?\s+)?href=(["'])(.*?)\1)");
			smatch aMatch;
			auto  searchStart(doc.cbegin());
			while (regex_search(searchStart, doc.cend(), aMatch, aRegEx))
			{
				string link { aMatch[0] };
				// now parse the actual link
				smatch linkMatch;
				if (regex_search(link, linkMatch, linkRegEx))
				{
//					cout << "  " << linkMatch[2] << "\n";
					links.push_back(linkMatch[2]);
				}
				searchStart += aMatch.position() + aMatch.length();
			}
			numLinks = links.size();
			webCrawler_->AddURLs(links);
		}
		catch (exception &e)
		{
			cerr << "Exception parsing document: " << e.what() << endl;
		}

		cout << "*** Loaded document " << urlStr << ", Content-Type= " << document->GetContentType() << ", Response Code=" << document->ResponseCode() << ", NumLinks= " << numLinks << ", Length = " << document->GetDocumentLength() / 1024 << "Kb\n";
		successCount_++;
		totalSize_ += document->GetDocumentLength();
	}
	
	void ResponseHandler::OnRedirect(std::string originalUrl, std::shared_ptr<HttpRedirectResponse> response)
	{
		string location = response->GetRedirectLocation();
		cout << "*** Redirect recieved for " << originalUrl << ", Response Code=" << response->ResponseCode() << ", Location=" << location << "\n";
		redirectCount_++;
		if (!location.empty())
			webCrawler_->AddURL(location);
	}

	void ResponseHandler::OnError(std::string originalUrl, std::shared_ptr<HttpErrorResponse> error)
	{
		if (error->IsClientError())
			cout << "*** Client Error";
		else
			cout << "*** Server Error";
		errorCount_++;
		cout << " recieved for " << originalUrl << ", Response Code = " << error->ResponseCode() << "\n";
	}

}


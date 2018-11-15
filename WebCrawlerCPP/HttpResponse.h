#pragma once
#include <string>
#include <exception>

namespace WebCrawler
{

	// 
	// Exception class for document format errors
	//
	class HttpResponseException : public std::exception
	{
	public:
		HttpResponseException(const char *msg) : std::exception(msg)
		{}

		HttpResponseException(std::string msg) : std::exception(msg.c_str())
		{}

	};

	// Baseclass for a response from a http request
	// Base class contains basic response details.
	class HttpResponse
	{
		public:
			HttpResponse(int responseCode) : responseCode_(responseCode)
			{}
			virtual ~HttpResponse()
			{}

			// return the HTTP response code
			int	ResponseCode() const { return responseCode_; }
			// return true if this is a redirect response. Use GetRedirectLocation to retrieve new location
			virtual bool			IsRedirect() const { return false; }
			// return true if this response contains a document body
			virtual bool			HasDocument() const { return false; }
			// return true if this represents a client or server error response
			virtual bool			IsClientError() const { return false; }
			virtual bool			IsServerError() const { return false; }

		private:
			int		responseCode_;
	};

	//
	// Response requesting a HTTP redirect
	//
	class HttpRedirectResponse : public HttpResponse
	{
	public:
		HttpRedirectResponse(int responseCode, std::string locationString) : HttpResponse(responseCode), location_(std::move(locationString))
		{}
		virtual ~HttpRedirectResponse()
		{}

		// HttpResponse methods
		virtual bool		IsRedirect() const override { return true; }

		// New methods
		std::string GetRedirectLocation() const { return location_; }

	private:
		std::string location_;
	};

	//
	// Represents a web document (including contents)
	//
	class HttpDocumentResponse : public HttpResponse
	{
	public:
		HttpDocumentResponse(int responseCode, std::string contentType, std::string body) :
			HttpResponse(responseCode), contentType_(std::move(contentType)), body_(std::move(body))
		{
			// TODO - throw if unsupported document type ?
		}

		virtual ~HttpDocumentResponse() 
		{}

		// HttpResponse methods
		virtual bool		HasDocument() const override{ return true; }

		// New methods
		const std::string &	GetContentType() const { return contentType_;  }
		size_t				GetDocumentLength() const { return body_.length(); }
		const std::string&	GetDocument() const { return body_; }

	private:
		std::string		contentType_;
		std::string		body_;
	};

	//
	// Represents an error returned from the server
	//
	class HttpErrorResponse : public HttpResponse
	{
	public:
		HttpErrorResponse(int responseCode, bool clientError) : HttpResponse(responseCode), clientError_(clientError)
		{}
		virtual ~HttpErrorResponse()
		{}

		// HttpResponse methods
		virtual bool	IsClientError() const override { return clientError_; }
		virtual bool	IsServerError() const override { return !clientError_; }

	private:
		bool clientError_;
	};

}





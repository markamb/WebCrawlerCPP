#pragma once

#include <string>
#include <memory>
#include "HttpResponse.h"

namespace WebCrawler
{
	//
	// Interface for processing a Http response
	//
	class IResponseHandler
	{
	public:
		IResponseHandler() {};
		virtual ~IResponseHandler() {};

		// Called when a document is sucessfully loaded
		virtual void OnSuccess(std::string urlStr, std::shared_ptr<HttpDocumentResponse> document) = 0;
		// Called when a redirect is recieved
		virtual void OnRedirect(std::string originalUrl, std::shared_ptr<HttpRedirectResponse> document) = 0;
		// Called when an error is returned from the server
		virtual void OnError(std::string originalUrl, std::shared_ptr<HttpErrorResponse> error) = 0;
	};
}


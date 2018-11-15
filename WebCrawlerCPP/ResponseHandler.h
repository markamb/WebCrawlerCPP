#pragma once
#include "IResponseHandler.h"
#include "IWebCrawler.h"

namespace WebCrawler
{
	//
	// Class for processing a http response
	// Documents are parsed, links extracted and passed back to the WebCrawler implementation
	// Basic stats are aggregated.
	//
	class ResponseHandler :	public IResponseHandler
	{
	public:
		ResponseHandler(IWebCrawler *webCrawler);
		virtual ~ResponseHandler();

		// IDocumentHandler methods
		virtual void OnSuccess(std::string urlStr, std::shared_ptr<HttpDocumentResponse> document) override;
		virtual void OnRedirect(std::string originalUrl, std::shared_ptr<HttpRedirectResponse> document) override;
		virtual void OnError(std::string originalUrl, std::shared_ptr<HttpErrorResponse> error) override;

		// New methods
		size_t	 SuccessCount() const { return successCount_; }
		size_t	 RedirectCount() const { return redirectCount_; }
		size_t	 ErrorCount() const { return errorCount_;  }
		int64_t	 TotalSize() const { return totalSize_; }

	private:
		IWebCrawler	 *	webCrawler_;
		size_t			successCount_;
		size_t			redirectCount_;
		size_t			errorCount_;
		int64_t			totalSize_;
	};
}



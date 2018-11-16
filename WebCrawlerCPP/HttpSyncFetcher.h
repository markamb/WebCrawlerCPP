#pragma once
#include <memory>
#include "IFetcher.h"
#include "IResponseHandler.h"
#include "HttpResponse.h"
#include "Url.h"

namespace WebCrawler
{

	//
	// An IFetcher implementation for syncronously loading using HTTP 
	// Urls are requested and the response sent to the IResponseHandler supplied in the constructor
	//
	class HttpSyncFetcher : public IFetcher
	{
	public:
		HttpSyncFetcher(IResponseHandler *handler);
		~HttpSyncFetcher();

		virtual void Fetch(Url url) override;

	private:
		IResponseHandler *handler_;
		std::shared_ptr<HttpResponse> LoadDocumentSSL(const Url & url);
		std::shared_ptr<HttpResponse> LoadDocument(const Url &url);
	};

}



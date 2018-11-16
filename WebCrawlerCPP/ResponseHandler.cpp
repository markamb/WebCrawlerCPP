#include "pch.h"
#include "ResponseHandler.h"
#include "HttpSyncFetcher.h"
#include "HtmlDocumentParser.h"
#include <iostream>

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

	void ResponseHandler::OnSuccess(Url srcUrl, std::shared_ptr<HttpDocumentResponse> document)
	{		
		using namespace std;

		size_t numLinks = 0;
		try
		{
			HtmlDocumentParser parser;
			auto webLinks{ parser.Parse(srcUrl, document->GetDocument()) };
			numLinks = webLinks.size();
			webCrawler_->AddURLs(webLinks);
		}
		catch (exception &e)
		{
			cerr << "Exception parsing document: " << e.what() << endl;
		}

		cout << "*** Loaded document " << srcUrl << ", Content-Type= " << document->GetContentType() << ", Response Code=" << document->ResponseCode() << ", NumLinks= " << numLinks << ", Length = " << document->GetDocumentLength() / 1024 << "Kb\n";
		successCount_++;
		totalSize_ += document->GetDocumentLength();
	}
	
	void ResponseHandler::OnRedirect(Url srcUrl, std::shared_ptr<HttpRedirectResponse> response)
	{
		string location = response->GetRedirectLocation();
		cout << "*** Redirect recieved for " << srcUrl << ", Response Code=" << response->ResponseCode() << ", Location=" << location << "\n";
		redirectCount_++;
		if (!location.empty())
			webCrawler_->AddURL(location);
	}

	void ResponseHandler::OnError(Url srcUrl, std::shared_ptr<HttpErrorResponse> error)
	{
		if (error->IsClientError())
			cout << "*** Client Error";
		else
			cout << "*** Server Error";
		errorCount_++;
		cout << " recieved for " << srcUrl << ", Response Code = " << error->ResponseCode() << "\n";
	}

}


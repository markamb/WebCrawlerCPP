#include "pch.h"
#include "WebCrawler.h"
#include "HttpResponse.h"
#include "IFetcher.h"
#include "IResponseHandler.h"
#include "IWebCrawler.h"
#include <iostream>
#include <vector>

namespace WebCrawler
{

	WebCrawler::WebCrawler() 
	{
	}

	WebCrawler::WebCrawler(const char * start_page) : startPage_(start_page)
	{
		AddURL(start_page);
	}

	WebCrawler::WebCrawler(const std::vector<std::string> &urls)
	{
		AddURLs(urls);
	}

	WebCrawler::~WebCrawler()
	{

	}

	void WebCrawler::AddURL(const std::string &url)
	{
		// Ignore if we have already loaded this once
		// TODO:  Need better url comparisons to allow for equivilent URLs
		// TODO:  Need a retry count and max retry
		if (visited_.find(url) != visited_.end())
			return;
		queue_.push(url);
	}

	void WebCrawler::AddURLs(const std::vector<std::string> &urls)
	{
		// Ignore if we have already loaded this once
		// TODO:  Need better url comparisons to allow for equivilent URLs
		// TODO:  Need a retry count and max retry
		for (auto next : urls)
		{
			if (visited_.find(next) != visited_.end())
				return;
			queue_.push(next);
		}
	}

	void WebCrawler::Run(IResponseHandler *handler, IFetcher *fetcher)
	{
		using namespace WebCrawler;
		using namespace std;

		int loadsLeft = 10;				// TEMP - Just load first few links for now

		while (!queue_.empty())
		{
			string next{ queue_.front() };
			queue_.pop();
			visited_.insert(next);
			fetcher->Fetch(next);

			if (--loadsLeft == 0)
				break;					// TEMP - Just load first few links for now
		}
	}
}


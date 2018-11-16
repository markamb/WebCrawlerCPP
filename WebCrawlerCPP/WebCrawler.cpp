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
		AddURL(Url(start_page));
	}

	WebCrawler::WebCrawler(const std::vector<std::string> &urls)
	{
		std::vector<Url> vec;
		for (auto next : urls)
		{
			try 
			{
				vec.push_back(Url(next));
			}
			catch (UrlException &)
			{
				std::cerr << "Skipping invalid seed Url << " << next << std::endl;
			}
		}
		AddURLs(vec);
	}

	WebCrawler::~WebCrawler()
	{

	}

	void WebCrawler::AddURL(const std::string & url)
	{
		AddURL(Url(url));
	}

	void WebCrawler::AddURL(const Url &url)
	{
		if (visited_.find(url) != visited_.end())
			return;
		std::cout << "Adding Url: " << url << "\n";
		queue_.push(url);
	}

	void WebCrawler::AddURLs(const std::vector<Url> &urls)
	{
		for (auto next : urls)
		{
			if (visited_.find(next) != visited_.end())
				continue;
			std::cout << "Adding Url: " << next << "\n";
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
			auto next { queue_.front() };
			queue_.pop();
			visited_.insert(next);
			fetcher->Fetch(next);
			if (--loadsLeft == 0)
				break;					// TEMP - Just load first few links for now
		}
	}
}


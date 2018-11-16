#pragma once
#include <string>
#include <vector>
#include "Url.h"

namespace WebCrawler
{
	class IWebCrawler
	{
	public:
		IWebCrawler() 
		{}
		virtual ~IWebCrawler()
		{}

		// Register a URL to be crawled		
		virtual void AddURL(const std::string &url) = 0;
		virtual void AddURL(const Url &url) = 0;
		virtual void AddURLs(const std::vector<Url> &urls) = 0;
	};
}



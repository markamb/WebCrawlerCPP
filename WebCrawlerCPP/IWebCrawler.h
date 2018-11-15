#pragma once
#include <string>
#include <vector>

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
		virtual void AddURLs(const std::vector<std::string> &urls) = 0;
	};
}



#pragma once

#include <string>
#include <unordered_set>
#include <queue>
#include "IWebCrawler.h"
#include "IResponseHandler.h"
#include "IFetcher.h"

namespace WebCrawler
{
	class WebCrawler : public IWebCrawler
	{
	public:
		WebCrawler();
		// Start from given seed url
		WebCrawler(const char *startPage);
		// start from given seed urls
		WebCrawler(const std::vector<std::string>&);
		~WebCrawler();

		// IWebCrawler methods
		virtual void AddURL(const std::string &url) override;
		virtual void AddURLs(const std::vector<std::string> &urls) override;

		// New Methods
		void Run(IResponseHandler *handler, IFetcher *fetcher);

	private:
		std::string startPage_;
		// set of all visted urls
		std::unordered_set<std::string> visited_;
		// queue of urls yet to be visited
		std::queue<std::string>	queue_;
	};
}


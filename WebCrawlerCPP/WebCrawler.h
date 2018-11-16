#pragma once

#include <string>
#include <unordered_set>
#include <queue>
#include "IWebCrawler.h"
#include "IResponseHandler.h"
#include "IFetcher.h"
#include "Url.h"

namespace WebCrawler
{

//	auto urlHash = [] (const Url& u) { return std::hash<std::string>()(u.AsString()); };
//	auto urlCompare = [] (const Url& u1, const Url &u2) { return u1.AsString().compare(u2.AsString()); };
//	using UrlSet = std::unordered_set<Url, decltype(urlHash), decltype(urlCompare)>;

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
		virtual void AddURL(const Url &url) override;
		virtual void AddURLs(const std::vector<Url> &urls) override;

		// New Methods
		void Run(IResponseHandler *handler, IFetcher *fetcher);

	private:
		std::string startPage_;
		// set of all visted urls
		std::unordered_set<Url>	 visited_;
		// queue of urls yet to be visited
		std::queue<Url>	queue_;
	};
}


// WebCrawlerCPP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include "WebCrawler.h"
#include "HttpSyncFetcher.h"
#include "ResponseHandler.h"
#include <iostream>

//const char * URL1 = "http://www.example.com/";
//const char * URL2 = "http://www.bbc.co.uk/news";					//     -redirect use case to test ???
//const char * URL3 = "https://en.wikipedia.org/wiki/Main_Page/";
//const char * URL4 = "https://www.bbc.co.uk/news";					//     -redirect use case to test ???
//const char * URL5 = "https://www.bbc.co.uk/newsnothere";			//     -no doc
//const char * URL6 = "https//www.bbc.co.uk/news";					//     -Invalid URL

namespace
{

	std::vector<std::string> seedUrls
	{
		// some tests cases
		"http://www.example.com/",
		"http://www.bbc.co.uk/news", //  redirect use case to test ???
		"https://en.wikipedia.org/wiki/Main_Page",
		"https://www.bbc.co.uk/newsnothere",   	// no doc present
		"https//www.bbc.co.uk/news",			// invalid url

		// some large web sites
		"www.bbc.co.uk",
		"www.amazon.co.uk",
		"www.microsoft.com",
		"google.co.uk",
		"youtube.com",
		"facebook.com",
		"baidu.com",
		"wikipedia.org",
		"reddit.com",
		"yahoo.com",
		"qq.com",
		"taobao.com",
		"google.co.in",
		"amazon.com",
		"tmall.com",
		"twitter.com",
		"https://www.bbc.co.uk/news"
		/*
		"sohu.com",
		"instagram.com",
//		"vk.com",			// redirect to timed out url
		"live.com",
		"jd.com",
		"sina.com.cn",
		"weibo.com",
		"yandex.ru",
		"360.cn",
		"google.co.jp",
		"google.co.uk",
		"list.tmall.com",
		"google.ru",
		"google.com.br",
		"netflix.com",
		"google.de",
		"google.com.hk",
		//	"pornhub.com",
		"twitch.tv",
		"google.fr",
		"linkedin.com",
		"yahoo.co.jp",
		"t.co",
		"csdn.net",
		"microsoft.com",
		"bing.com"
		//		"office.com",		times out
				"ebay.com",
				"alipay.com",
				//	"xvideos.com",
				"google.it",
				"google.ca",
				"mail.ru",
				"ok.ru",
				"google.es",
				"pages.tmall.com",
				"msn.com",
				"google.com.tr",
				"google.com.au",
				"whatsapp.com",
				"spotify.com",
				"google.pl",
				"google.co.id",
				//	"xhamster.com",
				"google.com.ar",
				"xnxx.com",
				"google.co.th",
				"Naver.com",
				"sogou.com",
				"samsung.com",
				"accuweather.com",
				"goo.gl",
				"sm.cn",
				"googleweblight.com"
		*/ };

}

int main(int argc, char* argv[])
{
	using std::cout;

	WebCrawler::WebCrawler crawler(seedUrls);				// multiple urls
//	WebCrawler::WebCrawler crawler("http://yahoo.com");		// chained re-directs
//	WebCrawler::WebCrawler crawler("bbc.co.uk");			// redirect
	WebCrawler::ResponseHandler handler(&crawler);
	WebCrawler::HttpSyncFetcher fetcher(&handler);
	crawler.Run(&handler, &fetcher);

	cout << "\n\nDone:\n";
	cout << "  SuccessCount=" << handler.SuccessCount() << "\n";
	cout << "  RedirectCount=" << handler.RedirectCount() << "\n";
	cout << "  ErrorCount=" << handler.ErrorCount() << "\n";
	cout << "  TotalSize=" << handler.TotalSize() / (1024 * 1024) << "Mb\n";

	return EXIT_SUCCESS;
}



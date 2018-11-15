#include "pch.h"
#include "Url.h"

namespace
{
	// Simple helper to instantiate a new URL. Exepoect an exception to be thrown.
	static void CreateUrl(const char *str)
	{
		WebCrawler::Url url(str);
	}
}

TEST(TestUrl, FullUrl) 
{
	using namespace WebCrawler;

	Url  url("http://bbc.co.uk:80/news/index.htm");
	EXPECT_EQ(url.GetProtocol(), Url::HTTP);
	EXPECT_EQ(url.GetHost(), "bbc.co.uk");
	EXPECT_EQ(url.GetPort(), "80");
	EXPECT_EQ(url.GetPath(), "/news/index.htm");
}

TEST(TestUrl, OverridePort)
{
	using namespace WebCrawler;

	Url  url("http://bbc.co.uk:81/news/index.htm");
	EXPECT_EQ(url.GetProtocol(), Url::HTTP);
	EXPECT_EQ(url.GetHost(), "bbc.co.uk");
	EXPECT_EQ(url.GetPort(), "81");
	EXPECT_EQ(url.GetPath(), "/news/index.htm");
}

TEST(TestUrl, FullDefaultPort)
{
	using namespace WebCrawler;

	Url  url("http://bbc.co.uk/news/index.htm");
	EXPECT_EQ(url.GetProtocol(), Url::HTTP);
	EXPECT_EQ(url.GetHost(), "bbc.co.uk");
	EXPECT_EQ(url.GetPort(), "80");
	EXPECT_EQ(url.GetPath(), "/news/index.htm");
}

TEST(TestUrl, UrlWithQuery)
{
	using namespace WebCrawler;

	Url  url("http://bbc.co.uk/news/index.htm?a=b;c=d");
	EXPECT_EQ(url.GetProtocol(), Url::HTTP);
	EXPECT_EQ(url.GetHost(), "bbc.co.uk");
	EXPECT_EQ(url.GetPort(), "80");
	EXPECT_EQ(url.GetPath(), "/news/index.htm?a=b;c=d");
}

TEST(TestUrl, UrlWithFragment)
{
	using namespace WebCrawler;

	Url  url("http://bbc.co.uk/news/index.htm#loc");
	EXPECT_EQ(url.GetProtocol(), Url::HTTP);
	EXPECT_EQ(url.GetHost(), "bbc.co.uk");
	EXPECT_EQ(url.GetPort(), "80");
	EXPECT_EQ(url.GetPath(), "/news/index.htm#loc");
}

TEST(TestUrl, UrlWithQueryAndFragment)
{
	using namespace WebCrawler;

	Url  url("http://bbc.co.uk/news/index.htm?a=b;c=d#here");
	EXPECT_EQ(url.GetProtocol(), Url::HTTP);
	EXPECT_EQ(url.GetHost(), "bbc.co.uk");
	EXPECT_EQ(url.GetPort(), "80");
	EXPECT_EQ(url.GetPath(), "/news/index.htm?a=b;c=d#here");
}

TEST(TestUrl, DefaultProtocol)
{
	using namespace WebCrawler;

	Url  url("bbc.co.uk/news/index.htm");
	EXPECT_EQ(url.GetProtocol(), Url::HTTP);
	EXPECT_EQ(url.GetHost(), "bbc.co.uk");
	EXPECT_EQ(url.GetPort(), "80");
	EXPECT_EQ(url.GetPath(), "/news/index.htm");
}

TEST(TestUrl, DefaultPath)
{
	using namespace WebCrawler;

	Url  url("www.bbc.co.uk");
	EXPECT_EQ(url.GetProtocol(), Url::HTTP);
	EXPECT_EQ(url.GetHost(), "www.bbc.co.uk");
	EXPECT_EQ(url.GetPort(), "80");
	EXPECT_EQ(url.GetPath(), "/");
}

TEST(TestUrl, SSLUrl)
{
	using namespace WebCrawler;

	Url  url("https://bbc.co.uk/");
	EXPECT_EQ(url.GetProtocol(), Url::HTTPS);
	EXPECT_EQ(url.GetHost(), "bbc.co.uk");
	EXPECT_EQ(url.GetPort(), "443");
	EXPECT_EQ(url.GetPath(), "/");
}

TEST(TestUrl, SSLWithPort)
{
	using namespace WebCrawler;

	Url  url("https://bbc.co.uk:500/");
	EXPECT_EQ(url.GetProtocol(), Url::HTTPS);
	EXPECT_EQ(url.GetHost(), "bbc.co.uk");
	EXPECT_EQ(url.GetPort(), "500");
	EXPECT_EQ(url.GetPath(), "/");
}

TEST(TestUrl, UnknownProtocol)
{
	using namespace WebCrawler;

	Url  url("ftp://bbc.co.uk:500/");
	EXPECT_EQ(url.GetProtocol(), Url::UNSUPPORTED);
	EXPECT_EQ(url.GetHost(), "bbc.co.uk");
	EXPECT_EQ(url.GetPort(), "500");
	EXPECT_EQ(url.GetPath(), "/");
}

TEST(TestUrl, MalformedUrlProtocol)
{
	EXPECT_THROW(CreateUrl("http:///bbc.co.uk/test.txt"), WebCrawler::UrlException);
	EXPECT_THROW(CreateUrl("http:://bbc.co.uk"), WebCrawler::UrlException);
	EXPECT_THROW(CreateUrl("http:://bbc.co.uk:"), WebCrawler::UrlException);
	EXPECT_THROW(CreateUrl("http//bbc.co.uk"), WebCrawler::UrlException);
	EXPECT_THROW(CreateUrl("h4ttp//bbc.co.uk"), WebCrawler::UrlException);
	EXPECT_THROW(CreateUrl("//bbc.co.uk::80"), WebCrawler::UrlException);
	EXPECT_THROW(CreateUrl("://bbc.co.uk::80"), WebCrawler::UrlException);
}

TEST(TestUrl, MalformedUrlPort)
{
	EXPECT_THROW(CreateUrl("http://bbc.co.uk:80:80/test.txt"), WebCrawler::UrlException);
	EXPECT_THROW(CreateUrl("http://bbc.co.uk::80/test.txt"), WebCrawler::UrlException);
	EXPECT_THROW(CreateUrl("bbc.co.uk::80/test.txt"), WebCrawler::UrlException);
	EXPECT_THROW(CreateUrl("bbc.co.uk::80"), WebCrawler::UrlException);
}

TEST(TestUrl, MalformedUrlHost)
{
	EXPECT_THROW(CreateUrl("http://bbc.co.uk//test.txt"), WebCrawler::UrlException);
	EXPECT_THROW(CreateUrl("bbc.co.uk//test.txt"), WebCrawler::UrlException);
	EXPECT_THROW(CreateUrl("bbc.co.uk//"), WebCrawler::UrlException);
}


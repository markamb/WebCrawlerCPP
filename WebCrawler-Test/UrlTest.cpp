#include "pch.h"
#include "Url.h"

namespace
{
	// Simple helper to instantiate a new URL. Expect an exception to be thrown.
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

TEST(TestUrl, ToString)
{
	using namespace WebCrawler;

	std::string result("http://bbc.co.uk:80/news/index.htm");

	Url  url1("http://bbc.co.uk:80/news/index.htm");
	EXPECT_EQ(url1.AsString(), result);
	
	Url  url2("bbc.co.uk/news/index.htm");
	EXPECT_EQ(url2.AsString(), result);

	Url  url3("http://bbc.co.uk/news/index.htm");
	EXPECT_EQ(url3.AsString(), result);
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

TEST(TestUrlCopying, UrlCopyConstruct)
{
	using namespace WebCrawler;

	Url  src("http://bbc.co.uk:80/news/index.htm");
	Url  copy(src);

	EXPECT_EQ(src.GetProtocol(), Url::HTTP);
	EXPECT_EQ(src.GetHost(), "bbc.co.uk");
	EXPECT_EQ(src.GetPort(), "80");
	EXPECT_EQ(src.GetPath(), "/news/index.htm");
	EXPECT_EQ(copy.GetProtocol(), src.GetProtocol());
	EXPECT_EQ(src.GetHost(), src.GetHost());
	EXPECT_EQ(src.GetPort(), src.GetPort());
	EXPECT_EQ(src.GetPath(), src.GetPath());
}

TEST(TestUrlCopying, UrlMove)
{
	using namespace WebCrawler;

	Url  src("http://bbc.co.uk:80/news/index.htm");
	Url  copy(std::move(src));

	EXPECT_EQ(src.GetHost(), "");
	EXPECT_EQ(src.GetPort(), "");
	EXPECT_EQ(src.GetPath(), "");
	EXPECT_EQ(copy.GetProtocol(), Url::HTTP);
	EXPECT_EQ(copy.GetHost(), "bbc.co.uk");
	EXPECT_EQ(copy.GetPort(), "80");
	EXPECT_EQ(copy.GetPath(), "/news/index.htm");
}

TEST(TestRelativeUrl, UrlRelativeNewPath1)
{
	using namespace WebCrawler;

	Url  src("http://bbc.co.uk:80/news/index.htm");
	Url  copy{ src.MakeRelative("/newpath") };

	EXPECT_EQ(copy.GetProtocol(), src.GetProtocol());
	EXPECT_EQ(copy.GetPort(), src.GetPort());
	EXPECT_EQ(copy.GetHost(), src.GetHost());
	EXPECT_EQ(copy.GetPath(), "/newpath");
}

TEST(TestRelativeUrl, UrlRelativeNewPath2)
{
	using namespace WebCrawler;

	Url  src("http://bbc.co.uk:80/news/index.htm");
	Url  copy{ src.MakeRelative("/newpath/2/3") };

	EXPECT_EQ(copy.GetProtocol(), src.GetProtocol());
	EXPECT_EQ(copy.GetPort(), src.GetPort());
	EXPECT_EQ(copy.GetHost(), src.GetHost());
	EXPECT_EQ(copy.GetPath(), "/newpath/2/3");
}

TEST(TestRelativeUrl, UrlRelativeNewPath3)
{
	using namespace WebCrawler;

	Url  src("http://bbc.co.uk:80/news/index.htm");
	Url  copy{ src.MakeRelative("/") };

	EXPECT_EQ(copy.GetProtocol(), src.GetProtocol());
	EXPECT_EQ(copy.GetPort(), src.GetPort());
	EXPECT_EQ(copy.GetHost(), src.GetHost());
	EXPECT_EQ(copy.GetPath(), "/");
}

TEST(TestRelativeUrl, UrlRelativeEmptyPath)
{
	using namespace WebCrawler;

	Url  src("http://bbc.co.uk:80/news/index.htm");
	Url  copy{ src.MakeRelative("") };

	EXPECT_EQ(copy.GetProtocol(), src.GetProtocol());
	EXPECT_EQ(copy.GetPort(), src.GetPort());
	EXPECT_EQ(copy.GetHost(), src.GetHost());
	EXPECT_EQ(copy.GetPath(), src.GetPath());
}

TEST(TestRelativeUrl, UrlRelativeNewUrl)
{
	using namespace WebCrawler;

	Url  src("http://bbc.co.uk:80/news/index.htm");
	Url  copy{ src.MakeRelative("https://newdomain:81/new") };

	EXPECT_EQ(copy.GetProtocol(), Url::HTTPS);
	EXPECT_EQ(copy.GetPort(), "81");
	EXPECT_EQ(copy.GetHost(), "newdomain");
	EXPECT_EQ(copy.GetPath(), "/new");
}

TEST(TestUrlComparsons, UrlsEqual)
{
	using namespace WebCrawler;

	// 4 different ways to construct the equvilant url
	Url  u1("http://bbc.co.uk:80/news/index.htm");
	Url  u2("http://bbc.co.uk:80/news/index.htm");
	Url  u3("bbc.co.uk/news/index.htm");
	Url  u4(u1);

	EXPECT_EQ(u1.GetHash(), u2.GetHash());
	EXPECT_EQ(u1.GetHash(), u3.GetHash());
	EXPECT_EQ(u1.GetHash(), u4.GetHash());

	EXPECT_TRUE(u1 == u2);
	EXPECT_TRUE(u1 == u3);
	EXPECT_TRUE(u1 == u4);

	EXPECT_FALSE(u1 != u2);
	EXPECT_FALSE(u1 != u2);
	EXPECT_FALSE(u1 != u4);
}

TEST(TestUrlComparsons, UrlsNotEqual)
{
	using namespace WebCrawler;

	Url  u1("http://bbc.co.uk:80/news/index.htm");
	Url  u2("http://bbc.co.uk:80/news/index2.htm");
	Url  u3("http://bbc.co.uk:81/news/index.htm");
	Url  u4("https://bbc.co.uk:80/news/index.htm");

	EXPECT_NE(u1.GetHash(), u2.GetHash());
	EXPECT_NE(u1.GetHash(), u3.GetHash());
	EXPECT_NE(u1.GetHash(), u4.GetHash());
	EXPECT_NE(u2.GetHash(), u3.GetHash());
	EXPECT_NE(u2.GetHash(), u4.GetHash());
	EXPECT_NE(u3.GetHash(), u4.GetHash());

	EXPECT_FALSE(u1 == u2);
	EXPECT_FALSE(u1 == u3);
	EXPECT_FALSE(u1 == u4);
	EXPECT_FALSE(u2 == u3);
	EXPECT_FALSE(u2 == u4);
	EXPECT_FALSE(u3 == u4);

	EXPECT_TRUE(u1 != u2);
	EXPECT_TRUE(u1 != u3);
	EXPECT_TRUE(u1 != u4);
	EXPECT_TRUE(u2 != u3);
	EXPECT_TRUE(u2 != u4);
	EXPECT_TRUE(u3 != u4);
}


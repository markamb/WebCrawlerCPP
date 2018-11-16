#include "pch.h"
#include "Url.h"
#include <algorithm>
#include <regex>
#include <iostream>
#include <sstream>
#include <boost/functional/hash.hpp>

namespace WebCrawler
{

	Url::Url(const std::string &url) 
	{
		Parse(url);
	}

	Url	Url::MakeRelative(const std::string &relativeUrlStr) const
	{
		// if empty just clone this
		if (relativeUrlStr.empty())
			return Url(*this);

		// if starts with "/" assume a relative path
		if (relativeUrlStr[0] == '/')
		{
			Url newUrl(*this);
			newUrl.path_ = relativeUrlStr;
			return newUrl;
		}

		// else just assume not a relative path and string is a full Url
		Url newUrl(relativeUrlStr);
		return newUrl;
	}

	const char * Url::GetProtocolStr() const
	{
		switch (protocol_)
		{
		case HTTPS:
			return "https";
		case HTTP:
			return "http";
		default:
			return "<Unknown>";
		}
		return nullptr;
	}

	void Url::Parse(const std::string &url) 
	{
		using namespace std;

		// A fairly simple regular expression to split the URL - requires additional processing of parts but doing all in one RegEx in extreemly error prone
		const char *REG_EXP = R"(^([A-Za-z]+:)?(//)?([A-Za-z0-9\-\.]+)(:[0-9]+)?(.*)$)";
		std::regex  regEx(REG_EXP);
		std::smatch match;
		std::regex_search(url, match, regEx);

		string scheme{ match[1] };
		string hasScheme{ match[2] };
		string authority{ match[3] };
		string port{ match[4] };
		string path{ match[5] };		// Note: Includes any query or fragment entry

		if (!path.empty() && path[0] == ':')
			throw UrlException(string("Invalid URL ") + url);
		if (path.length() > 1 && path[0] == '/' && path[1] == '/')
			throw UrlException(string("Invalid URL ") + url);

		// determine protocol (default to http)
		auto pos = scheme.find(':');
		if (pos != string::npos)
			scheme = scheme.substr(0, pos);
		if (scheme.empty() || scheme == "http")
			protocol_ = HTTP;
		else if (scheme == "https")
			protocol_ = HTTPS;
		else
			protocol_ = UNSUPPORTED;		// ftp, mailto etc

		// override default port if one was supplied
		if (!port.empty())
		{
			size_t start = 0;
			while (start <= port.length() && !isdigit(port[start]))
				start++;
			auto end = start + 1;
			while (end <= port.length() && isdigit(port[end]))
				end++;
			port_ = port.substr(start, end - start);
		}
		else
			port_ = DefaultPort(protocol_);

		if (authority.empty())
			throw UrlException(string("No host provided in URI ") + url);
		host_ = authority;
		path_ = path.empty() ? "/" : path;
	}

	std::string Url::DefaultPort(Protocol p)
	{
		switch (p)
		{
		case HTTP:
			return "80";
		case HTTPS:
			return "443";
		};
		return std::string();
	}

	bool Url::operator==(const Url &u2) const
	{
		if (this == &u2)
			return true;
		if (protocol_ != u2.protocol_)
			return false;
		if (host_ != u2.host_)
			return false;
		if (port_ != u2.port_)
			return false;
		if (path_ != u2.path_)
			return false;
		return true;
	}

	size_t Url::GetHash() const
	{
		using namespace std;
		size_t seed = 0;
		boost::hash_combine(seed, std::hash<int>()(static_cast<int>(protocol_)));
		boost::hash_combine(seed, std::hash<string>()(port_));
		boost::hash_combine(seed, std::hash<string>()(host_));
		boost::hash_combine(seed, std::hash<string>()(path_));
		return seed;
	}

}


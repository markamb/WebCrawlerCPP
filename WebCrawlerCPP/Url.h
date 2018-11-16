#pragma once

#include <string>
#include <exception>
#include <ostream>
#include <sstream>

namespace WebCrawler
{

	// URL Exception
	// Thrown when a Url is constructed with an invalid string
	class UrlException : public std::exception
	{
	public:
		UrlException(const char *msg) : exception(msg)
		{}

		UrlException(std::string msg) : exception(msg.c_str())
		{}
	};

	//
	// Utility class for storing and parsing an immutable URL
	//
	class Url
	{
	public:
		// Supported protocols
		enum Protocol { UNSUPPORTED, HTTP, HTTPS };

		// Construct the Url from the given string
		// Throws UrlException if string is not a valid or supported URL
		//
		// Missing parts of the URL are defaulted as follows:
		//	Protocol	"http"
		//  Port		Depends on protocol
		//	Host		None, throws execption if missing
		//	Path		"/"
		//
		explicit Url(const std::string &urlStr);
		
		Url(const Url &) = default;
		Url(Url &&) = default;

		// Create a new Url based on this one using the supplied (possibly) relative path
		Url		MakeRelative(const std::string &relativeUrlStr) const;

		Protocol			GetProtocol() const { return protocol_; }
		const char *		GetProtocolStr() const;
		const std::string &	GetHost() const { return host_; }
		const std::string &	GetPort() const { return port_; }
		const std::string &	GetPath() const { return path_; }

		// convert to a string
		std::string AsString() const;

		// return a hash code
		size_t  GetHash() const;

		bool operator==(const Url &u) const;
		bool operator!=(const Url &u) const { return !operator==(u); }

	private:
		void Parse(const std::string &url);
		std::string		DefaultPort(Protocol);

		Protocol		protocol_;
		std::string		host_;
		std::string		port_;
		std::string		path_;
	};

	inline std::ostream &operator<<(std::ostream &os, const Url &url)
	{
		return os << url.GetProtocolStr() << "://" << url.GetHost() << ":" << url.GetPort() << url.GetPath();
	}

	inline std::string Url::AsString() const
	{
		std::stringstream ss;
		ss << *this;
		return ss.str();
	}

	/*
	inline bool operator==(const Url &u1, const Url &u2)
	{
		if (u1.GetProtocol() != u2.GetProtocol())
			return false;
		if (u1.GetHost() != u2.GetHost())
			return false;
		if (u1.GetPort() != u2.GetPort())
			return false;
		if (u1.GetPath() != u2.GetPath())
			return false;
		return true;
	}
*/
}

//
// Create the hash and comparison function for a Url
//
namespace std
{
	template <>
	struct hash<WebCrawler::Url>
	{
		size_t operator()(const WebCrawler::Url& url) const
		{
			return url.GetHash();
		}
	};
}




#pragma once

#include <string>
#include <exception>
#include <ostream>

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
		Url(std::string urlStr);
		
		// Construct a new Url based on the source with a new protocol (and port)
		Url(const Url &, Protocol newProtocol);

		// Construct a new Url based on the source with a new path
		Url(const Url &, std::string newPath);

		Protocol			GetProtocol() const { return protocol_; }
		const std::string &	GetHost() const { return host_; }
		const std::string &	GetPort() const { return port_; }
		const std::string &	GetPath() const { return path_; }

	private:
		void Parse(std::string url);
		std::string		DefaultPort(Protocol);

		Protocol		protocol_;
		std::string		host_;
		std::string		port_;
		std::string		path_;
	};

	inline std::ostream &operator<<(std::ostream &os, const Url &url)
	{
		return os << url.GetProtocol() << "://" << url.GetHost() << ":" << url.GetPort() << url.GetPath();
	}

}



#pragma once

#include <string>

namespace WebCrawler
{

	//
	// Interface for fetching documents.
	// Loading may be synchronous or asyncronous depending on the implementation.
	// Processing performed with loaded documents is dependent on the implemntation.
	//
	class IFetcher
	{
	public:
		IFetcher() {};
		virtual ~IFetcher() {};

		virtual void Fetch(const std::string &url) = 0;
	};

}


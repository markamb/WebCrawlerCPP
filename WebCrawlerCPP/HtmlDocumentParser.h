#pragma once
#include <string>
#include <vector>
#include "Url.h"

namespace WebCrawler
{

	//
	// Simple class for extracting what we need from a HTTP document
	// At present we only care about links to other documents to be loaded
	// Any relative links are constructed relative to the supplied Url
	class HtmlDocumentParser
	{
	public:
		using WebLinks = std::vector<Url>;

		// Parse the given HTML document and return any links we can extract
		// Base
		WebLinks Parse(const Url &baseUrl, const std::string &doc);
	};

}


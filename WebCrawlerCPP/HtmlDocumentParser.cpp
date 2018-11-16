#include "pch.h"
#include "HtmlDocumentParser.h"
#include <iostream>
#include <regex>
#include <string>
#include "Url.h"

namespace WebCrawler
{
	HtmlDocumentParser::WebLinks HtmlDocumentParser::Parse(const Url &baseUrl, const std::string &doc)
	{
		using namespace std;

		WebLinks links;
		regex  aRegEx(R"(<a.*?>.*?</a>)");
		regex  linkRegEx(R"(<a\s+(?:[^>]*?\s+)?href=(["'])(.*?)\1)");
		smatch aMatch;
		auto  searchStart(doc.cbegin());
		while (regex_search(searchStart, doc.cend(), aMatch, aRegEx))
		{
			string link{ aMatch[0] };
			string url{ link };
			try
			{
				smatch linkMatch;
				if (regex_search(link, linkMatch, linkRegEx))
				{
					url = linkMatch[2];
					if (!url.empty() && url[0] != '#')
					{
						// ignore urls with only an anchor as these refer to page just loaded
						Url newUrl{ baseUrl.MakeRelative(linkMatch[2]) };
						links.push_back(std::move(newUrl));
					}
				}
			}
			catch (exception &e)
			{
				cout << "Ignoring invalid Url " << url << " : " << e.what() << "\n";
			}
			searchStart += aMatch.position() + aMatch.length();
		}
		return links;
	}

}
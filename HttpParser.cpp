#include "HttpParser.h"

#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

HttpParser::HttpParser(std::string_view data) : 
    mDataForParsing(data)
{

}

HttpParser::~HttpParser()
{

}

void HttpParser::parse()
{
    std::string data(mDataForParsing);
    std::stringstream io_str(data);

    std::vector<std::string> tokens;

    std::string token;
    char delim = '\n';
    while( std::getline(io_str,token,delim) )
    {
        tokens.emplace_back(token);
    }




}
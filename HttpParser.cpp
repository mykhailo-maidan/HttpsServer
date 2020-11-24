#include "HttpParser.h"

#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

HttpParser::HttpParser(const std::string& data) : 
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

    SHeader header;

    parseMethod(tokens[0],header);

    tokens.erase(tokens.begin());

    for(auto& it : tokens)
    {
        if(it.find("User-Agent:") != std::string::npos)
        {
            header.user_agent.append(it.begin() + it.find_last_of(':') + 2,it.end());
        }
        if(it.find("Accept:") != std::string::npos)
        {
            header.accept.append(it.begin() + it.find_last_of(':') + 2,it.end());
        }

        if(it.find("Host:") != std::string::npos)
        {
            header.host.append(it.begin() + it.find_last_of(':') + 2, it.end());
        } 
    }

}

void HttpParser::parseMethod(const std::string& token, SHeader& header)
{
    std::stringstream io(token);

    char delim = ' ';
    std::string t;
    std::vector<std::string> method;
    while( std::getline(io,t, delim) )
    {
        method.emplace_back(t);
    }

    if(method[0].compare("GET") == 0)
    {
        header.method = EMethod::eGet;
    }
    else if( method[0].compare("POST") == 0)
    {
        header.method = EMethod::ePost;
    }

    header.url = method[1];
    header.version = method[2];
}

#ifndef _CHUNK_BUILDER_H_
#define _CHUNK_BUILDER_H_

#include <string>
#include <iostream>
#include <sstream>
class ChunkBuilder
{
public:
  explicit ChunkBuilder( const std::string& data ) : mData( data )
  {
    std::stringstream ss;
    ss << std::hex << data.size(); // int decimal_value
    mData.insert( 0, ss.str() + "\r\n" );
    mData.append( "\r\n" );
  };
  ~ChunkBuilder() = default;

  operator std::string()
  {
    return std::move( mData );
  }

private:
  std::string mData;
};

#endif // _CHUNK_BUILDER_H_
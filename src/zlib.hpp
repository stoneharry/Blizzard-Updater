#ifndef ZLIB_HPP
#define ZLIB_HPP

#include <vector>

namespace zlib
{
  void inflate (const std::vector<unsigned char>& input, std::vector<unsigned char>* output);
  std::vector<unsigned char> deflate (const std::vector<unsigned char>& data);
}

#endif

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem; // replace with std::tr2 as soon as possible

fs::path makeRelative(fs::path fromPath, fs::path toPath)
{
  fs::path res;

  for(fs::path::iterator from = fromPath.begin(), to = toPath.begin(); to != toPath.end(); ++to)
  {
    if(*from != *to)
      res /= (*to);

    if(from != fromPath.end())
      ++from;
  }

  return res;
}

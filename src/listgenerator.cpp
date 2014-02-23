#include <vector>
#include <sstream>
#include <iostream>

#include "file.hpp"
#include "util.hpp"
#include "listruleset.hpp"

int main(int argc, char **argv)
try
{
  if(argc != 3)
    throw std::runtime_error("arguments missing \n usage: <inDir> <listFile>");

  fs::path inDir(argv[1]);
  std::ofstream listFile(argv[2]);

  if(!fs::exists("rules.txt"))
    throw std::runtime_error("rules.txt does not exist");

  std::cout << "reading replacement rules from rules.txt" << std::endl;
  RuleSet rules("rules.txt");

  for(fs::recursive_directory_iterator it(inDir), end; it != end; ++it)
  {
    if(!fs::is_regular_file(it->status()))
      continue;

    if(it->path().parent_path() == inDir)
      continue;

    std::cout << "processing file " << it->path() << std::endl;

    fs::path relative(makeRelative(inDir, it->path()));
    std::string line(rules.getLine(relative.string()));

    listFile << line << std::endl;
  }

  listFile.flush();

  return 0;
}
catch (const std::exception& e)
{
  std::cerr << "error: " << e.what() << "\n";
}

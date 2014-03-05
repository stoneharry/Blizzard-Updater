#include <iostream>
#include <string>
#include <iomanip>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <StormLib.h>

#include "util.hpp"

namespace fs = boost::filesystem;
namespace po = boost::program_options;

struct FileEntry
{
  FileEntry(fs::path pRealPath, fs::path pMpqPath)
    : realPath(pRealPath)
    , mpqPath(pMpqPath)
  {}

  fs::path realPath;
  fs::path mpqPath;
};


// copy & pasta
// from http://www.rosshemsley.co.uk/2011/02/creating-a-progress-bar-in-c-or-any-other-console-app/
static inline void loadbar(unsigned int x, unsigned int n, unsigned int w = 50)
{
  if (x > n) return;

  float ratio  =  n ? x/(float)n : 0;
  int   c      =  ratio * w;

  std::cout << std::setw(3) << x << "/" << n << " [";
  for (int x=0; x<c; x++) std::cout << "=";
  for (int x=c; x<w; x++) std::cout << " ";
  std::cout << "]\r" << std::flush;
}

int main(int argc, char** argv)
try
{
  po::options_description desc("Required options");
  desc.add_options()
      ("help,h", "produce help message")
      ("force,f", "overwrite existing files")
      ("mpq", po::value<std::string>(), "the mpq to create")
      ("files", po::value<std::vector<std::string> >(), "input files");

  po::positional_options_description p;

  p.add("mpq", 1);
  p.add("files", -1);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
  po::notify(vm);

  if (!vm.count("files") || !vm.count("mpq") || vm.count("help"))
  {
      std::cout << "usage: <mpq> [<files> ...]" << std::endl << std::endl
                << desc << std::endl;
      return 1;
  }

  std::vector<std::string> files(vm["files"].as< std::vector<std::string> >());
  std::vector<FileEntry> toAdd;
  fs::path mpqPath(vm["mpq"].as<std::string>());

  if(fs::exists(mpqPath))
  {
    if(vm.count("force"))
      fs::remove(mpqPath);
    else
      throw std::runtime_error("mpq does already exist");
  }

  for(std::vector<std::string>::iterator path = files.begin(); path != files.end(); ++path)
  {
    if(fs::is_regular_file(*path))
      toAdd.push_back(FileEntry(*path, *path));

    if(!fs::is_directory(*path)) //no symlinks etc
      continue;

    for(fs::recursive_directory_iterator file(*path), end; file != end; ++file)
      if(fs::is_regular_file(file->path()))
        toAdd.push_back(FileEntry(file->path(), makeRelative(*path, file->path())));
  }

  for(std::vector<FileEntry>::iterator it = toAdd.begin(); it != toAdd.end(); ++it)
    std::cout << it->realPath << " >> " << it->mpqPath << std::endl;

  HANDLE mpq;
  if(!SFileCreateArchive(mpqPath.string().c_str(), MPQ_CREATE_ARCHIVE_V2, toAdd.size(), &mpq))
    throw std::runtime_error("couldn't create mpq");

  SFileSetLocale(0);

  size_t counter(0);
  for(std::vector<FileEntry>::iterator it = toAdd.begin(); it != toAdd.end(); ++it)
  {
    loadbar(++counter, toAdd.size());

    if(!SFileAddFileEx(mpq, it->realPath.string().c_str(), it->mpqPath.string().c_str(), MPQ_FILE_COMPRESS, MPQ_COMPRESSION_BZIP2, MPQ_COMPRESSION_BZIP2))
      std::cout << "couldn't add file " << it->realPath << std::endl;
  }

  std::cout << std::endl;
  SFileCompactArchive(mpq, NULL, false);

  SFileFlushArchive(mpq);
  SFileCloseArchive(mpq);

  return 0;
}
catch (const std::exception& e)
{
  std::cerr << "error: " << e.what() << "\n";
}

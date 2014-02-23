#include <iostream>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/program_options.hpp>
#include <StormLib.h>

using namespace boost::filesystem;
namespace po = boost::program_options;

void addFile(HANDLE mpq, path file, std::string mpqPath)
{
  boost::algorithm::replace_all(mpqPath, "/", "\\");

  if(!SFileAddFileEx(mpq, file.string().c_str(), mpqPath.c_str(), MPQ_FILE_COMPRESS, MPQ_COMPRESSION_ZLIB, MPQ_COMPRESSION_ZLIB))
  {
    std::cout << "Error: " << GetLastError() << std::endl;
    return;
  }

  std::cout << "added file " << mpqPath << std::endl;
}

void addFile(HANDLE mpq, path file)
{
  addFile(mpq, file, file.string());
}

void addDir(HANDLE mpq, path dir)
{
  dir = absolute(dir);
  dir.make_preferred();

  size_t pos(dir.string().size() + 1);
  std::string preferredSlash = boost::filesystem::path("/").make_preferred().native();
  recursive_directory_iterator begin(dir), end;

  if(dir.native().find_last_of(preferredSlash) == pos-2)
    pos -= 1;

  BOOST_FOREACH(path const &p, std::make_pair(begin, end))
  {
    if(!is_regular_file(p)) continue;

    std::string mpqPath(p.string().substr(pos));
    addFile(mpq, p, mpqPath);
  }
}

int main(int argc, char** argv)
{
  int opt;
  po::options_description desc("Required options");
  desc.add_options()
      ("mpq", po::value<std::string>(), "the mpq to create")
      ("files", po::value<std::vector<std::string> >(), "input files")
  ;

  po::positional_options_description p;

  p.add("mpq", 1);
  p.add("files", -1);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
  po::notify(vm);

  if (!vm.count("files") || !vm.count("mpq"))
  {
      std::cout << desc << std::endl;
      return 1;
  }


  path mpqPath(vm["mpq"].as<std::string>());
  HANDLE mpq;

  SFileSetLocale(0);

  if(exists(mpqPath)) remove(mpqPath);
  if(!SFileCreateArchive(mpqPath.string().c_str(), MPQ_CREATE_ARCHIVE_V2, 8192, &mpq))
  {
    std::cout << "Error creating mpq: " << GetLastError() << std::endl;
    return 1;
  }


  std::vector<std::string> files(vm["files"].as< std::vector<std::string> >());

  for(std::vector<std::string>::iterator it = files.begin(); it != files.end(); ++it)
  {
    path filePath(*it);

    if(is_directory(filePath)) addDir(mpq, filePath);
    if(is_regular(filePath)) addFile(mpq, filePath);
  }

  std::cout << "compressing archive now" << std::endl;
  SFileCompactArchive(mpq, NULL, false);
  SFileFlushArchive(mpq);
  SFileCloseArchive(mpq);
}

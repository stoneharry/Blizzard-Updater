#include <vector>
#include <iostream>
#include <boost/crc.hpp>

#include "file.hpp"
#include "util.hpp"

struct UpdateFileHeader
{
  unsigned short HdrSize; // always 0x0018
  unsigned char version; // always 0x04
  unsigned char override; // 0x01 => override with new file; 0x04 => update file (bsdiff)
  unsigned int crc32; // of the file before update
  unsigned int OldFileSize; // file size before the update (uncompressed)
  unsigned int NewFileSize; // size of the entire file after the update is applied (uncompressed)
  unsigned int unknown6; // always 0
  unsigned int unknown7; // always 0
};

int main(int argc, char **argv)
try
{
  if(argc != 3)
    throw std::runtime_error ("no dir(s) given \n usage: <inDir> <outDir>");

  fs::path inDir(argv[1]);
  fs::path outDir(argv[2]);

  if(!fs::exists(inDir))
    throw std::runtime_error("inDir does not exist");

  for(fs::recursive_directory_iterator it(inDir), end; it != end; ++it)
  {
    if(!fs::is_regular_file(it->status()))
      continue;

    fs::path relative(makeRelative(inDir, it->path()));
    fs::path outFile(outDir / relative);

    file in(it->path().string(), file::read);

    std::vector<unsigned char> data(in.read_all());
    unsigned int crc = boost::crc<32, 0x04c11db7, 0xFFFF, 0, false, false>(data.data(), data.size());

    UpdateFileHeader header;
    header.HdrSize = 0x0018;
    header.NewFileSize = in.filesize();
    header.OldFileSize = header.NewFileSize;
    header.override = 0x01;
    header.crc32 = crc;
    header.unknown6 = 0;
    header.unknown7 = 0;
    header.version = 0x04;

    std::cout << "writing file " << outFile << std::endl;

    if(!fs::exists(outFile.parent_path()))
      fs::create_directories(outFile.parent_path());

    file out(outFile.string(), file::write);

    out.write_from(header);
    out.write_from(data);
  }

  return 0;
}
catch (const std::exception& e)
{
  std::cerr << "error: " << e.what() << "\n";
}

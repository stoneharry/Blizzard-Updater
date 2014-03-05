#include <zlib.hpp>
#include <file.hpp>
#include <bsni.hpp>

#include <stdexcept>
#include <iostream>
#include <cstring>
#include <string>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

int main(int argc, char **argv)
try
{
  if (argc <= 4)
  {
    throw std::runtime_error ("no filename(s) given (arguments: <base> <output> <mpq> <exe> [<files...>])");
  }

  const std::string base (argv[1]);
  const std::string output (argv[2]);
  const std::string mpq (argv[3]);

  std::cout << "using base " << base << ", writing to " << output << "\n";
  std::cout << "adding mpq " << mpq << "\n";

  file output_file (output, file::write);
  output_file.write_file (base);

  const int base_size (output_file.tell());

  const int entry_count (argc - 4);

  std::vector<unsigned char> entries (sizeof (BsnI_entry) * entry_count);

  for (int i (0); i < entry_count; ++i)
  {
    const fs::path name (argv[i+4]);
    std::cout << "adding " << name << "\n";

    if (name.string().size() >= 0x40)
    {
      std::cerr << "warning: filename " << name << " is longer than 0x3F characters and will be truncated!\n";
    }

    const std::vector<unsigned char> data (file (name.string(), file::read).read_all());
    const std::vector<unsigned char> deflated_data (zlib::deflate (data));

	const std::string filename(name.filename().string());
    BsnI_entry* entry (reinterpret_cast<BsnI_entry*> (&entries[sizeof(BsnI_entry)*i]));
    entry->data = output_file.tell();
    entry->inflated_data_size = data.size();
    entry->data_size = deflated_data.size();
	strncpy (entry->filename, filename.c_str(), 0x3F);
    entry->filename[0x3F] = '\0';

    output_file.write_from (deflated_data);
  }

  output_file.align (0x200);
  output_file.write_file (mpq);

  //! \todo SIGN is missing! {'SIGN', char[0x100] signature}
  output_file.write_from (std::vector<unsigned char> (0x104));

  BsnI table;
  table.base = base_size;
  table.entry_count = entry_count;
  table.entries = output_file.tell();
  table.magic = 0x496E7342;

  output_file.write_from (entries);
  output_file.write_from (table);

  return 0;
}
catch (const std::exception& e)
{
  std::cerr << "error: " << e.what() << "\n";
}

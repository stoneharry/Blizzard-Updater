#include <file.hpp>
#include <bsni.hpp>
#include <zlib.hpp>

#include <vector>
#include <iostream>

void extract_entry (const file& input_file, struct BsnI_entry* entry)
{
  std::cout << "extracting: " << entry->filename << "\n";

  std::vector<unsigned char> input_data;
  input_file.seek (entry->data);
  input_file.read_into (&input_data, entry->data_size);

  std::vector<unsigned char> output_data (entry->inflated_data_size);
  zlib::inflate (input_data, &output_data);

  file output (entry->filename, file::write);
  output.write_from (output_data);
}

int main(int argc, char **argv)
try
{
  if (argc != 2)
  {
    throw std::runtime_error ("no filename given");
  }

  const std::string input_filename (argv[1]);

  file input_file (input_filename, file::read);
  input_file.seek (-static_cast<int> (sizeof (BsnI)));

  std::vector<unsigned char> table_vec (sizeof (BsnI));
  BsnI* table (reinterpret_cast<BsnI*> (&table_vec[0]));
  table->magic = 'slpf';

  int pos (input_file.tell());
  while (table->magic != 'InsB' && pos != 0)
  {
    input_file.read_into (&table_vec, sizeof (BsnI));
    input_file.seek (pos = pos - 1);
  }
  if (pos == 0)
  {
    throw std::runtime_error ("'InsB' not found\n");
  }

  for (int i (0); i < table->entry_count; ++i)
  {
    BsnI_entry entry;
    input_file.seek (table->entries + sizeof (BsnI_entry) * i);
    input_file.read_into (&entry, sizeof (BsnI_entry));
    extract_entry (input_file, &entry);
  }

  std::cout << "extracting: base.exe" << "\n";

  file output("base.exe", file::write);
  std::vector<unsigned char> base;

  input_file.seek(0);
  input_file.read_into(&base, table->base);
  output.write_from (base);

  return 0;
}
catch (const std::exception& e)
{
  std::cerr << "error: " << e.what() << "\n";
}

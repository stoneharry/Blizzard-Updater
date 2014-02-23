#ifndef BSNI_HPP
#define BSNI_HPP

struct BsnI_entry
{
  unsigned int data; // data is deflated using zlib 1.2.3
  unsigned int data_size;
  unsigned int inflated_data_size;
  char filename[0x40];
};

struct BsnI
{
  unsigned int base;
  unsigned int entry_count;
  unsigned int entries;
  unsigned int magic; // 0x496E7342 'BsnI'
};

// Well, BsnI itself is only a pointer with a count. At that pointer, there is a list of count items with pointer to deflated data with a given size, which inflates to a given size, as well as a filename where that data will be written to. Two specific DLLs (RichEd20.dll and Unicows.dll) are actually ignored if GetVersionExA(&VersionInformation) && VersionInformation.dwPlatformId >= 2. The last extracted executable is then executed using \"%s\" %s --path=\"%s\"", last_extracted_executable, original_command_line, this_application_name.

#endif

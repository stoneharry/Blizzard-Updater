#ifndef FILE_HPP
#define FILE_HPP

#include <string>
#include <cstdlib>
#include <cstdio>
#include <stdexcept>
#include <vector>

class file
{
public:
  enum open_flag
  {
    read = 1,
    write = 2,
  };

  static const char* to_string (const open_flag& flag)
  {
    if (flag == read)
    {
      return "rb";
    }
    else if (flag == write)
    {
      return "wb";
    }
    else if (flag == (read|write))
    {
      return "rwb";
    }
    else
    {
      throw std::runtime_error ("bad opening flags");
    }
  }

  file (const std::string& filename, const open_flag& flag)
    : _handle (fopen (filename.c_str(), to_string (flag)))
  {
    if (!_handle)
    {
      throw std::runtime_error ("unable to open file");
    }
  }
  ~file()
  {
    fclose (_handle);
  }

  void seek (const int& pos) const
  {
    if (pos < 0)
    {
      fseek (_handle, -pos, SEEK_END);
    }
    else
    {
      fseek (_handle, pos, SEEK_SET);
    }
  }
  void skip (const int& bytes) const
  {
    fseek (_handle, bytes, SEEK_CUR);
  }
  int tell() const
  {
    return ftell (_handle);
  }
  int filesize() const
  {
    const int pos (tell());
    fseek (_handle, 0, SEEK_END);
    const int size (tell());
    seek (pos);
    return size;
  }

  void read_into (std::vector<unsigned char>* target, const int& length) const
  {
    target->resize (length);
    fread (&(*target)[0], length, 1, _handle);
  }
  void read_into (std::vector<unsigned char>* target) const
  {
    target->resize (target->size());
    fread (&(*target)[0], target->size(), 1, _handle);
  }
  template<typename T>
  void read_into (T* target, const int& length) const
  {
    fread (target, length, 1, _handle);
  }

  std::vector<unsigned char> read_all() const
  {
    const int size (filesize());
    std::vector<unsigned char> data;
    const int pos (tell());
    seek (0);
    read_into (&data, size);
    seek (pos);
    return data;
  }

  void write_from (const std::vector<unsigned char>& data) const
  {
    fwrite (&data[0], data.size(), 1, _handle);
  }
  void write_file (const std::string& filename) const
  {
    write_from (file (filename, read).read_all());
  }
  template<typename T>
  void write_from (const T& data) const
  {
    fwrite (&data, sizeof (T), 1, _handle);
  }

  void align (const int& alignment) const
  {
    write_from (std::vector<unsigned char> (alignment - (tell() % alignment)));
  }

private:
  FILE* _handle;
};

#endif

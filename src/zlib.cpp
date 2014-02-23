#include <zlib.hpp>
#include <iostream>
#include <zlib.h>
#include <stdexcept>

namespace
{
  int wrap_inflate ( unsigned char* output, int size_output
                   , const unsigned char* input, int size_input
                   , int* written_output
                   )
  {
    z_stream stream;

    stream.zalloc = NULL;
    stream.zfree = NULL;
    stream.next_in = const_cast<Bytef*> (input);
    stream.avail_in = size_input;
    stream.next_out = output;
    stream.avail_out = size_output;

    int result = inflateInit (&stream);
    if (result)
    {
      return result;
    }
    int inflateResult = inflate (&stream, 4);
    if (inflateResult == 1)
    {
      *written_output = stream.total_out;
      return inflateEnd (&stream);
    }
    else
    {
      inflateEnd (&stream);
      if (inflateResult != 2 && (inflateResult != -5 || stream.avail_in))
      {
        return inflateResult;
      }
      return -3; // this is being returned
    }
  }

  int wrap_deflate ( unsigned char* output, int size_output
                   , const unsigned char* input, int size_input
                   , unsigned int* written_output
                   )
  {
    z_stream stream;

    stream.zalloc = NULL;
    stream.zfree = NULL;
    stream.next_in = const_cast<Bytef*> (input);
    stream.avail_in = size_input;
    stream.next_out = output;
    stream.avail_out = size_output;

    int result = deflateInit (&stream, Z_DEFAULT_COMPRESSION);
    if (result)
    {
      return result;
    }
    int deflateResult = deflate (&stream, 4);
    if (deflateResult != Z_STREAM_ERROR)
    {
      *written_output = size_output - stream.avail_out;
      return deflateEnd (&stream);
    }
    else
    {
      deflateEnd (&stream);
      if (deflateResult != 2 && (deflateResult != -5 || stream.avail_in))
      {
        return deflateResult;
      }
      return -3;
    }
  }
}

namespace zlib
{
  void inflate (const std::vector<unsigned char>& input, std::vector<unsigned char>* output)
  {
    int inflated;
    const int res (wrap_inflate (&(*output)[0], output->size(), &input[0], input.size(), &inflated));
    if (res || inflated != output->size())
    {
      throw std::runtime_error ("error: not inflated correctly");
    }
  }

  std::vector<unsigned char> deflate (const std::vector<unsigned char>& data)
  {
    std::vector<unsigned char> output (data.size());
    unsigned int deflated_size (0);
    if (wrap_deflate (&output[0], output.size(), &data[0], data.size(), &deflated_size))
    {
      throw std::runtime_error ("deflating failed");
    }
    output.resize (deflated_size);
    return output;
  }
}

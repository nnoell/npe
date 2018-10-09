#pragma once

// Windows
#include <windows.h>

// STL
#include <string>

// NPE PIPE public
#include <npe/pipe/buffer/ptr.hpp>
#include <npe/pipe/buffer/data.hpp>

namespace npe {
namespace pipe {

// The pipe class
class Buffer final {
 public:
  // Constructor with empty data
  Buffer(DWORD allocated_size);

  // Constructs with raw data
  Buffer(buffer::Data data, DWORD size);

  // Constructor with string data
  Buffer(const std::string& data);

  // Destructor
  ~Buffer();

  // Gets the buffer data
  BYTE *GetData();

  // Gets the buffer allocated size
  DWORD GetAllocatedSize();

  // Gets the buffer size
  DWORD GetSize();

  // Sets the buffer size
  void SetSize(DWORD size);

  // Retruns a string representing the buffer data
  std::string ToString();

 private:
  // Copy Constructor
  Buffer(const Buffer&) = delete;

  // Move Constructor
  Buffer(Buffer &&) = delete;

  // Copy-Assign Constructor
  Buffer& operator=(const Buffer&) = delete;

  // Move-Assign Constructr
  Buffer& operator=(Buffer &&) = delete;

 private:
  // The buffer size
  DWORD size_;

  // The buffer allocated size
  DWORD allocated_size_;

  // The buffer data
  buffer::Data data_;
};

}  // namespace pipe
}  // namespace npe


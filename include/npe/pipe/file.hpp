#pragma once

// Windows
#include <windows.h>

// STL
#include <string>
#include <functional>

// NPE PIPE public
#include <npe/pipe/buffer.hpp>

// Default 
#define NPE_PIPE_FILE_DEFAULT_NAME "\\\\.\\pipe\\npe_pipe_file"

namespace npe {
namespace pipe {

// The file class
class File {
 public:
  // Constructor with handler
  explicit File(std::shared_ptr<void> handle);

  // Constructor with name
  explicit File(const std::string& name);

  // Destructor
  virtual ~File();

  // Check if file is Ok
  bool IsOk();

  // Sends data
  bool SendBuffer(buffer::Ptr buffer);

  // Receives data
  bool ReceiveBuffer(buffer::Ptr& buffer);

  // Sends a buffer and receives a response buffer
  bool SendReceiveBuffer(buffer::Ptr in, buffer::Ptr& out);

 private:
  // Copy Constructor
  File(const File&) = delete;

  // Move Constructor
  File(File &&) = delete;

  // Copy-Assign Constructor
  File& operator=(const File&) = delete;

  // Move-Assign Constructr
  File& operator=(File &&) = delete;

 protected:
  // The handle
  std::shared_ptr<void> handle_;

 private:
  // The number of bytes read
  DWORD bytes_read_;

  // The number of bytes written
  DWORD bytes_written_;
};

}  // namespace pipe
}  // namespace npe


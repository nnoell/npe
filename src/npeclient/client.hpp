#pragma once

// NPE PIPE public
#include <npe/pipe/file.hpp>

// NPE CLIENT private
#include "command.hpp"

// Default response buffer size in bytes
#define NPE_CLIENT_CLIENT_REQUEST_MAX_SIZE (1024 * 1024)  // 1MB

namespace npe {
namespace client {

// The Client class
class Client final {
 public:
  // Constructor
  explicit Client(const std::string& pipe_name, DWORD response_max_size = NPE_CLIENT_CLIENT_REQUEST_MAX_SIZE);

  // Destructor
  ~Client();

  // Runs the client
  void Run();

 private:
  // Sends a request and receives and waits for a response if the request is in sync mode, otherwise returns null
  pipe::buffer::Ptr SendRequest(pipe::buffer::Ptr request);

  // Processes a response
  void ProcessResponse(pipe::buffer::Ptr response);

  // Processes an Ok response
  void ProcessResponseOk(pipe::buffer::Ptr response, BYTE from);

  // Processes an Error response
  void ProcessResponseError(pipe::buffer::Ptr response, BYTE from);

 private:
  // Copy Constructor
  Client(const Client&) = delete;

  // Move Constructor
  Client(Client &&) = delete;

  // Copy-Assign Constructor
  Client& operator=(const Client&) = delete;

  // Move-Assign Constructr
  Client& operator=(Client &&) = delete;

 private:
  // The command object
  Command command_;

  // The pipe to receive and send data
  pipe::File pipe_;

  // The max response size
  const DWORD response_max_size_;
};

}  // namespace client
}  // namespace npe


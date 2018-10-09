#pragma once

// NPE PIPE public
#include <npe/pipe/pipe.hpp>

// NPE SERVER private
#include "state.hpp"

// Default request buffer size in bytes
#define NPE_SERVER_SERVER_REQUEST_MAX_SIZE (1024 * 1024)  // 1MB

namespace npe {
namespace server {

// The Server class
class Server final {
 public:
  // Constructor
  explicit Server(const std::string& pipe_name, DWORD request_max_size = NPE_SERVER_SERVER_REQUEST_MAX_SIZE);

  // Destructor
  ~Server();

  // Runs the server
  void Run();

 private:
  // Processes the request and generates the appropiate response
  pipe::buffer::Ptr ProcessRequest(const pipe::buffer::Ptr& request);

  // Processes a Last request
  pipe::buffer::Ptr ProcessRequestLast(const pipe::buffer::Ptr& request);

  // Processes a List request
  pipe::buffer::Ptr ProcessRequestList(const pipe::buffer::Ptr& request);

  // Processes a Construct request
  pipe::buffer::Ptr ProcessRequestConstruct(const pipe::buffer::Ptr& request);

  // Processes a Retrieve request
  pipe::buffer::Ptr ProcessRequestRetrieve(const pipe::buffer::Ptr& request);

  // Processes a Call request
  pipe::buffer::Ptr ProcessRequestCall(const pipe::buffer::Ptr& request);

  // Processes a Destroy request
  pipe::buffer::Ptr ProcessRequestDestroy(const pipe::buffer::Ptr& request);

  // Processes an Ascii request
  pipe::buffer::Ptr ProcessRequestAscii(const pipe::buffer::Ptr& request);

  // Processes a Primitive request
  template <typename T>
  pipe::buffer::Ptr ProcessRequestPrimitive(BYTE type, const pipe::buffer::Ptr& request) {
    // Parse the request
    T value;
    if (!pipe::request::ParsePrimitive<T>(request->GetData(), request->GetSize(), value))
      throw std::runtime_error("Could not parse primitive request");
  
    // Process the request
    std::cout << value << std::endl;
    return pipe::response::CreateOk(nullptr, 0, type);
  }

 private:
  // Copy Constructor
  Server(const Server&) = delete;

  // Move Constructor
  Server(Server &&) = delete;

  // Copy-Assign Constructor
  Server& operator=(const Server&) = delete;

  // Move-Assign Constructr
  Server& operator=(Server &&) = delete;

 private:
  // The pipe to receive and send data
  pipe::Pipe pipe_;

  // The state of the server
  State state_;

  // The max request size
  const DWORD request_max_size_;

  // The last response
  pipe::buffer::Ptr last_response_;
};

}  // namespace server
}  // namespace npe


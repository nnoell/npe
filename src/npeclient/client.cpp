// STL
#include <iostream>
#include <unordered_map>
#include <sstream>

// NPE PIPE public
#include <npe/pipe/request.hpp>
#include <npe/pipe/response.hpp>

// NPE CLIENT private
#include "client.hpp"

namespace npe {
namespace client {

Client::Client(const std::string& pipe_name, DWORD response_max_size) :
    command_(),
    pipe_(pipe_name),
    response_max_size_(response_max_size) {
}

Client::~Client() {
}

void Client::Run() {
  while (!command_.Quit() && pipe_.IsOk()) {
    // Read a command from cin
    std::cout << ">> ";
    command_.Read();

    // Execute the command
    pipe::buffer::Ptr request = command_.Execute();
    if (!request)
      continue;

    // Send the request
    pipe::buffer::Ptr response = SendRequest(std::move(request));
    if (!response)
      continue;

    // Process the response
    ProcessResponse(std::move(response));
  }
}

pipe::buffer::Ptr Client::SendRequest(pipe::buffer::Ptr request) {
  if (!request)
    throw std::runtime_error("Cannot send null request");

  // Parse the request sync
  bool sync = true;
  if (!pipe::RequestParseSync(request->GetData(), request->GetSize(), sync))
    throw std::runtime_error("Could not parse request sync");

  // Send async request if sync is false
  if (!sync) {
    if (!pipe_.SendBuffer(std::move(request)))
      throw std::runtime_error("Could not send async request");
    return nullptr;
  }

  // Otherwise, send sync request
  pipe::buffer::Ptr response = std::make_shared<pipe::Buffer>(response_max_size_);
  if (!pipe_.SendReceiveBuffer(std::move(request), response))
    throw std::runtime_error("Could not send sync request");
  return response;
}

void Client::ProcessResponse(pipe::buffer::Ptr response) {
  if (!response)
    throw std::runtime_error("Cannot process null response");

  // Get the response type
  BYTE response_type = 0;
  if (!pipe::ResponseParseType(response->GetData(), response->GetSize(), response_type)) {
    std::cerr << "Failed to parse the response type" << std::endl;
    return;
  }

  // Map the request type with the process members
  using ProcessMap = std::unordered_map<BYTE, std::function<void(pipe::buffer::Ptr, BYTE)>>;
  static const ProcessMap process_map {
    {pipe::response::T_Error, std::bind(&Client::ProcessResponseError, this, std::placeholders::_1, std::placeholders::_2)},
    {pipe::response::T_Ok, std::bind(&Client::ProcessResponseOk, this, std::placeholders::_1, std::placeholders::_2)}
  };

  // Find the process member
  ProcessMap::const_iterator it = process_map.find(response_type);
  if (it == process_map.end()) {
    std::cerr << "Response type not recognised" << std::endl;
    return;
  }

  // Parse the from
  BYTE from = -1;
  if (!pipe::ResponseParseFrom(response->GetData(), response->GetSize(), from)) {
    std::cerr << "Could not parse response 'from' value" << std::endl;
    return;
  }

  // Process the request
  try {
    it->second(response, from);
  } catch (std::runtime_error e) {
    std::cerr << ("Failed to process the response: " + std::string(e.what())) << std::endl;
  } catch (...) {
    std::cerr << "Failed to process the response" << std::endl;
  }
}

void Client::ProcessResponseOk(pipe::buffer::Ptr response, BYTE from) {
  // Parse the response
  const void *args = nullptr;
  std::size_t args_size = 0;
  if (!pipe::response::ParseOk(response->GetData(), response->GetSize(), args, args_size))
    std::runtime_error("Could not parse Ok response");

  // Process the response
  std::stringstream ss;
  ss << "OK (from '" << from << "'): args=" << args << ", args_size=" << args_size << std::endl;

  // Done if args are empty
  if (args == nullptr || args_size == 0) {
    std::cout << ss.str();
    return;
  }

  // Print the output for the different request types
  ss << "Output: "; 
  if (from == pipe::request::T_Construct)
    ss << *reinterpret_cast<const pipe::Id *>(args);
  else if (from == pipe::request::T_Call) {
    ss << command_.ArgsToString(args, args_size);
  } else {
    ss << pipe::buffer::DataToString(reinterpret_cast<const BYTE *>(args), args_size);
  }

  std::cout << ss.str() << std::endl;
}

void Client::ProcessResponseError(pipe::buffer::Ptr response, BYTE from) {
  // Parse the response
  std::string ascii = "";
  if (!pipe::response::ParseError(response->GetData(), response->GetSize(), ascii))
    std::runtime_error("Could not parse Ok response");

  // Process the response
  std::cout << "ERROR (from '" << from << "'): " << ascii << std::endl;
}

}  // namespace client
}  // namespace npe

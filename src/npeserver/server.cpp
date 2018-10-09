// STL
#include <iostream>
#include <unordered_map>

// NPE PIPE public
#include <npe/pipe/request.hpp>
#include <npe/pipe/response.hpp>

// NPE SERVER public
#include <npe/server/game.hpp>

// NPE SERVER private
#include "server.hpp"
#include "object.hpp"

namespace npe {
namespace server {

Server::Server(const std::string& pipe_name, DWORD request_max_size) :
    pipe_(pipe_name),
    state_(),
    request_max_size_(request_max_size),
    last_response_(pipe::response::CreateOk()) {
  // Wait for the client to connect
  if (!pipe_.Connect())
    throw std::runtime_error("Error while connecting to client");

  std::cout << ">> Client connected!" << std::endl;
}

void Server::Run() {
  // The buffer to receive the request
  pipe::buffer::Ptr request = std::make_shared<pipe::Buffer>(request_max_size_);

  while (pipe_.IsOk() && pipe_.IsConnected()) {
    // Receive the request
    std::cout << ">> Waiting for request..." << std::endl;
    if (!pipe_.ReceiveBuffer(request)) {
      std::cerr << ">> Failed to receive request, skipping..." << std::endl;
      continue;
    }
    std::cout << ">> Request received!" << std::endl;

    // Process the request
    last_response_ = ProcessRequest(request);
    std::cout << ">> Request processed!" << std::endl;

    // Check if it is a sync request
    bool sync = true;
    if (!pipe::RequestParseSync(request->GetData(), request->GetSize(), sync))
      std::cerr << ">> Failed to parse sync value, sending sync response..." << std::endl;

    // Send the response only if it is a sync request
    if (sync) {
      if (!pipe_.SendBuffer(last_response_)) {
        std::cerr << ">> Failed to send response, skipping..." << std::endl;
        continue;
      }
      std::cout << ">> Response provided!" << std::endl;
    }
  }
}

pipe::buffer::Ptr Server::ProcessRequest(const pipe::buffer::Ptr& request) {
  // Get the request type
  BYTE request_type = 0;
  if (!pipe::RequestParseType(request->GetData(), request->GetSize(), request_type))
    return pipe::response::CreateError("Failed to parse the request type");

  // Map the request type with the process members
  using ProcessMap = std::unordered_map<BYTE, std::function<pipe::buffer::Ptr(pipe::buffer::Ptr)>>;
  static const ProcessMap process_map {
    {pipe::request::T_Last, std::bind(&Server::ProcessRequestLast, this, std::placeholders::_1)},
    {pipe::request::T_List, std::bind(&Server::ProcessRequestList, this, std::placeholders::_1)},
    {pipe::request::T_Construct, std::bind(&Server::ProcessRequestConstruct, this, std::placeholders::_1)},
    {pipe::request::T_Retrieve, std::bind(&Server::ProcessRequestRetrieve, this, std::placeholders::_1)},
    {pipe::request::T_Call, std::bind(&Server::ProcessRequestCall, this, std::placeholders::_1)},
    {pipe::request::T_Destroy, std::bind(&Server::ProcessRequestDestroy, this, std::placeholders::_1)},
    {pipe::request::T_Ascii, std::bind(&Server::ProcessRequestAscii, this, std::placeholders::_1)},
    {pipe::request::T_Bool, std::bind(&Server::ProcessRequestPrimitive<bool>, this, pipe::request::T_Bool, std::placeholders::_1)},
    {pipe::request::T_Int, std::bind(&Server::ProcessRequestPrimitive<int>, this, pipe::request::T_Int, std::placeholders::_1)},
    {pipe::request::T_Char, std::bind(&Server::ProcessRequestPrimitive<char>, this, pipe::request::T_Char, std::placeholders::_1)},
    {pipe::request::T_Id, std::bind(&Server::ProcessRequestPrimitive<pipe::Id>, this, pipe::request::T_Id, std::placeholders::_1)},
    {pipe::request::T_Double, std::bind(&Server::ProcessRequestPrimitive<double>, this, pipe::request::T_Double, std::placeholders::_1)}
  };

  // Find the process member
  ProcessMap::const_iterator it = process_map.find(request_type);
  if (it == process_map.end())
    return pipe::response::CreateError("Request type not recognised");

  // Process the request
  try {
    return it->second(request);
  } catch (std::runtime_error e) {
    return pipe::response::CreateError("Failed to process the request: " + std::string(e.what()), request_type);
  } catch (...) {
    return pipe::response::CreateError("Failed to process the request", request_type);
  }
}

pipe::buffer::Ptr Server::ProcessRequestLast(const pipe::buffer::Ptr& request) {
  (void)request;
  return last_response_;
}

pipe::buffer::Ptr Server::ProcessRequestList(const pipe::buffer::Ptr& request) {
  (void)request;
  std::string list = state_.ToStringObjects();
  return pipe::response::CreateOk(list.c_str(), list.size(), pipe::request::T_List);
}

pipe::buffer::Ptr Server::ProcessRequestConstruct(const pipe::buffer::Ptr& request) {
  // Parse the request
  pipe::Id object_type = 0;
  const void *args = nullptr;
  std::size_t args_size = 0;
  if (!pipe::request::ParseConstruct(request->GetData(), request->GetSize(), object_type, args, args_size))
    throw std::runtime_error("Could not parse Construct request");

  // Process the request
  pipe::Id id = state_.CreateObject(object_type, args, args_size);
  return pipe::response::CreateOk(reinterpret_cast<BYTE *>(&id), sizeof(pipe::Id), pipe::request::T_Construct);
}

pipe::buffer::Ptr Server::ProcessRequestRetrieve(const pipe::buffer::Ptr& request) {
  // Parse the request
  pipe::Id object_id = 0;
  if (!pipe::request::ParseRetrieve(request->GetData(), request->GetSize(), object_id))
    return pipe::response::CreateError("Could not parse Destroy request");

  // Process the request
  std::string obj_str = state_.ToStringObject(object_id);
  return pipe::response::CreateOk(obj_str.c_str(), obj_str.size(), pipe::request::T_Retrieve);
}

pipe::buffer::Ptr Server::ProcessRequestCall(const pipe::buffer::Ptr& request) {
  // Parse the request
  pipe::Id object_id = 0;
  pipe::Id method_id = 0;
  void *args = nullptr;
  std::size_t args_size = 0;
  if (!pipe::request::ParseCall(request->GetData(), request->GetSize(), object_id, method_id, args, args_size))
    return pipe::response::CreateError("Could not parse Call request");

  // Process the request
  state_.CallObject(object_id, method_id, args, args_size);
  return pipe::response::CreateOk(args, args_size, pipe::request::T_Call);
}

pipe::buffer::Ptr Server::ProcessRequestDestroy(const pipe::buffer::Ptr& request) {
  // Parse the request
  pipe::Id object_id = 0;
  if (!pipe::request::ParseDestroy(request->GetData(), request->GetSize(), object_id))
    return pipe::response::CreateError("Could not parse Destroy request");

  // Process the request
  state_.DestroyObject(object_id);
  std::string ascii = "Destroyed";
  return pipe::response::CreateOk(ascii.c_str(), ascii.size(), pipe::request::T_Destroy);
}

pipe::buffer::Ptr Server::ProcessRequestAscii(const pipe::buffer::Ptr& request) {
  // Parse the request
  std::string ascii = "";
  if (!pipe::request::ParseAscii(request->GetData(), request->GetSize(), ascii))
    throw std::runtime_error("Could not parse Ascii request");
  
  // Process the request
  std::cout << ascii << std::endl;
  return pipe::response::CreateOk(nullptr, 0, pipe::request::T_Ascii);
}

Server::~Server() {
  // Disconnect from the client
  pipe_.Disconnect();
}

}  // namespace server
}  // namespace npe

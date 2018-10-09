// Windows
#include <windows.h>

// STL
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <functional>

// NPE PIPE public
#include <npe/pipe/request.hpp>

// NPE CLIENT private
#include "command.hpp"
#include "command/args.hpp"
#include "command/args/ptr.hpp"

namespace npe {
namespace client {

Command::Command() :
  tokens_(),
  last_args_(nullptr),
  quit_(false) {
}

Command::~Command() {
}

void Command::Read() {
  // Read the commnad
  std::string command;
  getline(std::cin, command);

  // Tokenize the command
  tokens_.clear();
  std::string token;
  std::istringstream is(command);
  while (is >> token)
    tokens_.push_back(token);
}

bool Command::Quit() const {
  return quit_;
}

std::string Command::ArgsToString(const void *args, std::size_t args_size) const {
  return last_args_->BufferToString(args, args_size);
}

pipe::buffer::Ptr Command::Execute() {
  // Return if there are no tokens
  if (tokens_.size() <= 0)
    return nullptr;

  // Return if the first token's size is not 1
  if (tokens_[0].size() != 1)
    return nullptr;

  // Command parser map
  using CommandExecuterMap = std::unordered_map<BYTE, std::function<pipe::buffer::Ptr()>>;
  static const CommandExecuterMap command_executer_map {
    {'h', std::bind(&Command::ExecuteHelp, this)},
    {'q', std::bind(&Command::ExecuteQuit, this)},
    {'v', std::bind(&Command::ExecuteVersion, this)},
    {'r', std::bind(&Command::ExecuteRequest, this)}
  };

  // Find the specific command executer in the map
  CommandExecuterMap::const_iterator it = command_executer_map.find(tokens_[0][0]);
  if (it == command_executer_map.end()) {
    std::cout << "Command not found. Type 'h' for help" << std::endl;
    return nullptr;
  }

  // Execute the specific command
  return it->second();
}

pipe::buffer::Ptr Command::ExecuteHelp() {
  std::stringstream ss;
  ss << "Usage: [h (help) | q (quit) | v (version) | r (request)]" << std::endl;
  ss << " h: Prints the help" << std::endl;
  ss << " q: Exits the client" << std::endl;
  ss << " v: Shows the version" << std::endl;
  ss << " r: Sends a request to the server" << std::endl;
  ss << "Request command usage: r <type> <sync=1|async=0> [param1] ... [paramN]" << std::endl;
  ss << " - last:      r 0 <1|0>" << std::endl;
  ss << " - list:      r 1 <1|0>" << std::endl;
  ss << " - construct: r 2 <1|0> <object_name> [arg1 t=v] ... [argN t=v]" << std::endl;
  ss << " - request:   r 3 <1|0> <object_id>" << std::endl;
  ss << " - call:      r 4 <1|0> <object_id> <method_id> [args t=v] ... [argN t=v]" << std::endl;
  ss << " - destroy:   r 5 <1|0> <object_id>" << std::endl;
  ss << " - ascii:     r 6 <1|0> <ascii_value>" << std::endl;
  ss << " - bool:      r 7 <1|0> <bool_value>" << std::endl;
  ss << " - int:       r 8 <1|0> <int_value>" << std::endl;
  ss << " - char:      r 9 <1|0> <char_value>" << std::endl;
  ss << " - id:        r a <1|0> <id_value>" << std::endl;
  ss << " - double:    r b <1|0> <double_value>" << std::endl;
  std::cout << ss.str();
  return nullptr;
}

pipe::buffer::Ptr Command::ExecuteQuit() {
  quit_ = true;
  return nullptr;
}

pipe::buffer::Ptr Command::ExecuteVersion() {
  std::cout << NPE_VERSION << std::endl;
  return nullptr;
}

pipe::buffer::Ptr Command::ExecuteRequest() {
  // Return if there are less than 3 tokens
  if (tokens_.size() < 3) {
     std::cout << "Invalid request command. Type 'h' for help" << std::endl;
     return nullptr;
  }

  // Return if the second and third token's size are not 1
  if (tokens_[1].size() != 1 || tokens_[2].size() != 1) {
    std::cout << "Invalid request command. Type 'h' for help" << std::endl;
    return nullptr;
  }

  // Command parser map
  using CommandExecuterRequestMap = std::unordered_map<BYTE, std::function<pipe::buffer::Ptr(bool sync)>>;
  static const CommandExecuterRequestMap command_executer_request_map {
    {pipe::request::T_Last, std::bind(&Command::ExecuteRequestLast, this, std::placeholders::_1)},
    {pipe::request::T_List, std::bind(&Command::ExecuteRequestList, this, std::placeholders::_1)},
    {pipe::request::T_Construct, std::bind(&Command::ExecuteRequestConstruct, this, std::placeholders::_1)},
    {pipe::request::T_Retrieve, std::bind(&Command::ExecuteRequestRetrieve, this, std::placeholders::_1)},
    {pipe::request::T_Call, std::bind(&Command::ExecuteRequestCall, this, std::placeholders::_1)},
    {pipe::request::T_Destroy, std::bind(&Command::ExecuteRequestDestroy, this, std::placeholders::_1)},
    {pipe::request::T_Ascii, std::bind(&Command::ExecuteRequestAscii, this, std::placeholders::_1)},
    {pipe::request::T_Bool, std::bind(&Command::ExecuteRequestPrimitive<bool>, this, pipe::request::T_Bool, std::placeholders::_1)},
    {pipe::request::T_Int, std::bind(&Command::ExecuteRequestPrimitive<int>, this, pipe::request::T_Int, std::placeholders::_1)},
    {pipe::request::T_Char, std::bind(&Command::ExecuteRequestPrimitive<char>, this, pipe::request::T_Char, std::placeholders::_1)},
    {pipe::request::T_Id, std::bind(&Command::ExecuteRequestPrimitive<pipe::Id>, this, pipe::request::T_Id, std::placeholders::_1)},
    {pipe::request::T_Double, std::bind(&Command::ExecuteRequestPrimitive<double>, this, pipe::request::T_Double, std::placeholders::_1)}
  };

  // Find the specific command parser in the map
  const BYTE type = tokens_[1][0];
  CommandExecuterRequestMap::const_iterator it = command_executer_request_map.find(type);
  if (it == command_executer_request_map.end()) {
     std::cout << "Invalid request command. Type 'h' for help" << std::endl;
     return nullptr;
  }

  // Get the sync value
  bool sync = false;
  if (tokens_[2][0] != '0')
    sync = true;

  // Execute the specific request command
  return it->second(sync);
}

pipe::buffer::Ptr Command::ExecuteRequestLast(bool sync) {
  return pipe::request::CreateLast(sync);
}

pipe::buffer::Ptr Command::ExecuteRequestList(bool sync) {
  return pipe::request::CreateList(sync);
}

pipe::buffer::Ptr Command::ExecuteRequestConstruct(bool sync) {
  if (tokens_.size() <= 3) {
    std::cout << "Construct request cannot be sent without an object type" << std::endl;
    return nullptr;
  }

  // Construct without args
  if (tokens_.size() <= 4)
    return pipe::request::CreateConstruct(tokens_[3], nullptr, 0, sync);

  // Get the args vector
  std::vector<std::string> args_vector(tokens_.begin() + 4, tokens_.end());

  // Parse the args
  last_args_ = std::make_shared<command::Args>(std::move(args_vector));
  pipe::buffer::Ptr args_buffer = last_args_->Parse();
  if (!args_buffer) {
    std::cout << "Args format is not valid, they must be a type/value pair: type=value" << std::endl;
    last_args_ = nullptr;
    return nullptr;
  }

  // Return the Construct request
  return pipe::request::CreateConstruct(tokens_[3], args_buffer->GetData(), args_buffer->GetSize(), sync);
}

pipe::buffer::Ptr Command::ExecuteRequestRetrieve(bool sync) {
  if (tokens_.size() <= 3) {
    std::cout << "Retrieve request cannot be sent without an object Id" << std::endl;
    return nullptr;
  }

  // Parse the object Id
  std::stringstream sstream(tokens_[3]);
  std::size_t object_id = 0;
  sstream >> object_id;

  // Return the Retrieve request
  return pipe::request::CreateRetrieve(object_id, sync);
}

pipe::buffer::Ptr Command::ExecuteRequestCall(bool sync) {
  if (tokens_.size() <= 4) {
    std::cout << "Call request cannot be sent without an object Id or method Id" << std::endl;
    return nullptr;
  }

  // Parse the object Id
  std::stringstream sstream(tokens_[3]);
  std::size_t object_id = 0;
  sstream >> object_id;
  

  // Call without args
  if (tokens_.size() <= 5)
    return pipe::request::CreateCall(object_id, tokens_[4], nullptr, 0, sync);

  // Get the args vector
  std::vector<std::string> args_vector(tokens_.begin() + 5, tokens_.end());

  
  // Parse the args
  last_args_ = std::make_shared<command::Args>(std::move(args_vector));
  pipe::buffer::Ptr args_buffer = last_args_->Parse();
  if (!args_buffer) {
    std::cout << "Args format is not valid, they must be a type/value pair: type=value" << std::endl;
    last_args_ = nullptr;
    return nullptr;
  }

  return pipe::request::CreateCall(object_id, tokens_[4], args_buffer->GetData(), args_buffer->GetSize(), sync);
}

pipe::buffer::Ptr Command::ExecuteRequestDestroy(bool sync) {
  if (tokens_.size() <= 3) {
    std::cout << "Destroy request cannot be sent without an object Id" << std::endl;
    return nullptr;
  }

  // Parse the object Id
  std::stringstream sstream(tokens_[3]);
  std::size_t object_id = 0;
  sstream >> object_id;

  // Return the Destroy request
  return pipe::request::CreateDestroy(object_id, sync);
}

pipe::buffer::Ptr Command::ExecuteRequestAscii(bool sync) {
  if (tokens_.size() <= 3)
    return pipe::request::CreateAscii("", sync);

  return pipe::request::CreateAscii(tokens_[3], sync);
}

}  // client
}  // npe
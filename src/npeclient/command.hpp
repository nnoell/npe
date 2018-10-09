#pragma once

// STL
#include <string>
#include <vector>
#include <sstream>

// NPE PIPE public
#include <npe/pipe/buffer/ptr.hpp>

// NPE CLIENT private
#include "command/args.hpp"
#include "command/args/ptr.hpp"

namespace npe {
namespace client {

// The Command class
class Command final {
 public:
  // Constructor
  Command();

  // Destructor
  ~Command();

  // Reads a command from cin
  void Read();

  // Checks whether a quite command has been introduced or not
  bool Quit() const;

  // Converts an args buffer into a readable string using the command args format as a reference
  std::string ArgsToString(const void *args, std::size_t args_size) const;

  // Executes the command read from cin, returns nullptr if the command was not a request
  pipe::buffer::Ptr Execute();

 private:
  // Executes the help command
  pipe::buffer::Ptr ExecuteHelp();

  // Executes the quit command
  pipe::buffer::Ptr ExecuteQuit();

  // Executes the version command
  pipe::buffer::Ptr ExecuteVersion();

  // Executes the request command
  pipe::buffer::Ptr ExecuteRequest();

  // Executes the request last command
  pipe::buffer::Ptr ExecuteRequestLast(bool sync);

  // Executes the request list command
  pipe::buffer::Ptr ExecuteRequestList(bool sync);

  // Executes the request construct command
  pipe::buffer::Ptr ExecuteRequestConstruct(bool sync);

  // Executes the request retrieve command
  pipe::buffer::Ptr ExecuteRequestRetrieve(bool sync);

  // Executes the request call command
  pipe::buffer::Ptr ExecuteRequestCall(bool sync);

  // Executes the request destroy command
  pipe::buffer::Ptr ExecuteRequestDestroy(bool sync);

  // Executes the request ascii command
  pipe::buffer::Ptr ExecuteRequestAscii(bool sync);

  // Executes the request primitive command
  template <typename T>
  pipe::buffer::Ptr ExecuteRequestPrimitive(BYTE type, bool sync) {
    if (tokens_.size() <= 3) {
      std::cout << "Primitive request cannot be sent without an primitive value" << std::endl;
      return nullptr;
    }

    // Parse the value
    std::stringstream sstream(tokens_[3]);
    T val;
    sstream >> val;
    return pipe::request::CreatePrimitive<T>(type, val, sync);
  }

 private:
  // Copy Constructor
  Command(const Command&) = delete;

  // Move Constructor
  Command(Command &&) = delete;

  // Copy-Assign Constructor
  Command& operator=(const Command&) = delete;

  // Move-Assign Constructr
  Command& operator=(Command &&) = delete;

 private:
  // The command tokens
  std::vector<std::string> tokens_;

  // The last command args for parsing the response output
  command::args::Ptr last_args_;

  // Flag that indicates wether a quite command has been intrudoced or not
  bool quit_;
};

}  // namespace client
}  // namespace npe


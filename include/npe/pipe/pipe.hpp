#pragma once

// Windows
#include <windows.h>

// STL
#include <string>

// NPE PIPE public
#include <npe/pipe/file.hpp>

namespace npe {
namespace pipe {

// The pipe class
class Pipe final : public File {
 public:
  // Constructor
  explicit Pipe(const std::string& name);

  // Destructor
  ~Pipe();

  // Connects
  bool Connect();

  // Disconnects
  bool Disconnect();

  // Check whether the pipe is connected or not
  bool IsConnected();

 private:
  // Copy Constructor
  Pipe(const Pipe&) = delete;

  // Move Constructor
  Pipe(Pipe &&) = delete;

  // Copy-Assign Constructor
  Pipe& operator=(const Pipe&) = delete;

  // Move-Assign Constructr
  Pipe& operator=(Pipe &&) = delete;
};

}  // namespace pipe
}  // namespace npe


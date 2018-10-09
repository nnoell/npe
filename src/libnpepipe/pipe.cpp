// NPE PIPE public
#include <npe/pipe/pipe.hpp>

namespace npe {
namespace pipe {

// Creates a default pipe handle
static std::shared_ptr<void> DefaultPipe(const std::string& name) {
  return std::shared_ptr<void>(
      CreateNamedPipe(
        name.c_str(),
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE,
        PIPE_UNLIMITED_INSTANCES,
        0,
        0,
        0,
        nullptr),
      [](HANDLE h){CloseHandle(h);}
  );
}

Pipe::Pipe(const std::string& name) :
    File(DefaultPipe(name)) {
}

Pipe::~Pipe() {
}

bool Pipe::Connect() {
  return ConnectNamedPipe(handle_.get(), NULL);
}

bool Pipe::Disconnect() {
  return DisconnectNamedPipe(handle_.get());
}

bool Pipe::IsConnected() {
  return ERROR_PIPE_NOT_CONNECTED != GetLastError();
}

}  // namespace pipe
}  // namespace npe
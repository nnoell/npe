// NPE PIPE public
#include <npe/pipe/file.hpp>

namespace npe {
namespace pipe {

// Creates a default file handle
static std::shared_ptr<void> DefaultFile(const std::string& name) {
  return std::shared_ptr<void>(
      CreateFile(
        name.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr
      ),
      [](HANDLE h){CloseHandle(h);}
  );
}

File::File(std::shared_ptr<void> handle) :
    handle_(std::move(handle)),
    bytes_read_(0),
    bytes_written_(0) {
  if (handle_ == nullptr || handle_.get() == INVALID_HANDLE_VALUE)
    throw std::runtime_error("Failed to create file");
  DWORD state = PIPE_READMODE_MESSAGE;
  SetNamedPipeHandleState(handle_.get(), &state, nullptr, nullptr);
}

File::File(const std::string& name) :
    File(DefaultFile(name)) {
}

File::~File() {
}

bool File::IsOk() {
  return ERROR_BROKEN_PIPE != GetLastError();
}

bool File::SendBuffer(buffer::Ptr buffer) {
  if (!buffer)
    return false;

  // Write the file
  if (!WriteFile(handle_.get(), buffer->GetData(), buffer->GetSize(), &bytes_written_, nullptr)
      || bytes_written_ != buffer->GetSize()) {
    return false;
  }

  return true;
}

bool File::ReceiveBuffer(buffer::Ptr& buffer) {
  if (!buffer)
    return false;

  // Read the file
  DWORD bytes_read_;
  if (!ReadFile(handle_.get(), buffer->GetData(), buffer->GetAllocatedSize(), &bytes_read_, nullptr)
      || bytes_read_ >= buffer->GetAllocatedSize())
    return false;

  // Set the buffer size
  buffer->SetSize(bytes_read_);
  return true;
}

bool File::SendReceiveBuffer(buffer::Ptr in, buffer::Ptr& out) {
  if (!in || !out)
    return false;

  DWORD bytes_read_;
  if (!TransactNamedPipe(handle_.get(), in->GetData(), in->GetSize(), out->GetData(), out->GetAllocatedSize(), &bytes_read_, nullptr)
      || bytes_read_ >= out->GetAllocatedSize())
    return false;

  // Set the out buffer size
  out->SetSize(bytes_read_);
  return true;
}

}  // namespace pipe
}  // namespace npe
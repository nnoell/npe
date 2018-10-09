// NPE PIPE public
#include <npe/pipe/buffer.hpp>

namespace npe {
namespace pipe {

Buffer::Buffer(DWORD allocated_size) :
    size_(0),
    allocated_size_(allocated_size),
    data_(new BYTE[allocated_size], std::default_delete<BYTE[]>()) {
}

Buffer::Buffer(buffer::Data data, DWORD size) :
    size_(size),
    allocated_size_(size),
    data_(std::move(data)) {
}

Buffer::Buffer(const std::string& data) :
    size_(data.size()),
    allocated_size_(size_),
    data_(new BYTE[allocated_size_], std::default_delete<BYTE[]>()) {
  std::memcpy(data_.get(), data.c_str(), data.size());
}

Buffer::~Buffer() {
}

BYTE *Buffer::GetData() {
  return data_.get();
}

DWORD Buffer::GetAllocatedSize() {
  return allocated_size_;
}

DWORD Buffer::GetSize() {
  return size_;
}

void Buffer::SetSize(DWORD size) {
  if (size > allocated_size_)
    throw std::runtime_error("Cannot set a buffer size biffer than the allocated size");

  size_ = size;
}

std::string Buffer::ToString() {
  return buffer::DataToString(data_.get(), size_);
}

}  // namespace pipe
}  // namespace npe
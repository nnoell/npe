// NPE PIPE public
#include <npe/pipe/buffer/data.hpp>

namespace npe {
namespace pipe {
namespace buffer {

// Converts buffer data to string
std::string DataToString(const BYTE *data, DWORD size) {
  // Return emtpy string if size is null
  if (size <= 0)
    return "";

  // Do the conversion
  const DWORD str_size = size + 1;  // 1 extra byte because of '\0'
  std::shared_ptr<char[]> str_ptr(new char[str_size], std::default_delete<char[]>());
  std::memcpy(str_ptr.get(), data, size);
  str_ptr[str_size - 1] = '\0';

  return std::string(str_ptr.get());
}

}  // namespace buffer
}  // namespace pipe
}  // namespace npe
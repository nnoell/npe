// NPE PIPE public
#include <npe/pipe/request/ascii.hpp>

namespace npe {
namespace pipe {
namespace request {

buffer::Ptr CreateAscii(const std::string& ascii, bool sync) {
  const DWORD total_size = 2 + ascii.size();
  buffer::Data buffer_data(new BYTE[total_size], std::default_delete<BYTE[]>());
  buffer_data[0] = T_Ascii;
  buffer_data[1] = sync;
  if (ascii.size() > 0)
    std::memcpy(buffer_data.get() + 2, ascii.c_str(), ascii.size());
  return std::make_shared<Buffer>(std::move(buffer_data), total_size);
}

bool ParseAscii(const BYTE *request, DWORD request_size, std::string& ascii) {
  if (!request)
    return false;
  if (request_size < 2)
    return false;

  // ascii str
  if (request_size <= 2) {
    ascii = "";
  } else {  
    const DWORD str_size = request_size - 2 + 1;  // 1 extra byte because of '\0'
    std::shared_ptr<char[]> str_ptr(new char[str_size], std::default_delete<char[]>());
    std::memcpy(str_ptr.get(), request + 2, request_size - 2);
    str_ptr[str_size - 1] = '\0';
    ascii = std::string(str_ptr.get());
  }

  return true;
}

}  // namespace request
}  // namespace pipe
}  // namespace npe
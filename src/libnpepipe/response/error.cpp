// NPE PIPE public
#include <npe/pipe/response/error.hpp>

namespace npe {
namespace pipe {
namespace response {

buffer::Ptr CreateError(const std::string& ascii, BYTE from) {
  const DWORD total_size = 2 + ascii.size();
  buffer::Data buffer_data(new BYTE[total_size], std::default_delete<BYTE[]>());
  buffer_data[0] = T_Error;
  buffer_data[1] = from;
  if (ascii.size() > 0)
    std::memcpy(buffer_data.get() + 2, ascii.c_str(), ascii.size());
  return std::make_shared<Buffer>(std::move(buffer_data), total_size);
}

bool ParseError(const BYTE *response, DWORD response_size, std::string& ascii) {
  if (!response)
    return false;

  // ascii str
  ascii = buffer::DataToString(response + 2, response_size -2);

  return true;
}

}  // namespace response
}  // namespace pipe
}  // namespace npe
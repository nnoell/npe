// NPE PIPE public
#include <npe/pipe/response/ok.hpp>

namespace npe {
namespace pipe {
namespace response {

buffer::Ptr CreateOk(const void *args, std::size_t args_size, BYTE from) {
  const DWORD total_size = 2 + args_size;
  buffer::Data buffer_data(new BYTE[total_size], std::default_delete<BYTE[]>());
  BYTE *tmp = buffer_data.get();
  tmp[0] = T_Ok;
  tmp[1] = from;

  // Args
  if (args && args_size >= 0) {
    tmp += 2;
    std::memcpy(tmp, args, args_size);
  }

  return std::make_shared<Buffer>(std::move(buffer_data), total_size);
}

bool ParseOk(const BYTE *request, DWORD request_size, const void *&args, std::size_t& args_size) {
  if (!request)
    return false;

  // Args
  args_size = request_size - 2;
  if (args_size > 0) {
    args = request + 2;
  } else {
    args = nullptr;
    args_size = 0;
  }

  return true;
}

}  // namespace response
}  // namespace pipe
}  // namespace npe
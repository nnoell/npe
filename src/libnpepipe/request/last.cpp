// NPE PIPE public
#include <npe/pipe/request/last.hpp>

namespace npe {
namespace pipe {
namespace request {

buffer::Ptr CreateLast(bool sync) {
  const DWORD total_size = 2;
  buffer::Data buffer_data(new BYTE[total_size], std::default_delete<BYTE[]>());
  BYTE *tmp = buffer_data.get();
  tmp[0] = T_Last;
  tmp[1] = sync;

  return std::make_shared<Buffer>(std::move(buffer_data), total_size);
}

}  // namespace request
}  // namespace pipe
}  // namespace npe
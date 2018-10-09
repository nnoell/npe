// NPE PIPE public
#include <npe/pipe/request/primitive.hpp>
#include <npe/pipe/request/destroy.hpp>

namespace npe {
namespace pipe {
namespace request {

buffer::Ptr CreateDestroy(Id object_id, bool sync) {
  return CreatePrimitive<Id>(T_Destroy, object_id, sync);
}

bool ParseDestroy(const BYTE *request, DWORD request_size, Id& object_id) {
  return ParsePrimitive(request, request_size, object_id);
}

}  // namespace request
}  // namespace pipe
}  // namespace npe
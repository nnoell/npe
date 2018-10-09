// NPE PIPE public
#include <npe/pipe/request/primitive.hpp>
#include <npe/pipe/request/retrieve.hpp>

namespace npe {
namespace pipe {
namespace request {

buffer::Ptr CreateRetrieve(Id object_id, bool sync) {
  return CreatePrimitive<Id>(T_Retrieve, object_id, sync);
}

bool ParseRetrieve(const BYTE *request, DWORD request_size, Id& object_id) {
  return ParsePrimitive<Id>(request, request_size, object_id);
}

}  // namespace request
}  // namespace pipe
}  // namespace npe
// NPE PIPE public
#include <npe/pipe/request/construct.hpp>
#include <npe/pipe/hash.hpp>

namespace npe {
namespace pipe {
namespace request {

buffer::Ptr CreateConstruct(const std::string& object_type_name, const void *args, std::size_t args_size, bool sync) {
  const DWORD total_size = 2 + sizeof(Id) + args_size;
  buffer::Data buffer_data(new BYTE[total_size], std::default_delete<BYTE[]>());
  BYTE *tmp = buffer_data.get();
  tmp[0] = T_Construct;
  tmp[1] = sync;

  // Object type
  tmp += 2;
  const Id object_hash = GetHash(object_type_name);
  std::memcpy(tmp, &object_hash, sizeof(Id));

  // Args
  if (args && args_size > 0) {
    tmp += sizeof(Id);
    std::memcpy(tmp, args, args_size);
  }

  return std::make_shared<Buffer>(std::move(buffer_data), total_size);
}

bool ParseConstruct(const BYTE *request, DWORD request_size, Id& object_type, const void *&args, std::size_t& args_size) {
  if (!request)
    return false;
  if (request_size < (2 + sizeof(Id)))
    return false;

  // Object type
  std::memcpy(&object_type, request + 2, sizeof(Id));

  // Args
  args_size = request_size - (2 + sizeof(Id));
  if (args_size > 0) {
    args = request + 2 + sizeof(Id);
  } else {
    args = nullptr;
    args_size = 0;
  }

  return true;
}

}  // namespace request
}  // namespace pipe
}  // namespace npe
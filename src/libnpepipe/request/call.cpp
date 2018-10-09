// NPE PIPE public
#include <npe/pipe/request/call.hpp>
#include <npe/pipe/hash.hpp>

namespace npe {
namespace pipe {
namespace request {

buffer::Ptr CreateCall(Id object_id, const std::string& method_name, const void *args, std::size_t args_size, bool sync) {
  const DWORD total_size = 2 + sizeof(Id) + sizeof(Id) + args_size;
  buffer::Data buffer_data(new BYTE[total_size], std::default_delete<BYTE[]>());
  BYTE *tmp = buffer_data.get();
  tmp[0] = T_Call;
  tmp[1] = sync;

  // Object Id
  tmp += 2;
  std::memcpy(tmp, &object_id, sizeof(Id));

  // Method name
  tmp += sizeof(Id);
  const Id method_hash = GetHash(method_name);
  std::memcpy(tmp, &method_hash, sizeof(Id));

  // Args
  if (args && args_size > 0) {
    tmp += sizeof(Id);
    std::memcpy(tmp, args, args_size);
  }

  return std::make_shared<Buffer>(std::move(buffer_data), total_size);
}

bool ParseCall(BYTE *request, DWORD request_size, Id& object_id, Id& method_id, void *&args, std::size_t& args_size) {
  if (!request)
    return false;
  if (request_size < (2 + sizeof(Id) + sizeof(Id)))
    return false;

  // Object Id
  std::memcpy(&object_id, request + 2, sizeof(Id));

  // Method Id
  std::memcpy(&method_id, request + 2 + sizeof(Id), sizeof(Id));

  // Args
  args_size = request_size - (2 + sizeof(Id) + sizeof(Id));
  if (args_size > 0) {
    args = request + 2 + sizeof(Id) + sizeof(Id);
  } else {
    args = nullptr;
    args_size = 0;
  }

  return true;
}

}  // namespace request
}  // namespace pipe
}  // namespace npe
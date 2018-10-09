#pragma once

// Windows
#include <windows.h>

// NPE PIPE public
#include <npe/pipe/id.hpp>
#include <npe/pipe/buffer.hpp>

namespace npe {
namespace pipe {
namespace request {

// Byte format:
// 1 (code) | 1 (sync) | n (primitive data)

// The Bool request type
static constexpr BYTE const T_Bool = '7';

// The Int request type
static constexpr BYTE const T_Int = '8';

// The Char request type
static constexpr BYTE const T_Char = '9';

// The Id request type
static constexpr BYTE const T_Id = 'a';

// The Double request type
static constexpr BYTE const T_Double = 'b';

// Creates a primitive request
template <typename T>
buffer::Ptr CreatePrimitive(BYTE type, T primitive, bool sync = true) {
  const DWORD total_size = 2 + sizeof(T);
  buffer::Data buffer_data(new BYTE[total_size], std::default_delete<BYTE[]>());
  BYTE *tmp = buffer_data.get();
  tmp[0] = type;
  tmp[1] = sync;

  // Object Id
  tmp += 2;
  std::memcpy(tmp, &primitive, sizeof(T));

  return std::make_shared<Buffer>(std::move(buffer_data), total_size);
}

// Parses a primitive request
template <typename T>
bool ParsePrimitive(const BYTE *request, DWORD request_size, T& primitive) {
  if (!request)
    return false;
  if (request_size < (2 + sizeof(T)))
    return false;

  // Primitive
  std::memcpy(&primitive, request + 2, sizeof(T));

  return true;
}

}  // namespace request
}  // namespace pipe
}  // namespace npe
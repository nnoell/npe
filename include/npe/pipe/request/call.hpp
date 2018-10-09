#pragma once

// Windows
#include <windows.h>

// STL
#include <cstddef>
#include <string>

// NPE PIPE public
#include <npe/pipe/id.hpp>
#include <npe/pipe/buffer.hpp>

namespace npe {
namespace pipe {
namespace request {

// Byte format:
// 1 (code) | 1 (sync) | 4 (object id) | 4 (method type hash) | n (raw args)

// The Call request type
static constexpr BYTE const T_Call = '4';

// Creates a Call request
buffer::Ptr CreateCall(Id object_id, const std::string& method_name, const void *args = nullptr, std::size_t args_size = 0, bool sync = true);

// Parses a Call request
bool ParseCall(BYTE *request, DWORD request_size, Id& object_id, Id& method_id, void *&args, std::size_t& args_size);

}  // namespace request
}  // namespace pipe
}  // namespace npe
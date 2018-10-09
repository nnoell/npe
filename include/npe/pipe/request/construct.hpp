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
// 1 (code) | 1 (sync) | 4 (object type hash) | n (raw args)

// The Construct request type
static constexpr BYTE const T_Construct = '2';

// Creates a Construct request
buffer::Ptr CreateConstruct(const std::string& object_type_name, const void *args = nullptr, std::size_t args_size = 0, bool sync = true);

// Parses a Construct request
bool ParseConstruct(const BYTE *request, DWORD request_size, Id& object_type, const void *&args, std::size_t& args_size);

}  // namespace request
}  // namespace pipe
}  // namespace npe
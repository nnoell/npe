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
namespace response {

// Byte format:
// 1 (code) | 1 (from) | n (args in raw)

// The OK response type
static constexpr BYTE const T_Ok = '1';

// Creates an Ok response with args
buffer::Ptr CreateOk(const void *args = nullptr, std::size_t args_size = 0, BYTE from = '?');

// Parses an Ok response
bool ParseOk(const BYTE *request, DWORD request_size, const void *&args, std::size_t& args_size);

}  // namespace response
}  // namespace pipe
}  // namespace npe
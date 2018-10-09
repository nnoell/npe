#pragma once

// Windows
#include <windows.h>

// STL
#include <string>

// NPE PIPE public
#include <npe/pipe/id.hpp>
#include <npe/pipe/buffer.hpp>

namespace npe {
namespace pipe {
namespace response {

// Byte format:
// 1 (code) | 1 (from) | n (ascii error)

// The Error response type
static constexpr BYTE const T_Error = '0';

// Creates an Ascii response
buffer::Ptr CreateError(const std::string& ascii = "", BYTE from = '?');

// Parses an Ascii response
bool ParseError(const BYTE *response, DWORD response_size, std::string& ascii);

}  // namespace response
}  // namespace pipe
}  // namespace npe
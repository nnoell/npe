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
namespace request {

// Byte format:
// 1 (code) | 1 (sync) | n (ascii data)

// The Ascii request type
static constexpr BYTE const T_Ascii = '6';

// Creates an Ascii request
buffer::Ptr CreateAscii(const std::string& ascii = "", bool sync = true);

// Parses an Ascii request
bool ParseAscii(const BYTE *request, DWORD request_size, std::string& ascii);

}  // namespace request
}  // namespace pipe
}  // namespace npe
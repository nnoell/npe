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
// 1 (code) | 1 (sync)

// The Last request type
static constexpr BYTE const T_Last = '0';

// Creates a Last request
buffer::Ptr CreateLast(bool sync = true);

}  // namespace request
}  // namespace pipe
}  // namespace npe
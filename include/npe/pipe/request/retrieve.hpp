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
// 1 (code) | 1 (sync) | 4 (object id)

// The Destroy request type
static constexpr BYTE const T_Retrieve = '3';

// Creates a Retrieve request
buffer::Ptr CreateRetrieve(Id object_id, bool sync = true);

// Parses a Retrieve request
bool ParseRetrieve(const BYTE *request, DWORD request_size, Id& object_id);

}  // namespace request
}  // namespace pipe
}  // namespace npe
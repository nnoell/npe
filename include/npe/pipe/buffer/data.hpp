#pragma once

// Windows
#include <windows.h>

// STL
#include <memory>
#include <string>

namespace npe {
namespace pipe {
namespace buffer {

// The buffer data type
using Data = std::shared_ptr<BYTE[]>;

// Converts buffer data to string
std::string DataToString(const BYTE *data, DWORD size);

}  // namespace buffer
}  // namespace pipe
}  // namespace npe

#pragma once

// STL
#include <memory>

namespace npe {
namespace pipe {

// Forward
class Buffer;

namespace buffer {

using Ptr = std::shared_ptr<Buffer>;

}  // namespace buffer
}  // namespace pipe
}  // namespace npe

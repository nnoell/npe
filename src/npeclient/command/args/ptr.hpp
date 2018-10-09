#pragma once

// STL
#include <memory>

namespace npe {
namespace client {
namespace command {

// Forward
class Args;

namespace args {

using Ptr = std::shared_ptr<Args>;

}  // namespace args
}  // namespace command
}  // namespace client
}  // namespace npe

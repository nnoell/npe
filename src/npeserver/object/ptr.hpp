#pragma once

// STL
#include <memory>

namespace npe {
namespace server {

// Forward
class Object;

namespace object {

// The Object Pointer
using Ptr = std::shared_ptr<Object>;

}  // namespace object
}  // namespace server
}  // namespace npe

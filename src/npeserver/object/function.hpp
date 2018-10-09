#pragma once

// STL
#include <functional>
#include <unordered_map>

// NPE PIPE public
#include <npe/pipe/id.hpp>

// NPE SERVER private
#include "ptr.hpp"

namespace npe {
namespace server {
namespace object {

// The Object Type
using Make = Ptr (*)(const void *args, std::size_t args_size);

// The Factory Type
template<typename Class>
Ptr Factory(const void *args, std::size_t args_size) {
  return std::make_shared<Class>(args, args_size);
}

// The Object Method Type
using Method = std::function<void(void *,std::size_t)>;

// The Method Map Type
using MethodMap = std::unordered_map<pipe::Id, Method>;

}  // namespace object
}  // namespace server
}  // namespace npe

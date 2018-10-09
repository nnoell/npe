// STL
#include <functional>

// NPE PIPE public
#include <npe/pipe/hash.hpp>

namespace npe {
namespace pipe {

Id GetHash(std::string str) {
  return std::hash<std::string>{}(str);
}

}  // namespace pipe
}  // namespace npe

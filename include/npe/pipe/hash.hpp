#pragma once

// STL
#include <string>

// NPE PIPE public
#include <npe/pipe/id.hpp>

namespace npe {
namespace pipe {

// Gets a unique hash Id for a string
Id GetHash(std::string str);

}  // namespace pipe
}  // namespace npe

// STL
#include <iostream>

// NPE PIPE public
#include <npe/pipe/hash.hpp>
#include <npe/pipe/request.hpp>

namespace npe {
namespace pipe {

bool RequestParseType(const BYTE *request, DWORD request_size, BYTE& type) {
  if (!request || request_size < 1)
    return false;

  // Get the type
  type = request[0];

  return true;
}

bool RequestParseSync(const BYTE *request, DWORD request_size, bool& sync) {
  if (!request || request_size < 2)
    return false;

  // Get the sync
  sync = request[1];

  return true;
}

}  // namespace pipe
}  // namespace npe
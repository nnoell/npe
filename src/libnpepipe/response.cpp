// NPE PIPE public
#include <npe/pipe/response.hpp>

namespace npe {
namespace pipe {

bool ResponseParseType(const BYTE *response, DWORD response_size, BYTE& type) {
  if (!response || response_size < 1)
    return false;

  // Get the type
  type = response[0];

  return true;
}

bool ResponseParseFrom(const BYTE *response, DWORD response_size, BYTE& from) {
  if (!response || response_size < 2)
    return false;

  // Get the sync
  from = response[1];

  return true;
}

}  // namespace pipe
}  // namespace npe
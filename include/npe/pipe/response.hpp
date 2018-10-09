#pragma once

// NPE PIPE public
#include <npe/pipe/response/error.hpp>
#include <npe/pipe/response/ok.hpp>

namespace npe {
namespace pipe {

// Parses the response type
bool ResponseParseType(const BYTE *request, DWORD request_size, BYTE& type);

// Parses the response sync
bool ResponseParseFrom(const BYTE *request, DWORD request_size, BYTE& from);

}  // namespace pipe
}  // namespace npe
#pragma once

// NPE PIPE public
#include <npe/pipe/request/last.hpp>
#include <npe/pipe/request/list.hpp>
#include <npe/pipe/request/construct.hpp>
#include <npe/pipe/request/retrieve.hpp>
#include <npe/pipe/request/call.hpp>
#include <npe/pipe/request/destroy.hpp>
#include <npe/pipe/request/ascii.hpp>
#include <npe/pipe/request/primitive.hpp>

namespace npe {
namespace pipe {

// Parses the request type
bool RequestParseType(const BYTE *request, DWORD request_size, BYTE& type);

// Parses the request sync
bool RequestParseSync(const BYTE *request, DWORD request_size, bool& sync);

// Sets the sync flag to false

}  // namespace pipe
}  // namespace npe
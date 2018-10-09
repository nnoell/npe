// Windows
#include <windows.h>

// STL
#include <iostream>
#include <sstream>

// NPE PIPE public
#include <npe/pipe/buffer.hpp>
#include <npe/pipe/buffer/args.hpp>

// NPE CLIENT private
#include "args.hpp"

namespace npe {
namespace client {
namespace command {

// Converts a string into a primitive and increases the position of the buffer
template <typename T>
static void StringToPrimitive(BYTE *&data, std::string str) {
  std::stringstream sstream(str);
  T val;
  sstream >> val;
  std::memcpy(data, &val, sizeof(T));
  data += sizeof(T);
}

// Converts a primitive into a string and increases the position of the buffer
template <typename T>
static std::string PrimitiveToString(const BYTE *&data) {
  std::stringstream ss;
  ss << *reinterpret_cast<const T *>(data);
  data += sizeof(T);
  return ss.str();
}

Args::Args(std::vector<std::string> args_vector) :
    args_vector_(std::move(args_vector)) {
}

Args::~Args() {
}

// TODO: Find a better way to do this because it looks awful...
pipe::buffer::Ptr Args::Parse() const {
  // Get the args size
  std::size_t args_size = 0;
  for (const std::string &str : args_vector_) {
    if (str.size() < 3 || str[1] != '=') return nullptr;
    switch(str[0]) {  // i=...
      case 'b': args_size += sizeof(bool); break;
      case 'i': args_size += sizeof(int); break;
      case 'c': args_size += sizeof(char); break;
      case 'k': args_size += sizeof(std::size_t); break;
      case 'd': args_size += sizeof(double); break;
      case 'a': args_size += sizeof(char) * NPE_PIPE_BUFFER_MAX_ASCII_ARGS_SIZE; break;
      default: return nullptr;
    }
  }

  // Get the args pointer
  pipe::buffer::Data args(new BYTE[args_size], std::default_delete<BYTE[]>());
  BYTE *tmp = args.get();
  for (const std::string &str : args_vector_) {
    if (str.size() > (NPE_PIPE_BUFFER_MAX_ASCII_ARGS_SIZE - 1)) return nullptr; // because of '\0'
    const char *str_value = str.c_str() + 2;
    if (std::strlen(str_value) <= 0) return nullptr;
    switch(str[0]) {  // i=...
      case 'b': StringToPrimitive<bool>(tmp, str.c_str() + 2); break;
      case 'i': StringToPrimitive<int>(tmp, str.c_str() + 2); break;
      case 'c': StringToPrimitive<char>(tmp, str.c_str() + 2); break;
      case 'k': StringToPrimitive<std::size_t>(tmp, str.c_str() + 2); break;
      case 'd': StringToPrimitive<double>(tmp, str.c_str() + 2); break;
      case 'a':
        std::memcpy(tmp, str.c_str() + 2, str.size());
        (tmp + str.size() - 1)[0] = '\0';  // Make sure the string has a '\0' at the end
        tmp += sizeof(char) * NPE_PIPE_BUFFER_MAX_ASCII_ARGS_SIZE;
        break;
      default: return nullptr;
    }
  }

  return std::make_shared<pipe::Buffer>(std::move(args), args_size);
}

// TODO: Find a better way to do this because it looks awful...
std::string Args::BufferToString(const void *args, std::size_t args_size) const {
  std::stringstream ss;
  ss << "{";
  const BYTE *tmp = reinterpret_cast<const BYTE *>(args);
  for (const std::string &str : args_vector_) {
    if (str.size() < 1) continue;
    switch(str[0]) {
      case 'b': ss << " b=" << PrimitiveToString<bool>(tmp); break;
      case 'i': ss << " i=" << PrimitiveToString<int>(tmp); break;
      case 'c': ss << " c=" << PrimitiveToString<char>(tmp); break;
      case 'k': ss << " k=" << PrimitiveToString<std::size_t>(tmp); break;
      case 'd': ss << " d=" << PrimitiveToString<double>(tmp); break;
      case 'a':
        ss << " a=\"" << std::string(reinterpret_cast<const char *>(tmp)) << "\"";
        tmp += sizeof(char) * NPE_PIPE_BUFFER_MAX_ASCII_ARGS_SIZE;
        break;
      default: continue;
    }
  }
  ss << " }";
  return ss.str();
}

}  // namespace command
}  // namespace client
}  // namespace npe
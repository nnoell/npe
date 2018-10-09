#pragma once

// STL
#include <string>
#include <vector>

// NPE PIPE public
#include <npe/pipe/buffer/ptr.hpp>

namespace npe {
namespace client {
namespace command {

// The Args class
class Args final {
 public:
  // Constructor
  explicit Args(std::vector<std::string> args_vector);

  // Destructor
  ~Args();

  // Parses the args into a buffer
  pipe::buffer::Ptr Parse() const;

  // Converts an args buffer into a readable string using the args_vector format as a reference
  std::string BufferToString(const void *args, std::size_t args_size) const;

 private:
  // Copy Constructor
  Args(const Args&) = delete;

  // Move Constructor
  Args(Args &&) = delete;

  // Copy-Assign Constructor
  Args& operator=(const Args&) = delete;

  // Move-Assign Constructr
  Args& operator=(Args &&) = delete;

 private:
  // The vector with all the args
  std::vector<std::string> args_vector_;
};

}  // namespace command
}  // namespace client
}  // namespace npe


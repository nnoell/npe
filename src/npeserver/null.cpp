// STL
#include <string>
#include <cstring>
#include <sstream>

// NPE PIPE public
#include <npe/pipe/hash.hpp>

// NPE SERVER public
#include <npe/server/null.hpp>

// NPE SERVER private
#include "object.hpp"
#include "registry.hpp"

namespace npe {
namespace server {

class Null final : public Object {
 public:
  // The registered type name
  static constexpr char const *TypeName = null::T_Name;

 public:
  // Constructor
  Null(const void *args, std::size_t args_size);

  // Destructor
  ~Null();

  // Gets a string representing the object
  std::string ToString() const override;

  // Gets the name
  void DoNothing(void *args, std::size_t args_size);

 private:
  // Copy Constructor
  Null(const Null&) = delete;

  // Move Constructor
  Null(Null &&) = delete;

  // Copy-Assign Constructor
  Null& operator=(const Null&) = delete;

  // Move-Assign Constructor
  Null& operator=(Null &&) = delete;
};

Null::Null(const void *args, std::size_t args_size) :
    Object(Null::TypeName, {
      // Register the methods
      {npe::pipe::GetHash(null::M_DoNothing), std::bind(&Null::DoNothing, this, std::placeholders::_1, std::placeholders::_2)}
    }) {
  (void)args;
  (void)args_size;
}

Null::~Null() {
}

std::string Null::ToString() const {
  std::stringstream ss;
  ss << GetId() << ": [" << GetTypeName() << "] { }";
  return ss.str();
}

void Null::DoNothing(void *args, std::size_t args_size) {
  (void)args;
  (void)args_size;
}

// Register the Null class
NPE_SERVER_REGISTER_OBJECT(Null)

}  // namespace server
}  // namespace npe

#pragma once

// STL
#include <string>

// NPE PIPE public
#include <npe/pipe/id.hpp>

// NPE SERVER private
#include "object/ptr.hpp"
#include "object/function.hpp"

namespace npe {
namespace server {

// The Object Class
class Object {
 public:
  // Makes an object of a specific type
  static object::Ptr Make(pipe::Id type, const void *args, std::size_t args_size);

 public:
  // Destructor
  virtual ~Object();

  // Gets the unique Id
  pipe::Id GetId() const;

  // Gets the type name
  std::string GetTypeName() const;

  // Calls a specific method of the object
  void Call(pipe::Id method_id, void *args, std::size_t args_size);

  // Gets a string representing the content of the object
  virtual std::string ToString() const = 0;

 protected:
  // Constructor
  Object(const char *type_name_, object::MethodMap method_map);

 private:
  // Copy Constructor
  Object(const Object&) = delete;

  // Move Constructor
  Object(Object &&) = delete;

  // Copy-Assign Constructor
  Object& operator=(const Object&) = delete;

  // Move-Assign Constructr
  Object& operator=(Object &&) = delete;

 private:
  // The Id
  const pipe::Id id_;

  // The type
  const pipe::Id type_;

  // The method list
  const object::MethodMap method_map_;
};

}  // namespace server
}  // namespace npe

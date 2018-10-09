#pragma once

// STL
#include <mutex>
#include <unordered_map>

// NPE public
#include <npe/pipe/id.hpp>

// NPE SERVER private
#include "object.hpp"
#include "registry.hpp"

namespace npe {
namespace server {

// The Object Map Type
using ObjectMap = std::unordered_map<pipe::Id, object::Ptr>;

// The state class
class State final {
 public:
  // Constructor
  State();

  // Destructor
  ~State();

  // Creates an Object
  pipe::Id CreateObject(pipe::Id object_type, const void *args, std::size_t args_size);

  // Call Object
  void CallObject(pipe::Id object_id, pipe::Id method_id, void *args, std::size_t args_size);

  // Destroys an object
  void DestroyObject(pipe::Id object_id);

  // Gets a string representing the object
  std::string ToStringObject(pipe::Id object_id);

  // Gets a string representing all the objects
  std::string ToStringObjects();

 private:
  // Copy Constructor
  State(const State&) = delete;

  // Move Constructor
  State(State &&) = delete;

  // Copy-Assign Constructor
  State& operator=(const State&) = delete;

  // Move-Assign Constructr
  State& operator=(State &&) = delete;

 private:
  // The mutext to make state changes safe
  mutable std::mutex mutex_;

  // The Map of objects
  ObjectMap object_map_;
};

}  // namespace server
}  // namespace npe


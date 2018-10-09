// STL
#include <sstream>

// NPE SERVER private
#include "state.hpp"

namespace npe {
namespace server {

State::State() :
    mutex_(),
    object_map_() {
}

State::~State() {
}

pipe::Id State::CreateObject(pipe::Id object_type, const void *args, std::size_t args_size) {
  std::unique_lock<std::mutex> lock(mutex_);

  // Create the object
  object::Ptr object = Object::Make(object_type, args, args_size);

  // Add the object into the map
  pipe::Id object_id = object->GetId();
  object_map_[object_id] = std::move(object);

  // return the object Id
  return object_id;
}

void State::CallObject(pipe::Id object_id, pipe::Id method_id, void *args, std::size_t args_size) {
  std::unique_lock<std::mutex> lock(mutex_);

  // Get the object from map
  ObjectMap::const_iterator it = object_map_.find(object_id);
  if (it == object_map_.end())
    throw std::runtime_error("Could not find the object in the object map");

  // Call the method
  it->second->Call(method_id, args, args_size);
}

void State::DestroyObject(pipe::Id object_id) {
  std::unique_lock<std::mutex> lock(mutex_);

  // Remove the object from the map
  if (!object_map_.erase(object_id))
    throw std::runtime_error("Could not find the object in the object map");
}

std::string State::ToStringObject(pipe::Id object_id) {
  std::unique_lock<std::mutex> lock(mutex_);

  // Get the object from map
  ObjectMap::const_iterator it = object_map_.find(object_id);
  if (it == object_map_.end())
    throw std::runtime_error("Could not find the object in the object map");
  
  // Call the ToString method
  return it->second->ToString();
}

std::string State::ToStringObjects() {
  // Call ToString on all objects
  std::stringstream ss;
  for (const auto& o : object_map_)
    ss << std::endl << o.second->ToString();
  
  return ss.str();
}

}  // namespace server
}  // namespace npe

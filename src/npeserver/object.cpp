// STL
#include <functional>
#include <atomic>

// NPE SERVER private
#include "object.hpp"
#include "registry.hpp"

namespace npe {
namespace server {

static pipe::Id GetUniqueId() {
  static std::atomic<pipe::Id> id(0);
  return id++;
}

object::Ptr Object::Make(pipe::Id type, const void *args, std::size_t args_size) {
  // Find the constructor in the registry
  const Entry& entry = Registry::Get().GetEntry(type);
  return entry.make_(args, args_size);
}

Object::Object(const char* type_name_, object::MethodMap method_map) :
    id_(GetUniqueId()),
    type_(pipe::GetHash(type_name_)),
    method_map_(std::move(method_map)) {
}

Object::~Object() {
}

pipe::Id Object::GetId() const {
  return id_;
}

std::string Object::GetTypeName() const {
  // Find the type name in the registry
  const Entry& entry = Registry::Get().GetEntry(type_);
  return entry.type_name_;
}

void Object::Call(pipe::Id method_id, void *args, std::size_t args_size) {
  // Find the method
  object::MethodMap::const_iterator it = method_map_.find(method_id);
  if (it == method_map_.end())
    throw std::runtime_error("The provided method Id is not in the method map");

  // Call the method
  it->second(args, args_size);
}

}  // namespace server
}  // namepsace npe

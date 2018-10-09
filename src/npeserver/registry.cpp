// STL
#include <stdexcept>

// NPE SERVER private
#include "registry.hpp"

namespace npe {
namespace server {

Registry& Registry::Get() {
  static Registry registry_;
  return registry_;
}

Registry::Registry() :
    entry_map_(),
    mutex_() {
}

Registry::~Registry() {
}

void Registry::InsertEntry(pipe::Id entry_id, Entry entry) {
  std::unique_lock<std::mutex> lock(mutex_);

  // Check if the entry Id is already in the registry
  EntryMap::iterator it = entry_map_.find(entry_id);
  if (it != entry_map_.end())
    throw std::runtime_error("The provided entry Id is already registered");

  // Add the entry in the registry
  entry_map_[entry_id] = entry;
}

const Entry& Registry::GetEntry(pipe::Id entry_id) const {
  std::unique_lock<std::mutex> lock(mutex_);

  // Find the entry
  EntryMap::const_iterator it = entry_map_.find(entry_id);
  if (it == entry_map_.end())
    throw std::runtime_error("The provided entry Id is not registered");

  return it->second;
}

}  // namespace server
}  // namespace npe


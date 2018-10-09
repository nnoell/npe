#pragma once

// STL
#include <mutex>
#include <unordered_map>
#include <iostream>

// NPE PIPE public
#include <npe/pipe/id.hpp>
#include <npe/pipe/hash.hpp>

// NPE_SERVER private
#include "object/function.hpp"

namespace npe {
namespace server {

// The Entry Type for the registry
struct Entry {
  // The type name
  const char *type_name_;

  // The constructor
  object::Make make_;
};

// The Entry Map for the registry
using EntryMap = std::unordered_map<pipe::Id, Entry>;

// The Registry Class
class Registry {
 public:
  // Gets the registry
  static Registry& Get();

  // Register a class
  template<typename Class>
  static bool Register() {
    try {
      Registry::Get().InsertEntry<Class>();
      return true;
    } catch (std::runtime_error e) {
      std::cerr << "Error while registering '" << Class::TypeName << "': " << e.what() << std::endl;
      return false;
    } catch (...) {
      std::cerr << "Error while registering '" << Class::TypeName << "'" << std::endl;
      return false;
    }
  }

 public:
  // Constructor
  Registry();

  // Destructor
  virtual ~Registry();

  // Inserts a new entry in the registry
  // Remarks: Throws if the entry Id already exists
  void InsertEntry(pipe::Id entry_id, Entry entry);

  // Inserts a new entry in the registry
  // Remarks: Throws if the entry Id already exists
  template <typename Class>
  void InsertEntry() {
    InsertEntry(pipe::GetHash(Class::TypeName), {Class::TypeName, object::Factory<Class>});
  }

  // Gets the reference of an entry in the registry
  // Remarks: Throws if the entry was not found
  const Entry& GetEntry(pipe::Id entry_id) const;

 private:
  // Copy Constructor
  Registry(const Registry&) = delete;

  // Move Constructor
  Registry(Registry &&) = delete;

  // Copy-Assign Constructor
  Registry& operator=(const Registry&) = delete;

  // Move-Assign Constructr
  Registry& operator=(Registry &&) = delete;

 private:
  // The Registry Map
  EntryMap entry_map_;

  // The mutex to make the registry safe
  mutable std::mutex mutex_;
};

// Registers an object in the registry by doing static initialization
#define NPE_SERVER_REGISTER_OBJECT(Class) \
    static const bool npe_server_registry_is_ ## Class ## _registered = Registry::Register<Class>();

}  // namespace server
}  // namespace npe

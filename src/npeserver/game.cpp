// STL
#include <string>
#include <cstring>
#include <sstream>

// NPE PIPE public
#include <npe/pipe/hash.hpp>

// NPE SERVER public
#include <npe/server/game.hpp>

// NPE SERVER private
#include "object.hpp"
#include "registry.hpp"

namespace npe {
namespace server {

class Game final : public Object {
 public:
  // The registered type name
  static constexpr char const *TypeName = game::T_Name;

 public:
  // Constructor
  Game(const void *args, std::size_t args_size);

  // Destructor
  ~Game();

  // Gets a string representing the object
  std::string ToString() const override;

  // Gets the name
  void GetName(void *args, std::size_t args_size);

  // Sets the name
  void SetName(void *args, std::size_t args_size);

  // Gets the year
  void GetYear(void *args, std::size_t args_size);

  // Sets the year
  void SetYear(void *args, std::size_t args_size);

 private:
  // Copy Constructor
  Game(const Game&) = delete;

  // Move Constructor
  Game(Game &&) = delete;

  // Copy-Assign Constructor
  Game& operator=(const Game&) = delete;

  // Move-Assign Constructor
  Game& operator=(Game &&) = delete;

 private:
  // The name
  std::string name_;

  // The year
  int year_;
};

Game::Game(const void *args, std::size_t args_size) :
    Object(Game::TypeName, {
      // Register the methods
      {npe::pipe::GetHash(game::M_GetName), std::bind(&Game::GetName, this, std::placeholders::_1, std::placeholders::_2)},
      {npe::pipe::GetHash(game::M_SetName), std::bind(&Game::SetName, this, std::placeholders::_1, std::placeholders::_2)},
      {npe::pipe::GetHash(game::M_GetYear), std::bind(&Game::GetYear, this, std::placeholders::_1, std::placeholders::_2)},
      {npe::pipe::GetHash(game::M_SetYear), std::bind(&Game::SetYear, this, std::placeholders::_1, std::placeholders::_2)}
    }),
    name_(),
    year_() {
  if (args_size != sizeof(game::A_Constructor))
    throw std::runtime_error("Args are invalid for this constructor");

  // Set the values one args has been checked
  name_ = std::string(reinterpret_cast<const game::A_Constructor *>(args)->name_);
  year_ = reinterpret_cast<const game::A_Constructor *>(args)->year_;
}

Game::~Game() {
}

std::string Game::ToString() const {
  std::stringstream ss;
  ss << GetId() << ": [" << GetTypeName() << "] { name='" << name_ << "', year=" << year_ << " }";
  return ss.str();
}

void Game::GetName(void *args, std::size_t args_size) {
  if (args_size != sizeof(game::A_GetName))
    throw std::runtime_error("Args are invalid for this method");

  std::memcpy(reinterpret_cast<game::A_GetName *>(args)->name_, name_.c_str(), NPE_PIPE_BUFFER_MAX_ASCII_ARGS_SIZE);
}

void Game::SetName(void *args, std::size_t args_size) {
  if (args_size != sizeof(game::A_GetName))
    throw std::runtime_error("Args are invalid for this method");

  name_ = reinterpret_cast<game::A_GetName *>(args)->name_;
}

void Game::GetYear(void *args, std::size_t args_size) {
  if (args_size != sizeof(game::A_GetYear))
    throw std::runtime_error("Args are invalid for this method");

  reinterpret_cast<game::A_GetYear *>(args)->year_ = year_;
}

void Game::SetYear(void *args, std::size_t args_size) {
  if (args_size != sizeof(game::A_SetYear))
    throw std::runtime_error("Args are invalid for this method");

  year_ = reinterpret_cast<game::A_SetYear *>(args)->year_;
}

// Register the Game class
NPE_SERVER_REGISTER_OBJECT(Game)

}  // namespace server
}  // namespace npe

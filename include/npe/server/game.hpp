#pragma once

// NPE PIPE public
#include <npe/pipe/buffer/args.hpp>

namespace npe {
namespace server {
namespace game {

// The Type Name of the Game class
static constexpr char const *T_Name = "Game";

// The Methods of the Game class
static constexpr char const *M_GetName = "GetName";
static constexpr char const *M_SetName = "SetName";
static constexpr char const *M_GetYear = "GetYear";
static constexpr char const *M_SetYear = "SetYear";

// The Arguments of the methods
#pragma pack(push, 1)
struct A_Constructor {
  char name_[NPE_PIPE_BUFFER_MAX_ASCII_ARGS_SIZE];
  int year_;
};

struct A_GetName {
  char name_[NPE_PIPE_BUFFER_MAX_ASCII_ARGS_SIZE];
};

struct A_SetName {
  char name_[NPE_PIPE_BUFFER_MAX_ASCII_ARGS_SIZE];
};

struct A_GetYear {
  int year_;
};

struct A_SetYear {
  int year_;
};
#pragma pack(pop)

}  // namespace game
}  // namespace server
}  // namespace npe

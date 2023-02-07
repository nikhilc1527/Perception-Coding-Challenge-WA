#include "lib.hpp"

auto main() -> int
{
  auto const lib = library {};

  return lib.name == "perception" ? 0 : 1;
}

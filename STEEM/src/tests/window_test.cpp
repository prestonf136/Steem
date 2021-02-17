#include "../library/Window/Window.hpp"

int main()
{
  auto win = Steem::Window("Hola! soy dora",800,600, false);
  win.StartLoop();
  return 0;
};

#include <hexgon/hexgon.hpp>

using namespace Hexgon;

int main(int argc, const char** argv) {

  ApplicationSpecification spec;

  spec.commandLineArgs.Count = argc;
  spec.commandLineArgs.Args = (char**)argv;

  Application app{spec};

  app.Run();

  return 0;
}
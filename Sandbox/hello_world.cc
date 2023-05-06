
#include <hexgon/hexgon.hpp>

#include "example_layer.hpp"

#include "config.hpp"

using namespace Hexgon;

int main(int argc, const char** argv) {
  ApplicationSpecification spec;

  spec.commandLineArgs.Count = argc;
  spec.commandLineArgs.Args = (char**)argv;
  spec.workingDirectory = ASSETS_DIR;

  Application app{spec};

  if (!app.Init()) {
    HEX_ERROR("Hexgon app init failed!");
    return -1;
  }

  app.PushLayer(CreateRef<ExampleLayer>());

  app.Run();

  return 0;
}
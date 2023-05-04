
#include <hexgon/hexgon.hpp>

#include "example_layer.hpp"

using namespace Hexgon;

int main(int argc, const char** argv) {
  ApplicationSpecification spec;

  spec.commandLineArgs.Count = argc;
  spec.commandLineArgs.Args = (char**)argv;
  spec.workingDirectory = "../../Sandbox";

  Application app{spec};

  app.PushLayer(CreateRef<ExampleLayer>());

  app.Run();

  return 0;
}
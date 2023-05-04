#include "example_layer.hpp"

ExampleLayer::ExampleLayer() : Hexgon::Layer("ExampleLayer") {}

ExampleLayer::~ExampleLayer() {}

void ExampleLayer::OnAttach() { HEX_INFO("ExampleLayer OnAttach"); }

void ExampleLayer::OnDetach() { HEX_INFO("ExampleLlayer OnDetach"); }

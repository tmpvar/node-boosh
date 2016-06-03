#include <node.h>

#include "window.h"
#include "node-nanovg.h"

using namespace v8;
using namespace node;

NAN_MODULE_INIT(init) {
  assert(glfwInit());

  Window::Init(target);
  NodeNanovg::Init(target);
}

NODE_MODULE(boosh, init)

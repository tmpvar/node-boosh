#include <node.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "uv.h"
#include <context2d.h>
#include "window.h"

using namespace v8;
using namespace node;

NAN_MODULE_INIT(init) {
  assert(glfwInit());

  Window::Init(target);
  Context2D::Init(target);

}

NODE_MODULE(boosh, init)

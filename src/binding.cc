#include <node.h>
#include <node_buffer.h>
#include <v8.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "uv.h"
#include <Context2D.h>
#include "window.h"

using namespace v8;
using namespace node;

void init(Handle<Object> exports) {
  assert(glfwInit());

  Window::Init(exports);
  Context2D::Init(exports);

}

NODE_MODULE(boosh, init)
#define BUILDING_NODE_EXTENSION
#include <node.h>
#include <node_buffer.h>
#include <v8.h>
#include <GL/glew.h>
#include <GL/glfw3.h>
#include "uv.h"
#include "stdio.h"
#include <Canvas.h>
#include <PixelArray.h>
#include "window.h"

using namespace v8;
using namespace node;

GLuint  texture[1], tex;

Handle<Value> CreateWindow(const Arguments& args) {
  HandleScope scope;

  /* Initialize the library */
  if (!glfwInit()) {
    return scope.Close(Undefined());
  }



  int width = args[0]->NumberValue();
  int height = args[1]->NumberValue();

  /* Create a windowed mode window and its OpenGL context */
  GLFWwindow *window = glfwCreateWindow(width, height, "BOOSH!", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return scope.Close(Undefined());
  }

  glfwMakeContextCurrent(window);

  glewInit();

  glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glViewport(0,0,width,height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, width, 0, height);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  return scope.Close(External::Wrap(window));
}

struct Obj {
  int x;
};

Handle<Value> CreateThing(const Arguments& args) {
  HandleScope scope;
  Obj *o = new Obj;
  o->x = 10;
  return scope.Close(External::Wrap(o));
}

Handle<Value> UseThing(const Arguments& args) {
  HandleScope scope;
  Obj *o = reinterpret_cast<Obj *>(External::Unwrap(args[0]));
  return scope.Close(Number::New(o->x));
}


Handle<Value> Flush(const Arguments& args) {
  HandleScope scope;
  GLFWwindow * w = reinterpret_cast<GLFWwindow *>(External::Unwrap(args[0]));

  int width, height;
  glfwGetWindowSize(w, &width, &height);

  glfwMakeContextCurrent(w);

  Canvas *canvas = ObjectWrap::Unwrap<Canvas>(args[1]->ToObject());

  glGenTextures(1, &texture[0]);
  glBindTexture(GL_TEXTURE_2D, texture[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, canvas->data());
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture[0]);
  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
  glScalef(1.0f, -1.0f, 1.0f);
  glMatrixMode(GL_MODELVIEW);

  glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.0f, 0.0f,  0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( width, 0.0f,  0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( width,  height,  0);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f, height,  0);
  glEnd();

  glfwSwapBuffers(w);
  glfwPollEvents();
  glDeleteTextures(1, &texture[0]);
  return scope.Close(Undefined());
}

void init(Handle<Object> exports) {
  exports->Set(String::NewSymbol("createWindow"),
      FunctionTemplate::New(CreateWindow)->GetFunction());
  exports->Set(String::NewSymbol("flush"),
      FunctionTemplate::New(Flush)->GetFunction());

  assert(glfwInit());

  Window::Init(exports);

}

NODE_MODULE(boosh, init)
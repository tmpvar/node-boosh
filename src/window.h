#ifndef WINDOW_H
#define WINDOW_H

#define NODE_PROTOTYPE_METHOD(name) \
  tpl->PrototypeTemplate()->Set(String::NewSymbol(#name), \
       FunctionTemplate::New(name)->GetFunction()); \


#define OBJECT_METHOD(name) static Handle<Value> name(const Arguments& args);


#define GLFW_INCLUDE_GLU 1

#include <node.h>
#include <Context2D.h>
#include <GLFW/glfw3.h>
#include <uv.h>

using namespace v8;
using namespace node;

class Window : public ObjectWrap {
 public:
  static void Init(Handle<Object> exports);
  static Handle<Value> NewInstance(const Arguments& args);

  Context2D *ctx;
  Persistent<Object> canvasHandle;
  GLFWwindow *handle;
  int width, height, x, y;
  GLuint surfaceTexture[1];
  Persistent<Function> eventCallback;
  void setupSize();
  void destroy();
  void swapBuffers();

 private:
  Window(int width, int height, const char *title);
  ~Window();

  static uv_timer_t *input_timer;
  static int window_count;

  static Persistent<Function> constructor;

  static Handle<Value> New(const Arguments& args);

  OBJECT_METHOD(resizeTo)
  OBJECT_METHOD(moveTo)
  OBJECT_METHOD(getRect)
  OBJECT_METHOD(setContext2d)
  OBJECT_METHOD(flush)
  OBJECT_METHOD(eventHandler)
  OBJECT_METHOD(setTitle)
  OBJECT_METHOD(close)
  OBJECT_METHOD(focus)
  OBJECT_METHOD(blur)
};

#endif
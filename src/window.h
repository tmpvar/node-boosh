#define BUILDING_NODE_EXTENSION
#ifndef WINDOW_H
#define WINDOW_H

#define NODE_PROTOTYPE_METHOD(name) \
  tpl->PrototypeTemplate()->Set(String::NewSymbol(#name), \
       FunctionTemplate::New(name)->GetFunction()); \


#define OBJECT_METHOD(name) static Handle<Value> name(const Arguments& args);

#ifndef GLFW_INCLUDE_GLU
#define GLFW_INCLUDE_GLU 1
#endif

#include <node.h>
#include <Canvas.h>
#include <GL/glfw3.h>

using namespace v8;

class Window : public node::ObjectWrap {
 public:
  static void Init(Handle<Object> exports);
  static Handle<Value> NewInstance(const Arguments& args);

  Canvas *canvas;
  Persistent<Object> canvasHandle;
  GLFWwindow *window;
  int width, height;
  GLuint surfaceTexture[1];
  Persistent<Function> eventCallback;
  void setupSize();

 private:
  Window(int width, int height, const char *title);
  ~Window();




  static Persistent<Function> constructor;

  static Handle<Value> New(const Arguments& args);

  OBJECT_METHOD(resizeTo)
  OBJECT_METHOD(getRect)
  OBJECT_METHOD(get2dContext)
  OBJECT_METHOD(flush)
  OBJECT_METHOD(eventHandler)
};

#endif
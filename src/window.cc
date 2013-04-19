#define BUILDING_NODE_EXTENSION
#include <node.h>
#include "window.h"
#include <GL/glfw3.h>
#include <stdio.h>
using namespace v8;

//
// Event processors
//


void APIENTRY resizedCallback(GLFWwindow* window,int width,int height) {

  Window *win = (Window *)glfwGetWindowUserPointer(window);

  Local<Object> event = Object::New();
  event->Set(String::NewSymbol("type"), String::NewSymbol("resize"));
  event->Set(String::NewSymbol("width"), Number::New(width));
  event->Set(String::NewSymbol("height"), Number::New(height));

  win->width = width;
  win->height = height;

  const unsigned argc = 1;
  Local<Value> argv[argc] = { event };
  win->eventCallback->Call(Context::GetCurrent()->Global(), argc, argv);

  win->setupSize();

}





Window::Window(int width, int height, const char *title)
  : ObjectWrap()
{
  this->width = width;
  this->height = height;
  this->window = glfwCreateWindow(width, height, title, NULL, NULL);

  glfwSetWindowUserPointer(this->window, this);

  glfwSetWindowSizeCallback(this->window, &resizedCallback);

}

Window::~Window() {
  if (this->window) {
    glfwDestroyWindow(this->window);
    this->window = NULL;
  }
  glfwTerminate();
}



Persistent<Function> Window::constructor;

void Window::Init(Handle<Object> exports) {

  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("Window"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  NODE_PROTOTYPE_METHOD(resizeTo);
  NODE_PROTOTYPE_METHOD(getRect);
  NODE_PROTOTYPE_METHOD(get2dContext);
  NODE_PROTOTYPE_METHOD(flush);
  NODE_PROTOTYPE_METHOD(eventHandler);

  Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
  exports->Set(String::NewSymbol("Window"), constructor);

}

Handle<Value> Window::eventHandler(const Arguments& args) {
  HandleScope scope;

  Window *win = ObjectWrap::Unwrap<Window>(args.This());
  win->eventCallback = Persistent<Function>::New(Handle<Function>::Cast(args[0]));

  return scope.Close(Undefined());
}

void Window::setupSize() {

  glfwMakeContextCurrent(this->window);
  glViewport(0,0, this->width, this->height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, this->width, 0, this->height);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  this->canvas->width = this->width;
  this->canvas->height = this->height;
  this->canvas->resurface(this->canvasHandle);
}

Handle<Value> Window::get2dContext(const Arguments& args) {
  HandleScope scope;

  Window *win = ObjectWrap::Unwrap<Window>(args.This());

  win->canvasHandle =   Persistent<Object>::New(Handle<Object>::Cast(args[0]));
  win->canvas = ObjectWrap::Unwrap<Canvas>(args[0]->ToObject());

  win->setupSize();

  glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  return scope.Close(Undefined());
}



Handle<Value> Window::resizeTo(const Arguments& args) {
  HandleScope scope;

  Window *win = ObjectWrap::Unwrap<Window>(args.This());
  win->width = args[0]->NumberValue();
  win->height = args[1]->NumberValue();

  win->setupSize();

  return scope.Close(Undefined());
}

Handle<Value> Window::flush(const Arguments& args) {
  HandleScope scope;

  Window *win = ObjectWrap::Unwrap<Window>(args.This());

  glfwMakeContextCurrent(win->window);

  glGenTextures(1, &win->surfaceTexture[0]);
  glBindTexture(GL_TEXTURE_2D, win->surfaceTexture[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, win->width, win->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, win->canvas->data());
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, win->surfaceTexture[0]);
  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
  glScalef(1.0f, -1.0f, 1.0f);
  glMatrixMode(GL_MODELVIEW);

  glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.0f, 0.0f,  0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( win->width, 0.0f,  0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( win->width,  win->height,  0);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f, win->height,  0);
  glEnd();

  glfwSwapBuffers(win->window);
  glfwPollEvents();
  glDeleteTextures(1, &win->surfaceTexture[0]);


  return scope.Close(Undefined());
}

Handle<Value> Window::getRect(const Arguments& args) {
  HandleScope scope;

  Window *win = ObjectWrap::Unwrap<Window>(args.This());

  int width, height, x, y;
  glfwGetWindowSize(win->window, &width, &height);
  glfwGetWindowPos(win->window, &x, &y);

  Local<Object> ret = Object::New();
  ret->Set(String::NewSymbol("x"), Number::New(x));
  ret->Set(String::NewSymbol("y"), Number::New(y));
  ret->Set(String::NewSymbol("width"), Number::New(width));
  ret->Set(String::NewSymbol("height"), Number::New(height));

  return scope.Close(ret);
}


Handle<v8::Value> Window::New(const v8::Arguments& args) {
  HandleScope scope;

  int width = args[0]->ToInteger()->Value();
  int height = args[1]->ToInteger()->Value();

  String::Utf8Value titleString(args[2]->ToString());

  Window *window = new Window(width, height, *titleString);
  window->Wrap(args.This());

  return args.This();
}

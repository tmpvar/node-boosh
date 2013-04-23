#define BUILDING_NODE_EXTENSION
#include <node.h>
#include "window.h"
#include <GL/glfw3.h>
#include <stdio.h>
using namespace v8;

//
// Event processors
//

void uv_gui_idler(uv_idle_t* handle, int status) {
  glfwPollEvents();
}

void APIENTRY resizedCallback(GLFWwindow* window,int width,int height) {

  Window *win = (Window *)glfwGetWindowUserPointer(window);

  if (!win->handle) {
    return;
  }

  Local<Object> event = Object::New();
  event->Set(String::NewSymbol("type"), String::NewSymbol("resize"));
  event->Set(String::NewSymbol("objectType"), String::NewSymbol("Event"));
  event->Set(String::NewSymbol("width"), Number::New(width));
  event->Set(String::NewSymbol("height"), Number::New(height));
  event->Set(String::NewSymbol("_defaultPrevented"), Boolean::New(false));

  const unsigned argc = 1;
  Local<Value> argv[argc] = { event };
  win->eventCallback->Call(Context::GetCurrent()->Global(), argc, argv);

  if (!event->Get(String::NewSymbol("_defaultPrevented"))->BooleanValue()) {
    win->width = width;
    win->height = height;
    win->setupSize();
  } else {
    glfwSetWindowSize(win->handle, win->width, win->height);
    glfwSetWindowPos(win->handle, win->x, win->y);
  }
}

void APIENTRY movedCallback(GLFWwindow* window,int x,int y) {

  Window *win = (Window *)glfwGetWindowUserPointer(window);

  if (!win->handle) {
    return;
  }

  Local<Object> event = Object::New();
  event->Set(String::NewSymbol("type"), String::NewSymbol("move"));
  event->Set(String::NewSymbol("objectType"), String::NewSymbol("Event"));
  event->Set(String::NewSymbol("x"), Number::New(x));
  event->Set(String::NewSymbol("y"), Number::New(y));
  event->Set(String::NewSymbol("_defaultPrevented"), Boolean::New(false));

  const unsigned argc = 1;
  Local<Value> argv[argc] = { event };
  win->eventCallback->Call(Context::GetCurrent()->Global(), argc, argv);

  if (!event->Get(String::NewSymbol("_defaultPrevented"))->BooleanValue()) {
    win->x = x;
    win->y = y;
  } else {
    glfwSetWindowPos(win->handle, win->x, win->y);
  }
};


void APIENTRY closeCallback(GLFWwindow* window) {

  Window *win = (Window *)glfwGetWindowUserPointer(window);

  if (!win->handle) {
    return;
  }

  Local<Object> event = Object::New();
  event->Set(String::NewSymbol("objectType"), String::NewSymbol("Event"));
  event->Set(String::NewSymbol("type"), String::NewSymbol("close"));
  event->Set(String::NewSymbol("_defaultPrevented"), Boolean::New(false));

  const unsigned argc = 1;
  Local<Value> argv[argc] = { event };
  win->eventCallback->Call(Context::GetCurrent()->Global(), argc, argv);

  bool closePrevented = event->Get(String::NewSymbol("_defaultPrevented"))->BooleanValue();

  if (!closePrevented) {
    win->destroy();
  }
};


void APIENTRY focusCallback(GLFWwindow* window, int hasFocus) {

  Window *win = (Window *)glfwGetWindowUserPointer(window);

  if (!win->handle) {
    return;
  }

  Local<Object> event = Object::New();
  event->Set(String::NewSymbol("objectType"), String::NewSymbol("Event"));
  if (hasFocus) {
    event->Set(String::NewSymbol("type"), String::NewSymbol("focus"));
  } else {
    event->Set(String::NewSymbol("type"), String::NewSymbol("blur"));
  }

  const unsigned argc = 1;
  Local<Value> argv[argc] = { event };
  win->eventCallback->Call(Context::GetCurrent()->Global(), argc, argv);
};

void APIENTRY mouseMoveCallback(GLFWwindow* window, double x, double y) {

  Window *win = (Window *)glfwGetWindowUserPointer(window);

  if (!win->handle) {
    return;
  }

  Local<Object> event = Object::New();
  event->Set(String::NewSymbol("type"), String::NewSymbol("mousemove"));
  event->Set(String::NewSymbol("objectType"), String::NewSymbol("MouseEvent"));
  event->Set(String::NewSymbol("x"), Number::New(x));
  event->Set(String::NewSymbol("y"), Number::New(y));


  const unsigned argc = 1;
  Local<Value> argv[argc] = { event };
  win->eventCallback->Call(Context::GetCurrent()->Global(), argc, argv);
};

void APIENTRY mouseEnterExitCallback(GLFWwindow* window, int inside) {

  Window *win = (Window *)glfwGetWindowUserPointer(window);

  if (!win->handle) {
    return;
  }

  Local<Object> event = Object::New();

  if (inside) {
    event->Set(String::NewSymbol("type"), String::NewSymbol("mouseenter"));
  } else {
    event->Set(String::NewSymbol("type"), String::NewSymbol("mouseleave"));
  }
  event->Set(String::NewSymbol("objectType"), String::NewSymbol("MouseEvent"));

  double x, y;
  glfwGetCursorPos(win->handle, &x, &y);
  event->Set(String::NewSymbol("x"), Number::New(x));
  event->Set(String::NewSymbol("y"), Number::New(y));

  const unsigned argc = 1;
  Local<Value> argv[argc] = { event };
  win->eventCallback->Call(Context::GetCurrent()->Global(), argc, argv);
};


void APIENTRY mouseButtonCallback(GLFWwindow* window, int button, int pressed) {

  Window *win = (Window *)glfwGetWindowUserPointer(window);

  if (!win->handle) {
    return;
  }

  Local<Object> event = Object::New();

  switch (pressed) {
    case GLFW_PRESS:
      event->Set(String::NewSymbol("type"), String::NewSymbol("mousedown"));
    break;

    case GLFW_RELEASE:
      event->Set(String::NewSymbol("type"), String::NewSymbol("mouseup"));
    break;
  }

  switch (button) {
    // left
    case GLFW_MOUSE_BUTTON_1:
      event->Set(String::NewSymbol("button"), Number::New(0));
    break;

    // middle
    case GLFW_MOUSE_BUTTON_3:
      event->Set(String::NewSymbol("button"), Number::New(1));
    break;

    // right
    case GLFW_MOUSE_BUTTON_2:
      event->Set(String::NewSymbol("button"), Number::New(2));
    break;

    // handle the other buttons
    default:
      event->Set(String::NewSymbol("button"), Number::New(button));
    break;
  }

  event->Set(String::NewSymbol("objectType"), String::NewSymbol("MouseEvent"));

  double x, y;
  glfwGetCursorPos(win->handle, &x, &y);
  event->Set(String::NewSymbol("x"), Number::New(x));
  event->Set(String::NewSymbol("y"), Number::New(y));

  const unsigned argc = 1;
  Local<Value> argv[argc] = { event };
  win->eventCallback->Call(Context::GetCurrent()->Global(), argc, argv);
};

void APIENTRY keyboardKeyCallback(GLFWwindow* window, int key, int pressed) {

  Window *win = (Window *)glfwGetWindowUserPointer(window);

  if (!win->handle || key == GLFW_KEY_RIGHT_SHIFT || key == GLFW_KEY_LEFT_SHIFT) {
    return;
  }

  Local<Object> event = Object::New();

  switch (pressed) {
    case GLFW_PRESS:
      event->Set(String::NewSymbol("type"), String::NewSymbol("keyup"));
    break;

    case GLFW_RELEASE:
      event->Set(String::NewSymbol("type"), String::NewSymbol("keydown"));
    break;

    case GLFW_REPEAT:
      event->Set(String::NewSymbol("type"), String::NewSymbol("keydown"));
    break;
  }

  int shift = glfwGetKey(win->handle, GLFW_KEY_RIGHT_SHIFT) ||
              glfwGetKey(win->handle, GLFW_KEY_RIGHT_SHIFT);

  int meta =  glfwGetKey(win->handle, GLFW_KEY_LEFT_SUPER) ||
              glfwGetKey(win->handle, GLFW_KEY_RIGHT_SUPER);

  int alt = glfwGetKey(win->handle, GLFW_KEY_LEFT_ALT) ||
            glfwGetKey(win->handle, GLFW_KEY_LEFT_ALT);

  int control = glfwGetKey(win->handle, GLFW_KEY_RIGHT_CONTROL) ||
                glfwGetKey(win->handle, GLFW_KEY_RIGHT_CONTROL);

  int capsLock =  glfwGetKey(win->handle, GLFW_KEY_CAPS_LOCK);

  if (key >= 65 && key<=90) {
    if (capsLock) {
      if(shift) {
        key+=32;
      }
    } else if (!shift) {
      key+=32;
    }
  }

  if (key == GLFW_KEY_ESCAPE) {
    key = 27;
  }

  event->Set(String::NewSymbol("keyCode"), Number::New(key));

  event->Set(String::NewSymbol("ctrlKey"), Boolean::New(control));
  event->Set(String::NewSymbol("shiftKey"), Boolean::New(shift));
  event->Set(String::NewSymbol("altKey"), Boolean::New(alt));
  event->Set(String::NewSymbol("metaKEy"), Boolean::New(meta));

  // TODO: KeyboardEvent.location

  event->Set(String::NewSymbol("objectType"), String::NewSymbol("KeyboardEvent"));

  const unsigned argc = 1;
  Local<Value> argv[argc] = { event };
  win->eventCallback->Call(Context::GetCurrent()->Global(), argc, argv);
};


uv_idle_t * Window::idler = NULL;

Window::Window(int width, int height, const char *title)
  : ObjectWrap()
{
  this->width = width;
  this->height = height;
  this->handle = glfwCreateWindow(width, height, title, NULL, NULL);

  glfwGetWindowPos(this->handle, &this->x, &this->y);

  glfwSetWindowUserPointer(this->handle, this);

  glfwSetWindowSizeCallback(this->handle, &resizedCallback);
  glfwSetWindowPosCallback(this->handle, &movedCallback);
  glfwSetWindowCloseCallback(this->handle, &closeCallback);
  glfwSetWindowFocusCallback(this->handle, &focusCallback);
  glfwSetMouseButtonCallback(this->handle, &mouseButtonCallback);
  // Mouse
  glfwSetCursorPosCallback(this->handle, &mouseMoveCallback);
  glfwSetCursorEnterCallback(this->handle, &mouseEnterExitCallback);

  // Keyboard
  glfwSetKeyCallback(this->handle, &keyboardKeyCallback);



  if (Window::idler == NULL) {
    // TODO: how to not leak?
    Window::idler = (uv_idle_t *)malloc(sizeof(uv_idle_t));
    uv_idle_init(uv_default_loop(), Window::idler);
    uv_idle_start(Window::idler, uv_gui_idler);
  }
}

Window::~Window() {
  // TODO: this shuts everything down which will
  //
  glfwTerminate();
}

void Window::destroy() {
  if (this->handle) {
    glfwDestroyWindow(this->handle);
    this->handle = NULL;

    uv_idle_stop(Window::idler);
  }
}

Persistent<Function> Window::constructor;

void Window::Init(Handle<Object> exports) {

  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("Window"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  NODE_PROTOTYPE_METHOD(resizeTo);
  NODE_PROTOTYPE_METHOD(moveTo);
  NODE_PROTOTYPE_METHOD(getRect);

  NODE_PROTOTYPE_METHOD(get2dContext);
  NODE_PROTOTYPE_METHOD(flush);

  NODE_PROTOTYPE_METHOD(eventHandler);
  NODE_PROTOTYPE_METHOD(setTitle);
  NODE_PROTOTYPE_METHOD(close);


  Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
  exports->Set(String::NewSymbol("Window"), constructor);

}

Handle<Value> Window::eventHandler(const Arguments& args) {
  HandleScope scope;

  Window *win = ObjectWrap::Unwrap<Window>(args.This());

  win->eventCallback = Persistent<Function>::New(Handle<Function>::Cast(args[0]));

  return scope.Close(Undefined());
}


Handle<Value> Window::setTitle(const Arguments& args) {
  HandleScope scope;
  Window *win = ObjectWrap::Unwrap<Window>(args.This());

  String::Utf8Value title(args[0]->ToString());
  if (win->handle) {
    glfwSetWindowTitle(win->handle, *title);
  }

  return scope.Close(Undefined());
}

Handle<Value> Window::close(const Arguments& args) {
  HandleScope scope;
  Window *win = ObjectWrap::Unwrap<Window>(args.This());

  if (win->handle) {
    glfwSetWindowShouldClose(win->handle, 1);

    win->destroy();
  }

  return scope.Close(Undefined());
}

void Window::setupSize() {
  if (this->handle) {
    glfwMakeContextCurrent(this->handle);

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
}

Handle<Value> Window::get2dContext(const Arguments& args) {
  HandleScope scope;

  Window *win = ObjectWrap::Unwrap<Window>(args.This());

  if (win->handle) {

    win->canvasHandle =   Persistent<Object>::New(Handle<Object>::Cast(args[0]));
    win->canvas = ObjectWrap::Unwrap<Canvas>(args[0]->ToObject());

    win->setupSize();

    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  }
  return scope.Close(Undefined());
}



Handle<Value> Window::resizeTo(const Arguments& args) {
  HandleScope scope;

  Window *win = ObjectWrap::Unwrap<Window>(args.This());

  win->width = args[0]->NumberValue();
  win->height = args[1]->NumberValue();

  if (win->handle) {
    glfwSetWindowSize(win->handle, win->width, win->height);
  }

  win->setupSize();

  return scope.Close(Undefined());
}

Handle<Value> Window::moveTo(const Arguments& args) {
  HandleScope scope;

  Window *win = ObjectWrap::Unwrap<Window>(args.This());

  win->x = args[0]->NumberValue();
  win->y = args[1]->NumberValue();

  if (win->handle) {
    glfwSetWindowPos(win->handle, win->x, win->y);
  }

  return scope.Close(Undefined());
}

void Window::swapBuffers() {
  if (this->handle) {
    glfwMakeContextCurrent(this->handle);
    glGenTextures(1, &this->surfaceTexture[0]);
    glBindTexture(GL_TEXTURE_2D, this->surfaceTexture[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, this->width, this->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, this->canvas->data());
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, this->surfaceTexture[0]);
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glScalef(1.0f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);

    glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 0.0f); glVertex3f( 0.0f, 0.0f,  0);
      glTexCoord2f(1.0f, 0.0f); glVertex3f( this->width, 0.0f,  0);
      glTexCoord2f(1.0f, 1.0f); glVertex3f( this->width,  this->height,  0);
      glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f, this->height,  0);
    glEnd();

    if (this->handle) {
      glfwSwapBuffers(this->handle);
    }
    glDeleteTextures(1, &this->surfaceTexture[0]);
  }
}

Handle<Value> Window::flush(const Arguments& args) {
  HandleScope scope;
  Window *win = ObjectWrap::Unwrap<Window>(args.This());
  glfwPollEvents();
  win->swapBuffers();
  return scope.Close(Undefined());
}

Handle<Value> Window::getRect(const Arguments& args) {
  HandleScope scope;

  Window *win = ObjectWrap::Unwrap<Window>(args.This());
  if (win->handle) {
    int width, height, x, y;
    glfwGetWindowSize(win->handle, &width, &height);
    glfwGetWindowPos(win->handle, &x, &y);

    Local<Object> ret = Object::New();
    ret->Set(String::NewSymbol("x"), Number::New(x));
    ret->Set(String::NewSymbol("y"), Number::New(y));
    ret->Set(String::NewSymbol("width"), Number::New(width));
    ret->Set(String::NewSymbol("height"), Number::New(height));

    return scope.Close(ret);
  } else {
    return scope.Close(Undefined());
  }
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

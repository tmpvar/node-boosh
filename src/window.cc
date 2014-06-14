#include <node.h>

#include "window.h"
#include <GLFW/glfw3.h>

using namespace v8;
using namespace node;
//
// Event processors
//

void uv_gui_idler(uv_timer_t* timer, int status) {
  glfwPollEvents();
}

void APIENTRY refreshCallback(GLFWwindow* window) {
  Window *win = (Window *)glfwGetWindowUserPointer(window);

  if (!win->handle) {
    return;
  }

  win->swapBuffers();
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


void APIENTRY mouseButtonCallback(GLFWwindow* window, int button, int pressed, int mods) {

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

void APIENTRY keyboardKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

  Window *win = (Window *)glfwGetWindowUserPointer(window);

  if (!win->handle || key == GLFW_KEY_RIGHT_SHIFT || key == GLFW_KEY_LEFT_SHIFT) {
    return;
  }

  Local<Object> event = Object::New();

  bool repeat = false;

  switch (action) {
    case GLFW_PRESS:
      event->Set(String::NewSymbol("type"), String::NewSymbol("keydown"));
    break;

    case GLFW_RELEASE:
      event->Set(String::NewSymbol("type"), String::NewSymbol("keyup"));
    break;

    case GLFW_REPEAT:
      event->Set(String::NewSymbol("type"), String::NewSymbol("keydown"));
      repeat = true;
    break;
  }

  bool shift = glfwGetKey(win->handle, GLFW_KEY_RIGHT_SHIFT) ||
              glfwGetKey(win->handle, GLFW_KEY_RIGHT_SHIFT);

  bool meta =  glfwGetKey(win->handle, GLFW_KEY_LEFT_SUPER) ||
              glfwGetKey(win->handle, GLFW_KEY_RIGHT_SUPER);

  bool alt = glfwGetKey(win->handle, GLFW_KEY_LEFT_ALT) ||
            glfwGetKey(win->handle, GLFW_KEY_LEFT_ALT);

  bool control = glfwGetKey(win->handle, GLFW_KEY_RIGHT_CONTROL) ||
                glfwGetKey(win->handle, GLFW_KEY_RIGHT_CONTROL);

  bool capsLock =  glfwGetKey(win->handle, GLFW_KEY_CAPS_LOCK) == 1;

  int location = 0;

  switch(key) {
    case GLFW_KEY_ESCAPE: key = 27; break;
    case GLFW_KEY_ENTER: key = 13; break;
    case GLFW_KEY_TAB: key = 9; break;
    case GLFW_KEY_BACKSPACE: key = 8; break;
    case GLFW_KEY_INSERT: key = 45; break;
    case GLFW_KEY_DELETE: key = 46; break;
    case GLFW_KEY_RIGHT: key = 39; break;
    case GLFW_KEY_LEFT: key = 37; break;
    case GLFW_KEY_DOWN: key = 40; break;
    case GLFW_KEY_UP: key = 38; break;
    case GLFW_KEY_PAGE_UP: key = 33; break;
    case GLFW_KEY_PAGE_DOWN: key = 34; break;
    case GLFW_KEY_HOME: key = 36; break;
    case GLFW_KEY_END: key = 35; break;
    case GLFW_KEY_CAPS_LOCK: key = 20; break;
    case GLFW_KEY_SCROLL_LOCK: key = 145; break;
    case GLFW_KEY_NUM_LOCK: key = 144; break;
    case GLFW_KEY_PRINT_SCREEN: key = 44; break;
    case GLFW_KEY_PAUSE: key = 19; break;
    case GLFW_KEY_F1: key = 112; break;
    case GLFW_KEY_F2: key = 113; break;
    case GLFW_KEY_F3: key = 114; break;
    case GLFW_KEY_F4: key = 115; break;
    case GLFW_KEY_F5: key = 116; break;
    case GLFW_KEY_F6: key = 117; break;
    case GLFW_KEY_F7: key = 118; break;
    case GLFW_KEY_F8: key = 119; break;
    case GLFW_KEY_F9: key = 120; break;
    case GLFW_KEY_F10: key = 121; break;
    case GLFW_KEY_F11: key = 122; break;
    case GLFW_KEY_F12: key = 123; break;
    case GLFW_KEY_F13: key = 124; break;
    case GLFW_KEY_F14: key = 125; break;
    case GLFW_KEY_F15: key = 126; break;
    case GLFW_KEY_F16: key = 127; break;
    case GLFW_KEY_F17: key = 128; break;
    case GLFW_KEY_F18: key = 129; break;
    case GLFW_KEY_F19: key = 130; break;
    case GLFW_KEY_F20: key = 131; break;
    case GLFW_KEY_F21: key = 132; break;
    case GLFW_KEY_F22: key = 133; break;
    case GLFW_KEY_F23: key = 134; break;
    case GLFW_KEY_F24: key = 135; break;
    case GLFW_KEY_F25: key = 136; break;
    case GLFW_KEY_KP_0: key = 96; location=3; break;
    case GLFW_KEY_KP_1: key = 97; location=3; break;
    case GLFW_KEY_KP_2: key = 98; location=3; break;
    case GLFW_KEY_KP_3: key = 99; location=3; break;
    case GLFW_KEY_KP_4: key = 100; location=3; break;
    case GLFW_KEY_KP_5: key = 101; location=3; break;
    case GLFW_KEY_KP_6: key = 102; location=3; break;
    case GLFW_KEY_KP_7: key = 103; location=3; break;
    case GLFW_KEY_KP_8: key = 104; location=3; break;
    case GLFW_KEY_KP_9: key = 105; location=3; break;
    case GLFW_KEY_KP_DECIMAL: key = 110; location=3; break;
    case GLFW_KEY_KP_DIVIDE: key = 111; location=3; break;
    case GLFW_KEY_KP_MULTIPLY: key = 106; location=3; break;
    case GLFW_KEY_KP_SUBTRACT: key = 109; location=3; break;
    case GLFW_KEY_KP_ADD: key = 107; location=3; break;
    case GLFW_KEY_KP_ENTER: key = 13; location=3; break;
    case GLFW_KEY_KP_EQUAL: key = 187; location=3; break;
    case GLFW_KEY_LEFT_SHIFT: key = 16; location=1; break;
    case GLFW_KEY_LEFT_CONTROL: key = 17; location=1; break;
    case GLFW_KEY_LEFT_ALT: key = 18; location=1; break;
    case GLFW_KEY_LEFT_SUPER: key = 91; location=1; break;
    case GLFW_KEY_RIGHT_SHIFT: key = 16; location=2; break;
    case GLFW_KEY_RIGHT_CONTROL: key = 17; location=2; break;
    case GLFW_KEY_RIGHT_ALT: key = 18; location=2; break;
    case GLFW_KEY_RIGHT_SUPER: key = 92; location=2; break;
    case GLFW_KEY_MENU: key = 18; break;
    case GLFW_KEY_LAST: key = 0; break
    case GLFW_KEY_M: key = 0; break;
  }

  event->Set(String::NewSymbol("keyCode"), Integer::New(key));
  event->Set(String::NewSymbol("which"), Integer::New(key));
  // event->Set(String::NewSymbol("keyIdentifier"), glfw);

  event->Set(String::NewSymbol("ctrlKey"), Boolean::New(control));
  event->Set(String::NewSymbol("shiftKey"), Boolean::New(shift));
  event->Set(String::NewSymbol("altKey"), Boolean::New(alt));
  event->Set(String::NewSymbol("metaKey"), Boolean::New(meta));
  event->Set(String::NewSymbol("repeat"), Boolean::New(repeat));
  event->Set(String::NewSymbol("location"), Integer::New(location));

  event->Set(String::NewSymbol("objectType"), String::NewSymbol("KeyboardEvent"));

  const unsigned argc = 1;
  Local<Value> argv[argc] = { event };
  win->eventCallback->Call(Context::GetCurrent()->Global(), argc, argv);
};


uv_timer_t * Window::input_timer = NULL;
int Window::window_count = 0;

Window::Window(int width, int height, const char *title, bool fullscreen)
  : ObjectWrap()
{
  this->width = width;
  this->height = height;

  this->handle = glfwCreateWindow(
    width,
    height,
    title,
    fullscreen ? glfwGetPrimaryMonitor() : NULL,
    NULL
  );

  Window::window_count++;

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

  if (Window::input_timer == NULL) {
    // TODO: how to not leak?
    Window::input_timer = (uv_timer_t *)malloc(sizeof(uv_timer_t));
    uv_timer_init(uv_default_loop(), Window::input_timer);
    uv_timer_start(
      Window::input_timer,
      uv_gui_idler,
      10,
      5
    );
  }
}

Window::~Window() {
  if (Window::window_count <= 0) {
    glfwTerminate();
  }
}

void Window::destroy() {
  if (this->handle) {
    glfwDestroyWindow(this->handle);
    this->handle = NULL;
  }

  Window::window_count--;
  if (Window::window_count <= 0) {
    uv_timer_stop(Window::input_timer);
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

  NODE_PROTOTYPE_METHOD(setContext2d);
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

    this->ctx->resizeCanvas(this->width, this->height);
  }
}

Handle<Value> Window::setContext2d(const Arguments& args) {
  HandleScope scope;

  Window *win = ObjectWrap::Unwrap<Window>(args.This());

  if (win->handle) {
    win->ctx = ObjectWrap::Unwrap<Context2D>(args[0]->ToObject());
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

  win->width = args[0]->Int32Value();
  win->height = args[1]->Int32Value();

  if (win->handle) {
    glfwSetWindowSize(win->handle, win->width, win->height);
  }

  win->setupSize();

  return scope.Close(Undefined());
}

Handle<Value> Window::moveTo(const Arguments& args) {
  HandleScope scope;

  Window *win = ObjectWrap::Unwrap<Window>(args.This());

  win->x = args[0]->Int32Value();
  win->y = args[1]->Int32Value();

  if (win->handle) {
    glfwSetWindowPos(win->handle, win->x, win->y);
  }

  return scope.Close(Undefined());
}

void Window::swapBuffers() {
  if (this->handle) {
    glfwMakeContextCurrent(this->handle);

    glBindTexture(GL_TEXTURE_2D, this->surfaceTexture[0]);

    void *data = this->ctx->getTextureData();
    glTexImage2D(GL_TEXTURE_2D, 0, 3, this->width, this->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);

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
      glTexCoord2f(1.0f, 0.0f); glVertex3f( (float)this->width, 0.0f,  0);
      glTexCoord2f(1.0f, 1.0f); glVertex3f( (float)this->width,  (float)this->height,  0);
      glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f, (float)this->height,  0);
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


Handle<Value> Window::New(const Arguments& args) {
  HandleScope scope;

  int width = args[0]->Int32Value();
  int height = args[1]->Int32Value();

  String::Utf8Value titleString(args[2]->ToString());

  Window *window = new Window(
    width,
    height,
    *titleString,
    args[3]->BooleanValue()
  );
  window->Wrap(args.This());

  return args.This();
}

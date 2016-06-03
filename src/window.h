#ifndef WINDOW_H
#define WINDOW_H

#include <node.h>
#include <nan.h>
#include <uv.h>

#define GLFW_INCLUDE_GLU 1

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifdef _WIN32
#include <GL/glu.h>
#endif

class Window : public Nan::ObjectWrap {
 public:
  static v8::Handle<v8::Value> NewInstance(const Nan::FunctionCallbackInfo<v8::Value>& info);

  v8::Handle<v8::Object> canvasHandle;
  GLFWwindow *handle;
  int width, height, x, y;
  GLuint surfaceTexture[1];
  Nan::Callback *eventCallback;
  bool hasEventHandler;
  bool frameStarted;
  void setupSize();
  void destroy();
  void swapBuffers();

  static NAN_MODULE_INIT(Init) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New<v8::String>("Window").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(tpl, "resizeTo", ResizeTo);
    Nan::SetPrototypeMethod(tpl, "moveTo", MoveTo);
    Nan::SetPrototypeMethod(tpl, "getRect", GetRect);
    Nan::SetPrototypeMethod(tpl, "beginFrame", BeginFrame);
    Nan::SetPrototypeMethod(tpl, "endFrame", EndFrame);
    Nan::SetPrototypeMethod(tpl, "eventHandler", EventHandler);
    Nan::SetPrototypeMethod(tpl, "setTitle", SetTitle);
    Nan::SetPrototypeMethod(tpl, "close", Close);

    Nan::Set(
      target,
      Nan::New("Window").ToLocalChecked(),
      Nan::GetFunction(tpl).ToLocalChecked()
    );
  }

 private:
  Window(int width, int height, const char *title, bool fullscreen);
  ~Window();

  static uv_timer_t *input_timer;
  static int window_count;

  static NAN_METHOD(New);
  static NAN_METHOD(ResizeTo);
  static NAN_METHOD(MoveTo);
  static NAN_METHOD(GetRect);
  static NAN_METHOD(SetContext2d);
  static NAN_METHOD(BeginFrame);
  static NAN_METHOD(EndFrame);
  static NAN_METHOD(EventHandler);
  static NAN_METHOD(SetTitle);
  static NAN_METHOD(Close);
  static NAN_METHOD(Focus);
  static NAN_METHOD(Blur);
};

#endif

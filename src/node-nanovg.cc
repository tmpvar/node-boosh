#include <nan.h>

#define GLFW_INCLUDE_GLU 1

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>

#include "node-nanovg.h"

#define NANOVG_GL3_IMPLEMENTATION

#include "nanovg_gl.h"

#define TAU 6.283185307179586

NodeNanovg::NodeNanovg() {
  this->ctx = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);

  if (this->ctx == NULL) {
    printf("Could not init nanovg.\n");
  }
}

NAN_METHOD(NodeNanovg::New) {
  NodeNanovg *handle = new NodeNanovg();
  handle->Wrap(info.Holder());
  info.GetReturnValue().Set(info.Holder());
}

NAN_METHOD(NodeNanovg::BeginFrame) {
  NodeNanovg *handle = ObjectWrap::Unwrap<NodeNanovg>(info.This());

  int width = info[0]->Int32Value();
  int height = info[1]->Int32Value();
  float pixelRatio = info[2]->NumberValue();

  nvgBeginFrame(handle->ctx, width, height, pixelRatio);
}

NAN_METHOD(NodeNanovg::CancelFrame) {
  NodeNanovg *handle = ObjectWrap::Unwrap<NodeNanovg>(info.This());
  nvgCancelFrame(handle->ctx);
}

NAN_METHOD(NodeNanovg::EndFrame) {
  NodeNanovg *handle = ObjectWrap::Unwrap<NodeNanovg>(info.This());
  nvgEndFrame(handle->ctx);
}

NAN_METHOD(NodeNanovg::Save) {
  NodeNanovg *handle = ObjectWrap::Unwrap<NodeNanovg>(info.This());
  nvgSave(handle->ctx);
}

NAN_METHOD(NodeNanovg::Restore) {
  NodeNanovg *handle = ObjectWrap::Unwrap<NodeNanovg>(info.This());
  nvgRestore(handle->ctx);
}

NAN_METHOD(NodeNanovg::Reset) {
  NodeNanovg *handle = ObjectWrap::Unwrap<NodeNanovg>(info.This());
  nvgReset(handle->ctx);
}

NAN_METHOD(NodeNanovg::StrokeColor) {
  NodeNanovg *handle = ObjectWrap::Unwrap<NodeNanovg>(info.This());
  unsigned char r = info[0]->Uint32Value();
  unsigned char g = info[1]->Uint32Value();
  unsigned char b = info[2]->Uint32Value();
  unsigned char a = info[3]->Uint32Value();

  nvgStrokeColor(handle->ctx, nvgRGBA(r, g, b, a));
}

NAN_METHOD(NodeNanovg::FillColor) {
  NodeNanovg *handle = ObjectWrap::Unwrap<NodeNanovg>(info.This());
  unsigned char r = info[0]->Uint32Value();
  unsigned char g = info[1]->Uint32Value();
  unsigned char b = info[2]->Uint32Value();
  unsigned char a = info[3]->Uint32Value();

  nvgFillColor(handle->ctx, nvgRGBA(r, g, b, a));
}

NAN_METHOD(NodeNanovg::StrokeWidth) {
  NodeNanovg *handle = ObjectWrap::Unwrap<NodeNanovg>(info.This());

  float size = info[0]->NumberValue();

  nvgStrokeWidth(handle->ctx, size);
}

NAN_METHOD(NodeNanovg::MiterLimit) {
  NodeNanovg *handle = ObjectWrap::Unwrap<NodeNanovg>(info.This());

  float limit = info[0]->NumberValue();

  nvgMiterLimit(handle->ctx, limit);
}

NAN_METHOD(NodeNanovg::SkewX) {
  NodeNanovg *handle = ObjectWrap::Unwrap<NodeNanovg>(info.This());

  float angle = info[0]->NumberValue();

  nvgSkewX(handle->ctx, angle);
}

NAN_METHOD(NodeNanovg::SkewY) {
  NodeNanovg *handle = ObjectWrap::Unwrap<NodeNanovg>(info.This());

  float angle = info[0]->NumberValue();

  nvgSkewY(handle->ctx, angle);
}

NAN_METHOD(NodeNanovg::GlobalAlpha) {
  NodeNanovg *handle = ObjectWrap::Unwrap<NodeNanovg>(info.This());

  float alpha = info[0]->NumberValue();

  nvgGlobalAlpha(handle->ctx, alpha);
}

NAN_METHOD(NodeNanovg::ResetTransform) {
  NodeNanovg *handle = ObjectWrap::Unwrap<NodeNanovg>(info.This());
  nvgResetTransform(handle->ctx);
}

NAN_METHOD(NodeNanovg::Translate) {
  NodeNanovg *handle = ObjectWrap::Unwrap<NodeNanovg>(info.This());

  float x = info[0]->NumberValue();
  float y = info[1]->NumberValue();

  nvgTranslate(handle->ctx, x, y);
}

NAN_METHOD(NodeNanovg::Scale) {
  NodeNanovg *handle = ObjectWrap::Unwrap<NodeNanovg>(info.This());

  float sx = info[0]->NumberValue();
  float sy = info[1]->NumberValue();

  nvgScale(handle->ctx, sx, sy);
}

NAN_METHOD(NodeNanovg::CurrentTransform) {
  NodeNanovg *handle = ObjectWrap::Unwrap<NodeNanovg>(info.This());

  v8::Handle<v8::Float32Array> array = info[0].As<v8::Float32Array>();
  v8::Local<v8::ArrayBuffer> buffer = array->Buffer();
  v8::ArrayBuffer::Contents contents = buffer->GetContents();
  float *mat3x3 = static_cast<float *>(contents.Data());

  nvgCurrentTransform(handle->ctx, mat3x3);
}

NAN_METHOD(NodeNanovg::Rotate) {
  NodeNanovg *handle = ObjectWrap::Unwrap<NodeNanovg>(info.This());

  float angle = info[0]->NumberValue();

  nvgRotate(handle->ctx, angle);
}

NAN_METHOD(NodeNanovg::BeginPath) {
  NodeNanovg *handle = ObjectWrap::Unwrap<NodeNanovg>(info.This());
  nvgBeginPath(handle->ctx);
}

NAN_METHOD(NodeNanovg::MoveTo) {
  NodeNanovg *handle = ObjectWrap::Unwrap<NodeNanovg>(info.This());

  float x = info[0]->NumberValue();
  float y = info[1]->NumberValue();

  nvgMoveTo(handle->ctx, x, y);
}

NAN_METHOD(NodeNanovg::LineTo) {
  NodeNanovg *handle = ObjectWrap::Unwrap<NodeNanovg>(info.This());

  float x = info[0]->NumberValue();
  float y = info[1]->NumberValue();

  nvgLineTo(handle->ctx, x, y);
}

NAN_METHOD(NodeNanovg::BezierTo) {
  NodeNanovg *handle = ObjectWrap::Unwrap<NodeNanovg>(info.This());

  float c1x = info[0]->NumberValue();
  float c1y = info[1]->NumberValue();
  float c2x = info[2]->NumberValue();
  float c2y = info[3]->NumberValue();
  float x = info[4]->NumberValue();
  float y = info[5]->NumberValue();

  nvgBezierTo(handle->ctx, c1x, c1y, c2x, c2y, x, y);
}

NAN_METHOD(NodeNanovg::QuadTo) {
  NodeNanovg *handle = ObjectWrap::Unwrap<NodeNanovg>(info.This());

  float cx = info[0]->NumberValue();
  float cy = info[1]->NumberValue();
  float x = info[2]->NumberValue();
  float y = info[3]->NumberValue();

  nvgQuadTo(handle->ctx, cx, cy, x, y);
}

NAN_METHOD(NodeNanovg::ArcTo) {
  NodeNanovg *handle = ObjectWrap::Unwrap<NodeNanovg>(info.This());

  float x1 = info[0]->NumberValue();
  float y1 = info[1]->NumberValue();
  float x2 = info[2]->NumberValue();
  float y2 = info[3]->NumberValue();
  float r = info[4]->NumberValue();

  nvgArcTo(handle->ctx, x1, y1, x2, y2, r);
}

NAN_METHOD(NodeNanovg::ClosePath) {
  NodeNanovg *handle = ObjectWrap::Unwrap<NodeNanovg>(info.This());
  nvgClosePath(handle->ctx);
}

NAN_METHOD(NodeNanovg::PathWinding) {
  NodeNanovg *handle = ObjectWrap::Unwrap<NodeNanovg>(info.This());

  int dir = info[0]->Int32Value();

  nvgPathWinding(handle->ctx, dir);
}

NAN_METHOD(NodeNanovg::Arc) {
  NodeNanovg *handle = ObjectWrap::Unwrap<NodeNanovg>(info.This());

  float cx = info[0]->NumberValue();
  float cy = info[1]->NumberValue();
  float r = info[2]->NumberValue();
  float a0 = info[3]->NumberValue();
  float a1 = info[4]->NumberValue();
  // flip to match canvas
  int dir = info[5]->BooleanValue() ? 0 : 1;

  nvgArc(handle->ctx, cx, cy, r, a0, a1, dir);
}

NAN_METHOD(NodeNanovg::Rect) {
  NodeNanovg *handle = ObjectWrap::Unwrap<NodeNanovg>(info.This());

  float x = info[0]->NumberValue();
  float y = info[1]->NumberValue();
  float w = info[2]->NumberValue();
  float h = info[3]->NumberValue();

  nvgRect(handle->ctx, x, y, w, h);
}

NAN_METHOD(NodeNanovg::RoundedRect) {
  NodeNanovg *handle = ObjectWrap::Unwrap<NodeNanovg>(info.This());

  float x = info[0]->NumberValue();
  float y = info[1]->NumberValue();
  float w = info[2]->NumberValue();
  float h = info[3]->NumberValue();
  float r = info[4]->NumberValue();

  nvgRoundedRect(handle->ctx, x, y, w, h, r);
}

NAN_METHOD(NodeNanovg::Circle) {
  NodeNanovg *handle = ObjectWrap::Unwrap<NodeNanovg>(info.This());

  float x = info[0]->NumberValue();
  float y = info[1]->NumberValue();
  float r = info[2]->NumberValue();

  nvgCircle(handle->ctx, x, y, r);
}

NAN_METHOD(NodeNanovg::Ellipse) {
  NodeNanovg *handle = ObjectWrap::Unwrap<NodeNanovg>(info.This());

  float x = info[0]->NumberValue();
  float y = info[1]->NumberValue();
  float rx = info[2]->NumberValue();
  float ry = info[3]->NumberValue();

  nvgEllipse(handle->ctx, x, y, rx, ry);
}

NAN_METHOD(NodeNanovg::Stroke) {
  NodeNanovg *handle = ObjectWrap::Unwrap<NodeNanovg>(info.This());
  nvgStroke(handle->ctx);
}

NAN_METHOD(NodeNanovg::Fill) {
  NodeNanovg *handle = ObjectWrap::Unwrap<NodeNanovg>(info.This());
  nvgFill(handle->ctx);
}


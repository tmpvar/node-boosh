#ifndef __NODE_NANOVG__
#define __NODE_NANOVG__

#include <nan.h>
#include "nanovg.h"

class NanovgPaint : public Nan::ObjectWrap {
  public:
    NVGpaint handle;
    NanovgPaint() {
      this->handle = handle;
    }

  private:
    static NAN_MODULE_INIT(Init) {
      v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
      tpl->SetClassName(Nan::New<v8::String>("NanovgPaint").ToLocalChecked());
      tpl->InstanceTemplate()->SetInternalFieldCount(1);

      Nan::Set(
        target,
        Nan::New("NanovgPaint").ToLocalChecked(),
        Nan::GetFunction(tpl).ToLocalChecked()
      );
    }

    static NAN_METHOD(New) {
      NanovgPaint *handle = new NanovgPaint();
      handle->Wrap(info.Holder());
      info.GetReturnValue().Set(info.Holder());
    }
};

class NodeNanovg : public Nan::ObjectWrap {
  public:
    NVGcontext *ctx;
    NodeNanovg();

    static NAN_MODULE_INIT(Init) {
      v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
      tpl->SetClassName(Nan::New<v8::String>("Nanovg").ToLocalChecked());
      tpl->InstanceTemplate()->SetInternalFieldCount(1);

      Nan::SetPrototypeMethod(tpl, "beginFrame", BeginFrame);
      Nan::SetPrototypeMethod(tpl, "cancelFrame", CancelFrame);
      Nan::SetPrototypeMethod(tpl, "endFrame", EndFrame);

      Nan::SetPrototypeMethod(tpl, "save", Save);
      Nan::SetPrototypeMethod(tpl, "restore", Restore);
      Nan::SetPrototypeMethod(tpl, "reset", Reset);

      Nan::SetPrototypeMethod(tpl, "setStrokeColor", StrokeColor);
      Nan::SetPrototypeMethod(tpl, "setFillColor", FillColor);
      Nan::SetPrototypeMethod(tpl, "strokeWidth", StrokeWidth);
      Nan::SetPrototypeMethod(tpl, "globalAlpha", StrokeWidth);
      Nan::SetPrototypeMethod(tpl, "miterLimit", MiterLimit);
      Nan::SetPrototypeMethod(tpl, "translate", Translate);
      Nan::SetPrototypeMethod(tpl, "rotate", Rotate);
      Nan::SetPrototypeMethod(tpl, "scale", Scale);
      Nan::SetPrototypeMethod(tpl, "skewX", SkewX);
      Nan::SetPrototypeMethod(tpl, "skewY", SkewY);

      Nan::SetPrototypeMethod(tpl, "resetTransform", ResetTransform);
      Nan::SetPrototypeMethod(tpl, "getCurrentTransform", CurrentTransform);

      Nan::SetPrototypeMethod(tpl, "moveTo", MoveTo);
      Nan::SetPrototypeMethod(tpl, "lineTo", LineTo);
      Nan::SetPrototypeMethod(tpl, "bezierTo", BezierTo);
      Nan::SetPrototypeMethod(tpl, "quadTo", QuadTo);
      Nan::SetPrototypeMethod(tpl, "arcTo", ArcTo);
      Nan::SetPrototypeMethod(tpl, "beginPath", BeginPath);
      Nan::SetPrototypeMethod(tpl, "closePath", ClosePath);
      Nan::SetPrototypeMethod(tpl, "pathWinding", PathWinding);
      Nan::SetPrototypeMethod(tpl, "arc", Arc);
      Nan::SetPrototypeMethod(tpl, "rect", Rect);
      Nan::SetPrototypeMethod(tpl, "roundedRect", RoundedRect);
      Nan::SetPrototypeMethod(tpl, "ellipse", Ellipse);
      Nan::SetPrototypeMethod(tpl, "circle", Circle);

      Nan::SetPrototypeMethod(tpl, "fill", Fill);
      Nan::SetPrototypeMethod(tpl, "stroke", Stroke);

      Nan::Set(
        target,
        Nan::New("Nanovg").ToLocalChecked(),
        Nan::GetFunction(tpl).ToLocalChecked()
      );
    }
  private:
    static NAN_METHOD(New);
    static NAN_METHOD(BeginFrame);
    static NAN_METHOD(CancelFrame);
    static NAN_METHOD(EndFrame);

    static NAN_METHOD(Save);
    static NAN_METHOD(Restore);
    static NAN_METHOD(Reset);

// // NVGcolor nvgRGB(unsigned char r, unsigned char g, unsigned char b);
// // NVGcolor nvgRGBf(float r, float g, float b);
// // NVGcolor nvgRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
// // NVGcolor nvgRGBAf(float r, float g, float b, float a);
// // NVGcolor nvgLerpRGBA(NVGcolor c0, NVGcolor c1, float u);
// // NVGcolor nvgTransRGBA(NVGcolor c0, unsigned char a);
// // NVGcolor nvgTransRGBAf(NVGcolor c0, float a);
// // NVGcolor nvgHSL(float h, float s, float l);
// // NVGcolor nvgHSLA(float h, float s, float l, unsigned char a);
    static NAN_METHOD(StrokeColor);
// NAN_METHOD(StrokePaint);
    static NAN_METHOD(FillColor);
// NAN_METHOD(FillPaint);
    static NAN_METHOD(MiterLimit);
    static NAN_METHOD(StrokeWidth);
// NAN_METHOD(LineCap);
// NAN_METHOD(LineJoin);
    static NAN_METHOD(GlobalAlpha);
    static NAN_METHOD(ResetTransform);
// NAN_METHOD(Transform);
    static NAN_METHOD(Translate);
    static NAN_METHOD(Rotate);
    static NAN_METHOD(SkewX);
    static NAN_METHOD(SkewY);
    static NAN_METHOD(Scale);

    static NAN_METHOD(CurrentTransform);

    // use something like gl-matrix instead...
    //   TransformIdentity
    //   TransformTranslate
    //   TransformScale
    //   TransformRotate
    //   TransformSkewX
    //   TransformSkewY
    //   TransformMultiply
    //   TransformPremultiply
    //   TransformInverse
    //   TransformPoint

    // This is easy enough to in JS
    //   float nvgDegToRad(float deg);
    //   float nvgRadToDeg(float rad);

// // int nvgCreateImage(NVGcontext* ctx, const char* filename, int imageFlags);
// // int nvgCreateImageMem(NVGcontext* ctx, int imageFlags, unsigned char* data, int ndata);
// // int nvgCreateImageRGBA(NVGcontext* ctx, int w, int h, int imageFlags, const unsigned char* data);
// NAN_METHOD(UpdateImage);
// NAN_METHOD(ImageSize);
// NAN_METHOD(DeleteImage);
// // NVGpaint nvgLinearGradient(NVGcontext* ctx, float sx, float sy, float ex, float ey,                NVGcolor icol, NVGcolor ocol);
// // NVGpaint nvgBoxGradient(NVGcontext* ctx, float x, float y, float w, float h,            float r, float f, NVGcolor icol, NVGcolor ocol);
// // NVGpaint nvgRadialGradient(NVGcontext* ctx, float cx, float cy, float inr, float outr,               NVGcolor icol, NVGcolor ocol);
// // NVGpaint nvgImagePattern(NVGcontext* ctx, float ox, float oy, float ex, float ey,              float angle, int image, float alpha);
// NAN_METHOD(Scissor);
// NAN_METHOD(IntersectScissor);
// NAN_METHOD(ResetScissor);
    static NAN_METHOD(BeginPath);
    static NAN_METHOD(MoveTo);
    static NAN_METHOD(LineTo);
    static NAN_METHOD(BezierTo);
    static NAN_METHOD(QuadTo);
    static NAN_METHOD(ArcTo);
    static NAN_METHOD(ClosePath);
    static NAN_METHOD(PathWinding);
    static NAN_METHOD(Arc);
    static NAN_METHOD(Rect);
    static NAN_METHOD(RoundedRect);
    static NAN_METHOD(Ellipse);
    static NAN_METHOD(Circle);
    static NAN_METHOD(Fill);
    static NAN_METHOD(Stroke);
// // int nvgCreateFont(NVGcontext* ctx, const char* name, const char* filename);
// // int nvgCreateFontMem(NVGcontext* ctx, const char* name, unsigned char* data, int ndata, int freeData);
// // int nvgFindFont(NVGcontext* ctx, const char* name);
// NAN_METHOD(FontSize);
// NAN_METHOD(FontBlur);
// NAN_METHOD(TextLetterSpacing);
// NAN_METHOD(TextLineHeight);
// NAN_METHOD(TextAlign);
// NAN_METHOD(FontFaceId);
// NAN_METHOD(FontFace);
// // float nvgText(NVGcontext* ctx, float x, float y, const char* string, const char* end);
// NAN_METHOD(TextBox);
// // float nvgTextBounds(NVGcontext* ctx, float x, float y, const char* string, const char* end, float* bounds);
// NAN_METHOD(TextBoxBounds);
// // int nvgTextGlyphPositions(NVGcontext* ctx, float x, float y, const char* string, const char* end, NVGglyphPosition* positions, int maxPositions);
// NAN_METHOD(TextMetrics);
// // int nvgTextBreakLines(NVGcontext* ctx, const char* string, const char* end, float breakRowWidth, NVGtextRow* rows, int maxRows);
// NAN_METHOD(DebugDumpPathCache);

};
#endif

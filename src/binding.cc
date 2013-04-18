#include <node.h>
#include <node_buffer.h>
#include <v8.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include "uv.h"
#include "stdio.h"
#include <Canvas.h>
#include <PixelArray.h>

using namespace v8;
using namespace node;

GLuint  texture[1], tex;
int width = 640, height = 480;

Handle<Value> CreateWindow(const Arguments& args) {
  HandleScope scope;

  /* Initialize the library */
  if (!glfwInit()) {
    scope.Close(Undefined());
  }



  width = 640;
  height = 480;

  /* Create a windowed mode window and its OpenGL context */
  if (!glfwOpenWindow(640, 480, 8, 8, 8, 0, 24, 0, GLFW_WINDOW)) {
    scope.Close(Undefined());
  }

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
  return scope.Close(String::New("world"));
}


Handle<Value> SetBuffer(const Arguments& args) {
  HandleScope scope;

  Canvas *canvas = ObjectWrap::Unwrap<Canvas>(args[0]->ToObject());

  glGenTextures(1, &texture[0]);
  glBindTexture(GL_TEXTURE_2D, texture[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_BGRA, GL_UNSIGNED_BYTE, canvas->data());
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

  return scope.Close(Undefined());
}

Handle<Value> Flush(const Arguments& args) {
  HandleScope scope;

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
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 640.0f, 0.0f,  0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 640.0f,  480.0f,  0);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 0.0f, 480.0f,  0);
  glEnd();
  // glFinish();
  //glFlush();
  glfwSwapBuffers();
  glDeleteTextures(1, &texture[0]);
  return scope.Close(Undefined());
}

void init(Handle<Object> exports) {
  exports->Set(String::NewSymbol("createWindow"),
      FunctionTemplate::New(CreateWindow)->GetFunction());
  exports->Set(String::NewSymbol("setBuffer"),
      FunctionTemplate::New(SetBuffer)->GetFunction());
  exports->Set(String::NewSymbol("flush"),
      FunctionTemplate::New(Flush)->GetFunction());
}

NODE_MODULE(boosh, init)
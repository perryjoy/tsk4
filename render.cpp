#include "win.h"
#include "Render.h"

VOID RENDER::StartFrame(VOID)
{
  glClearColor((FLT)0.0, (FLT)0.0, (FLT)0.0, (FLT)1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

VOID RENDER::EndFrame(VOID)
{
  SwapBuffers(hDC);
  glFinish();
}

VOID RENDER::Resize(INT w, INT h)
{
  glViewport(0, 0, w, h);
}

RENDER::RENDER(WIN * window) : hWin(window->hWnd)
{
  INT i;
  PIXELFORMATDESCRIPTOR pfd = { 0 };
  w = window->w;
  h = window->h;
  isSized = FALSE;
  hDC = GetDC(window->hWnd);

  ///* OpenGL initialization: setup pixel format */
  pfd.nSize = sizeof(pfd);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(hDC, &pfd);
  DescribePixelFormat(hDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(hDC, i, &pfd);

  ///* OpenGL initialization: setup rendering context */
  hGLRC = wglCreateContext(hDC);
  wglMakeCurrent(hDC, hGLRC);

  Resize(w, h);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
} 

// This file is an inline implmenentation (II) file which is included in the
// NAWINAPI.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAWINAPI.c for more information.
// Do not include this file anywhere else!


#if (NA_COMPILE_OPENGL == 1)


typedef struct NAWINAPIOpenGLSpace NAWINAPIOpenGLSpace;
struct NAWINAPIOpenGLSpace {
  NACoreOpenGLSpace coreOpenGLspace;
  HGLRC hRC;    // The rendering context for OpenGL
};



NAWINAPICallbackInfo naOpenGLSpaceWINAPIProc(NAUIElement* uiElement, UINT message, WPARAM wParam, LPARAM lParam){
  NAWINAPICallbackInfo info = {NA_FALSE, 0};

  switch(message){
  case WM_PAINT:
    info.hasbeenhandeled = naDispatchUIElementCommand(uiElement, NA_UI_COMMAND_REDRAW);
    info.result = 0;
    break;

  default:
    //printf("Uncaught OpenGL Space message" NA_NL);
    break;
  }
  
  return info;
}


typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALPROC)(int);

NA_DEF NAOpenGLSpace* naNewOpenGLSpace(NAWindow* window, NASize size, NAMutator initfunc, void* initdata){
	
  HWND hWnd;
  HDC hDC;
 	PIXELFORMATDESCRIPTOR pfd;
  int format;
  DWORD style;
	PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;
  const char *extensions;

  NAWINAPIOpenGLSpace* winapiOpenGLSpace = naAlloc(NAWINAPIOpenGLSpace);

  style = WS_CHILD | WS_VISIBLE | ES_READONLY;

	hWnd = CreateWindow(
		TEXT("NASpace"), TEXT(""), style,
		0, 0, (int)size.width, (int)size.height,
		(HWND)naGetUIElementNativeID(window), NULL, (HINSTANCE)naGetUIElementNativeID(naGetApplication()), NULL );
    
  hDC = GetDC(hWnd);

  // Expected to be called when initializing. Do not multithread!
	// define pixel format for device context
	ZeroMemory( &pfd, sizeof( pfd ) );
	pfd.nSize = sizeof( pfd );
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cAlphaBits = 8;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	format = ChoosePixelFormat( GetDC(hWnd), &pfd );

	SetPixelFormat( hDC, format, &pfd );
	
	// make render context with this device context.
	winapiOpenGLSpace->hRC = wglCreateContext(hDC);
	wglMakeCurrent(hDC, winapiOpenGLSpace->hRC);

	extensions = (char*)glGetString(GL_EXTENSIONS);
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC)wglGetProcAddress("wglSwapIntervalEXT");
	if (wglSwapIntervalEXT){wglSwapIntervalEXT(1);}

  naInitCoreOpenGLSpace(&(winapiOpenGLSpace->coreOpenGLspace), hWnd);

  // Now the OpenGL context is created and current. We can initialize it
  // if necessary.
  if(initfunc){
    initfunc(initdata);
  }

	//glewInit();
  return (NAOpenGLSpace*)winapiOpenGLSpace;
}


NA_DEF void naSwapOpenGLBuffer(NAOpenGLSpace* openglspace){
  NAWINAPIOpenGLSpace* winapiOpenGLSpace = (NAWINAPIOpenGLSpace*)openglspace;
  SwapBuffers(GetDC((HWND)naGetUIElementNativeID(&(winapiOpenGLSpace->coreOpenGLspace.uiElement))));
}



NA_API void naSetOpenGLInnerRect(NAOpenGLSpace* openglspace, NARect bounds){
  SetWindowPos((HWND)naGetUIElementNativeID(openglspace), HWND_TOP, 0, 0, (int)bounds.size.width, (int)bounds.size.height, SWP_NOREDRAW);
}




NA_DEF void naDestructOpenGLSpace(NAOpenGLSpace* openglspace){
  NACoreOpenGLSpace* coreOpenGLspace = (NACoreOpenGLSpace*)openglspace;
  naClearCoreOpenGLSpace(coreOpenGLspace);
}



  
#endif  // NA_COMPILE_OPENGL

NA_HDEF NARect naGetOpenGLSpaceAbsoluteInnerRect(NACoreUIElement* openglspace){
  return naGetSpaceAbsoluteInnerRect(openglspace);
}



// This is free and unencumbered software released into the public domain.

// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// For more information, please refer to <http://unlicense.org/>

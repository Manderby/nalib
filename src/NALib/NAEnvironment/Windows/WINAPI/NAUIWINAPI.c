
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


#include "NAUIWINAPI.h"

#if (NA_CONFIG_COMPILE_GUI == 1)
#if NA_OS == NA_OS_WINDOWS
// Now, we are sure, we compile for windows. The #if directive will be
// closed at the bottom of this file.


#include "../NAList.h"
#include "../NACoord.h"







// ///////////////////////////////////
// WINDOW CALLBACK
// ///////////////////////////////////

// This is the one and only, master of destruction, defender of chaos and
// pimp of the century function handling all and everything using the most
// beautiful and highly pattern-absent spaghetti coding structure ever
// seen to mankind.
//
// Send me a fax if you have a better idea. It works.

LRESULT CALLBACK WindowCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  NAWindow* window;
  NARect rect;
  PAINTSTRUCT ps; // Paint information. Needed for WM_PAINT messages
  NABool hasbeenhandeled = NA_FALSE;
  LRESULT retvalue;
  NAUIKeyCode keycode;
  NAUIKeyCode scancode;   // used as UINT, converted to NAUIKeyCode
  NASize size;
  NAPos pos;
  const NAMouseStatus* mousestatus;

  NACoreUIElement* uielement = (NACoreUIElement*)naGetUINALibEquivalent(hWnd);
  if (!uielement) {
    // If we do not have the hWnd registered in NALib UI, we do not process it. Simply
    // deletage it to the default window procedure and return.
    return DefWindowProc(hWnd, message, wParam, lParam);
  }


  //if((message >= WM_USER) && (message <= 0x7fff)){
  //  // User defined message
  //  naDispatchUIElementCommand(uielement, ???, wParam);
  //  return 0;
  //}

  switch (message) {
  case WM_SHOWWINDOW:
    // wParam: true for show, false for hide
    // lParam: status of window
    break;

  case WM_MOVE:
    // wParam: Unused
    // LOWORD(lParam): x coordinate
    // HIWORD(lParam): y coordinate
    window = naGetUIElementWindow(uielement);
    rect = naGetUIElementRect(window, NA_NULL, NA_FALSE);
    rect.pos.x = (double)((int)(short)LOWORD(lParam));
    rect.pos.y = (double)((int)(short)HIWORD(lParam));
    hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_RESHAPE, NA_NULL);
    if (hasbeenhandeled) { naDispatchUIElementCommand(uielement, NA_UI_COMMAND_REDRAW, NA_NULL); }
    break;

  case WM_SIZE:
    // wParam: Type of resizing (maximize, minimize, ...)
    // LOWORD(lParam): width
    // HIWORD(lParam): height
    window = naGetUIElementWindow(uielement);
    rect = naGetUIElementRect(window, NA_NULL, NA_FALSE);
    rect.size.width = LOWORD(lParam);
    rect.size.height = HIWORD(lParam);
    hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_RESHAPE, NA_NULL);
    if (hasbeenhandeled) { naDispatchUIElementCommand(uielement, NA_UI_COMMAND_REDRAW, NA_NULL); }
    break;

  case WM_PAINT:
    // wParam: Unused
    // lParam: Unused
    if (uielement->elementtype == NA_UI_OPENGLSPACE) {
      BeginPaint(hWnd, &ps);

      //if(uielement is opengl)
      //        wglMakeCurrent(GetDC(naGetUIElementNativeID(&(openglspace->uielement))), openglspace->hRC);

      hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_REDRAW, NA_NULL);
      //uielement->refreshrequested = NA_FALSE;
      EndPaint(hWnd, &ps);
    }
    break;

  case WM_ERASEBKGND:
    hasbeenhandeled = NA_TRUE;
    //  if(window->eraseBackground()){
    //    DefWindowProc(hWnd, message, wParam, lParam);
    //  }else{
    //  }
    break;

  case WM_TIMER:
    // Note: Does not work with hWnd == NULL. Will not be called here.
    // wParam: timer identifier as an UINT
    // lParam: callback function. Unused in NALib
    //hasbeenhandeled = naExecuteApplicationTimer((UINT)wParam);
    break;

  case WM_HSCROLL:
    //  uielement = window->getUIElement((HWND)lParam);
    //  if(!uielement){retvalue = 0;}else{uielement->dispatchCommand(LOWORD(wParam), 0);} // note that this is the loword
    break;

  case WM_KEYDOWN:
    // wParam: virtual key code
    // lParam: several values, attributes, flags, ...
    keycode = wParam;
    scancode = (NAUIKeyCode)MapVirtualKey((UINT)keycode, MAPVK_VK_TO_VSC);
    hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_KEYDOWN, &scancode);
    break;

  case WM_KEYUP:
    // wParam: virtual key code
    // lParam: several values, attributes, flags, ...
    keycode = wParam;
    scancode = MapVirtualKey((UINT)keycode, MAPVK_VK_TO_VSC);
    hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_KEYUP, &scancode);
    break;

  case WM_MOUSELEAVE:
    //  window->mouseLeave();
    break;

  case WM_MOUSEMOVE:
    // wParam: several special keys
    // GET_X_LPARAM(lParam): x coord relative to top left
    // GET_Y_LPARAM(lParam): y coord relative to top left
    window = naGetUIElementWindow(uielement);
    //if(window->flags & CUB_WINDOW_IGNORE_MOUSE_WARP){
    //  window->flags &= ~CUB_WINDOW_IGNORE_MOUSE_WARP;
    //  hasbeenhandeled = NA_TRUE;
    //}else{
    // todo: this should be GET_X_LPARAM and GET_Y_LPARAM
    // but is undefined somehow.
    size.width = LOWORD(lParam);
    size.height = HIWORD(lParam);
    rect = naGetUIElementRect(uielement, naGetApplication(), NA_FALSE);
    size.width += rect.pos.x;
    size.height += rect.pos.y;
    mousestatus = naGetMouseStatus();
    pos = naGetMousePos(mousestatus);
    naSetMouseMovedByDiff(size.width - pos.x, size.height - pos.y);
    hasbeenhandeled = naDispatchUIElementCommand(uielement, NA_UI_COMMAND_MOUSE_MOVED, &size);
    //}
    break;

  case WM_LBUTTONUP:
    //  window->leftMouseUp(LOWORD(lParam), HIWORD(lParam), wParam);
    break;

  case WM_COMMAND:
    //  uielement = window->getUIElement((HWND)lParam);
    //  if(!uielement){retvalue = 0;}else{uielement->dispatchCommand(HIWORD(wParam), 0);}
    break;

  case WM_NOTIFY:
    //  uielement = window->getUIElement(((LPNMHDR)lParam)->hwndFrom);
    //  if(!uielement){retvalue = 0;}else{uielement->dispatchCommand(((LPNMHDR)lParam)->code, lParam);}
    break;

  case WM_USER:
    //  uielement = window->getUIElement((HWND)lParam);
    //  if(!uielement){retvalue = 0;}else{uielement->dispatchCommand(HIWORD(wParam), 0);}
    break;

  case WM_CLOSE:
    //  window->close();
    //  if(!::gui->numwindows){PostQuitMessage(0);}
    break;

  default:
    break;

  }

  if(hasbeenhandeled) {
    retvalue = 0;
  }else{
    retvalue = DefWindowProc(hWnd, message, wParam, lParam);
  }
  return retvalue;
}





// ///////////////////////////////////
// TIMER
// ///////////////////////////////////


// This is the native WINAPI callback function. It finds the corresponding
// registered timer struct of the application and executes the function
// stored in that struct with the stored argument.
//
// Definitely not the fastest and best method. But as for now, it's ok. todo.
NA_HDEF static VOID CALLBACK naTimerCallbackFunction(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
  //todo something is wrong here with the type.
  NAWINAPIApplication* app;
  NAListIterator iter;

  UINT timerkey = (UINT)idEvent;
  app = (NAWINAPIApplication*)naGetApplication();

  naBeginListModifierIteration(NATimerStruct* timerstruct, &(app->timers), iter);
    if(timerstruct->key == timerkey) {
      naRemoveListCurMutable(&iter, NA_FALSE);
      KillTimer(hwnd, idEvent);
      timerstruct->func(timerstruct->arg);
      naFree(timerstruct);
      break;
    }
  naEndListIteration(iter);
}




// ///////////////////////////////////
// APPLICATION
// ///////////////////////////////////


NA_API void naStartApplication(NAMutator prestartup, NAMutator poststartup, void* arg){
  NAWINAPIApplication* app;
  WNDCLASS wndclass;
  MSG message;
  NANativeID nativeApp;

  // Start the WINAPI application and set the native ID of the application.
  nativeApp = GetModuleHandle(NULL);
  NAApplication* app = naNewApplication();

  // Init the timer list.
  app = (NAWINAPIApplication*)naGetApplication();
  naInitList(&(app->timers));

  // Call prestartup if desired.
  if(prestartup){prestartup(arg);}

  // Set the language of the translator
  TODO

  // Register the window class
  naZeron(&wndclass, sizeof(WNDCLASS));
	wndclass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WindowCallback;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = nativeApp;
	wndclass.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wndclass.hCursor = LoadCursor( NULL, IDC_ARROW );
	wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = TEXT("NAWindow");
	RegisterClass(&wndclass);

  // Register the space class
  naZeron(&wndclass, sizeof(WNDCLASS));
	wndclass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WindowCallback;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = nativeApp;
	wndclass.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wndclass.hCursor = LoadCursor( NULL, IDC_ARROW );
	wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = TEXT("NASpace");
	RegisterClass(&wndclass);

  // Call poststartup if desired.
  if(poststartup){poststartup(arg);}

  // Start the event loop.
  while(naIsCoreApplicationRunning()){
    BOOL response = GetMessage(&message, 0, 0, 0);
    naCollectGarbage();
    if(response == 0){break;}
    if (response == -1){
      // handle the error and possibly exit
      break;
    }else{
      if(message.message == WM_QUIT){
        break;
      }
      TranslateMessage(&message);
      DispatchMessage(&message);
    }
  }

//  naClearCoreApplication();
  naReleaseUIElement(app);
}



NA_DEF void naCallApplicationFunctionInSeconds(NAMutator function, void* arg, double timediff){
  NAWINAPIApplication* app;
  NATimerStruct* timerstruct = naAlloc(NATimerStruct);
  timerstruct->func = function;
  timerstruct->arg = arg;
  // todo: Check type
  timerstruct->key = (UINT)SetTimer((HWND)NA_NULL, (UINT_PTR)NA_NULL, (UINT)(1000 * timediff), naTimerCallbackFunction);
  app = (NAWINAPIApplication*)naGetApplication();
  naAddListLastMutable(&(app->timers), timerstruct);
}



#if (NA_SYSTEM_ADDRESS_BITS == 64)
  typedef intptr_t NAWINAPIHANDLE;
#elif (NA_SYSTEM_ADDRESS_BITS == 32)
  typedef long NAWINAPIHANDLE;
#else
  #error "Undefined system address bytesize"
#endif



#include <io.h>
#include <fcntl.h>
NA_DEF void naOpenConsoleWindow(const char* windowtitle){
//  int outHandle, errHandle, inHandle;
  FILE *outFile;
  FILE *errFile;
  FILE *inFile;
//  CONSOLE_SCREEN_BUFFER_INFO coninfo;
  AllocConsole();
  SetConsoleTitle(windowtitle);
//    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
//    coninfo.dwSize.Y = 9999;
//    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

  freopen_s(&inFile, "CONIN$", "r", stdin);
  freopen_s(&outFile, "CONOUT$", "w", stdout);
  freopen_s(&errFile, "CONERR$", "w", stderr);

  //outHandle = _open_osfhandle((NAWINAPIHANDLE)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
  //errHandle = _open_osfhandle((NAWINAPIHANDLE)GetStdHandle(STD_ERROR_HANDLE),_O_TEXT);
  //inHandle = _open_osfhandle((NAWINAPIHANDLE)GetStdHandle(STD_INPUT_HANDLE),_O_TEXT );

  //outFile = _fdopen(outHandle, "w" );
  //errFile = _fdopen(errHandle, "w");
  //inFile =  _fdopen(inHandle, "r");

  //*stdout = *outFile;
  //*stderr = *errFile;
  //*stdin = *inFile;

  //setvbuf( stdout, NULL, _IONBF, 0 );
  //setvbuf( stderr, NULL, _IONBF, 0 );
  //setvbuf( stdin, NULL, _IONBF, 0 );

}





// ///////////////////////////////////
// UI ELEMENT
// ///////////////////////////////////

NA_HDEF void naRefreshUIElementNow(NAUIElement* uielement){
  PostMessage(naGetUIElementNativeID(uielement), WM_PAINT, (WPARAM)NA_NULL, (LPARAM)NA_NULL);
}











































NA_HDEF NARect naGetApplicationAbsoluteRect(){
  NARect rect;
  rect.pos.x = 0;
  rect.pos.y = 0;
  rect.size.width = 1;
  rect.size.height = 1;
  return rect;
}



NA_HDEF NARect naGetScreenAbsoluteRect(NAUIElement* screen){
  NARect rect;
  rect.pos.x = 0;
  rect.pos.y = 0;
  rect.size.width = 1;
  rect.size.height = 1;
  return rect;
}



NA_HDEF NARect naGetWindowAbsoluteInnerRect(NAUIElement* window){
  NARect rect;
  NARect screenrect;
  RECT contentrect;
  POINT testpoint = {0, 0};
  NACoreUIElement* corewindow;

  corewindow = (NACoreUIElement*)window;

  GetClientRect(corewindow->nativeID, &contentrect);
  ClientToScreen(corewindow->nativeID, &testpoint);

  screenrect = naGetMainScreenRect();

  rect.pos.x = testpoint.x;
  rect.pos.y = screenrect.size.height - ((double)testpoint.y + (contentrect.bottom - contentrect.top));
  rect.size.width = (double)contentrect.right - (double)contentrect.left;
  rect.size.height = (double)contentrect.bottom - (double)contentrect.top;
  return rect;
}



NA_HDEF NARect naGetWindowAbsoluteOuterRect(NAUIElement* window){
  NARect rect;
  NARect screenrect;
  RECT windowrect;
  NACoreUIElement* corewindow;

  corewindow = (NACoreUIElement*)window;

  GetWindowRect(corewindow->nativeID, &windowrect);
  screenrect = naGetMainScreenRect();

  rect.pos.x = windowrect.left;
  rect.pos.y = screenrect.size.height - windowrect.bottom;
  rect.size.width = (double)windowrect.right - (double)windowrect.left;
  rect.size.height = (double)windowrect.bottom - (double)windowrect.top;
  return rect;
}



NA_HDEF NARect naGetSpaceAbsoluteInnerRect(NAUIElement* space){
  NARect rect;
  NARect screenrect;
  RECT contentrect;
  POINT testpoint = {0, 0};
  NACoreUIElement* corespace;

  corespace = (NACoreUIElement*)space;

  GetClientRect(corespace->nativeID, &contentrect);
  ClientToScreen(corespace->nativeID, &testpoint);
  screenrect = naGetMainScreenRect();

  rect.pos.x = testpoint.x;
  rect.pos.y = screenrect.size.height - ((double)testpoint.y + (contentrect.bottom - contentrect.top));
  rect.size.width = (double)contentrect.right - (double)contentrect.left;
  rect.size.height = (double)contentrect.bottom - (double)contentrect.top;
  return rect;
}



NA_DEF NARect naGetUIElementRect(NAUIElement* uielement, NAUIElement* relativeelement, NABool includebounds){
  NARect rect;
  NARect relrect;
  NACoreUIElement* coreelement;
  NACoreUIElement* corerelelement;
  NAWINAPIApplication* app;

  coreelement = (NACoreUIElement*)uielement;
  corerelelement = (NACoreUIElement*)relativeelement;
  app = (NAWINAPIApplication*)naGetApplication();

  // First, let's handle the root case: Returning the application rect.
  if(coreelement == (NACoreUIElement*)app){
    #ifndef NDEBUG
      if(relativeelement && (relativeelement != app))
        naError("naGetUIElementRect", "The relative element is invalid for the given uielement, which seems to be the application.");
    #endif
    return naGetApplicationAbsoluteRect();
  }

  // Now, we find the appropriate relative element.
  if(!corerelelement){corerelelement = naGetUIElementParent(coreelement);}

  switch(coreelement->elementtype){
  case NA_UI_APPLICATION: rect = naGetApplicationAbsoluteRect(); break;
  case NA_UI_SCREEN:      rect = naGetScreenAbsoluteRect(coreelement); break;
  case NA_UI_WINDOW:
    if(includebounds){
      rect = naGetWindowAbsoluteOuterRect(coreelement);
    }else{
      rect = naGetWindowAbsoluteInnerRect(coreelement);
    }
    break;
  case NA_UI_OPENGLSPACE:  rect = naGetSpaceAbsoluteInnerRect(coreelement); break;
  default:
    #ifndef NDEBUG
      naError("naGetUIElementRect", "Invalid UI type");
    #endif
    rect = naMakeRectSE(0., 0., 0., 0.);
    break;
  }

  switch(corerelelement->elementtype){
  case NA_UI_APPLICATION: relrect = naGetApplicationAbsoluteRect(); break;
  case NA_UI_SCREEN:      relrect = naGetScreenAbsoluteRect(corerelelement); break;
  case NA_UI_WINDOW:      relrect = naGetWindowAbsoluteInnerRect(corerelelement); break;
  case NA_UI_OPENGLSPACE:  relrect = naGetSpaceAbsoluteInnerRect(corerelelement); break;
  default:
    #ifndef NDEBUG
      naError("naGetUIElementRect", "Invalid UI type");
    #endif
    relrect = naMakeRectSE(0., 0., 0., 0.);
    break;
  }

  rect.pos.x = rect.pos.x - relrect.pos.x;
  rect.pos.y = rect.pos.y - relrect.pos.y;
  rect.size.width = rect.size.width;
  rect.size.height = rect.size.height;

  // Convert the rect into absolute coordinates.

  return rect;
}



NA_API NARect naGetMainScreenRect(){
  HMONITOR screen;
  MONITORINFO screeninfo;
  NARect rect;
  screen = MonitorFromWindow(NA_NULL, MONITOR_DEFAULTTOPRIMARY);
  screeninfo.cbSize = sizeof(MONITORINFO);
  GetMonitorInfo(screen, &screeninfo);
  rect.pos.x = screeninfo.rcMonitor.left;
  rect.pos.y = (double)screeninfo.rcMonitor.bottom - (double)screeninfo.rcMonitor.top;
  rect.size.width = (double)screeninfo.rcMonitor.right - (double)screeninfo.rcMonitor.left;
  rect.size.height = (double)screeninfo.rcMonitor.bottom - (double)screeninfo.rcMonitor.top;
  return rect;
}


//void MBUIElement::setEnabled(MBBool enabled){
//  SendMessage(hWnd, WM_ENABLE, (WPARAM)enabled, 0);
//}
//
//
//
//void MBUIElement::leftMouseUp(int32 x, int32 y, int32 modifierkeys){
//  mousex = x;
//  mousey = y;
//}
//
//void MBUIElement::mouseMove(int32 x, int32 y, int32 modifierkeys){
//  if(mousex == -1){
//    TRACKMOUSEEVENT tme;
//    tme.cbSize = sizeof(TRACKMOUSEEVENT);
//    tme.dwFlags = TME_LEAVE;
//    tme.hwndTrack = hWnd;
//    tme.dwHoverTime = HOVER_DEFAULT;
//    TrackMouseEvent(&tme);
//  }
//
//  mousex = x;
//  mousey = y;
//}
//
//void MBUIElement::mouseLeave(){
//  mousex = -1;
//  mousey = -1;
//
//  TRACKMOUSEEVENT tme;
//  tme.cbSize = sizeof(TRACKMOUSEEVENT);
//  tme.dwFlags = TME_CANCEL;
//  tme.hwndTrack = hWnd;
//  tme.dwHoverTime = HOVER_DEFAULT;
//  TrackMouseEvent(&tme);
//}






NA_DEF NAWindow* naNewWindow(const char* title, NARect rect, NABool resizeable){
  DWORD style;
  HWND hWnd;
  RECT windowrect;
  NAWINAPIWindow* window = naAlloc(NAWINAPIWindow);
  NARect screenrect;
  //DWORD lasterror;

	//hRC = NULL;
	//hDC = NULL;
 // elements = NULL;
 // numelements = 0;
 // winrect.posx = newposx;
 // winrect.posy = newposy;
 // winrect.width = newwidth;
 // winrect.height = newheight;
 // erasebackground = MB_TRUE;
 // quitonclose = MB_FALSE;
 // fullscreen = MB_FALSE;
 // controller = newcontroller;

  style = WS_OVERLAPPEDWINDOW;
  if(!resizeable){style &= ~WS_THICKFRAME;}
  screenrect = naGetMainScreenRect();

  windowrect.top = (int)(screenrect.size.height - rect.pos.y - rect.size.height);
  windowrect.right = (int)(rect.pos.x + rect.size.width);
  windowrect.bottom = (int)(screenrect.size.height - rect.pos.y);
  windowrect.left = (int)rect.pos.x;
  AdjustWindowRect(&windowrect, style, NA_FALSE);

	hWnd = CreateWindow(
		TEXT("NAWindow"), title,
		style,
		windowrect.left, windowrect.top, windowrect.right - windowrect.left, windowrect.bottom - windowrect.top,
		NULL, NULL, naGetUIElementNativeID(naGetApplication()), NULL);

  //lasterror = GetLastError();
	//hDC = GetDC(hWnd);

  // bugfix: probably remove applicatoin as parent
  naRegisterCoreUIElement((NACoreUIElement*)window, (NAUIElement*)naGetApplication(), NA_UI_WINDOW, hWnd);

  window->flags = 0;
  window->trackingcount = 0;
  window->fullscreen = NA_FALSE;

  return window;
}




NA_HDEF void naRenewWindowMouseTracking(NAWindow* window){
  //window->trackingarea = [[NSTrackingArea alloc] initWithRect:[[(NSWindow*)naGetUIElementNativeID(window) contentView] bounds]
  //    options:NSTrackingMouseMoved | NSTrackingMouseEnteredAndExited | NSTrackingActiveWhenFirstResponder
  //    owner:(NSWindow*)naGetUIElementNativeID(window) userInfo:nil];
  //[[(NSWindow*)naGetUIElementNativeID(window) contentView] addTrackingArea:window->trackingarea];
}



NA_HDEF void naClearWindowMouseTracking(NAWindow* window){
  //[[(NSWindow*)naGetUIElementNativeID(window) contentView] removeTrackingArea:window->trackingarea];
  //[window->trackingarea release];
  //window->trackingarea = nil;
}



NA_HDEF void naRetainWindowMouseTracking(NAWindow* window){
  NAWINAPIWindow* winapiwindow = (NAWINAPIWindow*)window;
  winapiwindow->trackingcount++;
  //if(winapiwindow->trackingcount == 1){
  //  [(NSWindow*)naGetUIElementNativeID(window) setAcceptsMouseMovedEvents:YES];
  //  naRenewWindowMouseTracking(window);
  //}
}



NA_HDEF void naReleaseWindowMouseTracking(NAWindow* window){
  NAWINAPIWindow* winapiwindow = (NAWINAPIWindow*)window;
  winapiwindow->trackingcount--;
  //if(winapiwindow->trackingcount == 0){
  //  [(NSWindow*)naGetUIElementNativeID(window) setAcceptsMouseMovedEvents:NO];
  //  naClearWindowMouseTracking(window);
  //}
}

NA_DEF void naClearWindow(NAWindow* window){
	DestroyWindow(naGetUIElementNativeID(window));
}



NA_DEF void naShowWindow(NAWindow* window){
  ShowWindow(naGetUIElementNativeID(window), SW_SHOW);
}



NA_DEF void naSetWindowContentSpace(NAWindow* window, void* uielement){
  NAWINAPIWindow* winapiwindow = (NAWINAPIWindow*)window;
  naAddListLastMutable(&(winapiwindow->corewindow.uielement.childs), uielement); // todo: this is a hack just for now.
  //NAUIElement* element = (NAUIElement*)uielement;
  //[(NANativeWindow*)(window->uielement.nativeID) setContentView:element->nativeID];
}



NA_DEF void naSetWindowFullscreen(NAWindow* window, NABool fullscreen){
  DWORD style;
  //NABool hasbeenhandeled;
  NARect newrect;
  NARect screenrect;
  NAWINAPIWindow* winapiwindow = (NAWINAPIWindow*)window;

  if(fullscreen != winapiwindow->fullscreen){
    winapiwindow->fullscreen = fullscreen;

    //HWND taskbar = FindWindow(TEXT("Shell_TrayWnd"), NULL);
    //HWND startbutton = FindWindow(TEXT("Button"), NULL);

    screenrect = naGetMainScreenRect();

    if(fullscreen){
      DEVMODE screenSettings;
      winapiwindow->windowedframe = naGetUIElementRect(window, naGetApplication(), NA_TRUE);

      newrect = naGetMainScreenRect();

      memset(&screenSettings, 0, sizeof(screenSettings)); // set everything to 0
      screenSettings.dmSize = sizeof(screenSettings);
      //memcpy(screenSettings.dmDeviceName, fullscreendevicename, CCHDEVICENAME * sizeof(WCHAR));
      screenSettings.dmPelsWidth = (DWORD)newrect.size.width;
      screenSettings.dmPelsHeight = (DWORD)newrect.size.height;
      screenSettings.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;

      style = WS_POPUP;
      SetWindowLongPtr(naGetUIElementNativeID(window), GWL_STYLE, style);
      SetWindowPos(naGetUIElementNativeID(window), HWND_TOPMOST, (int)screenrect.pos.x, (int)(screenrect.pos.y - screenrect.pos.y), (int)screenrect.size.width, (int)screenrect.size.height, SWP_SHOWWINDOW);
      //ChangeDisplaySettings(NULL, 0);
      ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN);
    }else{
      newrect = winapiwindow->windowedframe;
      style = WS_OVERLAPPEDWINDOW;
      SetWindowLongPtr(naGetUIElementNativeID(window), GWL_STYLE, style);
      SetWindowPos(naGetUIElementNativeID(window), HWND_NOTOPMOST, (int)winapiwindow->windowedframe.pos.x, (int)(screenrect.size.height - winapiwindow->windowedframe.pos.y), (int)winapiwindow->windowedframe.size.width, (int)winapiwindow->windowedframe.size.height, SWP_SHOWWINDOW);
      ChangeDisplaySettings(NULL, 0);
    }

    //hasbeenhandeled = naDispatchUIElementCommand(window, NA_UI_COMMAND_RESHAPE, &newrect);
    //if(hasbeenhandeled){naDispatchUIElementCommand(window, NA_UI_COMMAND_REDRAW, NA_NULL);}
  }
}



NA_DEF NABool naIsWindowFullscreen(NAWindow* window){
  NAWINAPIWindow* winapiwindow = (NAWINAPIWindow*)window;
  return winapiwindow->fullscreen;
}



//NA_DEF NARect naGetWindowRect(NAWindow* window){
//  NARect rect;
//  WINDOWINFO windowinfo;
//  windowinfo.cbSize = sizeof(WINDOWINFO);
//  GetWindowInfo(naGetUIElementNativeID(window), &windowinfo);
//  rect.size.width = windowinfo.rcWindow.right - windowinfo.rcWindow.left;
//  rect.size.height = windowinfo.rcWindow.bottom - windowinfo.rcWindow.top;
//  return rect;
//}

#if NA_CONFIG_COMPILE_OPENGL == 1
  NA_DEF NAOpenGLSpace* naNewOpenGLSpace(NAWindow* window, NASize size, NAMutator initfunc, void* initdata){
	
    HWND hWnd;
    HDC hDC;
 	  PIXELFORMATDESCRIPTOR pfd;
    int format;
    NAWINAPIOpenGLSpace* openglspace;

	  hWnd = CreateWindow(
		  TEXT("NASpace"), "OpenGL Space",
		  WS_CHILD | WS_VISIBLE | ES_READONLY,
		  0, 0, (int)size.width, (int)size.height,
		  (HWND)naGetUIElementNativeID(window), NULL, (HINSTANCE)naGetUIElementNativeID(naGetApplication()), NULL );

    openglspace = naAlloc(NAWINAPIOpenGLSpace);
    
    // Bugfix: Probably remove window as parent
    naRegisterCoreUIElement((NACoreUIElement*)openglspace, (NACoreUIElement*)window, NA_UI_OPENGLSPACE, hWnd);

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
	  openglspace->hRC = wglCreateContext(hDC);
	  wglMakeCurrent(hDC, openglspace->hRC);

	  typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALPROC)(int);
	  PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;
	  const char *extensions = (char*)glGetString(GL_EXTENSIONS);
	  wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC)wglGetProcAddress("wglSwapIntervalEXT");
	  if (wglSwapIntervalEXT){wglSwapIntervalEXT(1);}


    // Now the OpenGL context is created and current. We can initialize it
    // if necessary.
    if(initfunc){
      initfunc(initdata);
    }

	  //glewInit();
    return openglspace;
  }


  NA_DEF void naSwapOpenGLBuffer(NAOpenGLSpace* openglspace){
    NAWINAPIOpenGLSpace* winapiopenglspace = (NAWINAPIOpenGLSpace*)openglspace;
    SwapBuffers(GetDC((HWND)naGetUIElementNativeID(&(winapiopenglspace->coreopenglspace.uielement))));
  }



  NA_API void naSetOpenGLInnerRect(NAOpenGLSpace* openglspace, NARect bounds){
    //NARect windowrect = naGetUIElementRect(naGetUIElementParent(openglspace), naGetApplication(), NA_FALSE);
    SetWindowPos((HWND)naGetUIElementNativeID(openglspace), HWND_TOP, 0, 0, (int)bounds.size.width, (int)bounds.size.height, SWP_NOREDRAW);
  }

#endif


NA_DEF void naCenterMouse(void* uielement, NABool includebounds, NABool sendmovemessage){
  NARect spacerect;
  NARect screenframe;
  NAPos centerpos;
  spacerect = naGetUIElementRect(uielement, naGetApplication(), includebounds);
  // todo: screen not defined
  screenframe = naGetMainScreenRect();
  centerpos.x = spacerect.pos.x + spacerect.size.width * .5f;
  centerpos.y = spacerect.pos.y + spacerect.size.height * .5f;

//  naGetUIElementWindow(uielement)->flags |= CUB_WINDOW_IGNORE_MOUSE_WARP;
  naSetMouseWarpedTo(centerpos);
  SetCursorPos((int)centerpos.x, (int)screenframe.size.height - (int)centerpos.y);
}



NA_DEF void naShowMouse(){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  //if(!app->ismousevisible){
  //  ShowCursor(1);
  //  app->ismousevisible = NA_TRUE;
  //}
}


NA_DEF void naHideMouse(){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  //if(app->ismousevisible){
  //  ShowCursor(0);
  //  app->ismousevisible = NA_FALSE;
  //}
}


#endif // NA_OS == NA_OS_WINDOWS
#endif // (NA_CONFIG_COMPILE_GUI == 1)


// Copyright (c) NALib, Tobias Stamm
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
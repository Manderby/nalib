
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



#include "../../../../../NAString.h"
#include "../../../../../NAURL.h"

#include <commctrl.h>

// The following struct stores all relevant data which will then be stored in
// a list of the running NAWINAPIApplication.
typedef struct NATimerStruct NATimerStruct;
struct NATimerStruct {
  UINT key;
  NAMutator func;
  void* arg;
};

// The struct NAWINAPIApplication stores a list of timers which could otherwise
// not be done.
typedef struct NAWINAPIApplication NAWINAPIApplication;
struct NAWINAPIApplication {
  NACoreApplication coreapplication;
  NAList timers;
  HWND offscreenWindow;
  NONCLIENTMETRICS nonclientmetrics; 
};



NABool naApplicationWINAPIProc(NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam){
  NABool hasbeenhandeled = NA_FALSE;

  switch(message){
  default:
    //printf("Uncaught Application message\n");
    break;
  }
  
  return hasbeenhandeled;
}

//@implementation NANativeApplicationDelegate
//- (id) initWithCoreApplication:(NACoreApplication*)newcoreapplication{
//  self = [super init];
//  coreapplication = newcoreapplication;
//  return self;
//}
//- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender{
//  NA_UNUSED(sender);
//  naStopApplication();
//  return NSTerminateCancel;
//}
//- (void)applicationDidFinishLaunching:(NSNotification *)notification{
//  [NSApp applicationDidFinishLaunching:notification];
//}
//@end



NA_DEF void naStartApplication(NAMutator prestartup, NAMutator poststartup, void* arg){
  NAWINAPIApplication* app;
  WNDCLASS wndclass;
  MSG message;

  //SetProcessDPIAware();
  //DPI_AWARENESS awareness = DPI_AWARENESS_SYSTEM_AWARE;
  //SetProcessDpiAwarenessContext(&awareness);

  #if NA_CONFIG_USE_WINDOWS_COMMON_CONTROLS_6 == 1
    InitCommonControls();   // enable visual styles
  #endif
  
  // Register the window class
  naZeron(&wndclass, sizeof(WNDCLASS));
	wndclass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WindowCallback;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = GetModuleHandle(NULL);
	wndclass.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wndclass.hCursor = LoadCursor( NULL, IDC_ARROW );
	wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = TEXT("NAWindow");
	RegisterClass(&wndclass);

    // Register the window class
  naZeron(&wndclass, sizeof(WNDCLASS));
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WindowCallback;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = GetModuleHandle(NULL);
	wndclass.hIcon = NULL;
	wndclass.hCursor = NULL;
	wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = TEXT("NAOffscreenWindow");
	RegisterClass(&wndclass);

  // Register the space class
  naZeron(&wndclass, sizeof(WNDCLASS));
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_PARENTDC;
	wndclass.lpfnWndProc = WindowCallback;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = GetModuleHandle(NULL);
	wndclass.hIcon = LoadIcon( NULL, IDI_APPLICATION );
	wndclass.hCursor = LoadCursor( NULL, IDC_ARROW );
	wndclass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = TEXT("NASpace");
	RegisterClass(&wndclass);

  // Start the WINAPI application and set the native ID of the application.
  app = naNewApplication();

  // Call prestartup if desired.
  if(prestartup){prestartup(arg);}

  // Set the language of the translator
  naResetApplicationPreferredTranslatorLanguages();

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

  // When reaching here, the application had been stopped.
//  naClearCoreApplication();
//  naReleaseUIElement(app);
}




NA_DEF void naResetApplicationPreferredTranslatorLanguages(void){
//  NAInt lang = (NAInt)[[NSLocale preferredLanguages] count] - 1;
//  while(lang >= 0){
//    NSString* language = [[NSLocale preferredLanguages] objectAtIndex:(NSUInteger)lang];
//    NALanguageCode3 langcode = naGetLanguageCode([language UTF8String]);
//    naSetTranslatorLanguagePreference(langcode);
//    lang--;
//  }
}



NA_HDEF NAApplication* naNewApplication(void){

  NAWINAPIApplication* winapiapplication = naAlloc(NAWINAPIApplication);

  naInitCoreApplication(&(winapiapplication->coreapplication), GetModuleHandle(NULL));

  naInitList(&(winapiapplication->timers));

	winapiapplication->offscreenWindow = CreateWindow(
		TEXT("NAOffscreenWindow"), "Offscreen window", WS_OVERLAPPEDWINDOW,
		0, 0, 0, 0,
		NULL, NULL, GetModuleHandle(NULL), NULL);

  winapiapplication->nonclientmetrics.cbSize = sizeof(NONCLIENTMETRICS);
  SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &(winapiapplication->nonclientmetrics), 0);

  return (NAApplication*)winapiapplication;
}



NA_DEF void naDestructApplication(NAApplication* application){
  NAWINAPIApplication* winapiapplication = (NAWINAPIApplication*)application;
  // todo clear the rest of the app.
  naClearCoreApplication(&(winapiapplication->coreapplication));
  
}



HWND naGetApplicationOffscreenWindow(void){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  return app->offscreenWindow;
}

const NONCLIENTMETRICS* naGetApplicationMetrics(void){
  NAWINAPIApplication* app = (NAWINAPIApplication*)naGetApplication();
  return &(app->nonclientmetrics);
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


//#define NA_COCOA_BUNDLE_PLIST @"InfoPlist"
//#define NA_COCOA_BUNDLE_APPLICATION_NAME @"CFBundleDisplayName"
//#define NA_COCOA_BUNDLE_VERSION_SHORT_KEY @"CFBundleShortVersionString"applicationname
//#define NA_COCOA_BUNDLE_VERSION_KEY @"CFBundleVersion"
//#define NA_COCOA_BUNDLE_ICON_FILE_KEY @"CFBundleIconFile"

NA_DEF NAString* naNewApplicationName(void){
  TCHAR modulename[MAX_PATH];
  GetModuleFileName(NULL, modulename, MAX_PATH);

  NAURL url;
  naInitURLWithUTF8CStringLiteral(&url, modulename);
  NAString* applicationname = naNewStringWithURLFilename(&url);
  NAString* applicationbasename = naNewStringWithBasenameOfFilename(applicationname);
  naClearURL(&url);
  naDelete(applicationname);

  return applicationbasename;
}

NA_DEF NAString* naNewApplicationVersionString(void){
//  NSString* versionstring = [[NSBundle mainBundle] objectForInfoDictionaryKey:NA_COCOA_BUNDLE_VERSION_SHORT_KEY];
//  NAString* retstring = naNewStringWithFormat("%s", [versionstring UTF8String]);
//  return retstring;
  return NA_NULL;
}

NA_DEF NAString* naNewApplicationBuildString(void){
//  NSString* buildstring = [[NSBundle mainBundle] objectForInfoDictionaryKey:NA_COCOA_BUNDLE_VERSION_KEY];
//  NAString* retstring = naNewStringWithFormat("%s", [buildstring UTF8String]);
//  return retstring;
  return NA_NULL;
}

NA_DEF NAString* naNewApplicationIconPath(void){
//  NSString* iconfilename = [[NSBundle mainBundle] objectForInfoDictionaryKey:NA_COCOA_BUNDLE_ICON_FILE_KEY];
//  NSString* iconbasename = [iconfilename stringByDeletingPathExtension];
//  NSURL* url = [[NSBundle mainBundle] URLForResource:iconbasename withExtension:@"icns"];
//  NAString* retstring = naNewStringWithFormat("%s", [[url path] UTF8String]);
//  return retstring;
  return NA_NULL;
}

NA_DEF NAString* naNewApplicationResourcePath(const NAUTF8Char* dir, const NAUTF8Char* basename, const NAUTF8Char* suffix){
  NAString* retstring;
  if(dir){
    retstring = naNewStringWithFormat("%s/%s.%s", dir, basename, suffix);
  }else{
    retstring = naNewStringWithFormat("%s.%s", basename, suffix);
  }
  return retstring;
}



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

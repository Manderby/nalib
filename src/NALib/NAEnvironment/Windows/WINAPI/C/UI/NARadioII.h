
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file is an inline implmenentation (II) file which is included in the
// NACocoa.m file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NACocoa.m for more information.
// Do not include this file anywhere else!



typedef struct NAWINAPIRadio NAWINAPIRadio;
struct NAWINAPIRadio {
  NACoreRadio coreradio;
};



NABool naRadioWINAPIProc(NAUIElement* uielement, UINT message, WPARAM wParam, LPARAM lParam){
  NABool hasbeenhandeled = NA_FALSE;

  switch(message){
  default:
    //printf("Uncaught Radio message\n");
    break;
  }
  
  return hasbeenhandeled;
}



//@implementation NANativeRadio
//- (id) initWithCoreRadio:(NACoreRadio*)newcoreradio frame:(NSRect)frame{
//  NSRect newbounds = frame;
//  newbounds.origin.x = 0;
//  newbounds.origin.y = 0;
//
//  self = [super initWithFrame:newbounds];
//  
//  [self setButtonType:NSButtonTypeRadio];
////  [self setBezelStyle:NSBezelStyleRounded];
////  [self setBezelStyle:NSBezelStyleShadowlessSquare];
////  [self setBordered:YES];
//  coreradio = newcoreradio;
//  [self setTarget:self];
//  [self setAction:@selector(onPressed:)];
//
//  containingview = [[NSView alloc] initWithFrame:frame];
//  [containingview addSubview:self];
//
//  return self;
//}
//- (NSView*) getContainingView{
//  return containingview;
//}
//- (void) setText:(const NAUTF8Char*)text{
//  [self setTitle:[NSString stringWithUTF8String:text]];
//}
//- (void) onPressed:(id)sender{
//  NA_UNUSED(sender);
//  naDispatchUIElementCommand((NACoreUIElement*)coreradio, NA_UI_COMMAND_PRESSED);
//}
//- (void) setRadioState:(NABool)state{
//  [self setState:state ? NSOnState : NSOffState];
//}
//@end



NA_DEF NARadio* naNewRadio(const NAUTF8Char* text, NARect rect){
  HWND hWnd;
  DWORD style;

  NAWINAPIRadio* winapiradio = naAlloc(NAWINAPIRadio);

  style = WS_CHILD | WS_VISIBLE | BS_LEFT | BS_VCENTER | BS_TEXT | BS_AUTORADIOBUTTON;

	hWnd = CreateWindow(
		TEXT("BUTTON"), text, style,
		(int)rect.pos.x, (int)rect.pos.y, (int)rect.size.width, (int)rect.size.height,
		naGetApplicationOffscreenWindow(), NULL, (HINSTANCE)naGetUIElementNativeID(naGetApplication()), NULL );
  
  naInitCoreRadio(&(winapiradio->coreradio), hWnd);

  return (NARadio*)winapiradio;
}



NA_DEF void naDestructRadio(NARadio* radio){
//  NACoreRadio* coreradio = (NACoreRadio*)radio;
//  naClearCoreRadio(coreradio);
}



NA_HDEF NARect naGetRadioAbsoluteInnerRect(NACoreUIElement* radio){
  NA_UNUSED(radio);
  return naMakeRectS(20, 40, 100, 50);
}



NA_HDEF void naSetRadioState(NARadio* radio, NABool state){
//  naDefineNativeCocoaObject(NANativeRadio, nativeradio, radio);
//  [nativeradio setRadioState:state];
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

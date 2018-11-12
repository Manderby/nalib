
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

// This file contains inline implementations for the tree node.
// Do not include this file directly! It will automatically be included when
// including "NATreeII.h"



#define NA_TREE_NODE_CHILD_MASK 0x03
#define NA_TREE_NODE_CHILD_AVAILABLE_MASK 0x02



NA_HIDEF NABool naIsNodeChildTypeValid(NANodeChildType childtype){
  return naTestFlagi(childtype, NA_TREE_NODE_CHILD_AVAILABLE_MASK);
}



NA_HIDEF NANodeChildType naGetNodeChildType(NATreeNode* node, NAInt childindx){
  return (NANodeChildType)((node->flags >> (childindx * 2)) & NA_TREE_NODE_CHILD_MASK);
}



NA_HIDEF void naSetNodeChildType(NATreeNode* node, NAInt childindx, NANodeChildType childtype){
  // Clear the old type
  naSetFlagi(&(node->flags), NA_TREE_NODE_CHILD_MASK << (childindx * 2), NA_FALSE);
  // Set the new type
  naSetFlagi(&(node->flags), childtype << (childindx * 2), NA_TRUE);
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

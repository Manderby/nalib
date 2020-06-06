
#include "../../NATree.h"
#include "../../NAKey.h"
#include "NATreeBin.h"
#include "NATreeQuad.h"
#include "NATreeOct.h"



NA_DEF NATreeConfiguration* naCreateTreeConfiguration(NAInt flags){
  // This is just for testing if the implemented nodes "inheriting" from the
  // NATreeNode structure have their childs storage at the correct position.
  #ifndef NDEBUG
    int nodeChildsOffset;
  #endif

  NATreeConfiguration* config = naAlloc(NATreeConfiguration);
  naZeron(config, sizeof(NATreeConfiguration));
  config->flags = flags;
  naInitRefCount(&(config->refCount));

  #ifndef NDEBUG
    // Just some security measures in case the programmer sees no purpos in
    // setting it.
    config->leafKeyOffset        = -1;
    config->nodeKeyOffset        = -1;
    config->leafUserDataOffset   = -1;
    config->nodeUserDataOffset   = -1;
  #endif
  
  if(flags & NA_TREE_QUADTREE){
  
    #ifndef NDEBUG
      config->sizeofNode = sizeof(NATreeQuadNode);
      config->sizeofLeaf = sizeof(NATreeQuadLeaf);
    #endif
    
    config->childpernode            = 4;
    switch(flags & NA_TREE_CONFIG_KEY_TYPE_MASK){
    case NA_TREE_KEY_DOUBLE:
      config->childIndexGetter      = naGetChildIndexQuadDouble;
      config->keyIndexGetter        = naGetKeyIndexQuadDouble;
      config->keyEqualComparer      = NA_KEY_OP(Equal, NAPos);
      config->keyLowerComparer      = NA_KEY_OP(Lower, NAPos);
      config->keyLowerEqualComparer = NA_KEY_OP(LowerEqual, NAPos);
      config->keyAssigner           = NA_KEY_OP(Assign, NAPos);
      config->keyTester             = naTestKeyQuadDouble;
      config->keyNodeContainTester  = naTestKeyNodeContainQuadDouble;
      config->keyLeafContainTester  = naTestKeyLeafContainQuadDouble;
      break;
    default:
      #ifndef NDEBUG
        naError("Quadtree must have type double. Other options are not implemented yet.");
      #endif
      break;
    }
    if(flags & NA_TREE_BALANCE_AVL){
      #ifndef NDEBUG
        naError("Quadtree can not have AVL balance.");
      #endif
    }
    config->nodeDestructor          = naDestructTreeNodeQuad;
    config->leafDestructor          = naDestructTreeLeafQuad;

    config->bubbleLocator           = naLocateBubbleQuad;
    config->leafRemover             = naRemoveLeafQuad;
    config->leafInserter            = naInsertLeafQuad;

    #ifndef NDEBUG
      nodeChildsOffset                = NODE_CHILDS_OFFSET_QUAD;
    #endif
    config->leafKeyOffset           = LEAF_KEY_OFFSET_QUAD;
    config->nodeKeyOffset           = NODE_KEY_OFFSET_QUAD;
    config->leafUserDataOffset      = LEAF_USERDATA_OFFSET_QUAD;
    config->nodeUserDataOffset      = NODE_USERDATA_OFFSET_QUAD;

  }else if(flags & NA_TREE_OCTTREE){
  
    #ifndef NDEBUG
      config->sizeofNode = sizeof(NATreeOctNode);
      config->sizeofLeaf = sizeof(NATreeOctLeaf);
    #endif

    config->childpernode            = 8;
    switch(flags & NA_TREE_CONFIG_KEY_TYPE_MASK){
    case NA_TREE_KEY_DOUBLE:
      config->childIndexGetter      = naGetChildIndexOctDouble;
      config->keyIndexGetter        = naGetKeyIndexOctDouble;
      config->keyEqualComparer      = NA_KEY_OP(Equal, NAVertex);
      config->keyLowerComparer      = NA_KEY_OP(Lower, NAVertex);
      config->keyLowerEqualComparer = NA_KEY_OP(LowerEqual, NAVertex);
      config->keyAssigner           = NA_KEY_OP(Assign, NAVertex);
      config->keyTester             = naTestKeyOctDouble;
      config->keyNodeContainTester  = naTestKeyNodeContainOctDouble;
      config->keyLeafContainTester  = naTestKeyLeafContainOctDouble;
      break;
    default:
      #ifndef NDEBUG
        naError("Octtree must have type double. Other options are not implemented yet.");
      #endif
      break;
    }
    if(flags & NA_TREE_BALANCE_AVL){
      #ifndef NDEBUG
        naError("Octtree can not have AVL balance.");
      #endif
    }
    config->nodeDestructor          = naDestructTreeNodeOct;
    config->leafDestructor          = naDestructTreeLeafOct;
    
    config->bubbleLocator           = naLocateBubbleOct;
    config->leafRemover             = naRemoveLeafOct;
    config->leafInserter            = naInsertLeafOct;

    #ifndef NDEBUG
      nodeChildsOffset                = NODE_CHILDS_OFFSET_OCT;
    #endif
    config->leafKeyOffset           = LEAF_KEY_OFFSET_OCT;
    config->nodeKeyOffset           = NODE_KEY_OFFSET_OCT;
    config->leafUserDataOffset      = LEAF_USERDATA_OFFSET_OCT;
    config->nodeUserDataOffset      = NODE_USERDATA_OFFSET_OCT;

  }else{

    #ifndef NDEBUG
      config->sizeofNode = sizeof(NATreeBinNode);
      config->sizeofLeaf = sizeof(NATreeBinLeaf);
    #endif

    config->childpernode            = 2;
    switch(flags & NA_TREE_CONFIG_KEY_TYPE_MASK){
    case NA_TREE_KEY_NOKEY:
      config->keyIndexGetter        = NA_NULL;
      config->keyEqualComparer      = NA_NULL;
      config->keyLowerComparer      = NA_NULL;
      config->keyLowerEqualComparer = NA_NULL;
      config->keyAssigner           = NA_NULL;
      config->keyTester             = NA_NULL;
      config->keyNodeContainTester  = NA_NULL;
      config->keyLeafContainTester  = NA_NULL;
      break;
    case NA_TREE_KEY_DOUBLE:
      config->childIndexGetter      = naGetChildIndexBinDouble;
      config->keyIndexGetter        = naGetKeyIndexBinDouble;
      config->keyEqualComparer      = NA_KEY_OP(Equal, double);
      config->keyLowerComparer      = NA_KEY_OP(Lower, double);
      config->keyLowerEqualComparer = NA_KEY_OP(LowerEqual, double);
      config->keyAssigner           = NA_KEY_OP(Assign, double);
      config->keyTester             = naTestKeyBinDouble;
      config->keyNodeContainTester  = NA_NULL;
      config->keyLeafContainTester  = naTestKeyLeafContainBinDouble;
      break;
    case NA_TREE_KEY_NAINT:
      config->childIndexGetter      = naGetChildIndexBinNAInt;
      config->keyIndexGetter        = naGetKeyIndexBinNAInt;
      config->keyEqualComparer      = NA_KEY_OP(Equal, NAInt);
      config->keyLowerComparer      = NA_KEY_OP(Lower, NAInt);
      config->keyLowerEqualComparer = NA_KEY_OP(LowerEqual, NAInt);
      config->keyAssigner           = NA_KEY_OP(Assign, NAInt);
      config->keyTester             = naTestKeyBinNAInt;
      config->keyNodeContainTester  = NA_NULL;
      config->keyLeafContainTester  = naTestKeyLeafContainBinNAInt;
      break;
    default:
      #ifndef NDEBUG
        naError("Invalid key type in flags");
      #endif
      break;
    }

    config->nodeDestructor          = naDestructTreeNodeBin;
    config->leafDestructor          = naDestructTreeLeafBin;

    config->bubbleLocator           = naLocateBubbleBin;
    config->leafRemover             = naRemoveLeafBin;
    config->leafInserter            = naInsertLeafBin;
    
    #ifndef NDEBUG
      nodeChildsOffset                = NODE_CHILDS_OFFSET_BIN;
    #endif
    config->leafKeyOffset           = LEAF_KEY_OFFSET_BIN;
    config->nodeKeyOffset           = NODE_KEY_OFFSET_BIN;
    config->leafUserDataOffset      = LEAF_USERDATA_OFFSET_BIN;
    config->nodeUserDataOffset      = NODE_USERDATA_OFFSET_BIN;
  }
  
  #ifndef NDEBUG
    if(nodeChildsOffset != NA_TREE_NOTE_CHILDS_OFFSET)
      naError("The childs storage must come right after the node storage.");
  #endif
  
  return config;
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

/***************************************************************************
                          opstack.h  -  description
                             -------------------
    begin                : Sun Nov 3 2002
    copyright            : (C) 2002 by Derek Meek
    email                : kazan@cs.iastate.edu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#if !defined(__op_stack_h_)
#define __op_stack_h_

#if defined(WIN32)
#include <windows.h>
#pragma warning ( disable : 4786 )
#endif

#include "oplinkedlist.h"

template <class STYPE> class opstack {
  private:
   op_linkedlist<STYPE> myList;
  public:
   opstack() {};
   void Push(STYPE data) { myList.InsertAtHead(data); }
   void Pop() { myList.RemoveHead(); }
   void Pop(STYPE& top) { top = Top(); Pop(); }
   STYPE Top();
};

template <class STYPE>
STYPE opstack<STYPE>::Top()
{
  STYPE retval;
  memset(&retval, 0, sizeof(STYPE)); // this makes a safe null-retval
  if (myList.Head() != NULL)
      retval = myList.Head()->data;
  return retval;
}


#endif

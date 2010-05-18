/***************************************************************************
                          oplinkedlist.h  -  description
                             -------------------
    begin                : Sun Nov 3 2002
    copyright            : (C) 2002 by Derek Meek
    email                : kazan@cs.iastate.edu
 ***************************************************************************/


#if !defined(__oplinkedlist_h_)
#define __oplinkedlist_h_

#if defined(WIN32)
#include <windows.h>
#pragma warning ( disable : 4786 )
#endif

#include <iostream>

template <class NODETYPE> struct op_ll_node
{
  NODETYPE data;
  op_ll_node *next;
};

template <class LLTYPE> class op_linkedlist {
  private:
    op_ll_node<LLTYPE> *head, *tail;
  public:
    op_linkedlist() : head(NULL), tail(NULL) {}
    ~op_linkedlist() { DestoryList(); }

    void InsertAtHead(LLTYPE &data);
    void InsertAtTail(LLTYPE &data);
    void InsertInOrder(LLTYPE &data);

    void Remove(LLTYPE &data);

    // two quick and dirty shortcuts for opstack to use
    void RemoveHead();
    const op_ll_node<LLTYPE>* Head() { return head; }
    void DestoryList();

    // our iterator
    //friend class op_linkedlist_iterator;
};


template <class LLTYPE> class op_linkedlist_iterator
{
  private:
    op_ll_node<LLTYPE> *cur;
  public:
    op_linkedlist_iterator(const op_ll_node<LLTYPE> &ll)
        { cur = ll.Head(); }

    op_linkedlist_iterator(const op_linkedlist_iterator<LLTYPE> &lli)
        { cur = lli.cur; }

    const LLTYPE &CurData()
        { return cur->data; }

    bool Cur()
      { return cur != NULL; }

    void Next()
      { cur = cur->next; }

    bool IsNext()
      { return cur->next != NULL; }
};

// implementation of a templated function/class goes in it's header

template <class LLTYPE>
void op_linkedlist<LLTYPE>::DestoryList()
{
  if (head == NULL)
    return;

  op_ll_node<LLTYPE> *prev, *cur;
  cur = head;
  while (cur)
  {
    prev = cur;
    cur = cur->next;
    delete prev;
  }
}

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
template <class LLTYPE>
void op_linkedlist<LLTYPE>::InsertAtHead(LLTYPE &data)
{
        op_ll_node<LLTYPE> *nnode = new op_ll_node<LLTYPE>;

        if (head == NULL) // if the head is null we must also set the tail
          tail = nnode;

        nnode->data = data;
        nnode->next = head;
        head = nnode;

}

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

template <class LLTYPE>
void op_linkedlist<LLTYPE>::InsertAtTail(LLTYPE &data)
{
        op_ll_node<LLTYPE> *nnode = new op_ll_node<LLTYPE>;
        nnode->data = data;
        nnode->next = NULL;

        if (tail != NULL)
        {
            tail->next = nnode;
            tail = nnode;
        }
        else // we must be empy
        {
          head = tail = nnode;
        }
}


//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

template <class LLTYPE>
void op_linkedlist<LLTYPE>::InsertInOrder(LLTYPE &data)
{
        op_ll_node<LLTYPE> *nnode = new op_ll_node<LLTYPE>;
        nnode->data = data;
        nnode->next = NULL;

        if (head == NULL) // empty
        {
            head = tail = nnode;
            return;
        }

        if (head->data > data) // easy case to intercept
        {
          nnode->next = head;
          head = nnode;
          return;
        }

        if (tail->data < data) // another easy case to intercept
        {
          tail->next = nnode;
          tail = nnode;
          return;
        }

        op_ll_node<LLTYPE> *prev = head, *cur = head->next;

        while (cur && cur->data < data)
        {
            prev = cur;
            cur = cur->next;
        }
        assert(cur != NULL); // if it's null, something borked up really good
        prev->next = nnode;
        nnode->next = cur;
}

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

template <class LLTYPE>
void op_linkedlist<LLTYPE>::Remove(LLTYPE &data)
{
  if (head == NULL)
     return;

  if (head->data == data) // easy case to intercept
  {
     op_ll_node<LLTYPE> *rem = head;
     head = head->next;
     delete rem;
     return;
  }

  op_ll_node<LLTYPE> *prev = head, *cur = head->next;
  while (cur->data != data)
  {
      prev = cur;
      cur = cur->next;
  }

  if (cur == NULL)
    return;
  if (cur != tail)
  {
    prev->next = cur->next;
    delete cur;
  }
  else
  {
    tail = prev;
    prev->next = NULL;
    delete cur;
  }
}

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

template <class LLTYPE>
void op_linkedlist<LLTYPE>::RemoveHead()
{
  if (head == NULL)
    return;

  op_ll_node<LLTYPE> *rem = head;
  head = head->next;
  delete rem;
}

#endif

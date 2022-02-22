/***************************************************************************
Orignal Author:       Caz

$Author: vince $
$Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $
$Revision: 92 $
$Source: /cvsroot/libs/libti/src/tllist.h,v $

***************************************************************************/
#ifndef TLLIST_H
#define TLLIST_H "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(TLLIST_H);
#include "ti.h"
#include "xcept.h"

#define XLList xLList(__FILE__, __LINE__)

class xLList : public xCept
{

public:
  xLList ( pchar aFname, int aLine ) : xCept(aFname,aLine,"LList index error",0) {}
  xLList ( const xLList& aX) : xCept(aX) {}
};


/**
 * A Linked List template.
 *
 * It collects pointers to objects. Not objects.
 * It does not take ownership of the
 * pointer. U are still responsible for deleting the pointer once
 * you're done with it. Zap removes it from the list but delete has
 * not been called for the object you passed to append.
 *
 * Creating copies creates completely seperate linked lists. Objects
 * pointed to are still the same objects. You could delete either list with
 * no strange implications. Do NOT delete an object and forget to remove it
 * from BOTH lists tho.
 *
 * Indexing errors result in xLList exception being thrown
 */
template <class tItem>
struct tLList
{

  long len() {
    return m_size;
  }

  tLList() {
    top = bottom = 0;
    m_size = 0;
  }
  tLList(tLList<tItem> &ref) {
    top = bottom = 0;
    m_size = 0;
    for (int i = 0; i < ref.m_size; i++)
    {
      append(ref[i]);
    }
  }

  virtual ~tLList() {
    clear();
  }

  virtual void clear() {
    for (tNode *h,*w = top; w; w = h)
    {
      h = w->nxt;
      delete w;
    }
    top = bottom = 0;
    m_size = 0;
  }
  void clearDelete() {
    while (len())
    {
      delete zap(0);
    }
  }

  virtual void append(tItem *objI)
  {
    tNode *obj = new tNode(objI);
    if (m_size++)
    {
      obj->prv = bottom;
      obj->nxt = 0;
      bottom->nxt = obj;
      bottom = obj;
    }
    else
    {
      top = obj;
      bottom = obj;
      obj->nxt = 0;
      obj->prv = 0;
    }
  }
  virtual void append(tLList<tItem> &ref) {
    for (int i = 0; i < ref.m_size; i++)
    {
      append(ref[i]);
    }
  }

  virtual void prepend(tItem *objI)
  {
    tNode *obj = new tNode(objI);
    if (m_size++)
    {
      obj->prv = 0;
      obj->nxt = top;
      top->prv = obj;
      top = obj;
    }
    else
    {
      top = obj;
      bottom = obj;
      obj->nxt = 0;
      obj->prv = 0;
    }
  }

  virtual tItem *zap(int no) {
    tNode *obj = findNode(no);
    tItem *ret = obj->item;
    zapNode(obj);
    return ret;
  }


  tItem *find(int no)
  {
    if (no < 0 || no >= m_size)
    {
      throw XLList;
    }
    tNode *walk = top;
    for (; walk && no; walk = walk->nxt,no--);
    return(walk->item);
  }

  // Finds the object that is ==(obj)
  int find(tItem &obj)
  { tNode *walk = top;  int no = 0;
    for (; walk; walk = walk->nxt,no++)
      if (obj == *walk->item) return no;
    return(-1);
  }

  tItem* operator[](int i) {
    return(find(i));
  }
  int operator[](tItem &obj) {
    return(find(obj));
  }
  tLList<tItem> &operator=(tLList<tItem> &obj) {
    clear();
    for (int i = 0; i < obj.m_size; i++)
    {
      append(obj[i]);
    }
    return(*this);
  }

protected:
  struct tNode
  {
    tNode *nxt,*prv;
    tItem *item;
    tNode(tItem *i) {
      item = i;
    }
  }*top,*bottom,*iterate;

  long m_size;


  tNode *findNode(tItem *obj) {
    tNode *walk = top;
    for (; walk->item!=obj && walk; walk = walk->nxt);
    return(walk);
  }
  tNode *findNode(int no)
  {
    if (no < 0 || no >= m_size)
    {
      throw XLList;
    }
    tNode *walk = top;
    for (; walk && no; walk = walk->nxt,no--);
    return(walk);
  }

  void zapNode(tNode *obj){
    if (obj)
    {
      --m_size;
      if (obj == top) top = top->nxt;if (obj == bottom)  bottom = bottom->prv;
      if (obj->prv) obj->prv->nxt = obj->nxt;if (obj->nxt) obj->nxt->prv = obj->prv;
      obj->prv = obj->nxt = 0;
      delete obj;
    }
  }
};

#endif

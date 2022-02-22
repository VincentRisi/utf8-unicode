/***************************************************************************
System:               TIBOS Interface.
Orignal Author:       Caz

$Author: vince $
$Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $
$Revision: 92 $
$Source: /cvsroot/libs/libti/src/tstringlist.h,v $

***************************************************************************/
#ifndef TSTRINGLIST_H
#define TSTRINGLIST_H "$Revision: 92 $ $Date: 2005-04-21 11:34:40 +0200 (Thu, 21 Apr 2005) $"
#include "versions.h"
HEADER_VERSION(TSTRINGLIST_H);
#include "tllist.h"
#include "bstring.h"

/**
 * List of strings
 * Indexing errors causes xLList exceptions
 *
 * Example usage:
 *   tStringList lst;
 *
 *   lst.add("Halo");
 *   lst.add("World");
 *
 *   lst.add("!!!");
 *   lst.set(0,"Hello ");
 *
 *   for (int i = 0; i < lst.len(); i++)
 *     printf(lst[i]);
 *
 * Results in
 *   Hello World!!!
 */
class tStringList
{
  tLList<bstring> values;
public:
  tStringList() {
  }
  tStringList(tStringList &ref) {
    for (int i = 0; i < ref.len(); i++)
    {
      add(ref[i]);
    }
  }
  virtual ~tStringList() {
    for (int i = 0; i < values.len(); i++)
    {
      delete values[i];
    }
  }

  int len() {
    return values.len();
  }

  void add(char *str) {
    values.append(new bstring(str));
  }


  /**
   * Searches by comparing strings using the == operator.
   * In other words it searches for a string with equivalent content
   *
   * @param s  String to track down
   * @return -1 if string not found, else the index starting from 0
   */
  int indexOf(bstring s) {
    return values[s];
  }

  void remove(int i) {
    delete values[i];
    values.zap(i);
  }

  void set(int i, char *str) {
    while (i >= len()) add("");
    *values[i] = str;
  }

  void sizeto(int sz, char *str="") {
    while (sz > len()) add(str);
  }

  char *operator [](int i) {
    char *p = *values[i];
    return p ? p : (char*)"";
  }

  void clear() {
    for (int i = 0; i < values.len(); i++)
    {
      delete values[i];
    }
    values.clear();
  }
};



/**
 * String dictionary object
 *
 * Takes a name and value, creates copies of them and store them associated in the dictionary
 *
 * Examples of use:
 *   tStringDictionary dict;
 *
 *   dict.set("String1","Hello");
 *   dict.set("String2","world");
 *
 *   printf("%s %s\n",dict["String1"],dict["String2"]);
 * Prints
 *   Hello world
 */
class tStringDictionary
{
protected:
  tLList<bstring> names;
  tLList<bstring> values;
  bstring badIndex;                            // Return empty badIndex when not in dictionary

public:
  tStringDictionary() {
  }
  virtual ~tStringDictionary() {
    for (int i = 0; i < names.len(); i++)
    {
      delete names[i];
      delete values[i];
    }
  }

  void set(bstring name, bstring value) {
    int idx = names[name];
    if (idx < 0)
    {
      names.append(new bstring(name));
      values.append(new bstring(value));
    }
    else
    {
      *values[idx] = value;
    }
  }

  void clear() {
    for (int i = 0; i < names.len(); i++)
    {
      delete names[i];
      delete values[i];
    }
    names.clear();
    values.clear();
  }

  void remove(bstring &name) {
    int idx = names[name];
    if (idx >= 0)
    {
      delete names[idx];
      delete values[idx];
      names.zap(idx);
      values.zap(idx);
    }
  }

  /**
   * Gets the dictionary value for a key
   *
   * @param name   KeyName
   * @return Reference to the value, or simply an empty bstring
   *         if the key isn't in the dictionary
   */
  bstring& value(bstring name) {
    int idx = names[name];
    return idx >= 0 ? *values[idx] : badIndex;
  }

  int isNull(bstring name) {
    return names[name] < 0;
  }

  int len() {
    return names.len();
  }

  bstring& value(int idx) {
    return *values[idx];
  }
  bstring& name(int idx) {
    return *names[idx];
  }
  char *cstrValue(int idx) {
    char *p = *values[idx];
    return p ? p : (char*)"";
  }
  char *cstrName(int idx) {
    char *p = *names[idx];
    return p ? p : (char*)"";
  }

  char *operator [](char *name) {
    char *p = value(name);
    return p ? p : (char*)"";
  }
};

#endif /* TSTRINGLIST_H */

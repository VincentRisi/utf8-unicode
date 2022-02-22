#error "Do not include me, include xmlrecord.h"
//#ifndef __XMLREADER_H__
//#define __XMLREADER_H__
//
//#include "xmlparse.h"
//#include "xmlmake.h"
//#include "tbuffer.h"
//
//struct TXMLRecord
//{
//  TXMLElement *Top;
//  char memberChar;
//  char attributeChar;
//  TXMLRecord(char aMemberChar='.', char aAttributeChar='/')
//  : Top(0)
//  , memberChar(aMemberChar)
//  , attributeChar(aAttributeChar)
//  , CurrElement(Top)
//  {}
//  void InsertTag(char *key, char *value);
//  void AsXML(TBChar &Message);
//  void Start()
//  {
//    CurrElement = Top;
//    CurrElement->ElementIndex = 0;
//    CurrElement->AttrIndex    = 0;
//    CurrElement->ValueIndex   = 0;
//  }
//  bool Next(TBChar &key, TBChar &value, int &noOf, int &count);
//  void NewElement(char *key, char *value=0);
//  void InsertValue(char *value);
//  void InsertAttribute(char *key, char *value);
//  void ToParent() {if (CurrElement) CurrElement = CurrElement->Parent;}
//  void Load(TBChar &Input);
//protected:
//  TXMLElement *CurrElement;
//  void AsXML(TBChar &Message, TXMLElement* Entry, int pad);
//  void MakeKey(TBChar &Key);
//};
//
//#endif

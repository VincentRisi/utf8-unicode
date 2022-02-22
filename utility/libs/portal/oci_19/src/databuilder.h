#ifndef _DATABUILD_H_
#define _DATABUILD_H_
#include "dbportal.h"

// ---------------------------------------------------------
// This is the base class for dbportal generated .sh files
// It does not have count and show input outputs flag
// PLEASE DO NOT ADD APPLICATION FUNCTION AND VARIABLES HERE
// T H A T   M E A N S   Y  O  U
// ---------------------------------------------------------

class DataBuilder
{
public:
    virtual void name(const char *inName) = 0;

    virtual void fill(size_t size) {}
    virtual void add(const char *name, int8    value) = 0;
    virtual void add(const char *name, int16   value) = 0;
    virtual void add(const char *name, int32   value) = 0;
    virtual void add(const char *name, int64   value) = 0;
    virtual void add(const char *name, char   *value, size_t size) = 0;
    virtual void add(const char *name, double  value) = 0;
    virtual void add(const char *name, void   *value, size_t size) = 0;
    virtual void add(const char *name, tHugeCHAR &value) = 0;

    virtual void skip(size_t size) {}
    virtual void set(const char *name, int8   &value, size_t size) = 0;
    virtual void set(const char *name, int16  &value, size_t size) = 0;
    virtual void set(const char *name, int32  &value, size_t size) = 0;
    virtual void set(const char *name, int64  &value, size_t size) = 0;
    virtual void set(const char *name, char   *value, size_t size) = 0;
    virtual void set(const char *name, double &value, size_t size) = 0;
    virtual void set(const char *name, void   *value, size_t size) = 0;
    virtual void set(const char *name, tHugeCHAR &value, size_t size) = 0;
};

#endif

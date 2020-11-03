#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>

using std::exception;

struct NoLeftValueException : exception
{
  const char * what () const throw ()
  {
    return "NoLeftValueException";
  }
};

struct RedefineException : exception
{
  const char * what () const throw ()
  {
    return "RedefineException";
  }
};

struct InvalidTypeConversionException : exception
{
  const char * what () const throw ()
  {
    return "InvalidTypeConversionException";
  }
};

struct NoMatchCallException : exception
{
  const char * what () const throw ()
  {
    return "NoMatchCallException";
  }
};

struct AmbiguousCallException : exception
{
  const char * what () const throw ()
  {
    return "AmbiguousCallException";
  }
};

struct InvalidExpressionException : exception
{
  const char * what () const throw ()
  {
    return "InvalidExpressionException";
  }
};

struct UnknownIdentifierException : exception
{
  const char * what () const throw ()
  {
    return "UnknownIdentifierException";
  }
};


struct InvalidBreakContinueException : exception
{
  const char * what () const throw ()
  {
    return "InvalidBreakContinueException";
  }
};

#endif
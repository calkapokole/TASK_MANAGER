#ifndef INVALIDENUMEXCEPTION_H
#define INVALIDENUMEXCEPTION_H

#include <QtCore>

class InvalidEnumException : public QtConcurrent::Exception
{
public:
    void raise() const { throw *this; }
    Exception *clone() const { return new InvalidEnumException(*this); }
    virtual const char * what() const throw() { return "Enum out of range."; }

};

#endif // INVALIDENUMEXCEPTION_H

#ifndef INVALIDDATATYPEEXCEPTION_H
#define INVALIDDATATYPEEXCEPTION_H

#include <QtCore>

class InvalidDataTypeException : public QtConcurrent::Exception
{
public:
    void raise() const { throw *this; }
    Exception *clone() const { return new InvalidDataTypeException(*this); }
    virtual const char * what() const throw() { return "Data type is invalid."; }

};

#endif // INVALIDDATATYPEEXCEPTION_H

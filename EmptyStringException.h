#ifndef EMPTYSTRINGEXCEPTION_H
#define EMPTYSTRINGEXCEPTION_H

#include <QtCore>

class EmptyStringException : public QtConcurrent::Exception
{
public:
    void raise() const { throw *this; }
    Exception *clone() const { return new EmptyStringException(*this); }
    virtual const char * what() const throw() { return "Empty string is not acceptable."; }

};

#endif // EMPTYSTRINGEXCEPTION_H

#ifndef MONEY_H
#define MONEY_H

#include <cstddef>

class Money{
    private:
        static const size_t SIZE = 39;
        unsigned char* m_digits;
    
    public:
         Money();
         Money(long long ruble, unsigned char kop);
         Money(const Money& other);

         ~Money();

         Money add(const Money& other) const;
         Money subtract(const Money& other) const;

         bool isEqual(const Money& other) const;
         bool isGreater(const Money& other) const;
         bool isLess(const Money& other) const;
         
         void print() const;
};

#endif
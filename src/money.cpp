#include "../include/money.h"
#include <iostream>
#include <stdexcept>

using namespace std;

Money::Money() {
    m_digits = new unsigned char[SIZE];
    for(int i = 0; i < SIZE; i++) {
        m_digits[i] = 0;
    }
}

Money::Money(long long rubles, unsigned char kopecks) {
    if(rubles < 0) {
        throw invalid_argument("Рубли не могут быть отрицательными");
    }
    if(kopecks >= 100) {
        throw invalid_argument("Слишком много копеек");
    }
    
    m_digits = new unsigned char[SIZE];
    
    for(int i = 0; i < SIZE; i++) {
        m_digits[i] = 0;
    }
    
    m_digits[0] = kopecks % 10;
    m_digits[1] = kopecks / 10;
    
    int index = 2;
    long long temp = rubles;
    while(temp > 0 && index < SIZE) {
        m_digits[index] = temp % 10;
        temp = temp / 10;
        index++;
    }
}

Money::Money(const Money& other) {
    m_digits = new unsigned char[SIZE];
    for(int i = 0; i < SIZE; i++) {
        m_digits[i] = other.m_digits[i];
    }
}

Money::~Money() {
    delete[] m_digits;
}

Money Money::add(const Money& other) const {
    Money result;
    int carry = 0;
    
    for(int i = 0; i < SIZE; i++) {
        int sum = m_digits[i] + other.m_digits[i] + carry;
        result.m_digits[i] = sum % 10;
        carry = sum / 10;
    }
    
    if(carry > 0) {
        throw overflow_error("Переполнение");
    }
    
    return result;
}

Money Money::subtract(const Money& other) const {
    if(this->isLess(other)) {
        throw invalid_argument("Результат отрицательный");
    }
    
    Money result;
    int borrow = 0;
    
    for(int i = 0; i < SIZE; i++) {
        int diff = m_digits[i] - other.m_digits[i] - borrow;
        
        if(diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        
        result.m_digits[i] = diff;
    }
    
    return result;
}

bool Money::isEqual(const Money& other) const {
    for(int i = 0; i < SIZE; i++) {
        if(m_digits[i] != other.m_digits[i]) {
            return false;
        }
    }
    return true;
}

bool Money::isGreater(const Money& other) const {
    for(int i = SIZE - 1; i >= 0; i--) {
        if(m_digits[i] > other.m_digits[i]) {
            return true;
        } else if(m_digits[i] < other.m_digits[i]) {
            return false;
        }
    }
    return false;
}

bool Money::isLess(const Money& other) const {
    return !isEqual(other) && !isGreater(other);
}

void Money::print() const {
    bool found = false;
    for(int i = SIZE - 1; i >= 2; i--) {
        if(m_digits[i] != 0) found = true;
        if(found) cout << (int)m_digits[i];
    }
    if(!found) cout << "0";
    
    cout << " руб. ";
    
    cout << (int)m_digits[1] << (int)m_digits[0] << " коп." << endl;
}
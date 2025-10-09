#include "include/money.h"
#include <iostream>

using namespace std;

int main() {
    cout << "Тест класса Money" << endl;
    
    Money a;
    cout << "Пустой объект: ";
    a.print();
    
    Money b(150, 75);
    cout << "150.75: ";
    b.print();
    
    Money c = b;
    cout << "Копия: ";
    c.print();
    
    Money d(50, 25);
    Money e = b.add(d);
    cout << "150.75 + 50.25 = ";
    e.print();
    
    Money f = b.subtract(d);
    cout << "150.75 - 50.25 = ";
    f.print();
    
    if(b.isEqual(c)) {
        cout << "b и c равны" << endl;
    }
    
    if(b.isGreater(d)) {
        cout << "b больше d" << endl;
    }
    
    try {
        Money wrong(100, 150);
    } catch(...) {
        cout << "Поймали ошибку с копейками!" << endl;
    }
    
    cout << "Конец теста" << endl;
    return 0;
}
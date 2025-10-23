#include "../include/figure.h"

//Реализация оператора ввода-ввывода для любой фигуры
//figure - ссылка на фигуру, которой может быть любой наследник Figure
std::ostream& operator<<(std::ostream& os, const Figure& figure){
    figure.print(os);
    return os;
}

std::istream& operator>>(std::istream& is, Figure& figure){
    figure.read(is);
    return is;
}

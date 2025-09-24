#include "lab1_functions.h"

bool isPerfectSquare(int number){
    if (number < 1) return false;
    for(int i = 1; i <= number; i++){
        if (i*i == number){
            return true;
        }else if(i * i > number){
            break;
        }
    }
    return false;
}

std::pair<int, int> closest_pair_tonum(int upper_limit) {
    for (int m = upper_limit - 1; m >= 2; m--) {
        for (int n = m - 1; n >= 1; n--) {
            if (isPerfectSquare(m + n) && isPerfectSquare(m - n)) {
                return {m, n};
            }
        }
    }
    return {0, 0}; // Если пара не найдена
}
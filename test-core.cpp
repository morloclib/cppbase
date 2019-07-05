#include "core.hpp"
#include <vector>
#include <iostream>
#include <algorithm>

double inc(int x){
    double y = x + 1.0;
    return y;
}

int add(int x, int y){
    return x + y;
}

std::string makeName(std::string s, size_t i){
    s += "_" + std::to_string(i);
    return s;
}

std::string reverseInt(int x){
    std::string s = std::to_string(x);
    std::reverse(s.begin(), s.end());
    return s;
}

int main(int argc, char * argv[]){

    std::vector<int> xs{12,23,134};
    std::vector<double> ys = map(inc, xs);
    std::vector<std::string> ss = map(reverseInt, xs);

    std::vector<int> zs = zipWith(add, xs, xs);

    std::cout << ys[0] << " " << ys[1] << " " << ys[2] << std::endl; 
    std::cout << ss[0] << " " << ss[1] << " " << ss[2] << std::endl;

    std::cout << zs[0] << " " << zs[1] << " " << zs[2] << std::endl; 

    std::vector<std::string> bases{"asdf", "qwer", "wert"};
    std::vector<std::string> names = enumerateWith(makeName, bases);
    
    std::cout << names[0] << " " << names[1] << " " << names[2] << " " << std::endl;

    return 0;
}

#include "core.hpp"
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>

double inc(int x){
    double y = x + 1.0;
    return y;
}

int add(int x, int y){
    return x + y;
}


std::string makeName(std::string s, size_t i){
    std::ostringstream output;
    output << s << "_" << i;
    return output.str();
}

std::string reverseInt(int x){
    std::ostringstream output;
    output << x;
    std::string s = output.str();
    std::reverse(s.begin(), s.end());
    return s;
}

int main(int argc, char * argv[]){

    std::vector<int> xs{12,23,134};
    std::vector<double> ys = morloc_map(inc, xs);
    std::vector<std::string> ss = morloc_map(reverseInt, xs);

    std::vector<int> zs = morloc_zipWith(add, xs, xs);

    double b = 0.0;
    std::vector<double> ws{12.1,23.2,134.3};
    double summed = morloc_fold(morloc_add, b, ws);

    std::cout << ys[0] << " " << ys[1] << " " << ys[2] << std::endl; 
    std::cout << ss[0] << " " << ss[1] << " " << ss[2] << std::endl;

    std::cout << zs[0] << " " << zs[1] << " " << zs[2] << std::endl; 

    std::cout << "sum(12.1,23.2,134.3)=" << summed << std::endl;

    std::vector<std::string> bases{"asdf", "qwer", "wert"};
    std::vector<std::string> names = morloc_enumerateWith(makeName, bases);
    
    std::cout << names[0] << " " << names[1] << " " << names[2] << " " << std::endl;

    return 0;
}

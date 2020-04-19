#include "core.hpp"
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>

template <class A>
A inc(A x){
    return x + 1;
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
    std::function<int(int)> incFun =
        static_cast<int(*)(int)>(&inc);

    std::vector<int> ys = morloc_map(incFun, xs);

    std::function<std::string(int)> reverseIntFun =
        static_cast<std::string(*)(int)>(&reverseInt);
    std::vector<std::string> ss = morloc_map(reverseIntFun, xs);

    std::function<int(int,int)> addFun =
        static_cast<int(*)(int,int)>(&morloc_add);
    std::vector<int> zs = morloc_zipWith(addFun, xs, xs);

    double b = 0.0;
    std::vector<double> ws{12.1,23.2,134.3};
    std::function<double(double,double)> addFunDbl =
        static_cast<double(*)(double,double)>(&morloc_add);
    double summed = morloc_fold(addFunDbl, b, ws);

    std::cout << ys[0] << " " << ys[1] << " " << ys[2] << std::endl;
    std::cout << ss[0] << " " << ss[1] << " " << ss[2] << std::endl;

    std::cout << zs[0] << " " << zs[1] << " " << zs[2] << std::endl;

    std::cout << "sum(12.1,23.2,134.3)=" << summed << std::endl;

    std::vector<std::string> bases{"asdf", "qwer", "wert"};

    std::function<std::string(std::string,size_t)> makeNameFun =
        static_cast<std::string(*)(std::string,size_t)>(&makeName);
    std::vector<std::string> names = morloc_enumerateWith(makeNameFun, bases);

    std::cout << names[0] << " " << names[1] << " " << names[2] << " " << std::endl;

    return 0;
}

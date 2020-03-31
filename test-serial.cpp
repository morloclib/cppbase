// This program exists only to test the hpp header

#include <stdio.h>
#include "serial.hpp"

int main(int argc, char * argv[]){

    // parser a tuple
    bool a;
    std::tuple<double, bool> b;
    std::tuple<bool,std::tuple<double, bool>> tuple2 = unpackTuple2("[true, [42, false]]", a, b);
    std::cout << "(" << std::get<0>(tuple2)
              << ", (" << std::get<0>(std::get<1>(tuple2))
              << ", " << std::get<1>(std::get<1>(tuple2))
              << "))" << std::endl;
    std::cout << packTuple2(unpackTuple2("[true, [42, false]]", a, b)) << std::endl;
    // single value
    std::cout << unpackInt("42") << std::endl;
    std::cout << unpackInt("-42") << std::endl;
    std::cout << unpackSizeT("42") << std::endl;
    std::cout << unpackDouble("42.9") << std::endl;
    std::cout << unpackString("\"hello deary\"") << std::endl;
    std::cout << unpackBool("true") << std::endl;
    std::cout << packInt(42) << std::endl;
    std::cout << packSizeT(42) << std::endl;
    std::cout << packDouble(42.9) << std::endl;
    std::cout << packString("hello deary") << std::endl;
    std::cout << packBool(true) << std::endl;
    // vectors
    std::vector<int> ints = unpackInts("[1,2,3]");
    std::cout << ints[0] << ints[1] << ints[2] << std::endl;
    std::cout << packInts(unpackInts("[12,34,45]")) << std::endl;
    std::cout << packInts(unpackInts("[ 12f , -34.1, 45 ]")) << std::endl;
    std::cout << packSizeTs(unpackSizeTs("[12,34,45]")) << std::endl;
    std::cout << packDoubles(unpackDoubles("[12.1,34,45]")) << std::endl;
    std::cout << packStrings(unpackStrings("[\"yo,whatup?\",\"[bar, \\\"f\\\"]\" it doesn't \
      have to be a comma, but maybe it should be? \"my\\\"stuff\"]")) << std::endl;
    std::cout << packBools(unpackBools("[ true , false  , true]")) << std::endl;
    // foreign_call
    std::cout << std::endl;
    std::cout << foreign_call("echo hello") << std::endl;
    return 0;
}

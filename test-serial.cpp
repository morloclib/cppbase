// This program exists only to test the hpp header

#include <stdio.h>
#include "serial.hpp"

int main(int argc, char * argv[]){

    bool bool_schema;
    std::string bool_json = "true";
    bool bool_data = true;
    std::cout << pack(bool_data, bool_schema) << " | "
              << unpack(bool_json, bool_schema) << std::endl;

    std::string string_data = "yolo!";
    std::string string_json = "\"yolo!\"";
    std::string string_schema;
    std::cout << pack(string_data, string_schema) << " | "
              << unpack(string_json, string_schema) << std::endl;

    double double_data = 42.1;
    std::string double_json = "42.1";
    double double_schema;
    std::cout << pack(double_data, double_schema) << " | "
              << unpack(double_json, double_schema) << std::endl;

    std::vector<double> list_data = {4, 1.3, 5.6};
    std::string list_json = "[4, 1.3 , 5.6 ] ";
    std::vector<double> list_schema;
    std::cout << pack(list_data, list_schema) << " | "
              << pack(unpack(list_json, list_schema), list_schema) << std::endl;

    return 0;
}

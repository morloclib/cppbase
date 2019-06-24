#include <iostream>
#include <string>

std::string foreign_call(std::string cmd);

std::string packDouble(double x);
double unpackDouble(std::string json);

std::string packInt(int x);
int unpackInt(std::string json);

std::string packString(std::string x);
std::string unpackString(std::string x);

std::string foreign_call(std::string cmd){
    return("yolo");    
}

std::string packDouble(double x){
    return(std::to_string(x));
}

double unpackDouble(std::string json){
    return(std:stod(json));
}

std::string packInt(int x){
    return(std::to_string(x)); 
}

int unpackInt(std::string json){
    return(std:stoi(json));
}

std::string packString(std::string x){
    return(x);
}

std::string unpackString(std::string x){
    return(x);
}

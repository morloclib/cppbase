#include <iostream>
#include <string>
#include <stdexcept>
#include <stdio.h>

std::string foreign_call(char* cmd);

std::string packDouble(double x);
std::string packInt(int x);
std::string packString(std::string x);

double unpackDouble(char* json);
int unpackInt(char* json);
std::string unpackString(char* x);

std::string foreign_call(char* cmd){
    char buffer[256];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    while (fgets(buffer, sizeof buffer, pipe) != NULL) {
        result += buffer;
    }
    pclose(pipe);
    return(result);
}

std::string packDouble(double x){
    return(std::to_string(x));
}

double unpackDouble(char* json){
    return(std::stod(json));
}

std::string packInt(int x){
    return(std::to_string(x)); 
}

int unpackInt(char* json){
    return(std::stoi(json));
}

std::string packString(std::string x){
    return(x);
}

std::string unpackString(char* x){
    std::string str(x);
    return(str);
}

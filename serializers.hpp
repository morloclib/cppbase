#ifndef __SERIALIZERS_HPP__
#define __SERIALIZERS_HPP__

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <stdio.h>
#include <vector>

std::string foreign_call(const char* cmd);

std::string packDouble(double x);
std::string packInt(int x);
std::string packSizeT(size_t x);
std::string packString(std::string x);
std::string packBool(bool x);
std::string packDoubles(std::vector<double> xs);
std::string packInts(std::vector<int> xs);
std::string packSizeTs(std::vector<size_t> xs);
std::string packStrings(std::vector<std::string> xs);

double unpackDouble(const char* json);
int unpackInt(const char* json);
size_t unpackSizeT(const char* json);
std::string unpackString(const char* x);
bool unpackBool(const char* x);
std::vector<double> unpackDoubles(const char* json);
std::vector<int> unpackInts(const char* json);
std::vector<size_t> unpackSizeTs(const char* json);
std::vector<std::string> unpackStrings(const char* json);


// Handle foreign calls. This function is used inside of C++ manifolds. Any
// changes in the name will require a mirrored change in the morloc code. 
std::string foreign_call(const char* cmd){
    char buffer[256];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    while (fgets(buffer, sizeof buffer, pipe) != NULL) {
        result += buffer;
    }
    pclose(pipe);
    return(result);
}

// === UTILITIES ===

// Wrap a vector as a JSON list
template <class T>
std::string asList(std::vector<T> xs){
    std::string json = "[";
    for(size_t i = 0; i < xs.size(); i++){
        json += std::to_string(xs[i]);
        if(i+1 < xs.size()){
            json += ",";
        }
    }
    json += "]";
    return(json);
}

std::string unenclose(std::string s, char a, char b){
    try {
        if(s.size() < 2 || s[0] != a || s[s.size()-1] != b){
            throw 1;
        }
    } catch (int e) {
        std::cerr << "Expected string '" << s << "' to begin with "
                  << a << " and end with " << b << std::endl;
    }
    std::string s2 = s.substr(1, s.size()-2);
    return(s2);
}
std::string unenclose(std::string s, char a){
    return(unenclose(s, a, a));
}
std::string unenclose(const char* xs, char a, char b){
    std::string s(xs);
    return(unenclose(s, a, b));
}
std::string unenclose(const char* xs, char a){
    std::string s(xs);
    return(unenclose(s, a));
}

std::string strip(std::string s){
    while(s.size() > 0){
        if(s[0] == ' '){
            s.erase(0, 1);
        } else {
            break;
        }
    }
    while(s.size() > 0){
        if(s[s.size()-1] == ' '){
            s.erase(s.size()-1);
        } else {
            break;
        }
    }
    return(s);
}


// === Basic serialization functions ===

std::string packDouble(double x){
    return(std::to_string(x));
}
std::string packInt(int x){
    return(std::to_string(x)); 
}
std::string packSizeT(size_t x){
    return(std::to_string(x)); 
}
std::string packString(std::string x){
    return("\"" + x + "\"");
}
std::string packBool(bool x){
    return(x? "true" : "false");
}


double unpackDouble(const char* json){
    return(std::stod(json));
}

int unpackInt(const char* json){
    return(std::stoi(json));
}

size_t unpackSizeT(const char* json){
    return(std::stoi(json));
}

std::string unpackString(const char* x){
    return(unenclose(x, '"'));
}

bool unpackBool(const char* x){
    std::string s(x);
    return(s == "true" ? true : false);
}


std::string packDoubles(std::vector<double> xs){
    return(asList<double>(xs));
}
std::string packInts(std::vector<int> xs){
    return(asList<int>(xs));
}
std::string packSizeTs(std::vector<size_t> xs){
    return(asList<size_t>(xs));
}
std::string packStrings(std::vector<std::string> xs){
    std::string json = "[";
    for(size_t i = 0; i < xs.size(); i++){
        json += xs[i]; 
        if(i+1 < xs.size()){
            json += ",";
        }
    }
    json += "]";
    return(json);
}
std::string packBools(std::vector<bool> xs){
    std::string json = "[";
    for(size_t i = 0; i < xs.size(); i++){
        json += xs[i] ? "true" : "false"; 
        if(i+1 < xs.size()){
            json += ",";
        }
    }
    json += "]";
    return(json);
}

std::vector<double> unpackDoubles(const char* json){
    std::stringstream ss(unenclose(json, '[', ']'));
    std::vector<double> result;
    while(ss.good())
    {
        std::string substr;
        std::getline(ss, substr, ',');
        double x = std::stod(substr);
        result.push_back(x);
    }
    return(result);
}

std::vector<int> unpackInts(const char* json){
    std::stringstream ss(unenclose(json, '[', ']'));
    std::vector<int> result;
    while(ss.good())
    {
        std::string substr;
        std::getline(ss, substr, ',');
        int x = std::stoi(substr);
        result.push_back(x);
    }
    return(result);
}

std::vector<size_t> unpackSizeTs(const char* json){
    std::stringstream ss(unenclose(json, '[', ']'));
    std::vector<size_t> result;
    while(ss.good())
    {
        std::string substr;
        std::getline(ss, substr, ',');
        int x = std::stoi(substr);
        result.push_back(x);
    }
    return(result);
}

// Gracefully handles strings with commas and escaped quotes.
// If very forgiving of bad input, for example, all of these yield the same
// result:
//  ["a", "b", "c"]
//  ["a" | "b" som stuff "c"]
// It requires beginning and ending brackets. And then looks inside for quoted
// strings.
std::vector<std::string> unpackStrings(const char* json){
    std::string s = unenclose(json, '[', ']');
    std::vector<std::string> ss;
    bool inString = false;
    size_t a = 1;
    for(int i = 1; json[i] != 0; i++){
        if(json[i] == '"' && json[i-1] != '\\'){
            if(inString){
                inString = false;
                std::string s(json+a, i-a);
                ss.push_back('"' + s + '"');
            } else {
                inString = true;
                a = i+1;
            }
        }
    }
    return(ss);
}

std::vector<bool> unpackBools(const char* json){
    std::stringstream ss(unenclose(json, '[', ']'));
    std::vector<bool> result;
    while(ss.good())
    {
        std::string substr;
        std::getline(ss, substr, ',');
        bool x = strip(substr) == "true" ? true : false;
        result.push_back(x);
    }
    return(result);
}

#endif

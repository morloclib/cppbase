#ifndef __SERIALIZERS_HPP__
#define __SERIALIZERS_HPP__

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <stdio.h>
#include <vector>
#include <iomanip>
#include <limits>

std::string foreign_call(std::string cmd);

std::string packDouble(double x);
std::string packInt(int x);
std::string packSizeT(size_t x);
std::string packString(std::string x);
std::string packBool(bool x);
std::string packDoubles(std::vector<double> xs);
std::string packInts(std::vector<int> xs);
std::string packSizeTs(std::vector<size_t> xs);
std::string packStrings(std::vector<std::string> xs);

double unpackDouble(std::string json);
int unpackInt(std::string json);
size_t unpackSizeT(std::string json);
std::string unpackString(std::string x);
bool unpackBool(std::string x);
std::vector<double> unpackDoubles(std::string json);
std::vector<int> unpackInts(std::string json);
std::vector<size_t> unpackSizeTs(std::string json);
std::vector<std::string> unpackStrings(std::string json);


// Handle foreign calls. This function is used inside of C++ manifolds. Any
// changes in the name will require a mirrored change in the morloc code. 
std::string foreign_call(std::string cmd){
    char buffer[256];
    std::string result = "";
    FILE* pipe = popen(cmd.c_str(), "r");
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

    std::ostringstream json;

    json << "[";
    for(size_t i = 0; i < xs.size(); i++){
        json << std::setprecision(std::numeric_limits<double>::digits10 + 2) << xs[i];
        if(i+1 < xs.size()){
            json << ",";
        }
    }
    json << "]";
    return(json.str());
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

// parse sequences of digits from a larger string
// used as part of a larger number parser
std::string nat_str(std::string json, size_t &i){
    std::string num = "";
    while(json[i] >= '0' && json[i] <= '9'){
        num += json[i];
        i++;
    }
    return num;
}

// match a constant string, nothing is consumed on failure 
bool match(const std::string json, const std::string pattern, size_t &i){
    for(size_t j = 0; j < pattern.size(); j++){
        if(j + i >= json.size()){
            return false;
        }
        if(json[j + i] != pattern[j]){
            return false;
        }
    }
    i += pattern.size();
    return true;
}

// === Basic serialization functions ===

std::string packDouble(double x){
    std::ostringstream s;
    s << std::setprecision(std::numeric_limits<double>::digits10 + 2) << x;
    return(s.str());
}
std::string packInt(int x){
    std::ostringstream s;
    s << x;
    return(s.str());
}
std::string packSizeT(size_t x){
    std::ostringstream s;
    s << x;
    return(s.str());
}
std::string packString(std::string x){
    return("\"" + x + "\"");
}
std::string packBool(bool x){
    return(x? "true" : "false");
}


double unpackDouble(std::string json){
    double result;
    try {
        result = std::stod(json.c_str());
    } catch (const std::invalid_argument&) {
        std::cerr << "bad input to unpackDouble in C++";
        std::cerr << " expected a double, got '" << json << "'" << std::endl;
        throw;
    }
    return(result);
}

int unpackInt(std::string json){
    int result;
    try {
        result = std::stoi(json.c_str());
    } catch (const std::invalid_argument&) {
        std::cerr << "bad input to unpackInt in C++";
        std::cerr << " expected a integer, got '" << json << "'" << std::endl;
        throw;
    }
    return result;
}

size_t unpackSizeT(std::string json){
    return(std::stoi(json.c_str()));
}

std::string unpackString(std::string json){
    return(unenclose(json, '"'));
}

bool unpackBool(std::string s){
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

std::vector<double> unpackDoubles(std::string json){
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

std::vector<int> unpackInts(std::string json){
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

std::vector<size_t> unpackSizeTs(std::string json){
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
std::vector<std::string> unpackStrings(std::string json){
    std::string s = unenclose(json, '[', ']');
    std::vector<std::string> ss;
    bool inString = false;
    size_t a = 1;
    const char* json_c = json.c_str();
    for(int i = 1; json_c[i] != 0; i++){
        if(json_c[i] == '"' && json_c[i-1] != '\\'){
            if(inString){
                inString = false;
                std::string s(json_c+a, i-a);
                ss.push_back('"' + s + '"');
            } else {
                inString = true;
                a = i+1;
            }
        }
    }
    return(ss);
}

std::vector<bool> unpackBools(std::string json){
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

// combinator parser for bool
void unpack(std::string json, size_t &i, bool &x){
    if(match(json, "true", i)){
        x = true;
    }
    else if(match(json, "false", i)){
        x = false;
    }
}

// combinator parser for double
void unpack(std::string json, size_t &i, double &x){
    std::string num = "";
    if(json[i] == '-'){
        num = "";
        i++;
    }
    num += nat_str(json, i);
    if(json[i] == '.'){
        i++;
        num += '.';
        num += nat_str(json, i);
    }
    x = unpackDouble(num);
}

// combinator parser for 2-tuples
template <class A, class B>
void unpack(std::string json, size_t &i, std::tuple<A,B> &x){
    A a;
    B b;
    try
    {
        if(json[i] == '['){
            i++;
        } else {
            throw 1;
        }

        while(json[i] == ' ') i++;

        // read first parser element
        unpack(json, i, a);

        while(json[i] == ' ') i++;

        if(json[i] == ','){
            i++;
        } else {
            throw 1;
        }
        while(json[i] == ' ') i++;

        // read second parser element
        unpack(json, i, b);

        while(json[i] == ' ') i++;

        if(json[i] != ']'){
            throw 1;
        }
    }
    catch (int e)
    {
        std::cerr << "Parse error in unpackTuple2 at character: " << json[i] << std::endl;
    }
    x = std::tuple<A,B>{a, b}; 
}

// Top level handler for 2-tuple containers
template <class A, class B>
std::tuple<A,B> unpackTuple2(std::string json, A a, B b){
    size_t i = 0;
    std::tuple<A,B> x;
    unpack(json, i, x);
    return x;
}

#endif
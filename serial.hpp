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
#include <tuple>
#include <utility> 


std::string pack(bool x, bool schema);
std::string pack(int x, int schema);
std::string pack(int x, size_t schema);
std::string pack(int x, long schema);
std::string pack(double x, double schema);
std::string pack(std::string x, std::string schema);

template <class A> std::string pack(A x);

template<std::size_t I = 0, class... Rs>
inline typename std::enable_if<I == sizeof...(Rs), std::string>::type
  _pack_tuple(std::tuple<Rs...> x);

template<std::size_t I = 0, class... Rs>
inline typename std::enable_if<I < sizeof...(Rs), std::string>::type
  _pack_tuple(std::tuple<Rs...> x);

template <class... A>
std::string pack(std::tuple<A...> x, std::tuple<A...> schema);

template <class A>
std::string pack(std::vector<A> x, std::vector<A> schema);

bool match(const std::string json, const std::string pattern, size_t &i);
void whitespace(const std::string json, size_t &i);
std::string digit_str(const std::string json, size_t &i);
double read_double(std::string json);

// attempt a run a parser, on failure, consume no input
template <class A>
bool try_parse(std::string json, size_t &i, A &x, bool (*f)(std::string, size_t &, A &));

bool unpack(const std::string json, size_t &i, bool &x);
bool unpack(const std::string json, size_t &i, double &x);
bool unpack(const std::string json, size_t &i, std::string &x);

template <class A>
bool integer_unpack(const std::string json, size_t &i, A &x);
bool unpack(const std::string json, size_t &i, int &x);
bool unpack(const std::string json, size_t &i, size_t &x);
bool unpack(const std::string json, size_t &i, long &x);

template <class A>
bool unpack(const std::string json, size_t &i, std::vector<A> &x);

template <class A>
bool _unpack_tuple(const std::string json, size_t &i, std::tuple<A> &x);

template <class A, class... Rest>
bool _unpack_tuple(const std::string json, size_t &i, std::tuple<A, Rest...> &x);

template <class... Rest>
bool unpack(const std::string json, size_t &i, std::tuple<Rest...> &x);

template <class A>
A unpack(const std::string json, A output);




/* ---------------------------------------------------------------------- */
/*                       S E R I A L I Z A T I O N                        */
/* ---------------------------------------------------------------------- */

std::string pack(bool x, bool schema){
    return(x? "true" : "false");
}

std::string pack(int x, int schema){
    std::ostringstream s;
    s << x;
    return(s.str());
}
std::string pack(int x, size_t schema){
    std::ostringstream s;
    s << x;
    return(s.str());
}
std::string pack(int x, long schema){
    std::ostringstream s;
    s << x;
    return(s.str());
}

std::string pack(double x, double schema){
    std::ostringstream s;
    s << std::setprecision(std::numeric_limits<double>::digits10 + 2) << x;
    return(s.str());
}

std::string pack(std::string x, std::string schema){
    std::ostringstream s;
    s << '"' << x << '"';
    return(s.str());
}

template <class A>
std::string pack(std::vector<A> x, std::vector<A> schema){
    A element_schema;
    std::ostringstream s;
    s << "[";
    for(size_t i = 0; i < x.size(); i++){
        s << pack(x[i], element_schema);
        if((i+1) < x.size()){
            s << ',';
        }
    }
    s << "]";
    return (s.str());
}

template <class A>
std::string pack(A x){
    return pack(x, x);
}

// adapted from stackoverflow #1198260 answer from emsr
template<std::size_t I = 0, class... Rs>
inline typename std::enable_if<I == sizeof...(Rs), std::string>::type
  _pack_tuple(std::tuple<Rs...> x)
  { return ""; }

template<std::size_t I = 0, class... Rs>
inline typename std::enable_if<I < sizeof...(Rs), std::string>::type
  _pack_tuple(std::tuple<Rs...> x)
  {
    return pack(std::get<I>(x)) + "," + _pack_tuple<I + 1, Rs...>(x);
  }

template <class... A>
std::string pack(std::tuple<A...> x, std::tuple<A...> schema){
    std::ostringstream ss;
    ss << "[";
    ss << _pack_tuple(x);
    std::string json = ss.str();
    // _pack_tuple adds a terminal comma, replaced here with the end bracket
    json[json.size() - 1] = ']';
    return json;
}


/* ---------------------------------------------------------------------- */
/*                             P A R S E R S                              */
/* ---------------------------------------------------------------------- */

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

void whitespace(const std::string json, size_t &i){
    while(json[i] == ' ' || json[i] == '\n' || json[i] == '\t'){
        i++;
    }
}

// parse sequences of digits from a larger string
// used as part of a larger number parser
std::string digit_str(const std::string json, size_t &i){
    std::string num = "";
    while(json[i] >= '0' && json[i] <= '9'){
        num += json[i];
        i++;
    }
    return num;
}

double read_double(std::string json){
    return std::stod(json.c_str());
}

// attempt a run a parser, on failure, consume no input
template <class A>
bool try_parse(std::string json, size_t &i, A &x, bool (*f)(std::string, size_t &, A &)){
    size_t j = i;
    if(f(json, i, x)){
        return true;
    } else {
        i = j;
        return false;
    }
}

/* ---------------------------------------------------------------------- */
/*                      D E S E R I A L I Z A T I O N                     */
/* ---------------------------------------------------------------------- */

// All combinator functions have the following general signature:
//
//   template <class A>
//   bool unpack(const std::string json, size_t &i, A &x)

// The return value represents parse success.
// The index may be incremented even on failure.

// combinator parser for bool
bool unpack(const std::string json, size_t &i, bool &x){
    if(match(json, "true", i)){
        x = true;
    }
    else if(match(json, "false", i)){
        x = false;
    }
    else {
        return false;
    }
    return true;
}

// combinator parser for doubles
bool unpack(const std::string json, size_t &i, double &x){
    std::string lhs = "";
    std::string rhs = "";
    char sign = '+';
    
    if(json[i] == '-'){
        sign = '-';
        i++;
    }
    lhs = digit_str(json, i);
    if(json[i] == '.'){
        i++;
        rhs = digit_str(json, i);
    } else {
        rhs = "0";
    }

    if(lhs.size() > 0){
        x = read_double(sign + lhs + '.' + rhs);  
        return true;
    } else {
        return false;
    }
}

// combinator parser for double-quoted strings
bool unpack(const std::string json, size_t &i, std::string &x){
    try {
        x = "";
        if(! match(json, "\"", i)){
            throw 1;
        }
        // TODO: add full JSON specification support (escapes, magic chars, etc)
        while(i < json.size() && json[i] != '"'){
            x += json[i];
            i++;
        }
        if(! match(json, "\"", i)){
            throw 1;
        }
    } catch (int e) {
        return false;
    }
    return true;
}

template <class A>
bool integer_unpack(const std::string json, size_t &i, A &x){
    std::string x_str = digit_str(json, i);
    if(x_str.size() > 0){
        std::stringstream sstream(x_str);
        sstream >> x;
        return true;
    }
    return false; 
}
bool unpack(const std::string json, size_t &i, int &x){
    return integer_unpack(json, i, x);
}
bool unpack(const std::string json, size_t &i, size_t &x){
    return integer_unpack(json, i, x);
}
bool unpack(const std::string json, size_t &i, long &x){
    return integer_unpack(json, i, x);
}

// parser for vectors
template <class A>
bool unpack(const std::string json, size_t &i, std::vector<A> &x){
    x = {};
    try {
        if(! match(json, "[", i)){
            throw 1;
        }
        whitespace(json, i);
        while(true){
            A element;
            if(unpack(json, i, element)){
                x.push_back(element);
                whitespace(json, i);
                match(json, ",", i);
                whitespace(json, i);
            } else {
                break;
            }
        }
        whitespace(json, i);
        if(! match(json, "]", i)){
            throw 1;
        }
    } catch (int e) {
        return false;
    }
    return true;
}

template <class A>
bool _unpack_tuple(const std::string json, size_t &i, std::tuple<A> &x){
    A a;
    if(! unpack(json, i, a)){
        return false;
    }
    x = std::make_tuple(a);
    return true;
}
template <class A, class... Rest>
bool _unpack_tuple(const std::string json, size_t &i, std::tuple<A, Rest...> &x){
    A a;
    // parse the next element
    if(! unpack(json, i, a)){
        return false;
    }
    // skip whitespace and the comma
    whitespace(json, i);
    if(! match(json, ",", i)){
        return false;
    }
    whitespace(json, i);
    // parse the rest of the elements
    std::tuple<Rest...> rs;
    if(! _unpack_tuple(json, i, rs)){
        return false;
    }
    // cons
    x = std::tuple_cat(std::make_tuple(a), rs);
    return true;
}
template <class... Rest>
bool unpack(const std::string json, size_t &i, std::tuple<Rest...> &x){
    try {
        if(! match(json, "(", i)){
            throw 1;
        }
        whitespace(json, i);
        if(! _unpack_tuple(json, i, x)){
            throw 1;
        }
        whitespace(json, i);
        if(! match(json, ")", i)){
            throw 1;
        }
    } catch (int e) {
        return false;
    }
    return true;
}

template <class A>
A unpack(const std::string json, A output){
    size_t i = 0;
    unpack(json, i, output);
    return output;
}

template <class... Rest>
std::tuple<Rest...> unpack(const std::string json, std::tuple<Rest...> output){
    size_t i = 0;
    unpack(json, i, output);
    return output;
}

#endif

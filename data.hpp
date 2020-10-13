#ifndef __DATA_HPP__
#define __DATA_HPP__

#include <vector>
#include <map>
#include <utility>
#include <cassert>

template <class A, class B>
std::map<A,B> morloc_packMap(std::pair<std::vector<A>,std::vector<B>> items){
    std::map<A,B> m;
    std::vector<A> a = items.first;
    std::vector<B> b = items.second;
    assert(a.size() == b.size());
    for(size_t i = 0; i < a.size(); i++){
        m[a[i]] = b[i];
    }
    return m;
}

template <class A, class B>
std::pair<std::vector<A>,std::vector<B>> morloc_unpackMap(std::map<A,B> m){
    std::vector<A> a;
    std::vector<B> b;
    for (auto pair : m) {
        a.push_back(pair.first);
        b.push_back(pair.second);
    }
    return std::make_pair(a, b);
}

#endif

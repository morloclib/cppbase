#ifndef __CORE_HPP__
#define __CORE_HPP__

#include <vector>
#include <algorithm>
#include <functional>

template <class A, class B, class C>
C morloc_add(A x, B y){
    return x + y;
}

template <class A, class B, class C>
C morloc_sub(A x, B y){
    return x - y;
}

template <class A, class B, class C>
C morloc_mul(A x, B y){
    return x * y;
}

template <class A, class B, class C>
C morloc_div(A x, B y){
    return x / y;
}

template <class A, class B, class C>
C morloc_mod(A x, B y){
    return x % y;
}

template <class A, class B>
std::vector<B> morloc_map(std::function<B(A)> f, std::vector<A> xs){
    std::vector<B> ys(xs.size());
    std::transform(xs.begin(), xs.end(), ys.begin(), f);
    return ys;
}

template <class A, class B, class C>
std::vector<C> morloc_zipWith(
        std::function<C(A,B)> f,
        std::vector<A> xs,
        std::vector<B> ys
    )
{
    size_t N = std::min(xs.size(), ys.size());
    std::vector<C> zs(N);
    for(size_t i = 0; i < N; i++){
        zs[i] = f(xs[i], ys[i]);
    }
    return zs;
}

template <class A, class B>
B morloc_fold(std::function<B(B,A)> f, B y, std::vector<A> xs){
    for(size_t i=0; i < xs.size(); i++){
        y = f(y, xs[i]);
    }
    return y;
}

template <class A, class B, class Index>
std::vector<B> morloc_enumerateWith(std::function<B(A,Index)> f, std::vector<A> xs){
    std::vector<B> ys(xs.size());
    for(Index i = 0; i < xs.size(); i++){
       ys[i] = f(xs[i], i);  
    }
    return ys;
}

#endif

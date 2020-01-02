#ifndef __CORE_HPP__
#define __CORE_HPP__

#include <vector>
#include <algorithm>

template <class A, class B>
std::vector<B> morloc_map(B(*f)(A), const std::vector<A> &xs){
    std::vector<B> ys(xs.size());
    std::transform(xs.begin(), xs.end(), ys.begin(), f);
    return ys;
}

template <class A, class B, class C>
std::vector<C> morloc_zipWith(
        C(*f)(A,B),
        const std::vector<A> &xs,
        const std::vector<B> &ys
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
B morloc_fold(B(*f)(B,A), B y, const std::vector<A> xs){
    for(size_t i=0; i < xs.size(); i++){
        y = f(y, xs[i]);
    }
    return y;
}

template <class A, class B>
std::vector<B> morloc_enumerateWith(B(*f)(A,size_t), std::vector<A> xs){
    std::vector<B> ys(xs.size());
    for(size_t i = 0; i < xs.size(); i++){
       ys[i] = f(xs[i], i);  
    }
    return ys;
}

#endif

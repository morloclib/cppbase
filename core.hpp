#ifndef __CORE_HPP__
#define __CORE_HPP__

#include <vector>
#include <algorithm>
#include <functional>
#include <utility>
#include <assert.h>

template <class A>
A morloc_id(A x){
    return(x);
}

// tuple functions

// fst      :: forall a b . (a, b) -> a ;
template <class A, class B>
A morloc_fst(std::pair<A,B> x){
    return(x.first);
}

// snd      :: forall a b . (a, b) -> b;
template <class A, class B>
B morloc_snd(std::pair<A,B> x){
    return(x.second);
}

// pair    :: forall a b . a -> b -> (a, b);
template <class A, class B>
std::pair<A,B> morloc_pair(A x, B y){
    return(std::pair<A,B>(x,y));
}

// couple   :: forall a b . (a -> b) -> a -> (b, a);
template <class A, class B>
std::pair<B,A> morloc_pair(std::function<B(A)> f, A a){
    return(std::pair<B,A>(f(a),a));
}

// zip   :: forall a b . [a] -> [b] -> [(a, b)];
template <class A, class B>
std::vector<std::tuple<A,B>> morloc_zip(std::vector<A> a, std::vector<B> b){
    std::vector<std::tuple<A,B>> out;
    for (size_t i = 0; i < a.size() && i < b.size(); i++){
        out.push_back(std::make_tuple(a, b));
    }
    return out;
}

// unzip :: forall a b . [(a, b)] -> ([a], [b]);
template <class A, class B>
std::tuple<std::vector<A>,std::vector<B>> morloc_unzip(std::vector<std::tuple<A,B>> xs){
    std::vector<A> a;
    std::vector<B> b;
    for(size_t i = 0; i < xs.size(); i++){
       a.push_back(std::get<0>(xs[i])); 
       b.push_back(std::get<1>(xs[i])); 
    }
    return std::make_tuple(a,b);
}

// keys  :: forall a b . [(a, b)] -> [a];
template <class A, class B>
std::vector<A> morloc_keys(std::vector<std::tuple<A,B>> xs){
    std::vector<A> out;
    for (size_t i = 0; i < xs.size(); i++){
        out.push_back(std::get<0>(xs[i]));
    }
    return out;
}

// vals  :: forall a b . [(a, b)] -> [b];
template <class A, class B>
std::vector<A> morloc_vals(std::vector<std::tuple<A,B>> xs){
    std::vector<A> out;
    for (size_t i = 0; i < xs.size(); i++){
        out.push_back(std::get<1>(xs[i]));
    }
    return out;
}

// filter_key :: forall a b   . (a -> Bool)  -> [(a, b)] -> [(a, b)];
template <class A, class B>
std::vector<std::tuple<A,B>> morloc_filter_key(
    std::function<bool(A)> keep,
    std::vector<std::tuple<A,B>> xs
){
    std::vector<std::tuple<A,B>> ys;
    for(size_t i = 0; i < xs.size(); i++){
        if(keep(std::get<0>(xs[i])))
            ys.push_back(xs[i]);
    }
    return ys;
}

// filter_val :: forall a b   . (a -> Bool)  -> [(b, a)] -> [(b, a)];
template <class A, class B>
std::vector<std::tuple<A,B>> morloc_filter_val(
    std::function<bool(B)> keep,
    std::vector<std::tuple<A,B>> xs
){
    std::vector<std::tuple<A,B>> ys;
    for(size_t i = 0; i < xs.size(); i++){
        if(keep(std::get<1>(xs[i])))
            ys.push_back(xs[i]);
    }
    return ys;
}

template <class A>
A morloc_at(int i, std::vector<A> xs){
    return xs[i];
}

// map_key    :: forall a b c . (a -> b)     -> [(a, c)] -> [(b, c)];
template <class A, class B, class C>
std::vector<std::tuple<C,B>> morloc_map_key(
    std::function<C(A)> f,
    std::vector<std::tuple<A,B>> xs
){
    std::vector<std::tuple<A,C>> ys;
    for(size_t i = 0; i < xs.size(); i++){
        ys.push_back(std::make_tuple(f(std::get<0>(xs[i])), std::get<1>(xs[i])));
    }
    return ys;
}

// map_val    :: forall a b c . (a -> b)     -> [(c, a)] -> [(c, b)];
template <class A, class B, class C>
std::vector<std::tuple<A,C>> morloc_map_val(
    std::function<C(B)> f,
    std::vector<std::tuple<A,B>> xs
){
    std::vector<std::tuple<A,C>> ys;
    for(size_t i = 0; i < xs.size(); i++){
        ys.push_back(std::make_tuple(std::get<0>(xs[i]), f(std::get<1>(xs[i]))));
    }
    return ys;
}

// with_keys  :: forall a b c . ([a] -> [b]) -> [(a, c)] -> [(b, c)];
template <class A, class B, class C>
std::vector<std::tuple<C,A>> morloc_with_keys(
    std::function<std::vector<C>(std::vector<A>)> f,
    std::vector<std::tuple<A,B>> xs
){
    std::vector<std::tuple<C,A>> ys;
    std::vector<C> newkeys = f(morloc_keys(xs));
    assert(newkeys.size() == xs.size());
    for(size_t i = 0; i < newkeys.size(); i++){
        ys.push_back(std::make_tuple(newkeys[i], std::get<1>(xs[i])));
    }
    return ys;
}

// with_vals  :: forall a b c . ([a] -> [b]) -> [(c, a)] -> [(c, b)];
template <class A, class B, class C>
std::vector<std::tuple<A,C>> morloc_with_vals(
    std::function<std::vector<C>(std::vector<B>)> f,
    std::vector<std::tuple<A,B>> xs
){
    std::vector<std::tuple<A,C>> ys;
    std::vector<C> newvals = f(morloc_vals(xs));
    assert(newvals.size() == xs.size());
    for(size_t i = 0; i < newvals.size(); i++){
        ys.push_back(std::make_tuple(std::get<0>(xs[i]), newvals[i]));
    }
    return ys;
}


// -- -- Laws
// -- with_vals id xs == xs
// -- with_keys id xs == xs
// -- map_val id xs == xs
// -- map_key id xs == xs
// -- filter_val true xs == xs
// -- filter_val false xs == []
// -- filter_key true xs == xs
// -- filter_key false xs == []

// context_filter_key :: forall a b c . (a -> b -> (a, Bool)) -> a -> [(b,c)] -> [(b,c)];
// context_filter_val :: forall a b c . (a -> b -> (a, Bool)) -> a -> [(c,b)] -> [(c,b)];

// -- functional tools
// flip :: forall a b c . (a -> b -> c) -> b -> a -> c;
// until :: forall a . (a -> Bool) -> (a -> a) -> a -> a;

// -- foldable functions - replace once I get typeclasses working
// length :: forall a . [a] -> Int;
// reverse :: forall a . [a] -> [a];
// concat :: forall a . [[a]] -> [a];
// concatMap :: forall a b . (a -> [b]) -> [a] -> [b];

// -- list creation
// replicate :: forall a . Int -> a -> [a];

// -- general list processing
// head :: forall a . [a] -> a;        -- [1,2,3] => 1
// last :: forall a . [a] -> a;        -- [1,2,3] => 3
// tail :: forall a . [a] -> [a];      -- [1,2,3] => [2,3]
// init :: forall a . [a] -> [a];      -- [1,2,3] => [1,2]
// get  :: forall a . Int -> [a] -> a; -- get 1 [1,2,3] => 2
// take :: forall a . Int -> [a] -> [a];
// drop :: forall a . Int -> [a] -> [a];
// takeWhile :: forall a . (a -> Bool) -> [a] -> [a];
// dropWhile :: forall a . (a -> Bool) -> [a] -> [a];
// span      :: forall a . (a -> Bool) -> [a] -> ([a], [a]);
// break     :: forall a . (a -> Bool) -> [a] -> ([a], [a]);
// splitAt   :: forall a . Int -> [a] -> ([a], [a]);

// -- string processing
// lines   :: Str -> [Str];
// words   :: Str -> [Str];
// unlines :: [Str] -> Str;
// unwords :: [Str] -> Str;

// -- accumulating folds
// scanl  :: forall a b . (b -> a -> b) -> b -> [a] -> [b];
// scanr  :: forall a b . (a -> b -> b) -> b -> [a] -> [b];
// scanl1 :: forall a b . (a -> a -> a) -> [a] -> [a];
// scanr1 :: forall a b . (a -> a -> a) -> [a] -> [a];

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

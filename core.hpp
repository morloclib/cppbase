#ifndef __CORE_HPP__
#define __CORE_HPP__

#include <vector>
#include <algorithm>
#include <functional>
#include <utility>

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

// with_sec :: forall a b c . (b -> c) -> (a, b) -> (a, c);
// with_fst :: forall a b c . (a -> c) -> (a, b) -> (c, b);
// curry    :: forall a b c . ((a, b) -> c) -> a -> b -> c;
// uncurry  :: forall a b c . (a -> b -> c) -> (a, b) -> c;

// -- pairlists
// zip   :: forall a b . [a] -> [b] -> [(a, b)];
// unzip :: forall a b . [(a, b)] -> ([a], [b]);
// keys  :: forall a b . [(a, b)] -> [a];
// vals  :: forall a b . [(a, b)] -> [b];

// -- functions over pairlists
// filter_key :: forall a b   . (a -> Bool)  -> [(a, b)] -> [(a, b)];
// filter_val :: forall a b   . (a -> Bool)  -> [(b, a)] -> [(b, a)];
// map_key    :: forall a b c . (a -> b)     -> [(a, c)] -> [(b, c)];
// map_val    :: forall a b c . (a -> b)     -> [(c, a)] -> [(c, b)];
// with_keys  :: forall a b c . ([a] -> [b]) -> [(a, c)] -> [(b, c)];
// with_vals  :: forall a b c . ([a] -> [b]) -> [(c, a)] -> [(c, b)];

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

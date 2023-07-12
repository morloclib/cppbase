#ifndef __MORLOC_CPPBASE_CORE_HPP__
#define __MORLOC_CPPBASE_CORE_HPP__

#include <vector>
#include <algorithm>
#include <functional>
#include <utility>
#include <assert.h>
#include <map>


template <class A>
A morloc_run(std::function<A()> f){
    A x = f();
    return(x);
}

template <class A>
A morloc_id(A x){
    return(x);
}

template <class A, class B>
A morloc_seq(A x, B y){
    return(y);
}

template <class A, class B>
A morloc_const(A x, B y){
    return(x);
}

// tuple functions

// fst      :: forall a b . (a, b) -> a ;
template <class A, class B>
A morloc_fst(std::tuple<A,B> x){
    return(std::get<0>(x));
}

// snd      :: forall a b . (a, b) -> b;
template <class A, class B>
B morloc_snd(std::tuple<A,B> x){
    return(std::get<1>(x));
}


template <class A, class B, class C>
A morloc_fst3(std::tuple<A,B,C> x){
    return(std::get<0>(x));
}
template <class A, class B, class C>
B morloc_snd3(std::tuple<A,B,C> x){
    return(std::get<1>(x));
}
template <class A, class B, class C>
C morloc_thr3(std::tuple<A,B,C> x){
    return(std::get<2>(x));
}


// pair :: forall a b . a -> b -> (a, b);
template <class A, class B>
std::pair<A,B> morloc_pair(A x, B y){
    return(std::pair<A,B>(x,y));
}

// couple :: forall a b . (a -> b) -> a -> (b, a);
template <class A, class B>
std::pair<B,A> morloc_pair(std::function<B(A)> f, A a){
    return(std::pair<B,A>(f(a),a));
}

// zip :: forall a b . [a] -> [b] -> [(a, b)];
template <class A, class B>
std::vector<std::tuple<A,B>> morloc_zip(const std::vector<A> &a, const std::vector<B> &b){
    std::vector<std::tuple<A,B>> out;
    for (std::size_t i = 0; i < a.size() && i < b.size(); i++){
        out.push_back(std::make_tuple(a[i], b[i]));
    }
    return out;
}

// unzip :: forall a b . [(a, b)] -> ([a], [b]);
template <class A, class B>
std::tuple<std::vector<A>,std::vector<B>> morloc_unzip(const std::vector<std::tuple<A,B>> &xs){
    std::vector<A> a;
    std::vector<B> b;
    for(std::size_t i = 0; i < xs.size(); i++){
       a.push_back(std::get<0>(xs[i])); 
       b.push_back(std::get<1>(xs[i])); 
    }
    return std::make_tuple(a,b);
}


// keys :: Map a b -> [a]
template <typename Key, typename Value>
std::vector<Key> morloc_keys(const std::map<Key, Value>& map) {
    std::vector<Key> result;
    for (const auto& pair : map) {
        result.push_back(pair.first);
    }
    return result;
}

// vals :: Map a b -> [b]
template <typename Key, typename Value>
std::vector<Value> morloc_vals(const std::map<Key, Value>& map) {
    std::vector<Value> result;
    for (const auto& pair : map) {
        result.push_back(pair.second);
    }
    return result;
}

// mapKey :: (a -> a') -> Map a b -> Map a' b
template <typename Key, typename Value, typename NewKey>
std::map<NewKey, Value> morloc_map_key(std::function<NewKey(const Key&)> transform, const std::map<Key, Value>& map) {
    std::map<NewKey, Value> result;
    for (const auto& pair : map) {
        result[transform(pair.first)] = pair.second;
    }
    return result;
}

// mapVal :: (b -> b') -> Map a b -> Map a b'
template <typename Key, typename Value, typename NewValue>
std::map<Key, NewValue> morloc_map_val(std::function<NewValue(const Value&)> transform, const std::map<Key, Value>& map) {
    std::map<Key, NewValue> result;
    for (const auto& pair : map) {
        result[pair.first] = transform(pair.second);
    }
    return result;
}

// filterKey :: (k -> Bool) -> Map k v -> Map k v
template <typename Key, typename Value>
std::map<Key, Value> morloc_filter_key(std::function<bool(const Key&)> predicate, const std::map<Key, Value>& map) {
    std::map<Key, Value> result;
    for (const auto& pair : map) {
        if (predicate(pair.first)) {
            result[pair.first] = pair.second;
        }
    }
    return result;
}

// filterVal :: (v -> Bool) -> Map k v -> Map k v
template <typename Key, typename Value>
std::map<Key, Value> morloc_filter_val(std::function<bool(const Value&)> predicate, const std::map<Key, Value>& map) {
    std::map<Key, Value> result;
    for (const auto& pair : map) {
        if (predicate(pair.second)) {
            result[pair.first] = pair.second;
        }
    }
    return result;
}

template <class A>
A morloc_at(int i, std::vector<A> xs){
    return xs[i];
}

// with_fsts  :: forall a b c . ([a] -> [b]) -> [(a, c)] -> [(b, c)];
template <class A, class B, class C>
std::vector<std::tuple<C,A>> morloc_with_fsts(
    std::function<std::vector<C>(std::vector<A>)> f,
    std::vector<std::tuple<A,B>> xs
){
    std::vector<std::tuple<C,A>> ys;
    std::vector<C> newkeys = f(morloc_keys(xs));
    assert(newkeys.size() == xs.size());
    for(std::size_t i = 0; i < newkeys.size(); i++){
        ys.push_back(std::make_tuple(newkeys[i], std::get<1>(xs[i])));
    }
    return ys;
}

// with_snds  :: forall a b c . ([a] -> [b]) -> [(c, a)] -> [(c, b)];
template <class A, class B, class C>
std::vector<std::tuple<A,C>> morloc_with_snds(
    std::function<std::vector<C>(std::vector<B>)> f,
    std::vector<std::tuple<A,B>> xs
){
    std::vector<std::tuple<A,C>> ys;
    std::vector<C> newvals = f(morloc_vals(xs));
    assert(newvals.size() == xs.size());
    for(std::size_t i = 0; i < newvals.size(); i++){
        ys.push_back(std::make_tuple(std::get<0>(xs[i]), newvals[i]));
    }
    return ys;
}

template <class A>
std::vector<A> morloc_join(std::vector<A> xs, std::vector<A> ys){
    std::vector<A> zs; 
    for(std::size_t i = 0; i < xs.size(); i++){
        zs.push_back(xs[i]);
    }
    for(std::size_t i = 0; i < ys.size(); i++){
        zs.push_back(ys[i]);
    }
    return zs;
}


// -- -- Laws
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

template <class A>
A morloc_add(A x, A y){
    return x + y;
}

template <class A>
A morloc_sub(A x, A y){
    return x - y;
}

template <class A>
A morloc_mul(A x, A y){
    return x * y;
}

template <class A>
A morloc_div(A x, A y){
    return x / y;
}

template <class A>
A morloc_neg(A x){
    return (-1) * x;
}

template <class A>
A morloc_mod(A x, A y){
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
    std::size_t N = std::min(xs.size(), ys.size());
    std::vector<C> zs(N);
    for(std::size_t i = 0; i < N; i++){
        zs[i] = f(xs[i], ys[i]);
    }
    return zs;
}

template <class A, class B>
B morloc_fold(std::function<B(B,A)> f, B y, std::vector<A> xs){
    for(std::size_t i=0; i < xs.size(); i++){
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


// TODO: finish this implementation! I don't have internet and can't remember
// the methods of the C++ set type. Mutability is also a concern here.
//
// unique Cpp :: [a] -> [a]
template <class A>
std::vector<A> morloc_unique(std::vector<A> xs){
    // create set from xs
    // create vector from set 
    return xs;
}

// ifelse Cpp :: Bool -> a -> a
template <class A>
A morloc_ifelse(bool cond, A x, A y){
    if (cond) {
        return x;
    } else {
        return y;
    }
}

// head :: [a] -> a
template <class A>
A morloc_head(std::vector<A> x
){
    return x[0];
}

// filter :: (a -> Bool) -> [a] -> [a]
template <class A>
std::vector<A> morloc_filter(
    std::function<bool(A)> f,
    std::vector<A> xs
){
    std::vector<A> ys;
    for(std::size_t i = 0; i < xs.size(); i++){
        if (f(xs[i])){
            ys.push_back(xs[i]);
        }
    }
    return ys;
}

// strLength :: Str -> Int
template <class A>
int morloc_length(A x){
    return x.size();
}


template <class A>
bool morloc_gt(A x, A y){
   return x > y; 
}

template <class A>
bool morloc_lt(A x, A y){
   return x < y; 
}

template <class A>
bool morloc_ge(A x, A y){
   return x >= y; 
}

template <class A>
bool morloc_le(A x, A y){
   return x <= y; 
}

template <class A>
bool morloc_eq(A x, A y){
   return x == y; 
}

template <class A>
bool morloc_ne(A x, A y){
   return x != y; 
}


bool morloc_not(bool x){
    return !x;
}

bool morloc_and(bool x, bool y){
    return x && y;
}

bool morloc_or(bool x, bool y){
    return x || y;
}

#endif

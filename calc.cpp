#include <iostream>
#include <regex>
#include <string>
#include <type_traits>

using namespace std;

struct T;
struct F;

#define _$ ::template $

template <class T, class U>
concept Derived = is_base_of_v<U, T>;

template <class U>
concept Bit = is_base_of_v<T, U> || is_base_of_v<F, U>;

template <Bit...>
struct Int;

template <class U>
concept Func = requires {
  {U _$<T, F>};
};

struct T {
  template <Func a = T, Func b = F>
  using $ = a;
  static constexpr int value = 1;

  constexpr T() {}
  constexpr operator int() const { return value; }
};

struct F {
  template <Func a = T, Func b = F>
  using $ = b;
  static constexpr int value = 0;

  constexpr F() {}
  constexpr operator int() const { return value; }
};

template <Bit a>
struct Not : a _$<F, T> {};

template <Bit...>
struct And;
template <Bit...>
struct Or;
template <Bit...>
struct Xor;

template <Bit a, Bit b>
struct And<a, b> : a _$<b, a> {};

template <Bit a, Bit... t>
struct And<a, t...> : And<a, And<t...>> _$<> {};

template <Bit a, Bit b>
struct Or<a, b> : a _$<a, b> {};

template <Bit a, Bit... t>
struct Or<a, t...> : Or<a, Or<t...>> _$<> {};

template <Bit a, Bit b>
struct Xor<a, b> : a _$<Not<b>, b>  {};

template <Bit a, Bit... t>
struct Xor<a, t...> : Xor<a, Xor<t...>> _$<> {};

template <int, Bit...>
struct Nth;

template <int n, Bit h, Bit... t>
struct Nth<n, h, t...> : Nth<n - 1, t...> {};

template <Bit h, Bit... t>
struct Nth<0, h, t...> : h {};

template <int n>
struct Nth<n> : F {};

template <>
struct Int<> {
  constexpr Int() {}
  constexpr operator int() const { return value; }
  static constexpr int value = 0;
};

template <Bit h, Bit... bits>
struct Int<h, bits...> {
  template <int n>
  using Digit = typename Nth<n, h, bits...> _$<>;
  using Head = h;
  using Tail = Int<bits...>;
  static constexpr int value = h() | (Tail() << 1);

  constexpr Int() {}
  constexpr operator int() const { return value; }
};

template <Bit a, Bit b>
struct HalfAdder {
  using S = typename Xor<a, b> _$<>;
  using C = typename And<a, b> _$<>;
  using O = Int<S, C>;
};

template <Bit a, Bit b, Bit c = F>
struct FulAdder {
  using S = typename Xor<a, b, c> _$<>;
  using C = typename Xor<And<Xor<a, b>, c>, And<a, b>> _$<>;
  using O = Int<S, C>;
};

template <class... t>
struct List {
  template <template <class...> class T>
  using mapped = T<t...>;
};

template <class... t>
struct Reverse : Reverse<List<t...>, List<>> {};

template <class h, class... a, class... b>
struct Reverse<List<h, a...>, List<b...>> : Reverse<List<a...>, List<h, b...>> {
};

template <class... t>
struct Reverse<List<>, List<t...>> : List<t...> {};

template <class...>
struct Adder;

template <class...>
struct PackHead;

template <class h, class... x>
struct PackHead<h, x...> : h {
  using t = h;
};

template <class l, class r>
struct Adder<l, r> : Adder<typename l::Tail,
                           typename r::Tail,
                           FulAdder<typename l::Head, typename r::Head>> {};

template <class l, class r, class... p>
struct Adder<l, r, p...> : Adder<typename l::Tail,
                                 typename r::Tail,
                                 FulAdder<typename l::Head,
                                          typename r::Head,
                                          typename PackHead<p...>::t::C>,
                                 p...> {};

template <Bit h, Bit... t, class... p>
struct Adder<Int<>, Int<h, t...>, p...>
    : Adder<Int<>,
            Int<t...>,
            FulAdder<F, h, typename PackHead<p...>::t::C>,
            p...> {};

template <Bit... t, class... p>
struct Adder<Int<t...>, Int<>, p...> : Adder<Int<>, Int<t...>, p...> {};

template <class... p>
struct Adder<Int<>, Int<>, p...> {
  using T = typename Reverse<typename PackHead<p...>::t::C,
                             typename p::S...>::template mapped<Int>;
};

template <class A, class B>
using Add = typename Adder<A, B>::T;

int this_integer_is_69 = Add<Int<F, F, F, F, F, T>, Int<T, F, T, F, F, T>>{};

template <class A, class B>
void add() {
  cout << A() << " + " << B() << " = " << Add<A, B>() << "\n";

  string name = (string) typeid(A).name() + " + " + typeid(B).name() + " = " +
                typeid(Add<A, B>).name() + "\n";

  name = std::regex_replace(name, std::regex("(struct| |,)"), "");
  name = std::regex_replace(name, std::regex("T"), "1");
  name = std::regex_replace(name, std::regex("F"), "0");
  cout << name;
}

int main() {
  add<Int<F, F, F, F, F, T>, Int<T, F, T, F, F, T>>();
}
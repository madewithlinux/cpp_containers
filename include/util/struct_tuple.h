#pragma once
#include <sstream>
#include <type_traits>

template <size_t I, typename T>
struct struct_element;

template <typename T>
struct struct_element_count;

template <size_t idx, typename T>
const char *getname(const T &);

template <typename T>
const char *get_struct_name(const T &);

template <size_t idx, typename T>
typename struct_element<idx, T>::type &get(T &);

template <size_t I, typename T, typename Map>
void read_field(T &t, Map &m) {
  std::stringstream ss(m[getname<I>(t)]);
  ss >> get<I>(t);
}

template <typename T, size_t I = struct_element_count<T>().count() - 1, typename Map>
typename std::enable_if<(sizeof(T), I == 0), T>::type
read(Map &m) {
  T t;
  read_field<I>(t, m);
  return t;
}

template <typename T, size_t I = struct_element_count<T>().count() - 1, typename Map>
typename std::enable_if<(sizeof(T), I != 0), T>::type
read(Map &m) {
  T t = read<T, I - 1, Map>(m);
  read_field<I>(t, m);
  return t;
}

#define DEFINE_GET(T, m, I)                    \
  template <>                                  \
  struct struct_element<I, const T> {          \
    typedef const decltype(T::m) type;         \
  };                                           \
  template <>                                  \
  struct struct_element<I, T> {                \
    typedef decltype(T::m) type;               \
  };                                           \
  template <>                                  \
  inline struct_element<I, const T>::type &    \
  get<I, const T>(const T &t) { return t.m; }; \
  template <>                                  \
  inline struct_element<I, T>::type &          \
      get<I, T>(T & t) { return t.m; };        \
  template <>                                  \
  inline const char *                          \
  getname<I, T>(const T &) { return #m; };

#define DEFINE_COUNT(T, I)         \
  template <>                      \
  struct struct_element_count<T> { \
    inline constexpr size_t        \
    count() { return I; };         \
  };

#define DEFINE_NAME(T) \
  template <>          \
  inline const char *  \
  get_struct_name<T>(const T &) { return #T; };

#define ADAPT_FIELDS(T, ...)         \
  DEFINE_COUNT(T, NARG(__VA_ARGS__)) \
  DEFINE_NAME(T)                     \
  FIELDS(T, __VA_ARGS__)

#define FIELDS_MULTIPLE(T, N, ...) __FIELDS_##N(T, __VA_ARGS__)
// the following is necessary to make sure the processor has a chance to evaluate NARG16
#define _FIELDS_MULTIPLE(T, N, ...) FIELDS_MULTIPLE(T, N, __VA_ARGS__)
#define FIELDS(T, ...) _FIELDS_MULTIPLE(T, NARG(__VA_ARGS__), __VA_ARGS__)

#include "__generated/struct_tuple.h"

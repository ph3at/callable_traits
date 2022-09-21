#pragma once

#include <type_traits>

namespace helper {

template <typename T, typename = void>
struct has_arg0_type : std::false_type {};
template <typename T>
struct has_arg0_type<T, std::void_t<typename T::arg0_type>> : std::true_type {};
template <typename T>
inline constexpr auto has_arg0_type_v = has_arg0_type<T>::value;

template <typename T, typename = void>
struct has_arg1_type : std::false_type {};
template <typename T>
struct has_arg1_type<T, std::void_t<typename T::arg1_type>> : std::true_type {};
template <typename T>
inline constexpr auto has_arg1_type_v = has_arg1_type<T>::value;

template <typename T, typename = void>
struct has_arg2_type : std::false_type {};
template <typename T>
struct has_arg2_type<T, std::void_t<typename T::arg2_type>> : std::true_type {};
template <typename T>
inline constexpr auto has_arg2_type_v = has_arg2_type<T>::value;

template <typename T, typename = void>
struct has_class_type : std::false_type {};
template <typename T>
struct has_class_type<T, std::void_t<typename T::class_type>> : std::true_type {};
template <typename T>
inline constexpr auto has_class_type_v = has_class_type<T>::value;

} // namespace helper

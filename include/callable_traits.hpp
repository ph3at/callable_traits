#pragma once

#include <cstddef>
#include <tuple>
#include <type_traits>

namespace detail {

//////////////////////////////////////////////////////////////////////////
// Helper traits

template <typename T, typename = void>
struct get_call_operator {
	using type = T;
};

template <typename T>
struct get_call_operator<T, std::void_t<decltype(&std::remove_reference_t<T>::operator())>> {
	using type = decltype(&std::remove_reference_t<T>::operator());
};

template <typename T>
using get_call_operator_t = typename get_call_operator<T>::type;

// clang-format off
template <typename T>                            struct remove_mem_fn_specifier                                               { using type = T; };

template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A...)>                                { using type = R(C::*)(A...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A...) const>                          { using type = R(C::*)(A...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A...) volatile>                       { using type = R(C::*)(A...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A...) const volatile>                 { using type = R(C::*)(A...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A...) &>                              { using type = R(C::*)(A...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A...) const&>                         { using type = R(C::*)(A...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A...) volatile&>                      { using type = R(C::*)(A...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A...) const volatile&>                { using type = R(C::*)(A...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A...) &&>                             { using type = R(C::*)(A...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A...) const&&>                        { using type = R(C::*)(A...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A...) volatile&&>                     { using type = R(C::*)(A...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A...) const volatile&&>               { using type = R(C::*)(A...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A...) noexcept>                       { using type = R(C::*)(A...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A...) const noexcept>                 { using type = R(C::*)(A...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A...) volatile noexcept>              { using type = R(C::*)(A...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A...) const volatile noexcept>        { using type = R(C::*)(A...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A...) & noexcept>                     { using type = R(C::*)(A...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A...) const& noexcept>                { using type = R(C::*)(A...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A...) volatile& noexcept>             { using type = R(C::*)(A...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A...) const volatile& noexcept>       { using type = R(C::*)(A...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A...) && noexcept>                    { using type = R(C::*)(A...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A...) const&& noexcept>               { using type = R(C::*)(A...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A...) volatile&& noexcept>            { using type = R(C::*)(A...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A...) const volatile&& noexcept>      { using type = R(C::*)(A...); };

template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A..., ...)>                           { using type = R(C::*)(A..., ...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A..., ...) const>                     { using type = R(C::*)(A..., ...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A..., ...) volatile>                  { using type = R(C::*)(A..., ...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A..., ...) const volatile>            { using type = R(C::*)(A..., ...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A..., ...) &>                         { using type = R(C::*)(A..., ...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A..., ...) const&>                    { using type = R(C::*)(A..., ...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A..., ...) volatile&>                 { using type = R(C::*)(A..., ...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A..., ...) const volatile&>           { using type = R(C::*)(A..., ...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A..., ...) &&>                        { using type = R(C::*)(A..., ...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A..., ...) const&&>                   { using type = R(C::*)(A..., ...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A..., ...) volatile&&>                { using type = R(C::*)(A..., ...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A..., ...) const volatile&&>          { using type = R(C::*)(A..., ...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A..., ...) noexcept>                  { using type = R(C::*)(A..., ...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A..., ...) const noexcept>            { using type = R(C::*)(A..., ...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A..., ...) volatile noexcept>         { using type = R(C::*)(A..., ...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A..., ...) const volatile noexcept>   { using type = R(C::*)(A..., ...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A..., ...) & noexcept>                { using type = R(C::*)(A..., ...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A..., ...) const& noexcept>           { using type = R(C::*)(A..., ...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A..., ...) volatile& noexcept>        { using type = R(C::*)(A..., ...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A..., ...) const volatile& noexcept>  { using type = R(C::*)(A..., ...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A..., ...) && noexcept>               { using type = R(C::*)(A..., ...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A..., ...) const&& noexcept>          { using type = R(C::*)(A..., ...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A..., ...) volatile&& noexcept>       { using type = R(C::*)(A..., ...); };
template <typename C, typename R, typename... A> struct remove_mem_fn_specifier<R(C::*)(A..., ...) const volatile&& noexcept> { using type = R(C::*)(A..., ...); };

template <typename T> using remove_mem_fn_specifier_t = typename remove_mem_fn_specifier<T>::type;

template <typename T>                struct remove_fn_specifier                        { using type = T; };
template <typename R, typename... A> struct remove_fn_specifier<R(A...) noexcept>      { using type = R(A...); };
template <typename R, typename... A> struct remove_fn_specifier<R(A..., ...) noexcept> { using type = R(A..., ...); };
template <typename T> using remove_fn_specifier_t = typename remove_fn_specifier<T>::type;
// clang-format on

} // namespace detail

//////////////////////////////////////////////////////////////////////////
// Function traits for free standing functions, function pointers,
// member functions, functors, and lambdas.

template <typename Fn>
struct callable_traits
    : callable_traits<detail::remove_mem_fn_specifier_t<detail::remove_fn_specifier_t<
          std::remove_pointer_t<std::remove_cv_t<std::remove_reference_t<detail::get_call_operator_t<Fn>>>>>>> {
	static constexpr bool is_member_function = std::is_member_function_pointer_v<std::remove_reference_t<Fn>>;
};

template <typename C, typename R, typename... A>
struct callable_traits<R (C::*)(A...)> : callable_traits<R(A...)> {
	using class_type = C;
	static constexpr bool is_member_function = true;
};

template <typename C, typename R, typename... A>
struct callable_traits<R (C::*)(A..., ...)> : callable_traits<R(A..., ...)> {
	using class_type = C;
	static constexpr bool is_member_function = true;
};

template <typename R, typename... A>
struct callable_traits<R(A..., ...)> : callable_traits<R(A...)> {
	static constexpr bool is_variadic_function = true;
};

template <typename R>
struct callable_traits<R()> {
	using result_type = R;
	static constexpr std::size_t arity = 0;
	static constexpr bool is_member_function = false;
	static constexpr bool is_variadic_function = false;
};

template <typename R, typename A0>
struct callable_traits<R(A0)> {
	using result_type = R;
	using arg0_type = A0;
	template <std::size_t Index>
	using arg_type = typename std::tuple_element_t<Index, std::tuple<A0>>;
	static constexpr std::size_t arity = 1;
	static constexpr bool is_member_function = false;
	static constexpr bool is_variadic_function = false;
};

template <typename R, typename A0, typename A1>
struct callable_traits<R(A0, A1)> {
	using result_type = R;
	using arg0_type = A0;
	using arg1_type = A1;
	template <std::size_t Index>
	using arg_type = typename std::tuple_element_t<Index, std::tuple<A0, A1>>;
	static constexpr std::size_t arity = 2;
	static constexpr bool is_member_function = false;
	static constexpr bool is_variadic_function = false;
};

template <typename R, typename A0, typename A1, typename A2>
struct callable_traits<R(A0, A1, A2)> {
	using result_type = R;
	using arg0_type = A0;
	using arg1_type = A1;
	using arg2_type = A2;
	template <std::size_t Index>
	using arg_type = typename std::tuple_element_t<Index, std::tuple<A0, A1, A2>>;
	static constexpr std::size_t arity = 3;
	static constexpr bool is_member_function = false;
	static constexpr bool is_variadic_function = false;
};

template <typename R, typename A0, typename A1, typename A2, typename... A>
struct callable_traits<R(A0, A1, A2, A...)> {
	using result_type = R;
	using arg0_type = A0;
	using arg1_type = A1;
	using arg2_type = A2;
	template <std::size_t Index>
	using arg_type = typename std::tuple_element_t<Index, std::tuple<A0, A1, A2, A...>>;
	static constexpr std::size_t arity = 3 + sizeof...(A);
	static constexpr bool is_member_function = false;
	static constexpr bool is_variadic_function = false;
};

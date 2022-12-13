#pragma once

#include <cstddef>
#include <tuple>
#include <type_traits>

namespace detail {

//////////////////////////////////////////////////////////////////////////
// Helper traits

template <typename T>
struct get_function_types {};

template <typename R>
struct get_function_types<R()> {
	using result_type = R;
	static constexpr std::size_t arity = 0;
};

template <typename R, typename A0>
struct get_function_types<R(A0)> {
	using result_type = R;
	using arg0_type = A0;
	template <std::size_t Index>
	using arg_type = typename std::tuple_element_t<Index, std::tuple<A0>>;
	static constexpr std::size_t arity = 1;
};

template <typename R, typename A0, typename A1>
struct get_function_types<R(A0, A1)> {
	using result_type = R;
	using arg0_type = A0;
	using arg1_type = A1;
	template <std::size_t Index>
	using arg_type = typename std::tuple_element_t<Index, std::tuple<A0, A1>>;
	static constexpr std::size_t arity = 2;
};

template <typename R, typename A0, typename A1, typename A2>
struct get_function_types<R(A0, A1, A2)> {
	using result_type = R;
	using arg0_type = A0;
	using arg1_type = A1;
	using arg2_type = A2;
	template <std::size_t Index>
	using arg_type = typename std::tuple_element_t<Index, std::tuple<A0, A1, A2>>;
	static constexpr std::size_t arity = 3;
};

template <typename R, typename A0, typename A1, typename A2, typename... A>
struct get_function_types<R(A0, A1, A2, A...)> {
	using result_type = R;
	using arg0_type = A0;
	using arg1_type = A1;
	using arg2_type = A2;
	template <std::size_t Index>
	using arg_type = typename std::tuple_element_t<Index, std::tuple<A0, A1, A2, A...>>;
	static constexpr std::size_t arity = 3 + sizeof...(A);
};

template <bool IsConst, bool IsVolatile, bool IsLvalueReference, bool IsRvalueReference, bool IsNoexcept,
          bool IsVariadic>
struct add_specifiers {
	static constexpr bool is_const = IsConst;
	static constexpr bool is_volatile = IsVolatile;
	static constexpr bool is_lvalue_reference = IsLvalueReference;
	static constexpr bool is_rvalue_reference = IsRvalueReference;
	static constexpr bool is_noexcept = IsNoexcept;
	static constexpr bool is_variadic = IsVariadic;
};

// clang-format off
template <typename T>                struct get_function_specifiers                                                                                                         {};

template <typename R, typename... A> struct get_function_specifiers<R(A...)>                                : add_specifiers<0, 0, 0, 0, 0, 0>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A...) const>                          : add_specifiers<1, 0, 0, 0, 0, 0>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A...) volatile>                       : add_specifiers<0, 1, 0, 0, 0, 0>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A...) const volatile>                 : add_specifiers<1, 1, 0, 0, 0, 0>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A...) &>                              : add_specifiers<0, 0, 1, 0, 0, 0>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A...) const&>                         : add_specifiers<1, 0, 1, 0, 0, 0>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A...) volatile&>                      : add_specifiers<0, 1, 1, 0, 0, 0>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A...) const volatile&>                : add_specifiers<1, 1, 1, 0, 0, 0>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A...) &&>                             : add_specifiers<0, 0, 0, 1, 0, 0>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A...) const&&>                        : add_specifiers<1, 0, 0, 1, 0, 0>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A...) volatile&&>                     : add_specifiers<0, 1, 0, 1, 0, 0>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A...) const volatile&&>               : add_specifiers<1, 1, 0, 1, 0, 0>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A...) noexcept>                       : add_specifiers<0, 0, 0, 0, 1, 0>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A...) const noexcept>                 : add_specifiers<1, 0, 0, 0, 1, 0>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A...) volatile noexcept>              : add_specifiers<0, 1, 0, 0, 1, 0>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A...) const volatile noexcept>        : add_specifiers<1, 1, 0, 0, 1, 0>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A...) & noexcept>                     : add_specifiers<0, 0, 1, 0, 1, 0>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A...) const& noexcept>                : add_specifiers<1, 0, 1, 0, 1, 0>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A...) volatile& noexcept>             : add_specifiers<0, 1, 1, 0, 1, 0>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A...) const volatile& noexcept>       : add_specifiers<1, 1, 1, 0, 1, 0>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A...) && noexcept>                    : add_specifiers<0, 0, 0, 1, 1, 0>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A...) const&& noexcept>               : add_specifiers<1, 0, 0, 1, 1, 0>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A...) volatile&& noexcept>            : add_specifiers<0, 1, 0, 1, 1, 0>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A...) const volatile&& noexcept>      : add_specifiers<1, 1, 0, 1, 1, 0>, get_function_types<R(A...)> {};

template <typename R, typename... A> struct get_function_specifiers<R(A..., ...)>                           : add_specifiers<0, 0, 0, 0, 0, 1>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A..., ...) const>                     : add_specifiers<1, 0, 0, 0, 0, 1>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A..., ...) volatile>                  : add_specifiers<0, 1, 0, 0, 0, 1>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A..., ...) const volatile>            : add_specifiers<1, 1, 0, 0, 0, 1>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A..., ...) &>                         : add_specifiers<0, 0, 1, 0, 0, 1>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A..., ...) const&>                    : add_specifiers<1, 0, 1, 0, 0, 1>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A..., ...) volatile&>                 : add_specifiers<0, 1, 1, 0, 0, 1>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A..., ...) const volatile&>           : add_specifiers<1, 1, 1, 0, 0, 1>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A..., ...) &&>                        : add_specifiers<0, 0, 0, 1, 0, 1>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A..., ...) const&&>                   : add_specifiers<1, 0, 0, 1, 0, 1>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A..., ...) volatile&&>                : add_specifiers<0, 1, 0, 1, 0, 1>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A..., ...) const volatile&&>          : add_specifiers<1, 1, 0, 1, 0, 1>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A..., ...) noexcept>                  : add_specifiers<0, 0, 0, 0, 1, 1>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A..., ...) const noexcept>            : add_specifiers<1, 0, 0, 0, 1, 1>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A..., ...) volatile noexcept>         : add_specifiers<0, 1, 0, 0, 1, 1>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A..., ...) const volatile noexcept>   : add_specifiers<1, 1, 0, 0, 1, 1>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A..., ...) & noexcept>                : add_specifiers<0, 0, 1, 0, 1, 1>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A..., ...) const& noexcept>           : add_specifiers<1, 0, 1, 0, 1, 1>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A..., ...) volatile& noexcept>        : add_specifiers<0, 1, 1, 0, 1, 1>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A..., ...) const volatile& noexcept>  : add_specifiers<1, 1, 1, 0, 1, 1>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A..., ...) && noexcept>               : add_specifiers<0, 0, 0, 1, 1, 1>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A..., ...) const&& noexcept>          : add_specifiers<1, 0, 0, 1, 1, 1>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A..., ...) volatile&& noexcept>       : add_specifiers<0, 1, 0, 1, 1, 1>, get_function_types<R(A...)> {};
template <typename R, typename... A> struct get_function_specifiers<R(A..., ...) const volatile&& noexcept> : add_specifiers<1, 1, 0, 1, 1, 1>, get_function_types<R(A...)> {};
// clang-format on

template <typename T>
struct get_function_traits : get_function_specifiers<std::remove_pointer_t<T>> {};

template <typename T>
struct get_member_function_traits : get_function_traits<T> {
	static constexpr bool is_member_function = false;
};

template <typename C, typename F>
struct get_member_function_traits<F C::*> : get_function_traits<F> {
	using class_type = C;
	static constexpr bool is_member_function = true;
};

template <typename T, typename = void>
struct get_functor_traits : get_member_function_traits<T> {
	static constexpr bool is_functor = false;
};

template <typename T>
struct get_functor_traits<T, std::void_t<decltype(&T::operator())>>
    : get_member_function_traits<decltype(&T::operator())> {
	static constexpr bool is_functor = true;
	static constexpr bool is_member_function = false;
};

} // namespace detail

//////////////////////////////////////////////////////////////////////////
// Function traits for free standing functions, function pointers,
// member functions, functors, and lambdas.

template <typename Callable>
struct callable_traits : detail::get_functor_traits<std::remove_cv_t<std::remove_reference_t<Callable>>> {};

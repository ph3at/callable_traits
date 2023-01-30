#pragma once

#include <cstddef>
#include <type_traits>

namespace utils {

template <typename...>
struct always_false : std::false_type {};
template <typename... Ts>
inline constexpr auto always_false_v = always_false<Ts...>::value;

template <typename Fn, std::size_t... Idxs>
constexpr decltype(auto) for_constexpr(Fn&& func, std::index_sequence<Idxs...>)
{
	// Each iteration returns nothing.
	if constexpr ((std::is_void_v<std::invoke_result_t<Fn, std::integral_constant<std::size_t, Idxs>>> && ...)) {
		(func(std::integral_constant<std::size_t, Idxs>{}), ...);
	}
	// Each iteration returns boolean, allowing for short-circuit evaluation, and result is returned from the loop.
	else if constexpr ((std::is_same_v<std::invoke_result_t<Fn, std::integral_constant<std::size_t, Idxs>>,
	                                   bool> && ...)) {
		return (func(std::integral_constant<std::size_t, Idxs>{}) && ...);
	}
	// Each iteration returns arbitrary non-void type which will be returned as a tuple from the loop.
	else if constexpr ((!std::is_void_v<std::invoke_result_t<Fn, std::integral_constant<std::size_t, Idxs>>> && ...)) {
		return std::tuple{func(std::integral_constant<std::size_t, Idxs>{})...};
	} else {
		static_assert(always_false_v<Fn>,
		              "All control paths must either return void, bool, or arbitrary non-void types");
	}
}

template <typename Fn, typename Tuple>
constexpr decltype(auto) for_constexpr(Fn&& func, Tuple&& tuple)
{
	return for_constexpr([&](const auto& idx) { return func(std::get<idx.value>(tuple)); },
	                     std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
}

template <typename T>
constexpr const char* get_type_name()
{
	if constexpr (std::is_same_v<T, int>) {
		return "int";
	} else if constexpr (std::is_same_v<T, float>) {
		return "float";
	} else if constexpr (std::is_same_v<T, const char*>) {
		return "const char*";
	} else if constexpr (std::is_same_v<T, void>) {
		return "void";
	} else {
		static_assert(always_false_v<T>, "No name defined for the given type");
	}
}

} // namespace utils

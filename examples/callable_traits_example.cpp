#include <cstdio>
#include <functional>
#include <iostream>
#include <string_view>

#include <callable_traits.hpp>

#include "example_utils.hpp"

template <typename Callable>
void inspect_callable(const std::string_view signature)
{
	using traits = callable_traits<Callable>;

	std::cout << "Callable signature           : " << signature << std::endl;
	std::cout << "Callable is a member function: " << traits::is_member_function << std::endl;
	std::cout << "Callable is a functor        : " << traits::is_functor << std::endl;
	std::cout << "Callable is const            : " << traits::is_const << std::endl;
	std::cout << "Callable is volatile         : " << traits::is_volatile << std::endl;
	std::cout << "Callable is lvalue reference : " << traits::is_lvalue_reference << std::endl;
	std::cout << "Callable is rvalue reference : " << traits::is_rvalue_reference << std::endl;
	std::cout << "Callable is noexcept         : " << traits::is_noexcept << std::endl;
	std::cout << "Callable is variadic         : " << traits::is_variadic << std::endl;
	std::cout << "Callable has result type     : " << utils::get_type_name<typename traits::result_type>() << std::endl;
	std::cout << "Callable has arity           : " << traits::arity << std::endl;

	if constexpr (traits::arity > 0) {
		utils::for_constexpr(
		    [](auto idx) {
			    constexpr auto i = idx.value;
			    using arg_type = typename traits::template arg_type<i>;
			    std::cout << "Callable has argument " << i << " type : " << utils::get_type_name<arg_type>()
			              << std::endl;
		    },
		    std::make_index_sequence<traits::arity>{});
	}

	std::cout << std::endl;
}

struct foo {
	void volatile_ref() volatile& {}
	void rvalue_ref_noexcept() && noexcept {}
};

void fn(int, float);

int main()
{
	const auto lambda = [](float) {};

	inspect_callable<decltype(lambda)>("[](float) {}");
	inspect_callable<decltype(std::printf)>("int printf(const char* format, ...)");
	inspect_callable<decltype(&foo::volatile_ref)>("void volatile_ref() volatile&");
	inspect_callable<decltype(&foo::rvalue_ref_noexcept)>("void rvalue_ref_noexcept() && noexcept");
	inspect_callable<decltype(std::function{fn})>("std::function<void(int, float)>");

	return 0;
}

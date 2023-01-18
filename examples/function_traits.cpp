#include <iostream>
#include <type_traits>
#include <typeinfo>

#include <callable_traits.hpp>

int foo(int);

int main()
{
	using foo_traits = callable_traits<decltype(foo)>;
	const auto foo_arity = foo_traits::arity;

	std::cout << "foo takes " << foo_arity << " arguments" << std::endl;
	std::cout << "foo returns " << typeid(foo_traits::result_type).name() << std::endl;

	return 0;
}

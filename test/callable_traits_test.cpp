#include <cstdlib>
#include <functional>
#include <string>
#include <type_traits>

#include "../include/callable_traits.hpp"
#include "callable_traits_utils.hpp"

namespace callable_traits_tests {

namespace user_defined_functors {

void signature()
{
	struct void_to_void_functor {
		void operator()() {}
	};
	using void_to_void_functor_traits = callable_traits<void_to_void_functor>;
	static_assert(std::is_same_v<void_to_void_functor_traits::result_type, void>);
	static_assert(void_to_void_functor_traits::arity == 0);
	static_assert(std::is_same_v<void_to_void_functor_traits::class_type, void_to_void_functor>);
	static_assert(void_to_void_functor_traits::is_member_function == false);
	static_assert(void_to_void_functor_traits::is_variadic == false);
	struct int_to_int_functor {
		int operator()(int) { return {}; }
	};
	using int_to_int_functor_traits = callable_traits<int_to_int_functor>;
	static_assert(std::is_same_v<int_to_int_functor_traits::result_type, int>);
	static_assert(int_to_int_functor_traits::arity == 1);
	static_assert(std::is_same_v<int_to_int_functor_traits::arg0_type, int>);
	static_assert(std::is_same_v<int_to_int_functor_traits::arg_type<0>, int>);
	static_assert(std::is_same_v<int_to_int_functor_traits::class_type, int_to_int_functor>);
	static_assert(int_to_int_functor_traits::is_member_function == false);
	static_assert(int_to_int_functor_traits::is_variadic == false);
}

void specifier()
{
	struct const_functor {
		void operator()() const {}
	};
	using const_functor_traits = callable_traits<const_functor>;
	static_assert(std::is_same_v<const_functor_traits::result_type, void>);
	static_assert(const_functor_traits::arity == 0);
	static_assert(std::is_same_v<const_functor_traits::class_type, const_functor>);
	static_assert(const_functor_traits::is_member_function == false);
	static_assert(const_functor_traits::is_variadic == false);

	struct noexcept_functor {
		void operator()() noexcept {}
	};
	using noexcept_functor_traits = callable_traits<noexcept_functor>;
	static_assert(std::is_same_v<noexcept_functor_traits::result_type, void>);
	static_assert(noexcept_functor_traits::arity == 0);
	static_assert(std::is_same_v<noexcept_functor_traits::class_type, noexcept_functor>);
	static_assert(noexcept_functor_traits::is_member_function == false);
	static_assert(noexcept_functor_traits::is_variadic == false);
}

void reference()
{
	struct functor {
		void operator()() {}
	};

	[](const auto& fn) {
		using functor_traits = callable_traits<decltype(fn)>;
		static_assert(std::is_same_v<typename functor_traits::result_type, void>);
		static_assert(functor_traits::arity == 0);
		static_assert(std::is_same_v<typename functor_traits::class_type, functor>);
		static_assert(functor_traits::is_member_function == false);
		static_assert(functor_traits::is_variadic == false);
	}(functor{});

	[](auto&& fn) {
		using functor_traits = callable_traits<decltype(fn)>;
		static_assert(std::is_same_v<typename functor_traits::result_type, void>);
		static_assert(functor_traits::arity == 0);
		static_assert(std::is_same_v<typename functor_traits::class_type, functor>);
		static_assert(functor_traits::is_member_function == false);
		static_assert(functor_traits::is_variadic == false);
	}(functor{});
}

} // namespace user_defined_functors

namespace std_function_functors {

void signature()
{
	{
		using void_void_callable_traits = callable_traits<std::function<void()>>;
		static_assert(std::is_same_v<void_void_callable_traits::result_type, void>);
		static_assert(void_void_callable_traits::arity == 0);
		static_assert(void_void_callable_traits::is_member_function == false);
		static_assert(void_void_callable_traits::is_variadic == false);
	}

	{
		using int_to_int_callable_traits = callable_traits<std::function<int(int)>>;
		static_assert(std::is_same_v<int_to_int_callable_traits::result_type, int>);
		static_assert(int_to_int_callable_traits::arity == 1);
		static_assert(std::is_same_v<int_to_int_callable_traits::arg0_type, int>);
		static_assert(std::is_same_v<int_to_int_callable_traits::arg_type<0>, int>);
		static_assert(int_to_int_callable_traits::is_member_function == false);
		static_assert(int_to_int_callable_traits::is_variadic == false);
	}
}

} // namespace std_function_functors

namespace lambdas_without_captures {

void lambda_qualifiers()
{
	{
		const auto const_lambda = []() {};
		using const_lambda_traits = callable_traits<decltype(const_lambda)>;
		static_assert(std::is_same_v<const_lambda_traits::result_type, void>);
		static_assert(const_lambda_traits::arity == 0);
		static_assert(std::is_same_v<std::add_const_t<const_lambda_traits::class_type>, decltype(const_lambda)>);
		static_assert(const_lambda_traits::is_member_function == false);
		static_assert(const_lambda_traits::is_variadic == false);
	}

	{
		volatile auto volatile_lambda = []() {};
		using volatile_lambda_traits = callable_traits<decltype(volatile_lambda)>;
		static_assert(std::is_same_v<volatile_lambda_traits::result_type, void>);
		static_assert(volatile_lambda_traits::arity == 0);
		static_assert(
		    std::is_same_v<std::add_volatile_t<volatile_lambda_traits::class_type>, decltype(volatile_lambda)>);
		static_assert(volatile_lambda_traits::is_member_function == false);
		static_assert(volatile_lambda_traits::is_variadic == false);
	}

	{
		auto lambda = []() {};
		using lambda_traits = callable_traits<decltype(lambda)>;
		static_assert(std::is_same_v<lambda_traits::result_type, void>);
		static_assert(lambda_traits::arity == 0);
		static_assert(std::is_same_v<lambda_traits::class_type, decltype(lambda)>);
		static_assert(lambda_traits::is_member_function == false);
		static_assert(lambda_traits::is_variadic == false);
	}

	{
		auto mutable_lambda = []() mutable {};
		using mutable_lambda_traits = callable_traits<decltype(mutable_lambda)>;
		static_assert(std::is_same_v<mutable_lambda_traits::result_type, void>);
		static_assert(mutable_lambda_traits::arity == 0);
		static_assert(std::is_same_v<mutable_lambda_traits::class_type, decltype(mutable_lambda)>);
		static_assert(mutable_lambda_traits::is_member_function == false);
		static_assert(mutable_lambda_traits::is_variadic == false);
	}

	auto lambda = []() {};
	[](auto& ref_lambda) {
		using ref_lambda_traits = callable_traits<decltype(ref_lambda)>;
		static_assert(std::is_same_v<typename ref_lambda_traits::result_type, void>);
		static_assert(ref_lambda_traits::arity == 0);
		static_assert(
		    std::is_same_v<std::add_lvalue_reference_t<typename ref_lambda_traits::class_type>, decltype(ref_lambda)>);
		static_assert(ref_lambda_traits::is_member_function == false);
		static_assert(ref_lambda_traits::is_variadic == false);
	}(lambda);

	[](const auto& const_ref_lambda) {
		using const_ref_lambda_traits = callable_traits<decltype(const_ref_lambda)>;
		static_assert(std::is_same_v<typename const_ref_lambda_traits::result_type, void>);
		static_assert(const_ref_lambda_traits::arity == 0);
		static_assert(
		    std::is_same_v<std::add_lvalue_reference_t<std::add_const_t<typename const_ref_lambda_traits::class_type>>,
		                   decltype(const_ref_lambda)>);
		static_assert(const_ref_lambda_traits::is_member_function == false);
		static_assert(const_ref_lambda_traits::is_variadic == false);
	}([]() {});

	[](auto&& rvalue_ref_lambda) {
		using rvalue_ref_lambda_traits = callable_traits<decltype(rvalue_ref_lambda)>;
		static_assert(std::is_same_v<typename rvalue_ref_lambda_traits::result_type, void>);
		static_assert(rvalue_ref_lambda_traits::arity == 0);
		static_assert(std::is_same_v<std::add_rvalue_reference_t<typename rvalue_ref_lambda_traits::class_type>,
		                             decltype(rvalue_ref_lambda)>);
		static_assert(rvalue_ref_lambda_traits::is_member_function == false);
		static_assert(rvalue_ref_lambda_traits::is_variadic == false);
	}([]() {});
}

void arguments_and_return_types()
{
	{
		constexpr auto void_to_int_lambda = []() -> int { return {}; };
		using void_to_int_lambda_traits = callable_traits<decltype(void_to_int_lambda)>;
		static_assert(std::is_same_v<void_to_int_lambda_traits::result_type, int>);
		static_assert(void_to_int_lambda_traits::arity == 0);
		static_assert(void_to_int_lambda_traits::is_member_function == false);
		static_assert(void_to_int_lambda_traits::is_variadic == false);
	}

	{
		constexpr auto int_to_void_lambda = [](int) {};
		using int_to_void_lambda_traits = callable_traits<decltype(int_to_void_lambda)>;
		static_assert(std::is_same_v<int_to_void_lambda_traits::result_type, void>);
		static_assert(int_to_void_lambda_traits::arity == 1);
		static_assert(std::is_same_v<int_to_void_lambda_traits::arg0_type, int>);
		static_assert(std::is_same_v<int_to_void_lambda_traits::arg_type<0>, int>);
		static_assert(int_to_void_lambda_traits::is_member_function == false);
		static_assert(int_to_void_lambda_traits::is_variadic == false);
	}

	{
		constexpr auto int_to_int_lambda = [](int) -> int* { return {}; };
		using int_to_int_lambda_traits = callable_traits<decltype(int_to_int_lambda)>;
		static_assert(std::is_same_v<int_to_int_lambda_traits::result_type, int*>);
		static_assert(int_to_int_lambda_traits::arity == 1);
		static_assert(std::is_same_v<int_to_int_lambda_traits::arg0_type, int>);
		static_assert(std::is_same_v<int_to_int_lambda_traits::arg_type<0>, int>);
		static_assert(int_to_int_lambda_traits::is_member_function == false);
		static_assert(int_to_int_lambda_traits::is_variadic == false);
	}

	{
		constexpr auto two_arg_lambda = [](int, float*) -> const char* { return {}; };
		using two_arg_lambda_traits = callable_traits<decltype(two_arg_lambda)>;
		static_assert(std::is_same_v<two_arg_lambda_traits::result_type, const char*>);
		static_assert(two_arg_lambda_traits::arity == 2);
		static_assert(std::is_same_v<two_arg_lambda_traits::arg0_type, int>);
		static_assert(std::is_same_v<two_arg_lambda_traits::arg1_type, float*>);
		static_assert(std::is_same_v<two_arg_lambda_traits::arg_type<0>, int>);
		static_assert(std::is_same_v<two_arg_lambda_traits::arg_type<1>, float*>);
		static_assert(two_arg_lambda_traits::is_member_function == false);
		static_assert(two_arg_lambda_traits::is_variadic == false);
	}

	{
		constexpr auto three_arg_lambda = [](const volatile int*, const float*, volatile char*) -> volatile float*
		{
			return {};
		};
		using three_arg_lambda_traits = callable_traits<decltype(three_arg_lambda)>;
		static_assert(std::is_same_v<three_arg_lambda_traits::result_type, volatile float*>);
		static_assert(three_arg_lambda_traits::arity == 3);
		static_assert(std::is_same_v<three_arg_lambda_traits::arg0_type, const volatile int*>);
		static_assert(std::is_same_v<three_arg_lambda_traits::arg1_type, const float*>);
		static_assert(std::is_same_v<three_arg_lambda_traits::arg2_type, volatile char*>);
		static_assert(std::is_same_v<three_arg_lambda_traits::arg_type<0>, const volatile int*>);
		static_assert(std::is_same_v<three_arg_lambda_traits::arg_type<1>, const float*>);
		static_assert(std::is_same_v<three_arg_lambda_traits::arg_type<2>, volatile char*>);
		static_assert(three_arg_lambda_traits::is_member_function == false);
		static_assert(three_arg_lambda_traits::is_variadic == false);
	}

	{
		constexpr auto four_arg_lambda = [](int&, const float&, char&&, std::string) -> const volatile std::string*
		{
			return {};
		};
		using four_arg_lambda_traits = callable_traits<decltype(four_arg_lambda)>;
		static_assert(std::is_same_v<four_arg_lambda_traits::result_type, const volatile std::string*>);
		static_assert(four_arg_lambda_traits::arity == 4);
		static_assert(std::is_same_v<four_arg_lambda_traits::arg0_type, int&>);
		static_assert(std::is_same_v<four_arg_lambda_traits::arg1_type, const float&>);
		static_assert(std::is_same_v<four_arg_lambda_traits::arg2_type, char&&>);
		static_assert(std::is_same_v<four_arg_lambda_traits::arg_type<0>, int&>);
		static_assert(std::is_same_v<four_arg_lambda_traits::arg_type<1>, const float&>);
		static_assert(std::is_same_v<four_arg_lambda_traits::arg_type<2>, char&&>);
		static_assert(std::is_same_v<four_arg_lambda_traits::arg_type<3>, std::string>);
		static_assert(four_arg_lambda_traits::is_member_function == false);
		static_assert(four_arg_lambda_traits::is_variadic == false);
	}
}

} // namespace lambdas_without_captures

namespace lambdas_with_captures {

void test()
{
	auto value = 7;

	{
		const auto lambda_with_value_capture = [value]() { static_cast<void>(value); };
		using lambda_with_value_capture_traits = callable_traits<decltype(lambda_with_value_capture)>;
		static_assert(std::is_same_v<lambda_with_value_capture_traits::result_type, void>);
		static_assert(lambda_with_value_capture_traits::arity == 0);
		static_assert(lambda_with_value_capture_traits::is_member_function == false);
		static_assert(lambda_with_value_capture_traits::is_variadic == false);
	}

	{
		const auto lambda_with_implicit_value_capture = [=]() { static_cast<void>(value); };
		using lambda_with_implicit_value_capture_traits = callable_traits<decltype(lambda_with_implicit_value_capture)>;
		static_assert(std::is_same_v<lambda_with_implicit_value_capture_traits::result_type, void>);
		static_assert(lambda_with_implicit_value_capture_traits::arity == 0);
		static_assert(lambda_with_implicit_value_capture_traits::is_member_function == false);
		static_assert(lambda_with_implicit_value_capture_traits::is_variadic == false);
	}

	{
		const auto lambda_with_mutable_value_capture = [value]() mutable { value = 4; };
		using lambda_with_mutable_value_capture_traits = callable_traits<decltype(lambda_with_mutable_value_capture)>;
		static_assert(std::is_same_v<lambda_with_mutable_value_capture_traits::result_type, void>);
		static_assert(lambda_with_mutable_value_capture_traits::arity == 0);
		static_assert(lambda_with_mutable_value_capture_traits::is_member_function == false);
		static_assert(lambda_with_mutable_value_capture_traits::is_variadic == false);
	}

	{
		const auto lambda_with_ref_capture = [&value]() { value = 42; };
		using lambda_with_ref_capture_traits = callable_traits<decltype(lambda_with_ref_capture)>;
		static_assert(std::is_same_v<lambda_with_ref_capture_traits::result_type, void>);
		static_assert(lambda_with_ref_capture_traits::arity == 0);
		static_assert(lambda_with_ref_capture_traits::is_member_function == false);
		static_assert(lambda_with_ref_capture_traits::is_variadic == false);
	}

	{
		const auto lambda_with_implicit_ref_capture = [&]() { static_cast<void>(value); };
		using lambda_with_implicit_ref_capture_traits = callable_traits<decltype(lambda_with_implicit_ref_capture)>;
		static_assert(std::is_same_v<lambda_with_implicit_ref_capture_traits::result_type, void>);
		static_assert(lambda_with_implicit_ref_capture_traits::arity == 0);
		static_assert(lambda_with_implicit_ref_capture_traits::is_member_function == false);
		static_assert(lambda_with_implicit_ref_capture_traits::is_variadic == false);
	}

	{
		struct {
			int value;
			void f()
			{
				const auto lambda_with_this_ref_capture = [this]() { value = 1; };
				using lambda_with_this_ref_capture_traits = callable_traits<decltype(lambda_with_this_ref_capture)>;
				static_assert(std::is_same_v<lambda_with_this_ref_capture_traits::result_type, void>);
				static_assert(lambda_with_this_ref_capture_traits::arity == 0);
				static_assert(lambda_with_this_ref_capture_traits::is_member_function == false);
				static_assert(lambda_with_this_ref_capture_traits::is_variadic == false);

				const auto lambda_with_this_value_capture = [*this]() { static_cast<void>(value); };
				using lambda_with_this_value_capture_traits = callable_traits<decltype(lambda_with_this_value_capture)>;
				static_assert(std::is_same_v<lambda_with_this_value_capture_traits::result_type, void>);
				static_assert(lambda_with_this_value_capture_traits::arity == 0);
				static_assert(lambda_with_this_value_capture_traits::is_member_function == false);
				static_assert(lambda_with_this_value_capture_traits::is_variadic == false);
			}
			void c() const
			{
				const auto lambda_with_this_ref_capture = [this]() { static_cast<void>(value); };
				using lambda_with_this_ref_capture_traits = callable_traits<decltype(lambda_with_this_ref_capture)>;
				static_assert(std::is_same_v<lambda_with_this_ref_capture_traits::result_type, void>);
				static_assert(lambda_with_this_ref_capture_traits::arity == 0);
				static_assert(lambda_with_this_ref_capture_traits::is_member_function == false);
				static_assert(lambda_with_this_ref_capture_traits::is_variadic == false);

				const auto lambda_with_this_value_capture = [*this]() { static_cast<void>(value); };
				using lambda_with_this_value_capture_traits = callable_traits<decltype(lambda_with_this_value_capture)>;
				static_assert(std::is_same_v<lambda_with_this_value_capture_traits::result_type, void>);
				static_assert(lambda_with_this_value_capture_traits::arity == 0);
				static_assert(lambda_with_this_value_capture_traits::is_member_function == false);
				static_assert(lambda_with_this_value_capture_traits::is_variadic == false);
			}
		} foo;
		foo.f();
		foo.c();
	}
}

} // namespace lambdas_with_captures

namespace free_standing_functions {

static void void_to_void_fn() {}
static void int_to_void_fn(int) {}
static int int_to_int_fn(int)
{
	return {};
}
[[maybe_unused]] static void noexcept_fn() noexcept {}
[[maybe_unused]] static void var_arg0_fn(...) {}
[[maybe_unused]] static int var_arg1_fn(int, ...)
{
	return 0;
}

void function_type()
{
	{
		using void_to_void_fn_traits = callable_traits<decltype(void_to_void_fn)>;
		static_assert(std::is_same_v<void_to_void_fn_traits::result_type, void>);
		static_assert(void_to_void_fn_traits::arity == 0);
		static_assert(void_to_void_fn_traits::is_member_function == false);
		static_assert(void_to_void_fn_traits::is_variadic == false);
	}

	{
		using int_to_void_fn_traits = callable_traits<decltype(int_to_void_fn)>;
		static_assert(std::is_same_v<int_to_void_fn_traits::result_type, void>);
		static_assert(int_to_void_fn_traits::arity == 1);
		static_assert(std::is_same_v<int_to_void_fn_traits::arg0_type, int>);
		static_assert(std::is_same_v<int_to_void_fn_traits::arg_type<0>, int>);
		static_assert(int_to_void_fn_traits::is_member_function == false);
		static_assert(int_to_void_fn_traits::is_variadic == false);
	}

	{
		using int_to_int_fn_traits = callable_traits<decltype(int_to_int_fn)>;
		static_assert(std::is_same_v<int_to_int_fn_traits::result_type, int>);
		static_assert(int_to_int_fn_traits::arity == 1);
		static_assert(std::is_same_v<int_to_int_fn_traits::arg0_type, int>);
		static_assert(std::is_same_v<int_to_int_fn_traits::arg_type<0>, int>);
		static_assert(int_to_int_fn_traits::is_member_function == false);
		static_assert(int_to_int_fn_traits::is_variadic == false);
	}

	{
		using noexcept_fn_traits = callable_traits<decltype(noexcept_fn)>;
		static_assert(std::is_same_v<noexcept_fn_traits::result_type, void>);
		static_assert(noexcept_fn_traits::arity == 0);
		static_assert(noexcept_fn_traits::is_member_function == false);
		static_assert(noexcept_fn_traits::is_variadic == false);
	}

	{
		using var_arg0_fn_traits = callable_traits<decltype(var_arg0_fn)>;
		static_assert(std::is_same_v<var_arg0_fn_traits::result_type, void>);
		static_assert(var_arg0_fn_traits::arity == 0);
		static_assert(var_arg0_fn_traits::is_member_function == false);
		static_assert(var_arg0_fn_traits::is_variadic == true);
	}

	{
		using var_arg1_fn_traits = callable_traits<decltype(var_arg1_fn)>;
		static_assert(std::is_same_v<var_arg1_fn_traits::result_type, int>);
		static_assert(var_arg1_fn_traits::arity == 1);
		static_assert(std::is_same_v<var_arg1_fn_traits::arg0_type, int>);
		static_assert(std::is_same_v<var_arg1_fn_traits::arg_type<0>, int>);
		static_assert(var_arg1_fn_traits::is_member_function == false);
		static_assert(var_arg1_fn_traits::is_variadic == true);
	}
}

void function_pointer_type()
{
	{
		auto void_to_void_fn_ptr = void_to_void_fn;
		using void_to_void_fn_ptr_traits = callable_traits<decltype(void_to_void_fn_ptr)>;
		static_assert(std::is_same_v<void_to_void_fn_ptr_traits::result_type, void>);
		static_assert(void_to_void_fn_ptr_traits::arity == 0);
		static_assert(void_to_void_fn_ptr_traits::is_member_function == false);
		static_assert(void_to_void_fn_ptr_traits::is_variadic == false);
	}

	{
		volatile auto int_to_void_fn_ptr = int_to_void_fn;
		using int_to_void_fn_ptr_traits = callable_traits<decltype(int_to_void_fn_ptr)>;
		static_assert(std::is_same_v<int_to_void_fn_ptr_traits::result_type, void>);
		static_assert(int_to_void_fn_ptr_traits::arity == 1);
		static_assert(std::is_same_v<int_to_void_fn_ptr_traits::arg0_type, int>);
		static_assert(std::is_same_v<int_to_void_fn_ptr_traits::arg_type<0>, int>);
		static_assert(int_to_void_fn_ptr_traits::is_member_function == false);
		static_assert(int_to_void_fn_ptr_traits::is_variadic == false);
	}

	{
		const volatile auto int_to_int_fn_ptr = int_to_int_fn;
		using int_to_int_fn_ptr_traits = callable_traits<decltype(int_to_int_fn_ptr)>;
		static_assert(std::is_same_v<int_to_int_fn_ptr_traits::result_type, int>);
		static_assert(int_to_int_fn_ptr_traits::arity == 1);
		static_assert(std::is_same_v<int_to_int_fn_ptr_traits::arg0_type, int>);
		static_assert(std::is_same_v<int_to_int_fn_ptr_traits::arg_type<0>, int>);
		static_assert(int_to_int_fn_ptr_traits::is_member_function == false);
		static_assert(int_to_int_fn_ptr_traits::is_variadic == false);
	}

	{
		const auto var_arg0_fn_ptr = var_arg0_fn;
		using var_arg0_fn_ptr_traits = callable_traits<decltype(var_arg0_fn_ptr)>;
		static_assert(std::is_same_v<var_arg0_fn_ptr_traits::result_type, void>);
		static_assert(var_arg0_fn_ptr_traits::arity == 0);
		static_assert(var_arg0_fn_ptr_traits::is_member_function == false);
		static_assert(var_arg0_fn_ptr_traits::is_variadic == true);
	}

	{
		const auto var_arg1_fn_ptr = var_arg1_fn;
		using var_arg1_fn_ptr_traits = callable_traits<decltype(var_arg1_fn_ptr)>;
		static_assert(std::is_same_v<var_arg1_fn_ptr_traits::result_type, int>);
		static_assert(var_arg1_fn_ptr_traits::arity == 1);
		static_assert(std::is_same_v<var_arg1_fn_ptr_traits::arg0_type, int>);
		static_assert(std::is_same_v<var_arg1_fn_ptr_traits::arg_type<0>, int>);
		static_assert(var_arg1_fn_ptr_traits::is_member_function == false);
		static_assert(var_arg1_fn_ptr_traits::is_variadic == true);
	}
}

} // namespace free_standing_functions

namespace member_functions {
struct type {
	void void_to_void_mem_fn() {}
	int void_to_int_mem_fn() { return {}; }
	int int_to_int_mem_fn(int) { return {}; }
	void var_arg0_to_void_mem_fn(...) {}
	int var_arg1_to_int_mem_fn(int, ...) { return {}; }
};

void member_function_type()
{
	{
		using void_to_void_mem_fn_traits = callable_traits<decltype(&type::void_to_void_mem_fn)>;
		static_assert(std::is_same_v<void_to_void_mem_fn_traits::result_type, void>);
		static_assert(std::is_same_v<void_to_void_mem_fn_traits::class_type, type>);
		static_assert(void_to_void_mem_fn_traits::arity == 0);
		static_assert(void_to_void_mem_fn_traits::is_member_function == true);
		static_assert(void_to_void_mem_fn_traits::is_variadic == false);
	}

	{
		using void_to_int_mem_fn_traits = callable_traits<decltype(&type::void_to_int_mem_fn)>;
		static_assert(std::is_same_v<void_to_int_mem_fn_traits::result_type, int>);
		static_assert(std::is_same_v<void_to_int_mem_fn_traits::class_type, type>);
		static_assert(void_to_int_mem_fn_traits::arity == 0);
		static_assert(void_to_int_mem_fn_traits::is_member_function == true);
		static_assert(void_to_int_mem_fn_traits::is_variadic == false);
	}

	{
		using int_to_int_mem_fn_traits = callable_traits<decltype(&type::int_to_int_mem_fn)>;
		static_assert(std::is_same_v<int_to_int_mem_fn_traits::result_type, int>);
		static_assert(std::is_same_v<int_to_int_mem_fn_traits::class_type, type>);
		static_assert(int_to_int_mem_fn_traits::arity == 1);
		static_assert(std::is_same_v<int_to_int_mem_fn_traits::arg0_type, int>);
		static_assert(std::is_same_v<int_to_int_mem_fn_traits::arg_type<0>, int>);
		static_assert(int_to_int_mem_fn_traits::is_member_function == true);
		static_assert(int_to_int_mem_fn_traits::is_variadic == false);
	}

	{
		using var_arg0_to_void_mem_fn_traits = callable_traits<decltype(&type::var_arg0_to_void_mem_fn)>;
		static_assert(std::is_same_v<var_arg0_to_void_mem_fn_traits::result_type, void>);
		static_assert(std::is_same_v<var_arg0_to_void_mem_fn_traits::class_type, type>);
		static_assert(var_arg0_to_void_mem_fn_traits::arity == 0);
		static_assert(var_arg0_to_void_mem_fn_traits::is_member_function == true);
		static_assert(var_arg0_to_void_mem_fn_traits::is_variadic == true);
	}
}

struct specifiers {
	void mem_fn() {}
	void const_mem_fn() const {}
	void volatile_mem_fn() volatile {}
	void const_volatile_mem_fn() const volatile {}
	void ref_mem_fn() & {}
	void const_ref_mem_fn() const& {}
	void volatile_ref_mem_fn() volatile& {}
	void const_volatile_ref_mem_fn() const volatile& {}
	void rval_mem_fn() && {}
	void const_rval_mem_fn() const&& {}
	void volatile_rval_mem_fn() volatile&& {}
	void const_volatile_rval_mem_fn() const volatile&& {}
	void noexcept_mem_fn() noexcept {}
	void const_noexcept_mem_fn() const noexcept {}
	void volatile_noexcept_mem_fn() volatile noexcept {}
	void const_volatile_noexcept_mem_fn() const volatile noexcept {}
	void ref_noexcept_mem_fn() & noexcept {}
	void const_ref_noexcept_mem_fn() const& noexcept {}
	void volatile_ref_noexcept_mem_fn() volatile& noexcept {}
	void const_volatile_ref_noexcept_mem_fn() const volatile& noexcept {}
	void rval_noexcept_mem_fn() && noexcept {}
	void const_rval_noexcept_mem_fn() const&& noexcept {}
	void volatile_rval_noexcept_mem_fn() volatile&& noexcept {}
	void const_volatile_rval_noexcept_mem_fn() const volatile&& noexcept {}

	void var_arg_mem_fn(...) {}
	void var_arg_const_mem_fn(...) const {}
	void var_arg_volatile_mem_fn(...) volatile {}
	void var_arg_const_volatile_mem_fn(...) const volatile {}
	void var_arg_ref_mem_fn(...) & {}
	void var_arg_const_ref_mem_fn(...) const& {}
	void var_arg_volatile_ref_mem_fn(...) volatile& {}
	void var_arg_const_volatile_ref_mem_fn(...) const volatile& {}
	void var_arg_rval_mem_fn(...) && {}
	void var_arg_const_rval_mem_fn(...) const&& {}
	void var_arg_volatile_rval_mem_fn(...) volatile&& {}
	void var_arg_const_volatile_rval_mem_fn(...) const volatile&& {}
	void var_arg_noexcept_mem_fn(...) noexcept {}
	void var_arg_const_noexcept_mem_fn(...) const noexcept {}
	void var_arg_volatile_noexcept_mem_fn(...) volatile noexcept {}
	void var_arg_const_volatile_noexcept_mem_fn(...) const volatile noexcept {}
	void var_arg_ref_noexcept_mem_fn(...) & noexcept {}
	void var_arg_const_ref_noexcept_mem_fn(...) const& noexcept {}
	void var_arg_volatile_ref_noexcept_mem_fn(...) volatile& noexcept {}
	void var_arg_const_volatile_ref_noexcept_mem_fn(...) const volatile& noexcept {}
	void var_arg_rval_noexcept_mem_fn(...) && noexcept {}
	void var_arg_const_rval_noexcept_mem_fn(...) const&& noexcept {}
	void var_arg_volatile_rval_noexcept_mem_fn(...) volatile&& noexcept {}
	void var_arg_const_volatile_rval_noexcept_mem_fn(...) const volatile&& noexcept {}
};

void specifier()
{
	constexpr auto all_mem_fn_specifier = std::tuple{&specifiers::mem_fn,
	                                                 &specifiers::const_mem_fn,
	                                                 &specifiers::volatile_mem_fn,
	                                                 &specifiers::const_volatile_mem_fn,
	                                                 &specifiers::ref_mem_fn,
	                                                 &specifiers::const_ref_mem_fn,
	                                                 &specifiers::volatile_ref_mem_fn,
	                                                 &specifiers::const_volatile_ref_mem_fn,
	                                                 &specifiers::rval_mem_fn,
	                                                 &specifiers::const_rval_mem_fn,
	                                                 &specifiers::volatile_rval_mem_fn,
	                                                 &specifiers::const_volatile_rval_mem_fn,
	                                                 &specifiers::noexcept_mem_fn,
	                                                 &specifiers::const_noexcept_mem_fn,
	                                                 &specifiers::volatile_noexcept_mem_fn,
	                                                 &specifiers::const_volatile_noexcept_mem_fn,
	                                                 &specifiers::ref_noexcept_mem_fn,
	                                                 &specifiers::const_ref_noexcept_mem_fn,
	                                                 &specifiers::volatile_ref_noexcept_mem_fn,
	                                                 &specifiers::const_volatile_ref_noexcept_mem_fn,
	                                                 &specifiers::rval_noexcept_mem_fn,
	                                                 &specifiers::const_rval_noexcept_mem_fn,
	                                                 &specifiers::volatile_rval_noexcept_mem_fn,
	                                                 &specifiers::const_volatile_rval_noexcept_mem_fn};

	constexpr auto all_var_arg_mem_fn_specifier = std::tuple{&specifiers::var_arg_mem_fn,
	                                                         &specifiers::var_arg_const_mem_fn,
	                                                         &specifiers::var_arg_volatile_mem_fn,
	                                                         &specifiers::var_arg_const_volatile_mem_fn,
	                                                         &specifiers::var_arg_ref_mem_fn,
	                                                         &specifiers::var_arg_const_ref_mem_fn,
	                                                         &specifiers::var_arg_volatile_ref_mem_fn,
	                                                         &specifiers::var_arg_const_volatile_ref_mem_fn,
	                                                         &specifiers::var_arg_rval_mem_fn,
	                                                         &specifiers::var_arg_const_rval_mem_fn,
	                                                         &specifiers::var_arg_volatile_rval_mem_fn,
	                                                         &specifiers::var_arg_const_volatile_rval_mem_fn,
	                                                         &specifiers::var_arg_noexcept_mem_fn,
	                                                         &specifiers::var_arg_const_noexcept_mem_fn,
	                                                         &specifiers::var_arg_volatile_noexcept_mem_fn,
	                                                         &specifiers::var_arg_const_volatile_noexcept_mem_fn,
	                                                         &specifiers::var_arg_ref_noexcept_mem_fn,
	                                                         &specifiers::var_arg_const_ref_noexcept_mem_fn,
	                                                         &specifiers::var_arg_volatile_ref_noexcept_mem_fn,
	                                                         &specifiers::var_arg_const_volatile_ref_noexcept_mem_fn,
	                                                         &specifiers::var_arg_rval_noexcept_mem_fn,
	                                                         &specifiers::var_arg_const_rval_noexcept_mem_fn,
	                                                         &specifiers::var_arg_volatile_rval_noexcept_mem_fn,
	                                                         &specifiers::var_arg_const_volatile_rval_noexcept_mem_fn};

	detail::util::for_constexpr(
	    [](auto&& fn) {
		    using fn_traits = callable_traits<decltype(fn)>;
		    static_assert(std::is_same_v<typename fn_traits::result_type, void>);
		    static_assert(std::is_same_v<typename fn_traits::class_type, specifiers>);
		    static_assert(fn_traits::arity == 0);
		    static_assert(fn_traits::is_member_function == true);
		    static_assert(fn_traits::is_variadic == false);
	    },
	    all_mem_fn_specifier);

	detail::util::for_constexpr(
	    [](auto&& fn) {
		    using fn_traits = callable_traits<decltype(fn)>;
		    static_assert(std::is_same_v<typename fn_traits::result_type, void>);
		    static_assert(std::is_same_v<typename fn_traits::class_type, specifiers>);
		    static_assert(fn_traits::arity == 0);
		    static_assert(fn_traits::is_member_function == true);
		    static_assert(fn_traits::is_variadic == true);
	    },
	    all_var_arg_mem_fn_specifier);
}

struct base {
	virtual void pure_virtual_mem_fn() = 0;
	virtual void virtual_mem_fn() {}
};

struct inherited : base {
	void pure_virtual_mem_fn() override {}
	void virtual_mem_fn() override final {}
};

void inheritance_specifier()
{
	{
		using pure_virtual_mem_fn_traits = callable_traits<decltype(&base::pure_virtual_mem_fn)>;
		static_assert(std::is_same_v<pure_virtual_mem_fn_traits::result_type, void>);
		static_assert(std::is_same_v<pure_virtual_mem_fn_traits::class_type, base>);
		static_assert(pure_virtual_mem_fn_traits::arity == 0);
		static_assert(pure_virtual_mem_fn_traits::is_member_function == true);
		static_assert(pure_virtual_mem_fn_traits::is_variadic == false);
	}

	{
		using virtual_mem_fn_traits = callable_traits<decltype(&base::virtual_mem_fn)>;
		static_assert(std::is_same_v<virtual_mem_fn_traits::result_type, void>);
		static_assert(std::is_same_v<virtual_mem_fn_traits::class_type, base>);
		static_assert(virtual_mem_fn_traits::arity == 0);
		static_assert(virtual_mem_fn_traits::is_member_function == true);
		static_assert(virtual_mem_fn_traits::is_variadic == false);
	}

	{
		using inherited_pure_virtual_mem_fn_traits = callable_traits<decltype(&inherited::pure_virtual_mem_fn)>;
		static_assert(std::is_same_v<inherited_pure_virtual_mem_fn_traits::result_type, void>);
		static_assert(std::is_same_v<inherited_pure_virtual_mem_fn_traits::class_type, inherited>);
		static_assert(inherited_pure_virtual_mem_fn_traits::arity == 0);
		static_assert(inherited_pure_virtual_mem_fn_traits::is_member_function == true);
		static_assert(inherited_pure_virtual_mem_fn_traits::is_variadic == false);
	}

	{
		using inherited_virtual_mem_fn_traits = callable_traits<decltype(&inherited::virtual_mem_fn)>;
		static_assert(std::is_same_v<inherited_virtual_mem_fn_traits::result_type, void>);
		static_assert(std::is_same_v<inherited_virtual_mem_fn_traits::class_type, inherited>);
		static_assert(inherited_virtual_mem_fn_traits::arity == 0);
		static_assert(inherited_virtual_mem_fn_traits::is_member_function == true);
		static_assert(inherited_virtual_mem_fn_traits::is_variadic == false);
	}
}

} // namespace member_functions

} // namespace callable_traits_tests

int main()
{
	using namespace callable_traits_tests;

	user_defined_functors::signature();
	user_defined_functors::specifier();
	user_defined_functors::reference();

	std_function_functors::signature();

	lambdas_without_captures::lambda_qualifiers();
	lambdas_without_captures::arguments_and_return_types();

	lambdas_with_captures::test();

	free_standing_functions::function_type();
	free_standing_functions::function_pointer_type();

	member_functions::member_function_type();
	member_functions::specifier();
	member_functions::inheritance_specifier();

	return EXIT_SUCCESS;
}

# callable_traits - C++ Type Traits for All Callable Types

This header-only library provides type traits to inspect any callable type, like functions, member-functions, functors, and lambdas.

- [Features](#features)
- [Supported Traits](#supported-traits)
- [Usage](#usage)

## Features

- Header-only
- Works for any and all callable types
- Full support for all detectable qualifiers a callable can have
- C++17

## Supported Traits

| Type          | Name                  | Description                                                                                              |
|---------------|-----------------------|----------------------------------------------------------------------------------------------------------|
| `bool`        | `is_member_function`  | True when callable is a member function, unless it is the call operator (`operator()`), false otherwise. |
| `bool`        | `is_functor`          | True when callable is the call operator (`operator()`), or a lambda, false otherwise.                    |
| `bool`        | `is_const`            | True when callable has `const` specifier. Not applicable for regular functions.                          |
| `bool`        | `is_volatile`         | True when callable has `volatile` specifier. Not applicable for regular functions.                       |
| `bool`        | `is_lvalue_reference` | True when callable has `&` specifier. Not applicable for regular functions.                              |
| `bool`        | `is_rvalue_reference` | True when callable has `&&` specifier. Not applicable for regular functions.                             |
| `bool`        | `is_noexcept`         | True when callable has `noexcept` specifier.                                                             |
| `bool`        | `is_variadic`         | True when callable is a variadic function.                                                               |
| `std::size_t` | `arity`               | The number of arguments the callable takes. Does not take variadicness into account.                     |
| `R`           | `result_type`         | The type that the callable returns.                                                                      |
| `A0`          | `arg0_type`           | The type of the first argument of the callable. Only exists if `arity > 0`.                              |
| `A1`          | `arg1_type`           | The type of the second argument of the callable. Only exists if `arity > 1`.                             |
| `A2`          | `arg2_type`           | The type of the third argument of the callable. Only exists if `arity > 2`.                              |
| `A`           | `arg_type<idx>`       | The type of the `idx`th argument of the callable. Only exists if `arity > 0` and `idx < arity`.          |

## Usage

Callable traits can be used by passing the type of some callable as template argument to `callable_traits` like so:

```cpp
#include <callable_traits.hpp>

using traits = callable_traits<SomeCallableType>;
```

Then the applicable traits listed in [Supported Traits](#supported-traits) can be queried on `traits` like:

```cpp
static_assert(traits::arity == 1);
```

For a more complete example see [examples/callable_traits_example.cpp](examples/callable_traits_example.cpp).

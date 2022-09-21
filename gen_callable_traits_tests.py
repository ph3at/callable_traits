#!/usr/bin/python3

from asyncio import streams
from dataclasses import dataclass
from enum import Enum
from itertools import cycle, chain
from typing import Optional, Union


@dataclass
class Header:
    name: str
    system: bool

    def __str__(self):
        opening_symbol = '<' if self.system else '"'
        closing_symbol = '>' if self.system else '"'
        return f'#include {opening_symbol}{self.name}{closing_symbol}'


class Qualifier(Enum):
    CONST = "const"
    VOLATILE = "volatile"
    CONST_VOLATILE = "const volatile"


class Type(Enum):
    VOID = "void"
    BOOL = "bool"
    INT = "int"
    FLOAT = "float"
    STRING = "std::string"


@dataclass
class Pointer:
    qualifier: Optional[Qualifier]

    def __str__(self):
        formatted = '*'
        formatted += f' {self.qualifier.value}' if self.qualifier else ''
        return formatted


class Reference(Enum):
    LVALUE_REFERENCE = "&"
    RVALUE_REFERENCE = "&&"


class Specifier(Enum):
    NOEXCEPT = "noexcept"


@dataclass
class MemberSpecifier:
    qualifier: Optional[Qualifier]
    reference: Optional[Reference]
    specifier: Optional[Specifier]


@dataclass
class FullType:
    qualifier: Optional[Qualifier]
    type_value: Type
    pointer: Optional[Pointer]
    reference: Optional[Reference]

    def __str__(self):
        formatted = f'{self.qualifier.value}' if self.qualifier else ''
        formatted += ' ' if formatted else ''
        formatted += f'{str(self.type_value.value)}'
        formatted += f' {str(self.pointer)}' if self.pointer else ''
        formatted += f' {str(self.reference.value)}' if self.reference else ''
        return formatted


@dataclass
class Function:
    return_type: FullType
    name: str
    parameters: list[FullType]
    is_variadic: bool
    specifier: Optional[Specifier]

    def __str__(self):
        formatted = f'{str(self.return_type)} {self.name}'
        params = ', '.join(str(p) for p in self.parameters)
        params += ', ' if params and self.is_variadic else ''
        params += '...' if self.is_variadic else ''
        formatted += f'({params})'
        formatted += f' {str(self.specifier.value)}' if self.specifier else ''
        return formatted


@dataclass
class StaticAssert:
    condition: str

    def __str__(self):
        return f'static_assert({self.condition});'


def is_pointer_type(full_type: FullType):
    return True if full_type.pointer else False


def is_reference_type(full_type: FullType):
    return True if full_type.reference else False


def is_value_type(full_type: FullType):
    return not (is_pointer_type(full_type) or is_reference_type(full_type))


def is_cv_qualified(t: Union[FullType, Pointer]):
    return True if t.qualifier else False


def is_void_type(full_type: FullType):
    return full_type.type_value == Type.VOID


def is_cv_qualified_value_or_pointer(full_type: FullType):
    is_cv_qualified_value = is_value_type(
        full_type) and is_cv_qualified(full_type)
    is_cv_qualified_pointer = is_pointer_type(full_type) and is_cv_qualified(
        full_type.pointer) and not is_reference_type(full_type)
    return is_cv_qualified_value or is_cv_qualified_pointer


def is_valid_return_type(full_type: FullType):
    is_void_reference = is_void_type(full_type) and is_reference_type(
        full_type) and not is_pointer_type(full_type)
    return not (is_cv_qualified_value_or_pointer(full_type) or is_void_reference)


def is_valid_parameter_type(full_type: FullType):
    is_void_value_or_reference = is_void_type(full_type) and (
        is_value_type(full_type) or (is_reference_type(full_type) and not is_pointer_type(full_type)))
    return not (is_void_value_or_reference or is_cv_qualified_value_or_pointer(full_type))


def gen_functions(return_types: list[FullType], parameters: list[list[FullType]], specifiers: list[Optional[Specifier]]):
    counter = 0
    functions = []
    for return_type, parameter in zip(cycle(return_types), parameters):
        for specifier in specifiers:
            for is_variadic in [False, True]:
                functions += [Function(return_type,
                                       f'fn_{counter}', parameter, is_variadic, specifier)]
                counter += 1
    return functions


def gen_function_tests(functions: list[Function]) -> list[list[StaticAssert]]:
    tests = []
    for fn in functions:
        traits = f'callable_traits<decltype({fn.name})>'
        fn_tests = []
        fn_tests += [StaticAssert(
            f'std::is_same_v<{traits}::result_type, {str(fn.return_type)}>')]
        for idx, parameter in enumerate(fn.parameters):
            if idx < 3:
                fn_tests += [StaticAssert(
                    f'std::is_same_v<{traits}::arg{idx}_type, {str(parameter)}>')]
            fn_tests += [StaticAssert(
                f'std::is_same_v<{traits}::arg_type<{idx}>, {str(parameter)}>')]
        fn_tests += [StaticAssert(f'{traits}::arity == {len(fn.parameters)}')]
        fn_tests += [StaticAssert(f'{traits}::is_member_function == false')]
        fn_tests += [StaticAssert(
            f'{traits}::is_variadic_function == {"true" if fn.is_variadic else "false"}')]
        tests += [fn_tests]
    return tests


def gen_function_parameters(num_parameters: int, parameters: list[FullType]) -> list[list[FullType]]:
    if num_parameters == 0:
        return [[]]
    return [parameters[i:i + num_parameters] for i in range(0, len(parameters), num_parameters)]


def append_headers(source: str, headers: list[Header]):
    source += '\n'.join(str(h) for h in headers)
    return source + '\n'


def append_function_tests(source: str, functions: list[Function], assertions: list[list[StaticAssert]]):
    for fn, tests in zip(functions, assertions):
        source += f'{fn};\n'
        source += '\n'.join(str(t) for t in tests) + '\n'
    return source + '\n'


headers = [Header(h, True) for h in ['functional', 'string', 'type_traits']]
headers += [Header('include/callable_traits.hpp', False)]

all_qualifiers = [None] + [q for q in Qualifier]
all_types = [t for t in Type]
all_pointers = [None] + [Pointer(q) for q in all_qualifiers]
all_references = [None] + [r for r in Reference]
all_full_types = [FullType(q, t, p, r)
                  for t in all_types
                  for r in all_references
                  for p in all_pointers
                  for q in all_qualifiers]
all_return_types = [t for t in all_full_types if is_valid_return_type(t)]
all_parameter_types = [t for t in all_full_types if is_valid_parameter_type(t)]
all_parameters = list(chain.from_iterable(
    [gen_function_parameters(p, all_parameter_types) for p in range(0, 5)]))
all_specifiers = [None, Specifier.NOEXCEPT]

functions = gen_functions(all_return_types, all_parameters, all_specifiers)
function_tests = gen_function_tests(functions)

source = '/* This file was auto-generated */\n\n'
source = append_headers(source, headers)
source = append_function_tests(source, functions, function_tests)

print(source)

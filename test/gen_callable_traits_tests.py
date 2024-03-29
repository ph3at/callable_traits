#!/usr/bin/python3

from dataclasses import dataclass
from enum import Enum
from itertools import cycle, chain
from typing import Optional, Union


@dataclass
class Header:
    name: str
    system: bool

    def __str__(self) -> str:
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

    def __str__(self) -> str:
        formatted = '*'
        formatted += f' {self.qualifier.value}' if self.qualifier else ''
        return formatted


class Reference(Enum):
    LVALUE_REFERENCE = "&"
    RVALUE_REFERENCE = "&&"


class Specifier(Enum):
    NOEXCEPT = "noexcept"


class Mutable(Enum):
    MUTABLE = "mutable"


@dataclass
class MemberSpecifier:
    qualifier: Optional[Qualifier]
    reference: Optional[Reference]
    specifier: Optional[Specifier]

    def __bool__(self) -> bool:
        return bool(self.qualifier) or bool(self.reference) or bool(self.specifier)

    def __str__(self) -> str:
        formatted = f' {self.qualifier.value}' if self.qualifier else ''
        formatted += f' {self.reference.value}' if self.reference else ''
        formatted += f' {self.specifier.value}' if self.specifier else ''
        return formatted


@dataclass
class LambdaSpecifier:
    mutable: Optional[Mutable]
    specifier: Optional[Specifier]

    def __str__(self) -> str:
        formatted = f' {self.mutable.value}' if self.mutable else ''
        formatted += f' {self.specifier.value}' if self.specifier else ''
        return formatted


@dataclass
class FullType:
    qualifier: Optional[Qualifier]
    type_value: Type
    pointer: Optional[Pointer]
    reference: Optional[Reference]

    def __str__(self) -> str:
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

    def format_parameter_list(self) -> str:
        params = ', '.join(str(p) for p in self.parameters)
        params += ', ' if params and self.is_variadic else ''
        params += '...' if self.is_variadic else ''
        return params

    def __str__(self) -> str:
        formatted = f'{str(self.return_type)} {self.name}'
        params = self.format_parameter_list()
        formatted += f'({params})'
        formatted += f' {str(self.specifier.value)}' if self.specifier else ''
        return formatted


@dataclass
class MemberFunction(Function):
    member_specifier: MemberSpecifier
    class_type: str

    def __str__(self) -> str:
        formatted = f'struct {self.class_type} {{'
        formatted += f' {super().__str__()}'
        formatted += f'{str(self.member_specifier)};' if self.member_specifier else ';'
        formatted += f' }}'
        return formatted


@dataclass
class LambdaFunction(Function):
    lambda_specifier: LambdaSpecifier
    lambda_name: str

    def __str__(self) -> str:
        formatted = f'[[maybe_unused]] const auto {self.lambda_name} = []'
        formatted += f'({self.format_parameter_list()})'
        formatted += f'{str(self.lambda_specifier)}'
        formatted += f' -> {self.return_type}'
        formatted += ' {}'
        return formatted


@dataclass
class StaticAssert:
    condition: str

    def __str__(self) -> str:
        return f'static_assert({self.condition});'


@dataclass
class FunctionType:
    name: str
    variable: Union[None, tuple[str, str]]
    type_expression: Union[None, str]

    def __str__(self) -> str:
        formatted = f'{self.variable[0]} {self.name}_var = {self.variable[1]};' if self.variable else ''
        formatted += f'using {self.name}_t = decltype('
        formatted += f'{self.name}_var' if self.variable else f'{self.type_expression}'
        formatted += ');'
        return formatted


def cyclic_zip(*iterables):
    longest = max(iterables, key=len)
    elements = (cycle(elem) if elem != longest else elem for elem in iterables)
    return zip(*elements)


def make_function_type(function_name: str, qualifiers: tuple[Optional[Qualifier], Optional[Pointer], Optional[Reference]]):
    def qualify_type(base_type: str, qualifier: Optional[Qualifier]):
        qualified_type = f'std::add_const_t<{base_type}>' if qualifier == Qualifier.CONST else base_type
        qualified_type = f'std::add_volatile_t<{qualified_type}>' if qualifier == Qualifier.VOLATILE else qualified_type
        qualified_type = f'std::add_cv_t<{qualified_type}>' if qualifier == Qualifier.CONST_VOLATILE else qualified_type
        return qualified_type

    q, p, r = qualifiers
    fn_type = f'decltype({function_name})'
    fn_type = qualify_type(fn_type, q)
    fn_type = qualify_type(
        f'std::add_pointer_t<{fn_type}>', p.qualifier) if p else fn_type
    fn_type = f'std::add_lvalue_reference_t<{fn_type}>' if r == Reference.LVALUE_REFERENCE else f'std::add_rvalue_reference_t<{fn_type}>' if r == Reference.RVALUE_REFERENCE else fn_type
    return fn_type


def is_pointer_type(full_type: FullType) -> bool:
    return True if full_type.pointer else False


def is_reference_type(full_type: FullType) -> bool:
    return True if full_type.reference else False


def is_value_type(full_type: FullType) -> bool:
    return not (is_pointer_type(full_type) or is_reference_type(full_type))


def is_cv_qualified(t: Union[FullType, Pointer]) -> bool:
    return True if t.qualifier else False


def is_void_type(full_type: FullType) -> bool:
    return full_type.type_value == Type.VOID


def is_cv_qualified_value_or_pointer(full_type: FullType) -> bool:
    is_cv_qualified_value = is_value_type(
        full_type) and is_cv_qualified(full_type)
    is_cv_qualified_pointer = is_pointer_type(full_type) and is_cv_qualified(
        full_type.pointer) and not is_reference_type(full_type)
    return is_cv_qualified_value or is_cv_qualified_pointer


def is_valid_return_type(full_type: FullType) -> bool:
    is_void_reference = is_void_type(full_type) and is_reference_type(
        full_type) and not is_pointer_type(full_type)
    return not (is_cv_qualified_value_or_pointer(full_type) or is_void_reference)


def is_valid_parameter_type(full_type: FullType) -> bool:
    is_void_value_or_reference = is_void_type(full_type) and (
        is_value_type(full_type) or (is_reference_type(full_type) and not is_pointer_type(full_type)))
    return not (is_void_value_or_reference or is_cv_qualified_value_or_pointer(full_type))


def gen_functions(return_types: list[FullType], parameters: list[list[FullType]], specifiers: list[Optional[Specifier]]) -> list[Function]:
    counter = 0
    functions = []
    for return_type, parameter, is_variadic, specifier in cyclic_zip(return_types, parameters, [False, True], specifiers):
        functions += [Function(return_type,
                               f'fn_{counter}', parameter, is_variadic, specifier)]
        counter += 1
    return functions


def gen_member_functions(return_types: list[FullType], parameters: list[list[FullType]], member_specifiers: list[MemberSpecifier]) -> list[MemberFunction]:
    counter = 0
    functions = []
    for return_type, parameter, is_variadic, specifier in cyclic_zip(return_types, parameters, [False, True], member_specifiers):
        functions += [MemberFunction(return_type, 'mem_fn', parameter,
                                     is_variadic, None, specifier, f'class_{counter}')]
        counter += 1
    return functions


def gen_lambda_functions(return_types: list[FullType], parameters: list[list[FullType]], lambda_specifiers: list[LambdaSpecifier]) -> list[LambdaFunction]:
    counter = 0
    functions = []
    for return_type, parameter, is_variadic, specifier in zip(cycle(return_types), parameters, cycle([False, True]), cycle(lambda_specifiers)):
        functions += [LambdaFunction(return_type, '', parameter,
                                     is_variadic, None, specifier, f'lambda_{counter}')]
        counter += 1
    return functions


def gen_function_tests(functions: list[Function], function_types: list[tuple[Optional[Qualifier], Optional[Pointer], Optional[Reference]]]) -> list[list[StaticAssert]]:
    trait_idx = 0

    def traits(fn: Function):
        nonlocal trait_idx
        fn_type = make_function_type(fn.name, function_types[trait_idx])
        trait_idx = (trait_idx + 1) % len(function_types)
        return f'callable_traits<{fn_type}>'

    tests = []
    for fn in functions:
        fn_tests = []
        fn_tests += [StaticAssert(
            f'std::is_same_v<{traits(fn)}::result_type, {str(fn.return_type)}>')]
        for idx in range(0, 4):
            fn_tests += [StaticAssert(
                f'helper::has_arg{idx}_type_v<{traits(fn)}> == {"true" if len(fn.parameters) > idx and idx < 3 else "false"}')]
        for idx, parameter in enumerate(fn.parameters):
            if idx < 3:
                fn_tests += [StaticAssert(
                    f'std::is_same_v<{traits(fn)}::arg{idx}_type, {str(parameter)}>')]
            fn_tests += [StaticAssert(
                f'std::is_same_v<{traits(fn)}::arg_type<{idx}>, {str(parameter)}>')]
        fn_tests += [StaticAssert(f'{traits(fn)}::arity == {len(fn.parameters)}')]
        fn_tests += [StaticAssert(f'{traits(fn)}::is_functor == false')]
        fn_tests += [StaticAssert(f'{traits(fn)}::is_member_function == false')]
        fn_tests += [StaticAssert(
            f'{traits(fn)}::is_variadic == {"true" if fn.is_variadic else "false"}')]
        fn_tests += [StaticAssert(
            f'{traits(fn)}::is_noexcept == {"true" if fn.specifier == Specifier.NOEXCEPT else "false"}')]
        fn_tests += [StaticAssert(f'{traits(fn)}::is_const == false')]
        fn_tests += [StaticAssert(f'{traits(fn)}::is_volatile == false')]
        fn_tests += [StaticAssert(f'{traits(fn)}::is_lvalue_reference == false')]
        fn_tests += [StaticAssert(f'{traits(fn)}::is_rvalue_reference == false')]
        fn_tests += [StaticAssert(
            f'helper::has_class_type_v<{traits(fn)}> == false')]
        tests += [fn_tests]
    return tests


def gen_member_function_tests(functions: list[MemberFunction]) -> list[list[StaticAssert]]:
    tests = []
    for fn in functions:
        traits = f'callable_traits<decltype(&{fn.class_type}::mem_fn)>'
        fn_tests = []
        fn_tests += [StaticAssert(
            f'std::is_same_v<{traits}::result_type, {str(fn.return_type)}>')]
        for idx in range(0, 4):
            fn_tests += [StaticAssert(
                f'helper::has_arg{idx}_type_v<{traits}> == {"true" if len(fn.parameters) > idx and idx < 3 else "false"}')]
        for idx, parameter in enumerate(fn.parameters):
            if idx < 3:
                fn_tests += [StaticAssert(
                    f'std::is_same_v<{traits}::arg{idx}_type, {str(parameter)}>')]
            fn_tests += [StaticAssert(
                f'std::is_same_v<{traits}::arg_type<{idx}>, {str(parameter)}>')]
        fn_tests += [StaticAssert(f'{traits}::arity == {len(fn.parameters)}')]
        fn_tests += [StaticAssert(f'{traits}::is_functor == false')]
        fn_tests += [StaticAssert(f'{traits}::is_member_function == true')]
        fn_tests += [StaticAssert(
            f'{traits}::is_variadic == {"true" if fn.is_variadic else "false"}')]
        fn_tests += [StaticAssert(
            f'{traits}::is_noexcept == {"true" if fn.member_specifier.specifier == Specifier.NOEXCEPT else "false"}')]
        fn_tests += [StaticAssert(f'{traits}::is_const == {"true" if fn.member_specifier.qualifier == Qualifier.CONST or fn.member_specifier.qualifier == Qualifier.CONST_VOLATILE else "false"}')]
        fn_tests += [StaticAssert(f'{traits}::is_volatile == {"true" if fn.member_specifier.qualifier == Qualifier.VOLATILE or fn.member_specifier.qualifier == Qualifier.CONST_VOLATILE else "false"}')]
        fn_tests += [StaticAssert(
            f'{traits}::is_lvalue_reference == {"true" if fn.member_specifier.reference == Reference.LVALUE_REFERENCE else "false"}')]
        fn_tests += [StaticAssert(
            f'{traits}::is_rvalue_reference == {"true" if fn.member_specifier.reference == Reference.RVALUE_REFERENCE else "false"}')]
        fn_tests += [StaticAssert(
            f'helper::has_class_type_v<{traits}> == true')]
        fn_tests += [StaticAssert(
            f'std::is_same_v<{traits}::class_type, {fn.class_type}>')]
        tests += [fn_tests]
    return tests


def gen_lambda_function_tests(functions: list[LambdaFunction]) -> list[list[StaticAssert]]:
    tests = []
    for fn in functions:
        traits = f'callable_traits<decltype({fn.lambda_name})>'
        fn_tests = []
        fn_tests += [StaticAssert(
            f'std::is_same_v<{traits}::result_type, {str(fn.return_type)}>')]
        for idx in range(0, 4):
            fn_tests += [StaticAssert(
                f'helper::has_arg{idx}_type_v<{traits}> == {"true" if len(fn.parameters) > idx and idx < 3 else "false"}')]
        for idx, parameter in enumerate(fn.parameters):
            if idx < 3:
                fn_tests += [StaticAssert(
                    f'std::is_same_v<{traits}::arg{idx}_type, {str(parameter)}>')]
            fn_tests += [StaticAssert(
                f'std::is_same_v<{traits}::arg_type<{idx}>, {str(parameter)}>')]
        fn_tests += [StaticAssert(f'{traits}::arity == {len(fn.parameters)}')]
        fn_tests += [StaticAssert(f'{traits}::is_functor == true')]
        fn_tests += [StaticAssert(f'{traits}::is_member_function == false')]
        fn_tests += [StaticAssert(
            f'{traits}::is_variadic == {"true" if fn.is_variadic else "false"}')]
        fn_tests += [StaticAssert(
            f'{traits}::is_noexcept == {"true" if fn.lambda_specifier.specifier == Specifier.NOEXCEPT else "false"}')]
        fn_tests += [StaticAssert(
            f'{traits}::is_const == {"false" if fn.lambda_specifier.mutable == Mutable.MUTABLE else "true"}')]
        fn_tests += [StaticAssert(f'{traits}::is_volatile == false')]
        fn_tests += [StaticAssert(f'{traits}::is_lvalue_reference == false')]
        fn_tests += [StaticAssert(f'{traits}::is_rvalue_reference == false')]
        fn_tests += [StaticAssert(
            f'helper::has_class_type_v<{traits}> == true')]
        fn_tests += [StaticAssert(
            f'std::is_same_v<{traits}::class_type, std::remove_const_t<decltype({fn.lambda_name})>>')]
        tests += [fn_tests]
    return tests


def gen_function_parameters(num_parameters: int, parameters: list[FullType]) -> list[list[FullType]]:
    if num_parameters == 0:
        return [[]]
    return [parameters[i:i + num_parameters] for i in range(0, len(parameters), num_parameters)]


def append_headers(source: str, headers: list[Header]) -> str:
    source += '\n'.join(str(h) for h in headers)
    return source + '\n\n'


def append_tests(source: str, functions: Union[list[Function], list[MemberFunction], list[LambdaFunction]], assertions: list[list[StaticAssert]]) -> str:
    for fn, tests in zip(functions, assertions):
        source += f'{fn};\n'
        source += '\n'.join(str(t) for t in tests) + '\n'
    return source


def append_main(source: str) -> str:
    return source + 'int main() { return 0; }'


headers = [Header(h, True) for h in ['functional', 'string', 'type_traits']]
headers += [Header(h, False) for h in ['../include/callable_traits.hpp',
                                       'callable_traits_test_helper.hpp']]

all_qualifiers = [None] + [q for q in Qualifier]
all_types = [t for t in Type]
all_pointers = [None] + [Pointer(q) for q in all_qualifiers]
all_references = [None] + [r for r in Reference]
all_mutables = [None] + [m for m in Mutable]
all_full_types = [FullType(q, t, p, r)
                  for t in all_types
                  for r in all_references
                  for p in all_pointers
                  for q in all_qualifiers]
all_return_types = [t for t in all_full_types if is_valid_return_type(t)]
all_parameter_types = [t for t in all_full_types if is_valid_parameter_type(t)]
all_function_types = [(q, p, r)
                      for r in all_references
                      for p in all_pointers
                      for q in all_qualifiers]
all_parameters = list(chain.from_iterable(
    [gen_function_parameters(p, all_parameter_types) for p in range(0, 5)]))
all_specifiers = [None, Specifier.NOEXCEPT]
all_member_specifiers = [MemberSpecifier(q, r, s)
                         for s in all_specifiers
                         for r in all_references
                         for q in all_qualifiers]
all_lambda_specifiers = [LambdaSpecifier(m, s)
                         for s in all_specifiers
                         for m in all_mutables]

functions = gen_functions(all_return_types, all_parameters, all_specifiers)
function_tests = gen_function_tests(functions, all_function_types)

member_functions = gen_member_functions(
    all_return_types, all_parameters, all_member_specifiers)
member_function_tests = gen_member_function_tests(member_functions)

lambda_functions = gen_lambda_functions(
    all_return_types, all_parameters, all_lambda_specifiers)
lambda_function_tests = gen_lambda_function_tests(lambda_functions)

source = '/* This file was auto-generated */\n\n'
source = append_headers(source, headers)
source = append_tests(source, functions, function_tests)
source = append_tests(source, member_functions, member_function_tests)
source = append_tests(source, lambda_functions, lambda_function_tests)
source = append_main(source)

print(source)

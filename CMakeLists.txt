cmake_minimum_required(VERSION 3.18)

project(callable_traits LANGUAGES CXX)
set(CALLABLE_TRAITS_VERSION "1.0.0")

# Determine if callable_traits is built as a subproject (using add_subdirectory).
if (NOT DEFINED CALLABLE_TRAITS_MASTER_PROJECT)
	set(CALLABLE_TRAITS_MASTER_PROJECT OFF)
	if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
		set(CALLABLE_TRAITS_MASTER_PROJECT ON)
	endif()
endif()

option(CALLABLE_TRAITS_TESTS "Build callable_traits tests." ${CALLABLE_TRAITS_MASTER_PROJECT})
option(CALLABLE_TRAITS_EXAMPLES "Build callable_traits examples." ${CALLABLE_TRAITS_MASTER_PROJECT})
option(CALLABLE_TRAITS_INSTALL "Enable installation target for callable_traits." OFF)

add_library(callable_traits INTERFACE)
add_library(callable_traits::callable_traits ALIAS callable_traits)
target_compile_features(callable_traits INTERFACE cxx_std_17)
target_include_directories(callable_traits INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

# Function for setting compiler-specific parameters.
function(callable_traits_set_compiler_parameters TARGET)
	set_target_properties(${TARGET} PROPERTIES
		VERSION ${CALLABLE_TRAITS_VERSION}
		CXX_STANDARD 17
		CXX_EXTENSIONS OFF
	)
	target_compile_options(${TARGET} PRIVATE
		$<$<CXX_COMPILER_ID:MSVC>:/D_CRT_SECURE_NO_WARNINGS /MP /W4 /permissive- /bigobj /Zi /utf-8>
		$<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall -Wextra -pedantic -g>
		$<$<CXX_COMPILER_ID:GNU>:-fdiagnostics-color>
		$<$<CXX_COMPILER_ID:Clang,AppleClang>:-fcolor-diagnostics>
	)
	target_link_options(${TARGET} PRIVATE
		$<$<CXX_COMPILER_ID:MSVC>:/DEBUG>
	)
endfunction()

# Unit tests.
if(CALLABLE_TRAITS_TESTS)
	include(CTest)
	add_executable(callable_traits_tests
		"test/callable_traits_tests.cpp"
	)
	callable_traits_set_compiler_parameters(callable_traits_tests)
	target_compile_options(callable_traits_tests PRIVATE $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wno-return-type>)
	target_link_libraries(callable_traits_tests PRIVATE callable_traits)

	add_test(NAME callable_traits_tests COMMAND callable_traits_tests)

	# Set test as VS startup if callable_traits is master project.
	if(CALLABLE_TRAITS_MASTER_PROJECT)
		set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT callable_traits_tests)
	endif()
endif()

# Examples.
if(CALLABLE_TRAITS_EXAMPLES)
	file(GLOB_RECURSE CALLABLE_TRAITS_EXAMPLES_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/examples/*.cpp")
	foreach(CALLABLE_TRAITS_EXAMPLE_SOURCE ${CALLABLE_TRAITS_EXAMPLES_SOURCES})
		get_filename_component(CALLABLE_TRAITS_EXAMPLE_TARGET ${CALLABLE_TRAITS_EXAMPLE_SOURCE} NAME_WE)
		add_executable(${CALLABLE_TRAITS_EXAMPLE_TARGET} "${CALLABLE_TRAITS_EXAMPLE_SOURCE}")
		callable_traits_set_compiler_parameters(${CALLABLE_TRAITS_EXAMPLE_TARGET})
		target_link_libraries(${CALLABLE_TRAITS_EXAMPLE_TARGET} PRIVATE callable_traits)
	endforeach()
endif()

# Installation target.
if(CALLABLE_TRAITS_INSTALL)
	include(CMakePackageConfigHelpers)

	# Callable_traits installation.
	set(CALLABLE_TRAITS_PROJECT_CONFIG_OUT "${CMAKE_CURRENT_BINARY_DIR}/callable_traits-config.cmake")
	set(CALLABLE_TRAITS_VERSION_CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/callable_traits-config-version.cmake")
	install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
		DESTINATION include/
	)
	install(TARGETS callable_traits
		EXPORT callable_traits
	)
	install(EXPORT callable_traits
		DESTINATION lib/cmake/callable_traits
		NAMESPACE callable_traits::
		FILE callable_traits-config-targets.cmake
	)
	configure_package_config_file(cmake/callable_traits-config.cmake.in
		"${CALLABLE_TRAITS_PROJECT_CONFIG_OUT}"
		INSTALL_DESTINATION lib/cmake/callable_traits
	)
	write_basic_package_version_file("${CALLABLE_TRAITS_VERSION_CONFIG_FILE}"
		VERSION ${CALLABLE_TRAITS_VERSION}
		COMPATIBILITY SameMajorVersion
	)
	install(FILES "${CALLABLE_TRAITS_PROJECT_CONFIG_OUT}" "${CALLABLE_TRAITS_VERSION_CONFIG_FILE}"
		DESTINATION lib/cmake/callable_traits
	)
endif()

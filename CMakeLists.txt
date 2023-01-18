cmake_minimum_required(VERSION 3.18)

project(callable_traits LANGUAGES CXX)
set(CALLABLE_TRAITS_VERSION "1.0.0")

option(CALLABLE_TRAITS_INSTALL "Enable installation target for callable_traits." OFF)

add_library(callable_traits INTERFACE)
add_library(callable_traits::callable_traits ALIAS callable_traits)
target_compile_features(callable_traits INTERFACE cxx_std_17)
target_include_directories(callable_traits INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)

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

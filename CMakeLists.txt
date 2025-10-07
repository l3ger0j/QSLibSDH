cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(qslibsdh VERSION 5.7.0)

set(QSLIBSDH_GAMEMIN_VER "5.7.0")

include(CMakePackageConfigHelpers)
include(GenerateExportHeader)
include(GNUInstallDirs)
include(FetchContent)

option(BUILD_JVM "Build with JVM bindings" OFF)
option(USE_INSTALLED_ONIGURUMA "Use system-wide oniguruma library instead of bundling it" OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_BUILD_RPATH_USE_ORIGIN ON)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

set(TARGET qslibsdh PROPERTY CMAKE_C_STANDARD 17)
set(TARGET qslibsdh PROPERTY CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_POLICY_DEFAULT_CMP0063 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-char-subscripts -Wno-unused-variable")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -D_DEBUG -DDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -D_DEBUG -DDEBUG")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Ofast -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -DNDEBUG")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /D_DEBUG /DDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /D_DEBUG /DDEBUG")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /O2 /DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /DNDEBUG")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_C_COMPILER_ID}" STREQUAL "AppleClang")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -D_DEBUG -DDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -D_DEBUG -DDEBUG")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Ofast -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -DNDEBUG")
else()
    message(FATAL_ERROR "Compiler is not supported")
endif()

if (USE_INSTALLED_ONIGURUMA)
	find_package(oniguruma REQUIRED)
else()
	# Download oniguruma
	FetchContent_Declare(oniguruma-custom
			GIT_REPOSITORY https://github.com/kkos/oniguruma.git
			GIT_TAG master
			GIT_SHALLOW true # it works only with branch names and tags
			GIT_PROGRESS true
			SUBBUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/oniguruma-download"
			SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/oniguruma-src"
			BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/oniguruma-build"
	)

	# Disable oniguruma features we don't need
	set(INSTALL_DOCUMENTATION OFF)
	set(INSTALL_EXAMPLES OFF)
	set(BUILD_TEST OFF)
	set(BUILD_SHARED_LIBS OFF)
	set(MSVC_STATIC_RUNTIME ON)

	# For Windows: Prevent overriding the parent project's compiler/linker settings
	set(oniguruma_force_shared_crt ON CACHE BOOL "" FORCE)

	# Configure the CRNL handling
	add_compile_definitions(USE_CRNL_AS_LINE_TERMINATOR=1)

	# Add oniguruma directly to our build
	FetchContent_MakeAvailable(oniguruma-custom)
	add_library(oniguruma::onig ALIAS onig)
endif()

configure_file(qslibsdh_config.h.cmakein qslibsdh_config.h @ONLY)

set(QSLIBSDH_SOURCES
		src/bindings/bindings_config.h
		src/bindings/default/default_callbacks.c
		src/bindings/default/default_control.c
		src/bindings/default/qsp_default.h
		src/bindings/jvm/jvm_callbacks.c
		src/bindings/jvm/jvm_control.c
		src/bindings/jvm/jvm_coding.c
		src/bindings/jvm/qsp_jvm.h
		src/bindings/qsp.h
		src/actions.c src/actions.h
		src/callbacks.c src/callbacks.h
		src/codetools.c src/codetools.h
		src/coding.c src/coding.h
		src/common.c src/common.h
		src/errors.c src/errors.h
		src/game.c src/game.h
		src/locations.c src/locations.h
		src/mathops.c src/mathops.h
		src/memwatch.c src/memwatch.h
		src/menu.c src/menu.h
		src/objects.c src/objects.h
		src/playlist.c src/playlist.h
		src/statements.c src/statements.h
		src/text.c src/text.h
		src/time.c src/time.h
		src/towlower.c
		src/towupper.c
		src/variables.c src/variables.h
		src/variant.c src/variant.h
)
add_library(qslibsdh SHARED ${QSLIBSDH_SOURCES})
target_compile_definitions(qslibsdh PUBLIC _UNICODE)
if (WIN32)
	target_compile_definitions(qslibsdh PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
target_link_libraries(qslibsdh PRIVATE oniguruma::onig)

if (BUILD_JVM)
	target_compile_definitions(qslibsdh PRIVATE _JVM)
	if (NOT ANDROID)
		find_package(JNI)
		target_include_directories(qslibsdh PRIVATE ${JNI_INCLUDE_DIRS})
		target_link_libraries(qslibsdh PRIVATE ${JNI_LIBRARIES})
	endif()
endif()

target_include_directories(qslibsdh
		INTERFACE
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/bindings/default>"
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/bindings>"
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
		"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/src>"
		"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/src/default>"
)

generate_export_header(qslibsdh
		BASE_NAME QSLibSDH
		EXPORT_MACRO_NAME QSLIBSDH_EXTERN
)

write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/cmake/QSLibSDH/QSLibSDHConfigVersion.cmake COMPATIBILITY AnyNewerVersion)
configure_package_config_file(QSLibSDHConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/cmake/QSLibSDH/QSLibSDHConfig.cmake
		INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/QSLibSDH"
)

set(QSLIBSDH_INSTALL_TARGETS qslibsdh)

if (TARGET onig)
	get_target_property(ONIG_IS_IMPORTED onig IMPORTED)
	if (NOT ONIG_IS_IMPORTED)
		list(APPEND QSLIBSDH_INSTALL_TARGETS onig)
	endif()
endif()

export(TARGETS ${QSLIBSDH_INSTALL_TARGETS} NAMESPACE QSLibSDH:: FILE ${CMAKE_CURRENT_BINARY_DIR}/QSLibSDHTargets.cmake)

install(TARGETS ${QSLIBSDH_INSTALL_TARGETS} EXPORT QSLibSDHTargets)

install(EXPORT QSLibSDHTargets FILE QSLibSDHTargets.cmake NAMESPACE QSLibSDH:: DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/QSLibSDH")

install(
		DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cmake
		DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)

install(FILES
		src/bindings/qsp.h
		src/bindings/bindings_config.h
		"${CMAKE_CURRENT_BINARY_DIR}/qsp_export.h"
		DESTINATION
		"${CMAKE_INSTALL_INCLUDEDIR}/src"
)

install(FILES
		src/bindings/default/qsp_default.h
		DESTINATION
		"${CMAKE_INSTALL_INCLUDEDIR}/src/default"
)
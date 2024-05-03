# ------------------------------------------------------------------------------ 
#           File : CMakeLists.txt
#         Author : TBlauwe
#    Description : Root cmake file.
#
#                  Available targets (excluding dependencies) :
#                  * slog : main library
#                  * tests, documentation, benchmarks are only available
#                  when build as the main project.
#
#                  Noteworthy cmake folders :
#                  * extern/ : contains cmake files to find dependencies using CPM.
#                              Can be skipped by setting SLOG_DOWNLOAD_DEPENDENCIES 
#
#                  Noteworthy cmake options :
#                  * SLOG_DOWNLOAD_DEPENDENCIES : skip dependencies download and build (with CPM)
# ------------------------------------------------------------------------------ 
cmake_minimum_required (VERSION 3.21...3.26)
include(cmake/prevent_in_source_build.cmake)

# ----- Project information
project(SLOG
	DESCRIPTION "A simple logging library built around fmt"
	HOMEPAGE_URL "https://github.com/TBlauwe/slog"
	LANGUAGES C CXX
)

# Our CMake's behaviours is altered whether this project is consumed or if it is the main project
if (PROJECT_IS_TOP_LEVEL)
	set(CMAKE_CXX_EXTENSIONS OFF) # Ensure -std=c++xx is used instead of -std=g++xx. See: https://crascit.com/2015/03/28/enabling-cxx11-in-cmake/
	set_property(GLOBAL PROPERTY USE_FOLDERS ON) # Support folders in IDEs
	# Convience variable so that executables (tests, benchmarks, experiments, examples) are outputed to a specific folder.
	set(PROJECT_EXE_DIR "${CMAKE_BINARY_DIR}/bin") 
	find_package(Git)
endif ()

# --- Project options
option(${PROJECT_NAME}_DOWNLOAD_DEPENDENCIES "Disable automatic download of dependencies with CPM." ${PROJECT_IS_TOP_LEVEL})


include(cmake/tools.cmake)



# ------------------------------------------------------------------------------
# --- Dependencies
# ------------------------------------------------------------------------------
if(${${PROJECT_NAME}_DOWNLOAD_DEPENDENCIES})
	section(CHECK_START "Finding slog dependencies")

		unset(${PROJECT_NAME}_DEPENDENCIES)
		unset(${PROJECT_NAME}_MISSING_DEPENDENCIES)

		use_version(NAME fmt VERSION "10.0.0")
		download_library(
		  NAME fmt
		  TARGETS fmt 		  
		  GITHUB_REPOSITORY fmtlib/fmt
		  OPTIONS
			"FMT_INSTALL OFF"
		)

	end_section(
		CONDITION NOT ${PROJECT_NAME}_MISSING_DEPENDENCIES
		PASS "done. Dependencies : ${${PROJECT_NAME}_DEPENDENCIES}"
		FAIL "failed. Missing dependencies : ${${PROJECT_NAME}_MISSING_DEPENDENCIES}"
	)
endif()


# ------------------------------------------------------------------------------
# --- Target : slog
# ------------------------------------------------------------------------------
# --- Headers
SET(HEADER_LIST
    "include/slog/slog.hpp"
    "include/slog/reporter.hpp"
    "include/slog/typename.hpp"
 )

SET(SOURCE_LIST
	"src/reporter.cpp"
)

# --- Assets
# No assets here

section(CHECK_START "Configuring slog")

	add_library(slog ${HEADER_LIST} ${SOURCE_LIST})
	add_library(slog::slog ALIAS slog)

	target_include_directories(
		slog
		${${PROJECT_NAME}_WARNING_GUARD} PUBLIC 
		"$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
	)
	target_include_directories(
		slog
		SYSTEM PUBLIC
		"$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
	)

	target_link_libraries(slog PUBLIC fmt)

	target_compile_features(slog PRIVATE cxx_std_17)
	target_compile_options(slog PUBLIC $<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->) # being a cross-platform target, we enforce standards conformance on MSVC

	if(${PROJECT_IS_TOP_LEVEL})
	  target_set_compile_warnings_cxx(TARGET slog SCOPE PRIVATE)
	endif()

	if (NO_SLOG_LOG)
		target_compile_definitions(slog PUBLIC "NO_SLOG_LOG")
	endif ()

	if (NO_SLOG_ASSERT)
		target_compile_definitions(slog PUBLIC "NO_SLOG_ASSERT")
	endif ()


end_section( 
	CONDITION TARGET slog
	PASS "done."
	FAIL "failed."
)


# ------------------------------------------------------------------------------
# --- Additional targets : tests, benchmarks.
# ------------------------------------------------------------------------------
# Only available if this is the main project
if(${PROJECT_IS_TOP_LEVEL})
	include(CTest)
	add_subdirectory(tests)
	add_subdirectory(benchmarks)
	add_subdirectory(examples)
endif()

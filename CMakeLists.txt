# ------------------------------------------------------------------------------ 
#           File : CMakeLists.txt
#         Author : TBlauwe
#    Description : Root cmake file.
#
#                  Available targets (excluding dependencies) :
#                  * mylib : main library
#                  * tests, documentation, benchmarks are only available
#                  when build as the main project.
#
#                  Noteworthy cmake folders :
#                  * extern/ : contains cmake files to find dependencies using CPM.
#                              Can be skipped by setting SLOG_SKIP_DEPENDENCIES 
#
#                  Noteworthy cmake options :
#                  * SLOG_SKIP_DEPENDENCIES : skip dependencies download and build (with CPM)
#
#                  Noteworthy defines :
#                  * TRACY_ENABLE : If defined, instructions related to tracy will be included.
#                  * _NDEBUG : If undefined, some instructions specific for debugging, like
#                  vulkan validation layers and some logging, will be deactivated.
# ------------------------------------------------------------------------------ 
cmake_minimum_required (VERSION 3.17...3.26)


# ------------------------------------------------------------------------------
# --- CMake configuration 
# ------------------------------------------------------------------------------
# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()


# ----- Project information
project(SLOG
        VERSION 0.1.0
        DESCRIPTION "A small opinionated logging library built around fmt."
        HOMEPAGE_URL "https://github.com"
        LANGUAGES C CXX
)


# --- Include guard
# Only do these if this is the main project, and not if it is included through add_subdirectory
if (NOT DEFINED SLOG_MASTER_PROJECT)
  set(SLOG_MASTER_PROJECT OFF)
  set(SLOG_NOT_MASTER_PROJECT ON)
  if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(SLOG_MASTER_PROJECT ON)
    set(SLOG_NOT_MASTER_PROJECT OFF)
	set(CMAKE_CXX_EXTENSIONS OFF) # Let's ensure -std=c++xx instead of -std=g++xx
	set_property(GLOBAL PROPERTY USE_FOLDERS ON) # Let's nicely support folders in IDEs
	# Convience variable so that executables (tests, benchmarks, experiments, examples) are outputed to a specific folder.
	set(PROJECT_EXE_DIR "${CMAKE_BINARY_DIR}/bin") 
  endif ()
endif ()


# ------------------------------------------------------------------------------
# --- CMake options
# ------------------------------------------------------------------------------
option(SLOG_SKIP_DEPENDENCIES "If true, then dependencies will not be downloaded and build" ${SLOG_NOT_MASTER_PROJECT})


# ------------------------------------------------------------------------------
# --- CMake libraries
# ------------------------------------------------------------------------------
# Dependencies are added via CPM.
# See https://github.com/cpm-cmake/CPM.cmake for more info.
set(CPM_DOWNLOAD_VERSION 0.38.2)
include(CMakePrintHelpers)
include(cmake/cpm.cmake)
include(cmake/target_set_compile_warnings.cmake)
include(cmake/tools.cmake)


# ------------------------------------------------------------------------------
# --- Dependencies
# ------------------------------------------------------------------------------
if(NOT ${SLOG_SKIP_DEPENDENCIES})
	unset(missing_components)
	include(extern/misc.cmake)
endif()

banner_2(CHECK_START "Configuring target : slog")
if(missing_components)
    message(FATAL_ERROR "Missing dependencies: ${missing_components}")
elseif(NOT ${SLOG_SKIP_DEPENDENCIES})
    message("No dependencies missing")
endif()


# ------------------------------------------------------------------------------
# --- Sources & Headers
# ------------------------------------------------------------------------------
SET(SOURCE_LIST
)

SET(HEADER_LIST
    "include/slog/slog.hpp"
)

# ------------------------------------------------------------------------------
# --- Assets
# ------------------------------------------------------------------------------
# No assets here

# ------------------------------------------------------------------------------
# --- Target : slog
# ------------------------------------------------------------------------------
add_library (slog INTERFACE ${SOURCE_LIST} ${HEADER_LIST})
target_include_directories(slog INTERFACE include)
target_link_libraries(slog INTERFACE
    misc_deps
)
target_compile_features(slog INTERFACE cxx_std_17)
target_compile_options(slog INTERFACE $<$<CXX_COMPILER_ID:MSVC>:/Zc:preprocessor>) # if using __VA_OPT__, we enforce latest preprocessor on MSVC
target_compile_options(slog INTERFACE $<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/permissive->) # being a cross-platform target, we enforce standards conformance on MSVC

if(${SLOG_MASTER_PROJECT})
  target_set_compile_warnings_cxx(TARGET slog SCOPE INTERFACE)
endif()

outdent()
if(TARGET slog)
    message(CHECK_PASS "done.")
else()
    message(CHECK_FAIL "failed.")
endif()


# ------------------------------------------------------------------------------
# --- Additional targets : tests, benchmarks, docs 
# ------------------------------------------------------------------------------

# Only available if this is the main project
if(${SLOG_MASTER_PROJECT})
  include(CTest)
  add_subdirectory(benchmarks)
  add_subdirectory(docs)
  add_subdirectory(tests)
endif()

BANNER_2(CHECK_START "Finished configuration")
OUTDENT()

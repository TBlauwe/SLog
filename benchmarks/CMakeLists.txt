# ------------------------------------------------------------------------------ 
#           File : benchmarks/CMakeList.txt
#         author : TBlauwe
#    Description : CMake file to build benchmarks using GoogleBenchmark. 
#
#                  Version of each dependency can be set through options :
#                  * CPM_GOOGLE_BENCHMARK_VERSION
#
#                  CPM noteworthy options :
#                  * CPM_SOURCE_CACHE : download location.
#                  * CPM_USE_LOCAL_PACKAGES : search for locally installed dependencies first.
#                  * CPM_LOCAL_PACKAGES_ONLY : error if dependecy not found locally.
# ------------------------------------------------------------------------------ 
section(CHECK_START "Benchmarks")


# ------------------------------------------------------------------------------
# --- Dependencies
# ------------------------------------------------------------------------------

# A microbenchmark support library 
use_version(NAME benchmark VERSION "v1.8.2")
download_library(
        NAME benchmark 
        TARGETS benchmark
        GITHUB_REPOSITORY google/benchmark
        OPTIONS
            "BENCHMARK_ENABLE_TESTING OFF"
)
if(benchmark_ADDED)
    set_target_properties(benchmark PROPERTIES CXX_STANDARD 11) # enable c++11 to avoid compilation errors
    #target_compile_options(benchmark PUBLIC -D_CRT_USE_BUILTIN_OFFSETOF)
endif ()


# ------------------------------------------------------------------------------
# --- Target : benchmarks
# ------------------------------------------------------------------------------
set(SOURCE_LIST 
    "benchmark.cpp"
)

add_executable(benchmarks ${SOURCE_LIST})
target_compile_features(benchmarks PRIVATE cxx_std_20)
target_link_libraries(benchmarks PRIVATE slog benchmark::benchmark)
set_target_properties(benchmarks PROPERTIES OUTPUT_NAME "Benchmarks")
set_target_properties(benchmarks PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${${PROJECT_NAME}_EXE_DIR}")

# Copy google benchmark tools : compare.py and its requirements for ease of use
add_custom_command(TARGET benchmarks POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_EXE_DIR}/scripts/google_benchmark_tools"
)

add_custom_command(TARGET benchmarks POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${benchmark_SOURCE_DIR}/tools" "${PROJECT_EXE_DIR}/scripts/google_benchmark_tools"
)


# ------------------------------------------------------------------------------
# --- Closure
# ------------------------------------------------------------------------------
end_section(
    CONDITION TARGET benchmarks
    PASS "done."
    FAIL "failed."
)

#include <benchmark/benchmark.h>
#include <slog/slog.hpp>

// Define another benchmark
static void BM_Success(benchmark::State& state) {
	for (auto _ : state){}
}
BENCHMARK(BM_Success);

BENCHMARK_MAIN();
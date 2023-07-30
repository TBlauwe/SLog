#include <benchmark/benchmark.h>
#include <slog/slog.hpp>

// Define a logger by inheriting CRTP class slog::Logger
struct my_logger : public slog::Logger<my_logger>
{
	// Override / customize your parameters
	static constexpr const char * logger_name {"cool_logger"}; 
	static constexpr bool show_fatal_bg {true}; 
	static constexpr bool show_success_bg {true}; 
	static constexpr fmt::rgb fatal_bg {99,7,0}; 
	static constexpr fmt::rgb fatal_fg {232,80,69}; 
};

static void BM_string_info_no_arg(benchmark::State& state) {
	for (auto _ : state)
	{
		benchmark::DoNotOptimize(my_logger::to_string<slog::Level::Info>("message"));
	}
}
BENCHMARK(BM_string_info_no_arg);

static void BM_string_info_with_1_arg(benchmark::State& state) {
	for (auto _ : state)
	{
		benchmark::DoNotOptimize(my_logger::to_string<slog::Level::Info>("message with arg {}", 1));
	}
}
BENCHMARK(BM_string_info_with_1_arg);

BENCHMARK_MAIN();

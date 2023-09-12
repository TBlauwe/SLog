#include <doctest/doctest.h>
#include <slog/slog.hpp>

TEST_CASE("Default logger")
{
    CHECK_NOTHROW(slog::log::debug("Default logger - a debug message with an argument of value {}", 1));
    CHECK_NOTHROW(slog::log::info("Default logger - an info message with an argument of value : {}", 2));
    CHECK_NOTHROW(slog::log::success("Default logger - a success message with an argument of value : {}", 3));
    CHECK_NOTHROW(slog::log::warn("Default logger - a warning message with an argument of value : {}", 4));
    CHECK_NOTHROW(slog::log::error("Default logger - an error message with an argument of value : {}", 5));
    CHECK_NOTHROW(slog::log::fatal("Default logger - a fatal message with an argument of value : {}", 6));
}

// Custom logger are defined by inheriting publicly : slog::Logger<>
struct my_logger : public slog::Logger<my_logger>
{
	// Override / customize your parameters
	static constexpr std::string_view logger_name {"cool_logger"};
	static constexpr bool use_message_style {true}; 
	static constexpr bool show_fatal_bg {true}; 
	static constexpr bool show_success_bg {true}; 
	static constexpr fmt::rgb fatal_bg {99,7,0}; 
	static constexpr fmt::rgb fatal_fg {232,80,69}; 
};


TEST_CASE("Custom logger")
{
    CHECK_NOTHROW(my_logger::debug("Custom logger - a debug message with an argument of value {}", 1));
    CHECK_NOTHROW(my_logger::info("Custom logger - an info message with an argument of value : {}", 2));
    CHECK_NOTHROW(my_logger::success("Custom logger - a success message with an argument of value : {}", 3));
    CHECK_NOTHROW(my_logger::warn("Custom logger - a warning message with an argument of value : {}", 4));
    CHECK_NOTHROW(my_logger::error("Custom logger - an error message with an argument of value : {}", 5));
    CHECK_NOTHROW(my_logger::fatal("Custom logger - a fatal message with an argument of value : {}", 6));
}

TEST_CASE("Macros")
{
	// SLog also provides some macros to log only if a condition is verified
    // Why macros instead of functions ?
    // To prevent condition's evaluation when logging is disabled.
    CHECK_NOTHROW(slog_debug_if(slog::log, true, "Default logger - condition evaluated to true - a debug message with an argument of value {}", 1));
    CHECK_NOTHROW(slog_info_if(slog::log, true, "Default logger - condition evaluated to true - an info message with an argument of value : {}", 2));
    CHECK_NOTHROW(slog_success_if(slog::log, true, "Default logger - condition evaluated to true - a success message with an argument of value : {}", 3));
    CHECK_NOTHROW(slog_warn_if(slog::log, true, "Default logger - condition evaluated to true - a warning message with an argument of value : {}", 4));
    CHECK_NOTHROW(slog_error_if(slog::log, true, "Default logger - condition evaluated to true - an error message with an argument of value : {}", 5));
    CHECK_NOTHROW(slog_fatal_if(slog::log, true, "Default logger - condition evaluated to true - a fatal message with an argument of value : {}", 6));

	// An assert is also provided, that abort if condition is false
	slog_assert(my_logger, true, "Will not abort", 13);
}

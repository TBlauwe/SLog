#include <doctest/doctest.h>
#include <slog/slog.hpp>

// Define a logger by inheriting CRTP class slog::Logger
struct my_logger : public slog::Logger<my_logger>
{
	// Override / customize your parameters
	static constexpr const char * logger_name {"cool_logger"}; 
	static constexpr bool use_message_style {false}; 
	static constexpr bool show_fatal_bg {true}; 
	static constexpr bool show_success_bg {true}; 
	static constexpr fmt::rgb fatal_bg {99,7,0}; 
	static constexpr fmt::rgb fatal_fg {232,80,69}; 
};

TEST_CASE("Basic")
{
	// Now anywhere in code
	my_logger::debug("my message with or without args {}", 1);
	my_logger::info("my message with or without args {}", 2);
	my_logger::success("my message with or without args {}", 3);
	my_logger::warn("my message with or without args {}", 4);
	my_logger::error("my message with or without args {}", 5);
	my_logger::fatal("my message with or without args {}", 6);


	// SLog also provides some macros to log only if a condition is verified :
	slog_debug_if(my_logger, true, "my message with a condition {}", 7);
	slog_info_if(my_logger, true, "my message with a condition {}", 8);
	slog_success_if(my_logger, true, "my message with a condition {}", 9);
	slog_warn_if(my_logger, true, "my message with a condition {}", 10);
	slog_error_if(my_logger, true, "my message with a condition {}", 11);
	slog_fatal_if(my_logger, true, "my message with a condition {}", 12);

	// a default logger is also provided
	slog::log::debug("my message with or without args {}", 13);
	slog::log::info("my message with or without args {}", 14);
	slog::log::success("my message with or without args {}", 15);
	slog::log::warn("my message with or without args {}", 16);
	slog::log::error("my message with or without args {}", 17);
	slog::log::fatal("my message with or without args {}", 18);

	// An assert is also provided, that abort if condition is false
	slog_assert(my_logger, false, "Abort if false {}", 13);
}

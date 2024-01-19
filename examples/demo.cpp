#include <slog/slog.hpp>
#include <slog/reporter.hpp>

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

int main()
{
    slog::log::debug("Default logger - a debug message with an argument of value {}", 1);
    slog::log::info("Default logger - an info message with an argument of value : {}", 2);
    slog::log::success("Default logger - a success message with an argument of value : {}", 3);
    slog::log::warn("Default logger - a warning message with an argument of value : {}", 4);
    slog::log::error("Default logger - an error message with an argument of value : {}", 5);
    slog::log::fatal("Default logger - a fatal message with an argument of value : {}", 6);

    slog_debug_if(slog::log, true, "Default logger - condition evaluated to true - a debug message with an argument of value {}", 1);
    slog_info_if(slog::log, true, "Default logger - condition evaluated to true - an info message with an argument of value : {}", 2);
    slog_success_if(slog::log, true, "Default logger - condition evaluated to true - a success message with an argument of value : {}", 3);
    slog_warn_if(slog::log, true, "Default logger - condition evaluated to true - a warning message with an argument of value : {}", 4);
    slog_error_if(slog::log, true, "Default logger - condition evaluated to true - an error message with an argument of value : {}", 5);
    slog_fatal_if(slog::log, true, "Default logger - condition evaluated to true - a fatal message with an argument of value : {}", 6);

    my_logger::debug("Custom logger - a debug message with an argument of value {}", 1);
    my_logger::info("Custom logger - an info message with an argument of value : {}", 2);
    my_logger::success("Custom logger - a success message with an argument of value : {}", 3);
    my_logger::warn("Custom logger - a warning message with an argument of value : {}", 4);
    my_logger::error("Custom logger - an error message with an argument of value : {}", 5);
    my_logger::fatal("Custom logger - a fatal message with an argument of value : {}", 6);

    auto ncr = slog::NumericalConsoleReporter("My title");
    ncr.add_line("A label", 5, 6);
    ncr.add_line("An other label", 4.2, 7.8);
    ncr.add_line("A decrease", 9.1, 7.6);
    ncr.print();

    // An assert is also provided, that abort if condition is false
    slog_assert(slog::log, false, "We trigger an assert", 13);
}

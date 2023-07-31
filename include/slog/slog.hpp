/*****************************************************************//**
 * \file   slog.hpp
 * \brief  Main header file implementing logger class.
 *
 * Usage :
\code{.cpp}

// First define a logger 
// Don't forget to pass the identifier to Logger, otherwise configurations parameters will not be 
// taken into account
struct my_logger : public Logger<my_logger>
{
	// Override / customize your parameters
	static constexpr bool show_time {false};
	static constexpr bool show_logger_name {false};
	static constexpr const char * level_format {"[{:>7}] "};
};

// Now anywhere in code

my_logger::debug("my message with or without args {}", arg);
my_logger::info("my message with or without args {}", arg);
my_logger::success("my message with or without args {}", arg);
my_logger::warn("my message with or without args {}", arg);
my_logger::error("my message with or without args {}", arg);
my_logger::fatal("my message with or without args {}", arg);
\endcode

	 * SLog also provides some macros to log only if a condition is verified :
\code{.cpp}

slog_debug_if(my_logger, true, "my message with or without args {}", arg);
slog_info_if(my_logger, true, "my message with or without args {}", arg);
slog_success_if(my_logger, true, "my message with or without args {}", arg);
slog_warn_if(my_logger, true, "my message with or without args {}", arg);
slog_error_if(my_logger, true, "my message with or without args {}", arg);
slog_fatal_if(my_logger, true, "my message with or without args {}", arg);

// An assert is also provided, that abort if condition is false
slog_assert(my_logger, false, "Abort if false {}", arg);
\endcode
	 * 
	 * Optimisation :
	 * If _NDEBUG is defined, function body are empty. Function call is still called, but with -O2, 
	 * function call is optimised and removed, if there is no arg.
	 *
	 * To make sure that function call is removed when desired, the macros can be used.
 * \author Tristan
 * \date   July 2023
 *********************************************************************/
#pragma once

#include <string_view>

#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/chrono.h>


// ------------------------------------------------------------------------------
// --- Macros
// ------------------------------------------------------------------------------

/** 
 *  \brief Runtime assert only when \c NO_SLOG_ASSERT is defined.

 *	\param logger A logger type.
 *	\param condition Assert if \c condition i **false** !
 *	\param message A fmt string literal, followed by your arguments.
 *
 *	Usage:
\code{.cpp}
 *	slog_assert(false, my_logger, "This will assert and this {} will be formatted", "argument");
\endcode
 */
#ifndef NO_SLOG_ASSERT
#include <cassert>
#define slog_assert(logger, condition, message, ...)\
	if(!(condition))\
	{\
		logger::fatal(message __VA_OPT__(,) __VA_ARGS__);\
		assert(false && "See message above");\
	}\
	static_assert(true, "")
#else
#define slog_assert(logger, condition, message, ...) ((void)0)
#endif


/** 
 *  \brief Runtime fatal message emitted only if \c condition is evaluated to true. 
 *  Line removed from code if @c NO_SLOG_LOG is not defined.

 *	\param logger A logger type.
 *	\param condition Log only if @c condition is true.
 *	\param message A fmt string literal, followed by your arguments.
 *
 *	Usage:
\code{.cpp}
 *	slog_fatal_if(logger, true, "This will emit a fatal message and this {} will be formatted", "argument");
\endcode
 */
#ifndef NO_SLOG_LOG
#define slog_fatal_if(logger, condition, message, ...)\
	if(condition)\
	{\
		logger::fatal(message __VA_OPT__(,) __VA_ARGS__);\
	}\
	static_assert(true, "")
#else
#define slog_fatal_if(logger, condition, message, ...) ((void)0)
#endif

/** 
 *  \brief Runtime error message emitted only if \c condition is evaluated to true. 
 *  Line removed from code if @c NO_SLOG_LOG is not defined.

 *	\param logger A logger type.
 *	\param condition Log only if @c condition is true.
 *	\param message A fmt string literal, followed by your arguments.
 *
 *	Usage:
\code{.cpp}
 *	slog_error_if(logger, true, "This will emit an error message and this {} will be formatted", "argument");
\endcode
 */
#ifndef NO_SLOG_LOG
#define slog_error_if(logger, condition, message, ...)\
	if(condition)\
	{\
		logger::error(message __VA_OPT__(,) __VA_ARGS__);\
	}\
	static_assert(true, "")
#else
#define slog_error_if(logger, condition, message, ...) ((void)0)
#endif
/** 
 *  \brief Runtime warning message emitted only if \c condition is evaluated to true. 
 *  Line removed from code if @c NO_SLOG_LOG is not defined.

 *	\param logger A logger type.
 *	\param condition Log only if @c condition is true.
 *	\param message A fmt string literal, followed by your arguments.
 *
 *	Usage:
\code{.cpp}
 *	slog_warn_if(logger, true, "This will emit a warn message and this {} will be formatted", "argument");
\endcode
 */
#ifndef NO_SLOG_LOG
#define slog_warn_if(logger, condition, message, ...)\
	if(condition)\
	{\
		logger::warn(message __VA_OPT__(,) __VA_ARGS__);\
	}\
	static_assert(true, "")
#else
#define slog_warn_if(logger, condition, message, ...) ((void)0)
#endif
/** 
 *  \brief Runtime success message emitted only if \c condition is evaluated to true. 
 *  Line removed from code if @c NO_SLOG_LOG is not defined.

 *	\param logger A logger type.
 *	\param condition Log only if @c condition is true.
 *	\param message A fmt string literal, followed by your arguments.
 *
 *	Usage:
\code{.cpp}
 *	slog_success_if(logger, true, "This will emit a success message and this {} will be formatted", "argument");
\endcode
 */
#ifndef NO_SLOG_LOG
#define slog_success_if(logger, condition, message, ...)\
	if(condition)\
	{\
		logger::success(message __VA_OPT__(,) __VA_ARGS__);\
	}\
	static_assert(true, "")
#else
#define slog_success_if(logger, condition, message, ...) ((void)0)
#endif

/** 
 *  \brief Runtime info message emitted only if \c condition is evaluated to true. 
 *  Line removed from code if @c NO_SLOG_LOG is not defined.

 *	\param logger A logger type.
 *	\param condition Log only if @c condition is true.
 *	\param message A fmt string literal, followed by your arguments.
 *
 *	Usage:
\code{.cpp}
 *	slog_info_if(logger, true, "This will emit an info message and this {} will be formatted", "argument");
\endcode
 */
#ifndef NO_SLOG_LOG
#define slog_info_if(logger, condition, message, ...)\
	if(condition)\
	{\
		logger::info(message __VA_OPT__(,) __VA_ARGS__);\
	}\
	static_assert(true, "")
#else
#define slog_info_if(logger, condition, message, ...) ((void)0)
#endif


/** 
 *  \brief Runtime debug message emitted only if \c condition is evaluated to true. 
 *  Line removed from code if @c NO_SLOG_LOG is not defined.

 *	\param logger A logger type.
 *	\param condition Log only if @c condition is true.
 *	\param message A fmt string literal, followed by your arguments.
 *
 *	Usage:
\code{.cpp}
 *	slog_debug_if(logger, true, "This will emit a debug message and this {} will be formatted", "argument");
\endcode
 */
#ifndef NO_SLOG_LOG
#define slog_debug_if(logger, condition, message, ...)\
	if(condition)\
	{\
		logger::debug(message __VA_OPT__(,) __VA_ARGS__);\
	}\
	static_assert(true, "")
#else
#define slog_debug_if(logger, condition, message, ...) ((void)0)
#endif


/**
 * \brief Private macro do not use ! Generate function body for handling level format.
 */
#define PRIVATE_SLOG_GEN_CAT_BODY(out, style, prefix, name)\
	static constexpr fmt::text_style level_style { Self::show_##prefix##_bg ?\
		fmt::bg(Self::prefix##_bg) | fmt::fg(Self::prefix##_fg) :\
		fmt::fg(Self::prefix##_fg)\
	};\
	fmt::format_to(std::back_inserter(out), level_style, Self::level_format, name);\
	if constexpr (Self::inherit_level_style)\
		_message_style  = Self::level_style;\
	else if constexpr (Self::propagate_level_fg)\
		_message_style  |=  fmt::fg(Self::prefix##_fg);\
	else if constexpr (Self::propagate_level_bg)\
		_message_style  |=  fmt::bg(Self::prefix##_bg)


// ------------------------------------------------------------------------------
// --- Classes
// ------------------------------------------------------------------------------

/**
 * \brief SLog namespace containing a default logger \c log, the base logger template \c Logger<Self> and log levels.
 */

namespace slog
{
	/**
	 * 7 level of message.
	 */
	enum class Level
	{
		Fatal,
		Error,
		Warn,
		Success,
		Info,
		Debug	
	};


	/**
	 * @brief CTRP template class that implements necessary functions to log.
	 * 
	 * Usage :
\code{.cpp}

// First define a logger 
// Don't forget to pass the identifier to Logger, otherwise configurations parameters will not be 
// taken into account
struct my_logger : public Logger<my_logger>
{
	// Override / customize your parameters
	static constexpr bool show_time {false};
	static constexpr bool show_logger_name {false};
	static constexpr const char * level_format {"[{:>7}] "};
};

// Now anywhere in code

my_logger::debug("my message with or without args {}", arg);
my_logger::info("my message with or without args {}", arg);
my_logger::success("my message with or without args {}", arg);
my_logger::warn("my message with or without args {}", arg);
my_logger::error("my message with or without args {}", arg);
my_logger::fatal("my message with or without args {}", arg);
\endcode

	 * SLog also provides some macros to log only if a condition is verified :
\code{.cpp}

slog_debug_if(my_logger, true, "my message with or without args {}", arg);
slog_info_if(my_logger, true, "my message with or without args {}", arg);
slog_success_if(my_logger, true, "my message with or without args {}", arg);
slog_warn_if(my_logger, true, "my message with or without args {}", arg);
slog_error_if(my_logger, true, "my message with or without args {}", arg);
slog_fatal_if(my_logger, true, "my message with or without args {}", arg);

// An assert is also provided, that abort if condition is false
slog_assert(my_logger, false, "Abort if false {}", arg);
\endcode
	 * 
	 * Optimisation :
	 * If _NDEBUG is defined, function body are empty. Function call is still called, but with -O2, 
	 * function call is optimised and removed, if there is no arg.
	 *
	 * To make sure that function call is removed when desired, the macros can be used.
	 */
	template<typename Self>
	struct Logger
	{
		template <typename Input, typename... Args>
		inline static void fatal(Input&& fmt, Args... args)
		{
			log<Level::Fatal>(std::forward<Input>(fmt), std::forward<Args>(args)...);
		}

		template <typename Input, typename... Args>
		inline static void error(Input fmt, Args... args)
		{
			log<Level::Error>(fmt, std::forward<Args>(args)...);
		}

		template <typename Input, typename... Args>
		inline static void warn(Input fmt, Args... args)
		{
			log<Level::Warn>(fmt, std::forward<Args>(args)...);
		}

		template <typename Input, typename... Args>
		inline static void success(Input fmt, Args... args)
		{
			log<Level::Success>(fmt, std::forward<Args>(args)...);
		}

		template <typename Input, typename... Args>
		inline static void info(Input fmt, Args... args)
		{
			log<Level::Info>(fmt, std::forward<Args>(args)...);
		}

		template <typename Input, typename... Args>
		inline static void debug(Input fmt, Args... args)
		{
			log<Level::Debug>(fmt, std::forward<Args>(args)...);
		}

		// --- TIME ---
		static constexpr bool show_time {true};
		static constexpr bool show_time_bg {false};
		static constexpr fmt::rgb time_bg {20,20,20};
		static constexpr fmt::rgb time_fg {100,100,100};
		static constexpr const char * time_format {"[{:%H:%M:%S}]"};


		// --- LOGGER ---
		static constexpr bool show_logger_name {true};
		static constexpr const char * logger_name {"default"};
		static constexpr bool show_logger_bg {false};
		static constexpr fmt::rgb logger_bg {20,20,20};
		static constexpr fmt::rgb logger_fg {200,200,200};
		static constexpr const char* logger_format { "{:>12}" };

		// --- CATEGORY ---
		static constexpr bool show_level {true};
		static constexpr const char* level_format {"({:>7})"};

		// Fatal
		static constexpr bool show_fatal_bg {false};
		static constexpr fmt::rgb fatal_bg {99,0,99};
		static constexpr fmt::rgb fatal_fg {232,69,232};

		// Error
		static constexpr bool show_error_bg {false};
		static constexpr fmt::rgb error_bg {99,7,0};
		static constexpr fmt::rgb error_fg {232,80,69};

		// Warn 
		static constexpr bool show_warn_bg {false};
		static constexpr fmt::rgb warn_bg {99,68,0};
		static constexpr fmt::rgb warn_fg {232,180,69};

		// Success 
		static constexpr bool show_success_bg {false};
		static constexpr fmt::rgb success_bg {0,99,8};
		static constexpr fmt::rgb success_fg {69,232,83};

		// Info 
		static constexpr bool show_info_bg {false};
		static constexpr fmt::rgb info_bg {0, 61, 99};
		static constexpr fmt::rgb info_fg {69, 169, 232};

		// Debug
		static constexpr bool show_debug_bg {false};
		static constexpr fmt::rgb debug_fg {200,200,200};
		static constexpr fmt::rgb debug_bg {100,100,100};

		// --- MESSAGE ---
		static constexpr bool add_new_line {true};
		static constexpr bool use_message_style {false};
		static constexpr fmt::text_style message_style {};
		static constexpr bool inherit_level_style {false};
		static constexpr bool propagate_level_fg {true};
		static constexpr bool propagate_level_bg {false};

		Logger() = delete;
		Logger(Logger const&) = delete;
		void operator=(Logger const&) = delete;

		template <Level level, typename Input, typename... Args>
		inline static void log(Input&& fmt, Args&&... args)
		{
#ifndef _NDEBUG
			if constexpr(Self::add_new_line)
				fmt::print("{}\n", Self::template to_string<level>(std::forward<Input>(fmt), std::forward<Args>(args)...));
			else
				fmt::print("{}", Self::template to_string<level>(std::forward<Input>(fmt), std::forward<Args>(args)...));
#endif
		}

		template <Level level, typename Input, typename... Args>
		inline static std::string to_string(Input&& fmt, Args&&... args)
		{
#ifndef _NDEBUG
			// Our subsequent characters will be inserted into this.
			fmt::memory_buffer out;

			// --- TIME ---
			if constexpr (Self::show_time)
			{
				static constexpr fmt::text_style time_style { Self::show_time_bg ?
					fmt::bg(Self::time_bg) | fmt::fg(Self::time_fg) :
					fmt::fg(Self::time_fg)
				};
				std::time_t now = std::time(nullptr);
				fmt::format_to(std::back_inserter(out), time_style, Self::time_format, fmt::localtime(now));
				fmt::format_to(std::back_inserter(out), " ");
			}

			// --- LOGGER ---
			if constexpr (Self::show_logger_name)
			{
				static constexpr fmt::text_style logger_style { Self::show_logger_bg ?
					fmt::bg(Self::logger_bg) | fmt::fg(Self::logger_fg) :
					fmt::fg(Self::logger_fg)
				};
				fmt::format_to(std::back_inserter(out), logger_style, Self::logger_format, Self::logger_name);
				fmt::format_to(std::back_inserter(out), " ");
			}

			// --- CATEGORY ---
			fmt::text_style _message_style {Self::message_style};
			if constexpr (Self::show_level)
			{
				if constexpr (level == Level::Fatal)
				{
					PRIVATE_SLOG_GEN_CAT_BODY(out, _message_style, fatal, "FATAL");
				}
				else if constexpr (level == Level::Error)
				{
					PRIVATE_SLOG_GEN_CAT_BODY(out, _message_style, error, "ERROR");
				}
				else if constexpr (level == Level::Warn)
				{
					PRIVATE_SLOG_GEN_CAT_BODY(out, _message_style, warn, "WARN");
				}
				else if constexpr (level == Level::Success)
				{
					PRIVATE_SLOG_GEN_CAT_BODY(out, _message_style, success, "SUCCESS");
				}
				else if constexpr (level == Level::Info)
				{
					PRIVATE_SLOG_GEN_CAT_BODY(out, _message_style, info, "INFO");
				}
				else if constexpr (level == Level::Debug)
				{
					PRIVATE_SLOG_GEN_CAT_BODY(out, _message_style, debug, "DEBUG");
				}
				fmt::format_to(std::back_inserter(out), " ");
			}

			if constexpr (Self::use_message_style)
			{
				fmt::format_to(std::back_inserter(out), _message_style, fmt, std::forward<Args>(args)...);
			}
			else
			{
				fmt::format_to(std::back_inserter(out), fmt::runtime(fmt), std::forward<Args>(args)...);
			}
			return {out.data(), out.size()};
#else
			return {};
#endif
		}
	};

	/**
	 * \brief Default logger with minimal information.
	 */
	struct log : public Logger<log>
	{
		static constexpr bool show_time {false};
		static constexpr bool show_logger_name {false};
		static constexpr const char * level_format {"[{:>7}]"};
	};
}

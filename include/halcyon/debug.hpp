#pragma once

// debug.hpp:
// Various debugging functions. Configurable via preprocessor definitions.

// Debugging functionality is configured as such:
//  - HAL_DEBUG_ENABLED enables debugging.
//  - HAL_DEBUG_ADVANCED additionally provides timestamps and logs to an output file.
//  - if NDEBUG is defined, both of the aforementioned macros are implicitly defined as well.

#ifndef NDEBUG
    #define HAL_DEBUG_ENABLED
    #define HAL_DEBUG_ADVANCED
#endif

#ifdef HAL_DEBUG_ENABLED

    #include <fstream>
    #include <iostream>

    #include <halcyon/utility/printing.hpp>
    #include <halcyon/utility/strutil.hpp>
    #include <halcyon/utility/timer.hpp>

    // For compatibility with MSVC.
    #ifdef _MSC_VER
        #define __PRETTY_FUNCTION__ __FUNCSIG__
    #endif

    #ifndef __FILE_NAME__
        #define __FILE_NAME__ __FILE__
    #endif
#endif

// Necessary include files.
#include <string_view>

namespace hal
{
    class debug
    {
    public:
        enum class severity
        {
            info,
            warning,
            error,
            init,
            load
        };

        static constexpr bool enabled {
#ifdef HAL_DEBUG_ENABLED
            true
#else
            false
#endif
        };

        // No clue why someone would want to do this, but it's not allowed either way.
        debug(const debug&)            = delete;
        debug& operator=(const debug&) = delete;

        static std::string_view last_error();

#ifdef HAL_DEBUG_ENABLED
        // Output any amount of arguments to stdout/stderr and an output file.
        template <printable... Args>
        static void print(Args&&... extra_info)
        {
            debug::print_severity(severity::info, std::forward<Args>(extra_info)...);
        }

        // Output any amount of arguments to stdout/stderr and an output file.
        // This overload additionally specifies the type of message to output.
        template <printable... Args>
        static void print(severity sev, Args&&... extra_info)
        {
            debug::print_severity(sev, std::forward<Args>(extra_info)...);
        }

        // Show a message box with an error message.
        template <printable... Args>
        [[noreturn]] static void panic(std::string_view function, std::string_view file, u32 line, Args&&... extra_info)
        {
            debug::print_severity(severity::error, "In file ", file, ", line ", line, ", function ", function);
            debug::print_severity(severity::error, string_from_pack(std::forward<Args>(extra_info)...));

            std::exit(EXIT_FAILURE);
        }

        template <printable... Args>
        static void warn_if(bool condition, Args&&... extra_info)
        {
            if (condition) [[unlikely]]
                debug::print(severity::warning, std::forward<Args>(extra_info)...);
        }

        // Check a condition, and panic if it's false.
        template <printable... Args>
        static void verify(bool condition, std::string_view cond_string, std::string_view func, std::string_view file, u32 line,
            Args&&... extra_info)
        {
            if (!condition) [[unlikely]]
                debug::panic(func, file, line, cond_string, " failed: ", std::forward<Args>(extra_info)...);
        }

    private:
        template <printable... Args>
        static void print_severity(severity type, Args&&... extra_info)
        {
            std::stringstream fwd;

    #ifdef HAL_DEBUG_ADVANCED
            fwd << std::fixed << std::setprecision(3) << '[' << m_timer()
                << "s] ";
    #endif

            using enum severity;

            switch (type)
            {
            case info:
                fwd << "[info]  ";
                break;

            case warning:
                fwd << "[warn]  ";
                break;

            case error:
                fwd << "[error] ";
                break;

            case init:
                fwd << "[init]  ";
                break;

            case load:
                fwd << "[load]  ";
                break;

            default:
                fwd << "[????]  ";
                break;
            }

            const std::string with_info { fwd.str() + string_from_pack(extra_info...) };

    #ifdef HAL_DEBUG_ADVANCED
            m_output << with_info << std::endl;
    #endif

            (type == error ? std::cerr : std::cout) << with_info << std::endl;
        }

    #ifdef HAL_DEBUG_ADVANCED
        static std::ofstream m_output;
        static const timer   m_timer;
    #endif
#endif
    };
}

#ifdef HAL_DEBUG_ENABLED

    #define HAL_DEBUG(...) __VA_ARGS__
    #define HAL_PRINT      hal::debug::print
    #define HAL_PANIC(...) hal::debug::panic(__PRETTY_FUNCTION__, __FILE_NAME__, __LINE__, __VA_ARGS__)

    #define HAL_WARN_IF(cond, ...)       HAL_WARN_IF_VITAL(cond, __VA_ARGS__)
    #define HAL_WARN_IF_VITAL(cond, ...) hal::debug::warn_if(cond, __VA_ARGS__)

    #define HAL_ASSERT(cond, ...) HAL_ASSERT_VITAL(cond, __VA_ARGS__)
    #define HAL_ASSERT_VITAL(cond, ...) \
        hal::debug::verify(cond, #cond, __PRETTY_FUNCTION__, __FILE_NAME__, __LINE__, __VA_ARGS__)

#else

    #ifdef HAL_DEBUG_ADVANCED
        #warning HAL_DEBUG_ENABLED is not defined, but HAL_DEBUG_ADVANCED is - this will have no effect
    #endif

    #define HAL_DEBUG(...)

    #define HAL_PRINT(...)
    #define HAL_PANIC(...)

    #define HAL_WARN_IF(...)
    #define HAL_WARN_IF_VITAL(cond, ...) (static_cast<void>(cond))

    #define HAL_ASSERT(...)
    #define HAL_ASSERT_VITAL(cond, ...) (static_cast<void>(cond))

#endif
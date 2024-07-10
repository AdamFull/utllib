#pragma once

#include <logger/log_output/base_output.h>
#include <standart_library.h>
#include <source_location>

namespace utl
{
    namespace logger
    {
        namespace colorize
        {
            inline std::string black(const std::string& input) { return "\x1B[30m" + input + "\033[0m\t\t"; }
            inline std::string red(const std::string& input) { return "\x1B[31m" + input + "\033[0m\t\t"; }
            inline std::string green(const std::string& input) { return "\x1B[32m" + input + "\033[0m\t\t"; }
            inline std::string yellow(const std::string& input) { return "\x1B[33m" + input + "\033[0m\t\t"; }
            inline std::string blue(const std::string& input) { return "\x1B[34m" + input + "\033[0m\t\t"; }
            inline std::string magenta(const std::string& input) { return "\x1B[35m" + input + "\033[0m\t\t"; }
            inline std::string cyan(const std::string& input) { return "\x1B[36m" + input + "\033[0m\t\t"; }
            inline std::string white(const std::string& input) { return "\x1B[37m" + input + "\033[0m\t\t"; }

            inline std::string br_black(const std::string& input) { return "\x1B[90m" + input + "\033[0m\t\t"; }
            inline std::string br_red(const std::string& input) { return "\x1B[91m" + input + "\033[0m\t\t"; }
            inline std::string br_green(const std::string& input) { return "\x1B[92m" + input + "\033[0m\t\t"; }
            inline std::string br_yellow(const std::string& input) { return "\x1B[93m" + input + "\033[0m\t\t"; }
            inline std::string br_blue(const std::string& input) { return "\x1B[94m" + input + "\033[0m\t\t"; }
            inline std::string br_magenta(const std::string& input) { return "\x1B[95m" + input + "\033[0m\t\t"; }
            inline std::string br_cyan(const std::string& input) { return "\x1B[96m" + input + "\033[0m\t\t"; }
            inline std::string br_white(const std::string& input) { return "\x1B[97m" + input + "\033[0m\t\t"; }
        }

        struct formatter
        {
            using time_format_t = std::chrono::zoned_time<std::chrono::system_clock::duration, const std::chrono::time_zone*>;
            static inline time_format_t get_time()
            {
                return std::chrono::zoned_time{ std::chrono::current_zone(), std::chrono::system_clock::now() };
            }

            static inline std::string get_formatted_datetime()
            {
                auto _time = get_time();
                return std::format("{} {} {}", get_formatted_date(_time), get_formatted_time(_time), get_formatted_timezone());
            }

            static inline std::string get_formatted_date(time_format_t _time = get_time())
            {
                return std::format("{:%F}", _time);
            }

            static inline std::string get_formatted_time(time_format_t _time = get_time())
            {
                auto time = std::format("{:%T}", _time);
                time = time.erase(time.find('.'), time.size() - 1);
                return time;
            }

            static inline std::string get_formatted_timezone(time_format_t _time = get_time())
            {
                return std::format("{:%Z}", _time);
            }

            static inline std::string get_source_path(const std::source_location& loc)
            {
#ifdef MAIN_PROJECT_DIRECTORY_PATH
                return std::filesystem::relative(loc.file_name(), MAIN_PROJECT_DIRECTORY_PATH).generic_string();
#else
                return loc.file_name();
#endif
            }

            static inline std::string get_source_info(std::source_location&& loc)
            {
                return std::format("{} {}({}:{})",
                    get_source_path(loc), loc.function_name(),
                    loc.line(), loc.column());
            }

            template<ELogLevel _level, class... _Args>
            static inline std::string format(std::source_location&& loc, const std::string& trace, const std::string_view fmt, _Args&&... args)
            {
                auto _formatted_time = get_formatted_time();
                auto _base = std::format("[{}][{}]",
                    get_level<_level>(), _formatted_time);
                auto _user_log = std::vformat(fmt, std::make_format_args(args...));

                std::string _trace_data{};
                if constexpr (_level == ELogLevel::eError)
                    _trace_data = std::format("\nCalled from function {}. \nStacktrace: {}", get_source_info(std::move(loc)), trace);

                return std::format("{} : {}{}", _base, _user_log, _trace_data);
            }

            template<ELogLevel _level>
            static inline constexpr const char* get_level()
            {
                switch (_level)
                {
                case ELogLevel::eDebug: return "D";
                case ELogLevel::eVerbose: return "V";
                case ELogLevel::eInfo: return "I";
                case ELogLevel::eWarning: return "W";
                case ELogLevel::eError: return "E";
                }
                return "-";
            }

            static inline constexpr std::string colorize(ELogLevel _level, const std::string& input)
            {
                switch (_level)
                {
                case ELogLevel::eDebug: return colorize::cyan(input);
                case ELogLevel::eVerbose: return colorize::white(input);
                case ELogLevel::eInfo: return colorize::green(input);
                case ELogLevel::eWarning: return colorize::yellow(input);
                case ELogLevel::eError: return colorize::red(input);
                }

                return input;
            }
        };
    }
}

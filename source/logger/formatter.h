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
            inline stl::string black(const stl::string& input) { return "\x1B[30m" + input + "\033[0m\t\t"; }
            inline stl::string red(const stl::string& input) { return "\x1B[31m" + input + "\033[0m\t\t"; }
            inline stl::string green(const stl::string& input) { return "\x1B[32m" + input + "\033[0m\t\t"; }
            inline stl::string yellow(const stl::string& input) { return "\x1B[33m" + input + "\033[0m\t\t"; }
            inline stl::string blue(const stl::string& input) { return "\x1B[34m" + input + "\033[0m\t\t"; }
            inline stl::string magenta(const stl::string& input) { return "\x1B[35m" + input + "\033[0m\t\t"; }
            inline stl::string cyan(const stl::string& input) { return "\x1B[36m" + input + "\033[0m\t\t"; }
            inline stl::string white(const stl::string& input) { return "\x1B[37m" + input + "\033[0m\t\t"; }

            inline stl::string br_black(const stl::string& input) { return "\x1B[90m" + input + "\033[0m\t\t"; }
            inline stl::string br_red(const stl::string& input) { return "\x1B[91m" + input + "\033[0m\t\t"; }
            inline stl::string br_green(const stl::string& input) { return "\x1B[92m" + input + "\033[0m\t\t"; }
            inline stl::string br_yellow(const stl::string& input) { return "\x1B[93m" + input + "\033[0m\t\t"; }
            inline stl::string br_blue(const stl::string& input) { return "\x1B[94m" + input + "\033[0m\t\t"; }
            inline stl::string br_magenta(const stl::string& input) { return "\x1B[95m" + input + "\033[0m\t\t"; }
            inline stl::string br_cyan(const stl::string& input) { return "\x1B[96m" + input + "\033[0m\t\t"; }
            inline stl::string br_white(const stl::string& input) { return "\x1B[97m" + input + "\033[0m\t\t"; }
        }

        struct formatter
        {
            using time_format_t = stl::chrono::zoned_time<stl::chrono::system_clock::duration, const stl::chrono::time_zone*>;
            static inline time_format_t get_time()
            {
                return stl::chrono::zoned_time{ stl::chrono::current_zone(), stl::chrono::system_clock::now() };
            }

            static inline stl::string get_formatted_datetime()
            {
                auto _time = get_time();
                return stl::format("{} {} {}", get_formatted_date(_time), get_formatted_time(_time), get_formatted_timezone());
            }

            static inline stl::string get_formatted_date(time_format_t _time = get_time())
            {
                return stl::format("{:%F}", _time);
            }

            static inline stl::string get_formatted_time(time_format_t _time = get_time())
            {
                auto time = stl::format("{:%T}", _time);
                time = time.erase(time.find('.'), time.size() - 1);
                return time;
            }

            static inline stl::string get_formatted_timezone(time_format_t _time = get_time())
            {
                return stl::format("{:%Z}", _time);
            }

            static inline stl::string get_source_path(const stl::source_location& loc)
            {
#ifdef MAIN_PROJECT_DIRECTORY_PATH
                return stl::filesystem::relative(loc.file_name(), MAIN_PROJECT_DIRECTORY_PATH).generic_string();
#else
                return loc.file_name();
#endif
            }

            static inline stl::string get_source_info(stl::source_location&& loc)
            {
                return stl::format("{} {}({}:{})",
                    get_source_path(loc), loc.function_name(),
                    loc.line(), loc.column());
            }

            template<ELogLevel _level, class... _Args>
            static inline stl::string format(stl::source_location&& loc, const stl::string& trace, const stl::string_view fmt, _Args&&... args)
            {
                auto _formatted_time = get_formatted_time();
                auto _base = stl::format("[{}][{}]",
                    get_level<_level>(), _formatted_time);
                auto _user_log = stl::vformat(fmt, stl::make_format_args(stl::forward<_Args>(args)...));

                stl::string _trace_data{};
                if constexpr (_level == ELogLevel::eError)
                    _trace_data = stl::format("\nCalled from function {}. \nStacktrace: {}", get_source_info(stl::move(loc)), trace);

                return stl::format("{} : {}{}", _base, _user_log, _trace_data);
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

            static inline constexpr stl::string colorize(ELogLevel _level, const stl::string& input)
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

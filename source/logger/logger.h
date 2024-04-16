#pragma once

#include "formatter.h"

#include "log_output/cout_output.h"
#include "log_output/file_output.h"
#include "log_output/win_output.h"

//#include <backward.hpp>
#include <debugging/debugbreak.h>

namespace utl
{
    namespace logger
    {
        class CLogger
        {
        protected:
            CLogger() = default;
        public:
            friend std::unique_ptr<CLogger> std::make_unique<CLogger>();

            static const std::unique_ptr<CLogger>& getInstance()
            {
                static std::unique_ptr<CLogger> instance{ nullptr };
                if (!instance)
                    instance = std::make_unique<CLogger>();
                return instance;
            }

            void init(const std::string& app_name, const std::string& app_version);

            void addOutput(std::shared_ptr<COutputBase>&& output);

            template<ELogLevel _level = ELogLevel::eDebug, class ..._Args>
            void log(std::source_location&& loc, const std::string& trace, const std::string_view fmt, _Args&&... args)
            {
                auto formatted = formatter::format<_level>(std::move(loc), trace, fmt, std::forward<_Args>(args)...);

                for (auto& output : vOutputs)
                    output->log(formatted, _level);
            }

        private:
            std::vector<std::shared_ptr<COutputBase>> vOutputs;
        };
    }
}

#define log_add_file_output(filename) utl::logger::CLogger::getInstance()->addOutput(std::make_shared<utl::logger::COutputFile>(filename))
#define log_add_cout_output() utl::logger::CLogger::getInstance()->addOutput(std::make_shared<utl::logger::COutputCOUT>())
#define log_add_wincmd_output() utl::logger::CLogger::getInstance()->addOutput(std::make_shared<utl::logger::COutputWinCmd>())

#define log_init(app_name, app_version) utl::logger::CLogger::getInstance()->init(app_name, app_version);

#define log_declare_scope_name(name) \
constexpr const char* utl_logger_scope = name; \
static_assert(__INCLUDE_LEVEL__ == 0, "log_declare_scope should not be used in header files!")

//#define log_error(fmt, ...) \
//{ \
//backward::StackTrace st; \
//st.load_here(32); \
//backward::Printer p; \
//p.object = true; \
//p.color_mode = backward::ColorMode::always; \
//p.address = true; \
//std::stringstream ss; \
//ss << "\n"; \
//p.print(st, ss); \
//utl::logger::CLogger::getInstance()->log<utl::logger::ELogLevel::eError>(std::source_location::current(), ss.str(), fmt, __VA_ARGS__); \
//debugbreak(); \
//}
#define log_error(fmt, ...) \
{ \
utl::logger::CLogger::getInstance()->log<utl::logger::ELogLevel::eError>(std::source_location::current(), "", fmt, __VA_ARGS__); \
debugbreak(); \
}
#define log_cerror(cond, fmt, ...) if((!cond)) log_error(fmt, __VA_ARGS__)
#define log_warning(fmt, ...) utl::logger::CLogger::getInstance()->log<utl::logger::ELogLevel::eWarning>(std::source_location::current(), "", fmt, __VA_ARGS__)
#define log_info(fmt, ...) utl::logger::CLogger::getInstance()->log<utl::logger::ELogLevel::eInfo>(std::source_location::current(), "", fmt, __VA_ARGS__)

#ifdef NDEBUG
#define log_verbose(fmt, ...)
#define log_debug(fmt, ...)
#else
#define log_verbose(fmt, ...) utl::logger::CLogger::getInstance()->log<utl::logger::ELogLevel::eVerbose>(std::source_location::current(), "", fmt, __VA_ARGS__)
#define log_debug(fmt, ...) utl::logger::CLogger::getInstance()->log<utl::logger::ELogLevel::eDebug>(std::source_location::current(), "", fmt, __VA_ARGS__)
#endif
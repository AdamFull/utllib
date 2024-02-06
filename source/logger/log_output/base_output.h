#pragma once

#include <standart_library.h>

namespace utl
{
	namespace logger
	{
        enum class EOutputType
        {
            eCOUT,
            eFile,
            eWinCmd
        };

        enum class ELogLevel
        {
            eDebug,
            eVerbose,
            eError,
            eWarning,
            eInfo
        };

        class COutputBase
        {
        public:
            virtual ~COutputBase() = default;
            virtual void init(const stl::string& app_name, const stl::string& app_version);
            virtual void log(const stl::string& message, ELogLevel eLevel) = 0;

        protected:
            virtual void write(const stl::string& write) = 0;
        };
	}
}
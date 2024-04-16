#pragma once

#include "base_output.h"

namespace utl
{
    namespace logger
    {
        class COutputWinCmd : public COutputBase
        {
        public:
            virtual ~COutputWinCmd() override;
            void log(const std::string& message, ELogLevel eLevel) override;

        protected:
            void write(const std::string& write) override;
        };
    }
}
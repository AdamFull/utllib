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
            void log(const stl::string& message, ELogLevel eLevel) override;

        protected:
            void write(const stl::string& write) override;
        };
    }
}
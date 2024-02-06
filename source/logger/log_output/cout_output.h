#pragma once

#include "base_output.h"

namespace utl
{
    namespace logger
    {
        class COutputCOUT : public COutputBase
        {
        public:
            virtual ~COutputCOUT() override;
            void log(const stl::string& message, ELogLevel eLevel) override;

        protected:
            void write(const stl::string& write) override;
        };
    }
}
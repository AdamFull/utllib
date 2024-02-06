#pragma once

#include "base_output.h"

namespace utl
{
    namespace logger
    {
        class COutputFile : public COutputBase
        {
        public:
            COutputFile(const stl::string& filepath);
            virtual ~COutputFile() override;
            void log(const stl::string& message, ELogLevel eLevel) override;

        protected:
            void write(const stl::string& write) override;
        private:
            stl::ofstream file;
        };
    }
}
#pragma once

#include "base_output.h"

namespace utl
{
    namespace logger
    {
        class COutputFile : public COutputBase
        {
        public:
            COutputFile(const std::string& filepath);
            virtual ~COutputFile() override;
            void log(const std::string& message, ELogLevel eLevel) override;

        protected:
            void write(const std::string& write) override;
        private:
            std::ofstream file;
        };
    }
}
#pragma once

#include <standart_library.h>
#include <iostream>

namespace utl
{
	namespace parsing
	{
		struct argument
		{
			std::string help;
			bool is_required{ false };
			bool is_requires_value{ false };

			argument& set_help(const std::string& help_text)
			{
				help = help_text;
				return *this;
			}

			argument& required()
			{
				is_required = true;
				return *this;
			}

			argument& require_value()
			{
				is_requires_value = true;
				return *this;
			}
		};

		class argument_parser
		{
		public:
			argument_parser()
			{
				register_arg("-h", "--help").set_help("Show help message");
			}

			template<class... _Args>
			argument& register_arg(_Args... argkeys)
			{
				u64 argument_index = arguments.size();
				std::vector<std::string> keys{ argkeys... };

				auto& arg = arguments.emplace_back();
				
				for (const auto& key : keys)
					argument_mappings[key] = argument_index;

				return arg;
			}

			void parse_arguments(int argc, char* argv[]) 
			{
				for (int i = 1; i < argc; ++i) 
				{
					std::string key = argv[i];
					if (auto found_arg = argument_mappings.find(key); found_arg != argument_mappings.end())
					{
						argument& arg = arguments[found_arg->second];

						if (key == "-h" || key == "-help") 
						{
							print_help();
							exit(0);
						}
						else if (arg.is_requires_value) 
						{
							if (i + 1 < argc)
								parsed[key] = argv[++i];
							else
								throw std::invalid_argument("Argument " + key + " requires a value.");
						}
						else
							parsed[key] = "";
					}
					else
						throw std::invalid_argument("Unknown argument: " + key);
				}
			}

			template <typename _Ty>
			_Ty get_value(const std::string& key) 
			{
				_Ty result;

				if (parsed.find(key) == parsed.end())
					throw std::invalid_argument("Argument " + key + " not found.");

				const auto& svalue = parsed[key];
				if constexpr (std::is_same_v<_Ty, std::string>)
					return svalue;

				std::istringstream iss(svalue);
				if (!(iss >> result))
					throw std::invalid_argument("Invalid value for argument " + key);

				return result;
			}
			
			template<class _Ty>
			_Ty try_get(const std::string& key, const _Ty& default_value)
			{
				_Ty result;

				const auto& svalue = parsed[key];
				if constexpr (std::is_same_v<_Ty, std::string>)
					return svalue;

				std::istringstream iss(svalue);
				if (!(iss >> result))
					return default_value;

				return result;
			}

			void print_help() const 
			{
				std::cout << "Available arguments:\n";
				for (u64 idx = 0ull; idx < arguments.size(); ++idx) 
				{
					auto& arg = arguments[idx];

					std::vector<std::string> keys;
					for (const auto& [key, index] : argument_mappings) 
					{
						if (index == idx)
							keys.emplace_back(key);
					}

					if (!keys.empty() && !arguments[idx].help.empty()) 
					{
						std::cout << "  ";
						for (size_t i = 0; i < keys.size(); ++i) 
						{
							std::cout << keys[i];
							if (i < keys.size() - 1)
								std::cout << ", ";
						}

						std::cout << ": " << arguments[idx].help;
						if (arguments[idx].is_requires_value)
							std::cout << " (requires value)";
						std::cout << "\n";
					}
				}
			}
		private:
			std::unordered_map<std::string, u64> argument_mappings;
			std::vector<argument> arguments;
			std::unordered_map<std::string, std::string> parsed;
		};
	}
}
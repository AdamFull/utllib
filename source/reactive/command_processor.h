#pragma once

#include "function.h"

namespace utl
{
	namespace reactive
	{
		class command_processor;

		namespace detail
		{
			namespace parser
			{
				enum class command_token_type
				{
					unknown,
					number,
					string,
					identifier,
					dot,
					comma,
					left_paren,
					right_paren,
					assign,
					colon,
					eof
				};

				class command_token
				{
				public:
					inline bool check(command_token_type t) const { return type == t; }

					command_token_type type;
					std::string value;
				};

				class scanner
				{
				public:
					void init(const std::string& data)
					{
						this->data = data;
						line = pos = local_pos = 0ull;
					}

					char peek(size_t offset = 0ull)
					{
						size_t element_index = offset + pos;
						if (element_index >= data.size())
							return '\0';

						return data.at(element_index);
					}

					char next()
					{
						char current = peek();
						if (current == '\n')
						{
							line++;
							local_pos = 0ull;
						}
						else
							local_pos++;

						pos++;

						return peek();
					}
				private:
					std::string data;
					size_t line{}, pos{}, local_pos{};
				};

				class command_parser
				{
				public:
					friend class command_processor;

					command_parser()
					{
						operators =
						{
							{".", command_token_type::dot},
							{",", command_token_type::comma},
							{"(", command_token_type::left_paren},
							{")", command_token_type::right_paren},
							{"=", command_token_type::assign},
							{":", command_token_type::colon},
						};
					}

					std::vector<command_token> parse(const std::string& command_str)
					{
						static std::string other_char("=:(.),");
						std::vector<command_token> tokens{};

						sc.init(command_str);

						auto cur = sc.peek();
						while (cur != '\0')
						{
							// Parse identifier
							if (isalpha(cur))
							{
								auto& ntok = tokens.emplace_back(command_token{ command_token_type::identifier, "" });
								while (isalpha(cur))
								{
									if (cur == '\0')
									{
										// error. Incorrect identifier
										break;
									}

									ntok.value += cur;
									cur = sc.next();
								}
							}
							// Parse number
							else if (isdigit(cur))
							{
								auto& ntok = tokens.emplace_back(command_token{ command_token_type::number, "" });
								while (isdigit(cur) || cur == '.')
								{
									if (ntok.value.find('.') != std::string::npos)
									{
										// error. Incorrect number
										return {};
									}

									ntok.value += cur;
									cur = sc.next();
								}
							}
							// Parse operator
							else if (other_char.find(cur) != std::string::npos)
							{
								auto& ntok = tokens.emplace_back(command_token{ command_token_type::unknown, "" });
								while (true)
								{
									if (!ntok.value.empty() && !operators.contains(ntok.value + cur))
									{
										ntok.type = operators.at(ntok.value);
										break;
									}

									ntok.value += cur;
									cur = sc.next();
								}
							}
							// Parse string
							else if (cur == '"')
							{
								auto& ntok = tokens.emplace_back(command_token{ command_token_type::string, "" });

								cur = sc.next();
								while (cur != '"')
								{
									if (cur == '\n')
									{
										// error. Invalid string
										return {};
									}

									ntok.value += cur;
									cur = sc.next();
								}
								cur = sc.next();
							}
							else
								cur = sc.next();
						}

						tokens.emplace_back(command_token{ command_token_type::eof, "" });

						return tokens;
					}
				private:
					scanner sc{};
					std::unordered_map<std::string, command_token_type> operators{};
				};

				class token_scanner
				{
				public:
					void init(const std::string& command_str)
					{
						tokens = parser.parse(command_str);
						pos = 0ull;
					}

					const command_token& peek(size_t offset = 0ull)
					{
						static command_token eof{ command_token_type::eof, "" };

						size_t element_index = offset + pos;
						if (element_index >= tokens.size())
							return eof;

						return tokens.at(element_index);
					}

					const command_token& next()
					{
						pos++;
						return peek();
					}

					bool match(command_token_type type)
					{
						auto& cur = peek();
						if (cur.type != type)
							return false;

						next();
						return true;
					}

					bool check(command_token_type type)
					{
						return peek().type == type;
					}
				private:
					command_parser parser{};
					std::vector<command_token> tokens{};
					size_t pos{};
				};
			}

			class command_argument_value
			{
			public:
				command_argument_value() = default;
				command_argument_value(const std::string& data) : iss(data) {}

				template<class _Ty>
				_Ty as() const
				{
					if constexpr (std::is_same_v<_Ty, std::string>)
						return iss.str();
					else if constexpr (std::is_same_v<_Ty, const char*>)
						return iss.str().c_str();

					_Ty res{};
					if (!(iss >> res))
						return {};

					return res;
				}

				void set(const std::string& data)
				{
					iss = std::istringstream(data);
				}
			private:
				mutable std::istringstream iss{};
			};

			using command_arguments_t = std::unordered_map<std::string, command_argument_value>;
			using command_executor_delegate_t = function<void(const command_arguments_t&)>;

			class command_argument_desc
			{
			public:
				friend class command;
				command_argument_desc& set_description(const std::string& desc)
				{
					description = desc;
					return *this;
				}

				template<class _Ty>
				command_argument_desc& set_default(_Ty data)
				{
					if constexpr (std::is_same_v<_Ty, std::string> || std::is_same_v<_Ty, const char*> || std::is_same_v<_Ty, char*>)
						by_default = std::forward<_Ty>(data);
					else
						by_default = std::to_string(data);

					return *this;
				}

				const std::string& get_default() const { return by_default; }
				const std::string& get_description() const { return description; }

				command& end() { return *owner; }
			private:
				std::string description;
				std::string by_default{};
				command* owner{ nullptr };
			};

			class command
			{
			public:
				friend class command_processor;

				template<class... _Args>
				command& set_executor(_Args&& ...args)
				{
					executor = command_executor_delegate_t(std::forward<_Args>(args)...);
					return *this;
				}

				command_argument_desc& add_argument(const std::string& arg_name)
				{
					auto& new_argument = arguments[arg_name];
					new_argument.owner = this;
					return new_argument;
				}

				command_processor& end() { return *owner; }
			private:
				command_executor_delegate_t executor{};
				std::unordered_map<std::string, command_argument_desc> arguments{};
				command_processor* owner{ nullptr };
			};
		}

		class command_processor
		{
		public:
			detail::command& register_command(const std::string& command_name)
			{
				auto& new_command = command_descriptions[command_name];
				new_command.owner = this;
				return new_command;
			}

			command_processor& register_processor(const std::string& processor_name)
			{
				auto& new_subprocessor = sub_processors[processor_name];
				new_subprocessor.owner = this;
				return new_subprocessor;
			}

			// Command examples:
			// {scope}: {command_name} {argument0}={value} ... {argumentN}={value}
			// render: draw_rect offset_x=0, offset_y=0 size_x=10 size_y=20
			// : get_stats (not all commands can have their's arguments. For example this command should return result after execution).
			void execute(const std::string& command_str)
			{
				command_processor* selected_processor{ this };

				scanner.init(command_str);

				auto cur = scanner.peek();
				while (!scanner.check(detail::parser::command_token_type::eof))
				{
					auto& next = scanner.next();
					if (cur.check(detail::parser::command_token_type::identifier) && next.check(detail::parser::command_token_type::dot))
					{
						if (auto found_proc = selected_processor->sub_processors.find(cur.value); found_proc != selected_processor->sub_processors.end())
						{
							selected_processor = &found_proc->second;
							cur = scanner.next();
						}

						// error. Not found valid sub processor.
					}
					else if (cur.check(detail::parser::command_token_type::identifier) && next.check(detail::parser::command_token_type::left_paren))
					{
						detail::command* cmd{ nullptr };
						if (auto found_command = selected_processor->command_descriptions.find(cur.value); found_command != selected_processor->command_descriptions.end())
							cmd = &found_command->second;

						detail::command_arguments_t command_args{};

						cur = scanner.next();
						while (!scanner.check(detail::parser::command_token_type::right_paren))
						{
							if (scanner.match(detail::parser::command_token_type::identifier))
							{
								if (auto found_arg = cmd->arguments.find(cur.value); found_arg != cmd->arguments.end())
								{
									auto& new_argument = command_args[cur.value];

									if (scanner.match(detail::parser::command_token_type::colon))
									{
										cur = scanner.peek();
										new_argument.set(cur.value);
										cur = scanner.next();
										continue;
									}
								}

								// error. Unknown argument
							}
						}
						cur = scanner.next();

						// Check for defaults
						for (const auto& [name, arg_desc] : cmd->arguments)
						{
							if (auto found_arg = command_args.find(name); found_arg == command_args.end())
								command_args.emplace(name, arg_desc.get_default());
						}

						cmd->executor(command_args);
					}
				}
			}

			command_processor& end() { return *owner; }
		private:
			detail::parser::token_scanner scanner{};
			std::unordered_map<std::string, command_processor> sub_processors{};
			std::unordered_map<std::string, detail::command> command_descriptions{};
			command_processor* owner{ nullptr };
		};
	}
}
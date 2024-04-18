#pragma once

#include "function.h"
#include <type_traits>

namespace utl
{
	namespace reactive
	{
		using event_id = u32;

		class event
		{
		public:
			explicit event(event_id type) : type(type) {}

			template<class _Ty>
			void set_param(event_id id, _Ty&& value) { data[id] = std::forward<_Ty>(value); }

			template<class _Ty>
			_Ty get_param(event_id id) { return std::any_cast<const _Ty&>(data.at(id)); }

			event_id get_type() const { return type; }
		private:
			event_id type{};
			std::unordered_map<event_id, std::any> data{};
		};

		using default_delegate_t = function<void(const std::unique_ptr<event>&)>;

		class event_dispatcher
		{
		public:
			std::unique_ptr<event> make(event_id type) { return std::make_unique<event>(type); }

			void add_listener(event_id id, default_delegate_t const& listener) { listeners[id].emplace_back(listener); }
			void dispatch(const std::unique_ptr<event>& evt) { for (auto& listener : listeners[evt->get_type()]) listener(evt); }
			void dispatch(event_id id) 
			{
				auto evt = make(id);
				for (auto& listener : listeners[id]) listener(evt);
			}

			void unsubscribe(event_id id, const default_delegate_t& listener) 
			{
				auto& listeners_vec = listeners[id];
				listeners_vec.erase(std::remove_if(listeners_vec.begin(), listeners_vec.end(),
					[&listener](const auto& l) { return l == listener; }),
					listeners_vec.end());
			}
		private:
			std::unordered_map<event_id, utl::vector<default_delegate_t>> listeners;
		};
	}
}
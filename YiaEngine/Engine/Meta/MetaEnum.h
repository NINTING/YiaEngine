#pragma once
#ifndef META_ENUM_H
#define META_ENUM_H


#include<array>
#include<unordered_map>

#include"MetaCore.h"

namespace YiaEngine
{
	namespace Meta
	{
		static constexpr std::string_view pretty_name(std::string_view name) noexcept
		{
			for (std::size_t i = name.size(); i > 0; --i) {
				if (!((name[i - 1] >= '0' && name[i - 1] <= '9') ||
					(name[i - 1] >= 'a' && name[i - 1] <= 'z') ||
					(name[i - 1] >= 'A' && name[i - 1] <= 'Z') ||
					(name[i - 1] == '_')))
				{
					name.remove_prefix(i);
					break;
				}
			}
			if (name.size() > 0 && ((name.front() >= 'a' && name.front() <= 'z') ||
				(name.front() >= 'A' && name.front() <= 'Z') ||
				(name.front() == '_'))) {
				return name;
			}
			return {}; // Invalid name.
		}

		template<typename E, typename U = std::underlying_type<E>>
		static constexpr int reflected_min()noexcept
		{
			constexpr int lhs = -128;
			/*constexpr auto rhs = (std::numeric_limits<U>::min)();
			if constexpr (static_cast<int> lhs < static_cast<int> rhs)
			{
				return rhs;
			}*/
			return lhs;
		}

		template<typename E, typename U = std::underlying_type<E>>
		static constexpr int reflected_max()noexcept
		{
			constexpr int rhs = 128;
			/*constexpr auto rhs = (std::numeric_limits<U>::max)();
			if constexpr (static_cast<int> lhs > static_cast<int>  rhs)
			{
				return lhs;
			}*/
			return rhs;
		}

		template <typename E, E V>
		static constexpr std::string_view enum_string_value() noexcept
		{
			auto as = __FUNCSIG__;
			constexpr std::string_view name{ __FUNCSIG__ + 40, sizeof(__FUNCSIG__) - 57 };
			return std::string_view{ pretty_name(std::string_view{ __FUNCSIG__, sizeof(__FUNCSIG__) - 17 }) };

		}

		template <typename E, int O, typename U = std::underlying_type_t<E>>
		static constexpr E value(std::size_t i) noexcept {

			return static_cast<E>(static_cast<U>(i) + O);

		}

		template <typename E, auto V>
		static constexpr bool is_valid() noexcept {
			constexpr auto s = enum_string_value<E, V>();
			return s.size() != 0;
		}

		template<typename T, std::size_t N, std::size_t...I>
		static constexpr std::array<std::remove_cv_t<T>, N> ToArray(T(&a)[N], std::index_sequence<I...>)
		{
			return { {a[I]...} };
		}

		template <std::size_t N>
		static constexpr std::size_t values_count(const bool(&valid)[N]) noexcept {
			auto count = std::size_t{ 0 };
			for (std::size_t i = 0; i < N; ++i) {
				if (valid[i]) {
					++count;
				}
			}

			return count;
		}

		template<typename E, int Min, std::size_t...I>
		static constexpr size_t enum_count(std::index_sequence<I...>)
		{
			constexpr bool valid[sizeof...(I)] = { is_valid<E,value<E, Min>(I)>()... };
			constexpr size_t count = values_count(valid);
			return count;
		}

		template<typename E>
		static constexpr size_t enum_count()
		{
			constexpr int min_index = reflected_min<E>();
			constexpr int max_index = reflected_max<E>();
			constexpr int range_size = max_index - min_index + 1;

			return enum_count<E, min_index>(std::make_index_sequence<range_size>());
		}

		template<typename E, int Min, std::size_t...I>
		static constexpr auto enum_values(std::index_sequence<I...>) noexcept
		{
			constexpr bool valid[sizeof...(I)] = { is_valid<E,value<E, Min>(I)>()... };
			constexpr size_t count = values_count(valid);
			if constexpr (count > 0)
			{
				E vs[count] = {};
				for (size_t i = 0, v = 0; v < count; ++i)
				{
					if (valid[i])
					{
						vs[v++] = value<E, Min>(i);
					}
				}
				return ToArray<E, count>(vs, std::make_index_sequence<count>{});
			}
			else
				return std::array<E, 0>{};
		}

		

		template<typename E>
		static constexpr auto enum_values()
		{
			constexpr int min_index = reflected_min<E>();
			constexpr int max_index = reflected_max<E>();
			constexpr int range_size = max_index - min_index + 1;
			return enum_values<E, min_index>(std::make_index_sequence<range_size>{});
		}
		template<typename E, std::size_t...I>
		static constexpr auto enum_string(std::index_sequence<I...>) noexcept
		{
			return std::array<std::string_view, sizeof...(I)>{enum_string_value<E, enum_values<E>()[I]>()...};
		}
		template<typename E>
		static constexpr auto enum_string() noexcept
		{
			return enum_string<E>(std::make_index_sequence< enum_values<E>().size()>{});
		}

		template<typename E,  typename U = std::underlying_type_t<E>,size_t... I,size_t N>
		static constexpr auto enum_value2index(const std::array<E,N>&values,
			const std::array<std::string_view, N>& names,
			std::index_sequence<I...>)
		{
			return std::unordered_map<U, std::pair<std::string_view, E>>
			{ {static_cast<U>(values[I]), {names[I], values[I]}}...};
		}

		template<typename E, typename U = std::underlying_type_t<E>>
		static constexpr std::unordered_map<U, std::pair<std::string_view, E>> enum_value2index()
		{
			auto values = enum_values<E>();
			auto names = enum_string<E>();
			return enum_value2index(values, names, std::make_index_sequence<values.size()>());
		}
		
		
		template<typename E>
		struct metatt{};
		template<typename E,typename U = std::underlying_type_t<E>>
		struct EnumMeta
		{
			typedef E type;
			static constexpr std::string_view type_name{};
			static constexpr std::unordered_map<std::underlying_type_t<E>, std::pair<std::string_view, E>> datas{};
			static constexpr Core::CategoryTag tag = Core::CategoryTag::kEnumClassTag;
			static constexpr std::unordered_map<U, size_t> value_to_index{};
		};
		template<typename E>
		constexpr std::string_view Serialize(E V)
		{
			for (int i = 0; i < EnumMeta<E>::count; i++)
			{
				if (V == EnumMeta<E>::values[i])
				{
					return EnumMeta<E>::names[i];
				}
			}
		}

	}//Meta
}//YiaEngine

#define META_ENUM(E) \
		template<>	struct YiaEngine::Meta::EnumMeta<E>			\
		{ \
			typedef E type;																	\
			static constexpr std::string_view type_name {#E};								\
			static constexpr size_t count = enum_count<E>();								\
			static constexpr auto values = enum_values<E>();		\
			static constexpr auto names = enum_string<E>();	\
			static constexpr Core::CategoryTag tag = Core::CategoryTag::kEnumClassTag;	\
		};\
	

#endif // !META_ENUM_H
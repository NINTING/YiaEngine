#pragma once

#include<type_traits>
#include<tuple>
namespace YiaEngine::Meta
{

	template<typename type, typename first, size_t first_I>
	constexpr bool contain_type(std::index_sequence<first_I>)
	{
		return std::is_same<type, first>::value;
	}

	template<typename type, typename first, typename... Args, size_t first_I, size_t... I>
	constexpr bool contain_type(std::index_sequence<first_I, I...>)
	{
		return std::is_same<type, first>::value && contain_type<type, Args...>(std::index_sequence<I...>{});
	}
	template<typename type, typename... Args>
	constexpr bool contain_type(std::tuple<Args...>)
	{
		return sizeof...(Args) && contain_type<type, Args...>(std::make_index_sequence<sizeof...(Args)>());
	}
	template<typename type>
	constexpr bool contain_type(std::tuple<>)
	{
		return false;
	}

}
#pragma once

#include<tuple>

#include"MetaHelper.h"

namespace YiaEngine
{
	namespace Meta {
		class Attribute {};
		
		class Serializable :Attribute {};
		

		template<typename T>
		struct is_attribute :std::bool_constant< std::is_base_of<Attribute, T>::value >
		{};
		
		template<typename... Args>
		constexpr auto MakeAttribute(Args... args)
		{
			static_assert((... &&  is_attribute<Args>::value), "must be inherite Attribute");
			return std::make_tuple(args...);
		}

		template<typename attribute, typename T, typename Meta = ClassMetaData<T>>
		bool HasAttribute(T meta)
		{
			return contain_type<Serializable>(Meta::attributes);
		}

		template<typename attribute, typename meta>
		struct has_attribute : std::bool_constant<contain_type<attribute>(meta::attributes)>
		{};
	}//Meta

}//YiaEngine
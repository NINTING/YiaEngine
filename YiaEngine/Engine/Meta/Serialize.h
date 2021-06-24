#pragma once

#include"MetaEnum.h"
#include"MetaClass.h"

namespace YiaEngine
{
	namespace Meta {

		template<typename E>
		std::string Serialize(E V,std::enable_if_t<std::is_enum<E>>*=nullptr)
		{
			for (int i = 0; i < EnumMeta<E>::count; i++)
			{
				if (V == EnumMeta<E>::values[i])
				{
					return EnumMeta<E>::names[i];
				}
			}
		}
		template<typename T, typename member_meta>
		std::string Serialize(T any, member_meta member, int level)
		{

			std::stringstream ss;

			for (int i = 0; i < level * 4; i++)
				ss << " ";

			ss << member_meta::name;
			ss << " : ";
			auto tf = any.*member_meta::pointer;



			ss << any.*member_meta::pointer;
			ss << "\n";
			return ss.str();
		}
		template<typename T>
		std::string Serialize(T any, int level = 0)
		{
			std::stringstream ss;
			if (is_reflectable<T>::value)
			{
				if constexpr (has_attribute<Serializable, is_reflectable<T>::metadata>::value)
				{
					typedef is_reflectable<T>::metadata metadata;
					for (int i = 0; i < level * 4; i++)
						ss << " ";

					ss << metadata::name;
					ss << ":\n";

					ForEachMember<metadata>([&](auto&& member) {
						typedef std::remove_cvref_t<decltype(member)> member_meta_data;

						if constexpr (is_variable<member_meta_data>::value && has_attribute<Serializable, member_meta_data>::value)
						{
							if constexpr (std::is_class_v<typename member_meta_data::value_type>)
							{
								//类类型
								ss << Serialize(any.*member_meta_data::pointer, level + 1);
							}
							else
							{
								auto tf = any.*member_meta_data::pointer;
								//基本类型
								ss << Serialize(any, member, level + 1);
							}
							
						}

						});
				}
			}
			return ss.str();
		}
	}
}
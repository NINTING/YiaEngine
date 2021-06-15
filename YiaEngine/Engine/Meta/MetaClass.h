#pragma once

#include<string_view>
#include<tuple>
namespace YiaEngine
{
	namespace Meta
	{
		template<typename T>
		class ClassMetaData
		{
			static constexpr std::string_view name{};
			static constexpr int member_index_offset = 0;
			template<int member_index>
			class MemberMetaData;
			static constexpr int member_count = 0;
			static constexpr std::tuple<> attributes{};
		};
	}
}

#define BEGIN_META_CLASS(class_name)\
	template<>\
	class ClassMetaData<class_name>	\
	{\
		static constexpr std::string_view name {#class_name};
		static constexpr int member_index_offset = __COUNTER__ + 1;
	}\

#define END_META_CALSS\
	static constexpr size_t member_count{ __COUNTER__ - member_index_offset }; \
	}; }

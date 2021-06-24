#pragma once

#include<string_view>
#include<tuple>


namespace YiaEngine
{
	namespace Meta
	{
		namespace MemberType
		{
			struct function {};
			struct variable {};
		}

		template<typename T>
		struct ClassMetaData
		{
			struct unregist {};
			static constexpr std::string_view name{};
			static constexpr int member_index_offset = 0;
			template<int member_index>
			struct MemberMetaData;
			static constexpr int member_count = 0;
			static constexpr std::tuple<> attributes{};
			using member_type_list = type_list<>;
		};

		/*template<int offset>
		class MemberMetaData
		{
			typedef void member_type;
			static constexpr std::string_view name{};
			static constexpr void* pointer;
			static constexpr std::tuple<> attributes{};
		};*/


		template<typename T>
		std::false_type is_reflectable_test(typename ClassMetaData<T>::unregist* = nullptr)
		{
			return std::false_type{};
		}
		template<typename T>
		std::true_type is_reflectable_test()
		{
			return std::true_type{};
		}
		template<typename T>
		struct  is_reflectable : decltype(is_reflectable_test<T>())
		{
			typedef ClassMetaData<T> metadata;
		};

		template<typename member>
		struct is_function
			:public std::bool_constant<std::is_same_v<typename member::member_type, MemberType::function>> {};
		template<typename member>
		struct is_variable
			:public std::bool_constant<std::is_same_v<typename member::member_type, MemberType::variable>>{};


		template<typename list, typename F>
		void ForEachMember(F&& func, std::index_sequence<>)noexcept
		{
		}
		template<typename list, typename F, size_t first>
		void ForEachMember(F&& func, std::index_sequence<first>)noexcept
		{
			func(front_t<list>{});
		}
		template<typename list, typename F, size_t first, size_t... I>
		void ForEachMember(F&& func, std::index_sequence<first, I...>)noexcept
		{
			func(front_t<list>{});
			ForEachMember<pop_list_t<list>>(func, std::index_sequence<I...>{});
		}
		template<typename class_meta, typename F>
		void ForEachMember(F&& func)noexcept
		{
			ForEachMember<class_meta::member_type_list>(func, std::make_index_sequence<class_meta::member_count>{});
		}


		template<typename class_meta, size_t... I>
		static constexpr auto PushMemberTypeList(std::index_sequence<I...>)
		{
			return type_list<class_meta::template MemberMetaData<I>...>{};
		}
		template<typename class_meta>
		static constexpr auto PushMemberTypeList()
		{
			return PushMemberTypeList<class_meta>(std::make_index_sequence<class_meta::member_count>{});
		}
	}
}

#define BEGIN_META_CLASS(class_name,...)\
	template<>\
	struct YiaEngine::Meta::ClassMetaData<class_name>	\
	{\
		typedef class_name type;\
		static constexpr std::string_view name {#class_name};\
		static constexpr int member_index_offset = __COUNTER__ + 1;\
		static constexpr auto attributes{YiaEngine::Meta::MakeAttribute(__VA_ARGS__)};\
		template<size_t>\
		struct MemberMetaData{};\


#define END_META_CALSS\
		static constexpr size_t member_count{ __COUNTER__ - member_index_offset}; \
		using member_type_list = decltype(PushMemberTypeList<ClassMetaData<type>>());\
	}; 


#define META_MEMBER_VAR(member_name,...)\
	template<>\
	struct MemberMetaData<__COUNTER__ - member_index_offset>\
	{\
		typedef YiaEngine::Meta::MemberType::variable member_type;\
		typedef decltype( type::member_name  ) value_type;\
		static constexpr std::string_view name{ #member_name };\
		static constexpr auto pointer{&type::member_name};\
		static constexpr auto attributes{YiaEngine::Meta::MakeAttribute(__VA_ARGS__)};\
	};\


#define META_MEMBER_FUNC(func_name,...)\
	template<>\
	struct MemberMetaData<__COUNTER__ - member_index_offset>\
	{\
		typedef YiaEngine::Meta::MemberType::function member_type;\
		typedef decltype( &type::func_name) value_type;\
		static constexpr std::string_view name{ #func_name };\
		static constexpr auto pointer{&type::func_name};\
		static constexpr std::tuple<> attributes = YiaEngine::Meta::MakeAttribute(__VA_ARGS__);\
		template<typename Self,typename...Args >\
		static constexpr auto invoke(Self && self,Args&&...args) -> decltype(std::declval<type>().func_name(std::forward<Args>(args)...))\
		{\
			return self.func_name(std::forward<Args>(args)...);\
		}\
		template<typename...Args >\
		static constexpr auto invoke(Args&&...args) -> decltype(type::func_name(std::forward<Args>(args)...))\
		{\
			return type::func_name(std::forward<Args>(args)...); \
		}\
	};\

#define REFLECT_CLASS(class_name)	\
	friend ClassMetaData<class_name>; \

BEGIN_META_CLASS(float)
END_META_CALSS

BEGIN_META_CLASS(int)
END_META_CALSS

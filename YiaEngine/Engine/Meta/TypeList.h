#include<type_traits>
namespace YiaEngine
{
	namespace Meta {
		template<bool cond, typename true_type, typename false_type>
		struct if_then_else
		{
			using type = true_type;
		};

		template<typename true_type, typename false_type>
		struct if_then_else < false, true_type, false_type >
		{
			using type = false_type;
		};

		template<bool cond, typename true_type, typename false_type>
		using if_then_else_t = typename if_then_else<cond, true_type, false_type>::type;

		template<typename... type>
		struct type_list
		{};

		template<typename List>
		struct front;

		template<typename Head, typename...Tail>
		struct front<type_list<Head, Tail...>>
		{
			typedef  Head type;
		};

		template<typename List>
		using front_t = front<List>::type;



		template<typename head, typename List>
		struct push_front_list;

		template<typename head, typename... types>
		struct push_front_list<head, type_list<types...>>
		{
			using type = type_list<head, types...>;
		};

		template<typename head, typename... types>
		using push_front_list_t = push_front_list<head, types...>::type;


		template<typename list>
		struct pop_list;

		template<typename head, typename...trail>
		struct pop_list<type_list<head, trail...>>
		{
			using type = type_list<trail...>;
		};

		template<typename list>
		using pop_list_t = pop_list<list>::type;


		template<typename list, size_t N>
		struct index :index<pop_list_t<list>, N - 1>
		{};


		template<typename list>
		struct index<list, 0> :front_t<list>
		{};

		template<typename list>
		struct is_empty
		{
			static constexpr bool value = false;
		};

		template<>
		struct is_empty<type_list<>>
		{
			static constexpr bool value = true;
		};

	}//Meta
}//YiaEngine

#pragma once

#include <cstdint>

namespace MP 
{
	//=========================================================================
	//====================== TEMPLATE VALUE METAFUNCTIONS =====================
	//=========================================================================

	template <typename T, T VALUE>
	struct constant { static constexpr T value{ VALUE }; };

	//IS_SAME
	//
	struct true_type : constant<bool, true> {};
	struct false_type : constant<bool, false> {};

	template <typename T1, typename T2>
	struct is_same : false_type {};

	template <typename T>
	struct is_same<T, T> : true_type {};

	template<typename T1, typename T2> //Helper
	constexpr bool is_same_v = is_same<T1, T2>::value;

	//=========================================================================

	//=========================================================================
	//====================== TEMPLATE TYPE METAFUNCTIONS ======================
	//=========================================================================

	//POS_TYPE
	//
	template <typename T, typename... Ts>
	struct pos_type
	{ static_assert(sizeof...(Ts) != 0, "ERROR: The list is empty!"); };

	template<typename T, typename... Ts>
	constexpr std::size_t pos_type_v = pos_type<T, Ts...>::value;

	template<typename T, typename... Ts>
	struct pos_type<T, T, Ts...> : constant<std::size_t, 0>{};

	template <typename T, typename U, typename... Ts>
	struct pos_type<T, U, Ts...> : constant <std::size_t, 1 + pos_type_v<T, Ts...>> {};

	//NTH_TYPE
	//
	template<typename T>
	struct type_id
	{
		using type = T;
	};

	template<std::size_t N, typename... Ts>
	struct nth_type
	{ static_assert(sizeof...(Ts) != 0, "ERROR: The List is empty!"); };

	template<std::size_t N, typename...Ts>
	using nth_type_t = typename nth_type<N, Ts...>::type;

	template<typename T, typename... Ts>
	struct nth_type<0, T, Ts...> : type_id<T>{};

	template<std::size_t N, typename T, typename... Ts>
	struct nth_type<N, T, Ts...> : type_id< nth_type<N-1, Ts...> >{};

	//=========================================================================

	//=========================================================================
	//======================= TYPELIST & TRAITS ===============================
	//=========================================================================
		
	//TODO: Las typelist las tengo que definir en un fichero que se llame Types y meter ahi todos los tipos de structs y typelists
	//TYPE_LIST
	//
	template <typename... Ts>
	struct TypeList
	{
		consteval static std::size_t size() noexcept { return sizeof...(Ts); }

		template<typename T>
		consteval static bool contains() noexcept 
		{
			return (false || ... || is_same_v<T, Ts>);
		}

		template<typename T>
		consteval static std::size_t pos() noexcept
		{
			static_assert(contains<T>());
			return pos_type_v<T, Ts...>;
		}
	};

	//=========================================================================

	//TODO: Los traits los voy a usar dentro del EntityManager
	//TAG_TRAITS
	//
	template<typename TAGLIST>
	struct tag_traits
	{
		consteval static std::size_t size() noexcept { return TAGLIST::size(); };

		template <typename TAG>
		consteval static uint8_t id() noexcept { 
			static_assert(TAGLIST::template contains<TAG>()); //We need to explicitly signal that contains is a member template (not a member value nor a member function)
			return TAGLIST::template pos<TAG>(); 
		}

		template<typename TAG>
		consteval static uint8_t mask() noexcept { return (1 << id<TAG>()); }
	};

	//=========================================================================

	//CMP_TRAITS
	//
	template<typename CMPLIST>
	struct cmp_traits : tag_traits<CMPLIST>
	{};

	//=========================================================================
};
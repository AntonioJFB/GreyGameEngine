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
			static_assert(TAGLIST::template contains<TAG>());
			return 1; /*!!!Placeholder*/ 
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
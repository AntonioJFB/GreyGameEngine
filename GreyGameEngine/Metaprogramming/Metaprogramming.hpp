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

	//IFT
	//
	template <bool Cond, typename T, typename F>
	struct IFT : type_id<F>{};

	template<typename T, typename F>
	struct IFT<true, T, F> : type_id<T>{};

	template<bool Cond, typename T, typename F>
	using IFT_t = typename IFT<Cond, T, F>::type;

	//=========================================================================

	//=========================================================================
	//================================ TYPELIST ===============================
	//=========================================================================
		
	//TODO: Las typelist las tengo que definir en un fichero que se llame Types y meter ahi todos los tipos de structs y typelists
	//TYPE_LIST
	//
	template <typename... Ts>
	struct TypeList
	{
		consteval static std::uint8_t size() noexcept { return sizeof...(Ts); }

		template<typename T>
		consteval static bool contains() noexcept 
		{
			return (false || ... || is_same_v<T, Ts>);
		}

		template<typename T>
		consteval static std::uint8_t pos() noexcept
		{
			static_assert(contains<T>());
			return pos_type_v<T, Ts...>;
		}
	};

	//=========================================================================

	//=========================================================================
	//=================== TEMPLATE TEMPLATE PARAMETERS ========================
	//=========================================================================

	//REPLACE
	//
	template<template<typename...> class NewT, typename List> //We need to specify that it now is a class
	struct replace {};

	template<template<typename...> class NewT, typename...Ts>
	struct replace<NewT, TypeList<Ts...>> : type_id<NewT<Ts...>> {};

	template<template<typename...> class NewT, typename List>
	using replace_t = typename replace<NewT, List>::type; //We need to specify that is a typename

	//FORALL INSERT (El de Fran)
	//
	template<template <typename...> class T, typename List>
	struct forall_insert_template{};

	template<template <typename...> class T, typename...Ts>
	struct forall_insert_template<T, TypeList<Ts...>> : type_id<TypeList<T<Ts>...>>{};

	template<template<class...> class T, typename List>
	using forall_insert_template_t = typename forall_insert_template<T, List>::type;

	//============================================================================

	//CONTAINER REPLACE
	//
	//Example: container_replace_t<std::list, std::vector<int, float>> == std::list<int, float>

	template<template<typename...> class NewCont, class OldCont>
	struct replace_container{};

	template<template<typename...> class NewCont, template<typename...> class OldCont, typename...Ts>
	struct replace_container<NewCont, OldCont<Ts...>> : type_id<NewCont<Ts...>>{};

	template<template<typename...> class NewCont, class OldCont>
	using replace_container_t = typename replace_container<NewCont, OldCont>::type;

	//FOR ALL ELEMENTS LIST REPLACE
	//
	/* using CMPs = TypeList<PhyCmp, InputCmp, RenderCmp>
	 * for_all_elements_list_replace_t<std::vector, CMPs> == TypeList<std::vector<PhyCmp>, std::vector<InputCmp>....>
	 */

	template<template<typename...> class T, typename List>
	struct forall_elements_list_insert{};

	template<template<typename...> class T, typename...Ts>
	struct forall_elements_list_insert<T, TypeList<Ts...>> : type_id<TypeList<T<Ts>...>>{};

	template<template<typename...> class T, typename List>
	using forall_elements_list_insert_t = forall_elements_list_insert<T, List>::type;

	//REPLACE LIST
	//
	/*
	* using CMPs = TypeList<PhyCmp, InputCmp, RenderCmp>
	* replace_list_t<std::tuple, CMPs> == std::tuple<PhyCmp, InputCmp, RenderCmp>
	*/

	template<template<typename...> class T, typename List>
	struct replace_list{};

	template<template<typename...> class T, typename... Ts>
	struct replace_list<T, TypeList<Ts...>> : type_id <T<Ts...>>{};

	template<template<typename...> class T, typename List>
	using replace_list_t = replace_list<T, List>::type;

	//REPLACE ALL LIST
	//
	/*
	* using CMPs = TypeList<PhyCmp, InputCmp, RenderCmp>
	* replace_all_list_t<std::tuple, std::vector, CMPs> == std::tuple<std::vector<PhyCmp>, std::vector<InputCmp>...>
	*/

	template<template<typename...> class T1, template<typename...> class T2, typename List>
	struct replace_all_list{};

	template<template<typename...> class T1, template<typename...> class T2, typename... Ts>
	struct replace_all_list<T1, T2, TypeList<Ts...>> : type_id<T1<T2<Ts>...>>{};

	template<template<typename...> class T1, template<typename...> class T2, typename List>
	using replace_all_list_t = replace_all_list<T1, T2, List>::type;

	//ADD TYPES TO LIST
	//

	template<typename List, typename...Ts>
	struct add_types_to_list{};

	template<typename... Ts, typename...Ns>
	struct add_types_to_list<TypeList<Ts...>, Ns...> : type_id<TypeList<Ts..., Ns...>>{};

	template<typename List, typename...Ts>
	using add_types_to_list_t = typename add_types_to_list<List, Ts...>::type;

	//COMBINE TYPELISTS
	//

	template<typename List1, typename List2>
	struct combine_typelists{};

	template<typename... Ts1, typename... Ts2>
	struct combine_typelists<TypeList<Ts1...>, TypeList<Ts2...>> : type_id<TypeList<Ts1..., Ts2...>>{};

	template<typename List1, typename List2>
	using combine_typelists_t = typename combine_typelists<List1, List2>::type;

	//ERASE FIRST TYPE
	//

	template<typename List>
	struct erase_first_type{};

	template<typename T, typename... Ts>
	struct erase_first_type<TypeList<T, Ts...>> : type_id<TypeList<Ts...>>{};

	template<typename List>
	using erase_first_type_t = typename erase_first_type<List>::type;

	//TODO: Si tengo ganas hacer funciones para borrar el ultimo o un elemento exacto

	//=========================================================================

	//=========================================================================
	//================================ TRAITS =================================
	//=========================================================================

	//TODO: Los traits los voy a usar dentro del EntityManager
	//TAG_TRAITS
	//
	template<typename TYPELIST>
	struct common_traits
	{
		using mask_type = 
			IFT_t< TYPELIST::size() <= 8, uint8_t, 
				IFT_t< TYPELIST::size() <= 16, uint16_t, 
					IFT_t < TYPELIST::size() <= 32, uint32_t, 
						uint64_t
					>
				>
			>;

		consteval static std::uint8_t size() noexcept { return TYPELIST::size(); };

		template <typename T>
		consteval static uint8_t id() noexcept { 
			static_assert(TYPELIST::template contains<T>()); //We need to explicitly signal that contains is a member template (not a member value nor a member function)
			return TYPELIST::template pos<T>(); 
		}

		template<typename... Ts>
		consteval static mask_type mask() noexcept { return (0 | ... | (1 << id<Ts>())); }
	};

	//=========================================================================

	//TAG_TRAITS
	//
	template<typename TAGLIST>
	struct tag_traits : common_traits<TAGLIST>
	{};

	//CMP_TRAITS
	//
	template<typename CMPLIST>
	struct cmp_traits : tag_traits<CMPLIST>
	{};

	//=========================================================================
};
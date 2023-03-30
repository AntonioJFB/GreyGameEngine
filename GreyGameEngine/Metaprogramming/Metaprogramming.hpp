#pragma once

#include <cstdint>

namespace MP 
{
	//=========================================================================
	//====================== TEMPLATE VALUE METAFUNCTIONS =====================
	//=========================================================================

	/*
	* @brief Metafunction that has a constant member value of the specified Type
	* @tparam T Type for the constant member value
	* @tparam Value Constant of the T type
	*/
	template <typename T, T VALUE>
	struct constant { static constexpr T value{ VALUE }; };

	//IS_SAME
	//

	/*
	* @brief Metafunction to return a true or false constant value
	*/
	struct true_type : constant<bool, true> {};
	struct false_type : constant<bool, false> {};

	/*
	* @brief Metafunction to see if two tipes are the same or not
	* @tparam T1 First type
	* @tparam T2 Second type
	*/
	template <typename T1, typename T2>
	struct is_same : false_type {};

	template <typename T>
	struct is_same<T, T> : true_type {};

	//Helper
	template<typename T1, typename T2>
	constexpr bool is_same_v = is_same<T1, T2>::value;

	//=========================================================================

	//=========================================================================
	//====================== TEMPLATE TYPE METAFUNCTIONS ======================
	//=========================================================================

	//POS_TYPE
	//

	/*
	* @brief Metafunction to know which position has a certain type inside a list of types
	* @tparam T type to know its position
	* @tparam Ts... List of types to search T position
	*/
	template <typename T, typename... Ts>
	struct pos_type
	{ static_assert(sizeof...(Ts) != 0, "ERROR: The list is empty!"); };

	template<typename T, typename... Ts>
	struct pos_type<T, T, Ts...> : constant<std::size_t, 0>{};
	
	//Helper
	template<typename T, typename... Ts>
	constexpr std::size_t pos_type_v = pos_type<T, Ts...>::value;

	template <typename T, typename U, typename... Ts>
	struct pos_type<T, U, Ts...> : constant <std::size_t, 1 + pos_type_v<T, Ts...>> {};

	//NTH_TYPE
	//

	/*
	* @brief Metafunction that returns the specified Type
	* @tparam T type to be returned
	*/
	template<typename T>
	struct type_struct
	{
		using type = T;
	};

	/*
	* @brief Metafunction to know the nth type of a list of types.
	* @tparam N position to know the type
	* @tparam Ts... List of types to search in
	*/
	template<std::size_t N, typename... Ts>
	struct nth_type
	{ static_assert(sizeof...(Ts) != 0, "ERROR: The List is empty!"); };

	template<typename T, typename... Ts>
	struct nth_type<0, T, Ts...> : type_struct<T>{};

	template<std::size_t N, typename T, typename... Ts>
	struct nth_type<N, T, Ts...> : type_struct< nth_type<N-1, Ts...> >{};
	
	//Helper
	template<std::size_t N, typename...Ts>
	using nth_type_t = typename nth_type<N, Ts...>::type;

	//IFT
	//

	/*
	* @brief Metafunction to choos a type depending on a bool condition
	* @tparam boolean condition
	* @tparam T true type to return if the condition is true
	* @tparam F false type to return if the condition is false
	*/
	template <bool Cond, typename T, typename F>
	struct IFT : type_struct<F>{};

	template<typename T, typename F>
	struct IFT<true, T, F> : type_struct<T>{};

	//Helper
	template<bool Cond, typename T, typename F>
	using IFT_t = typename IFT<Cond, T, F>::type;

	//=========================================================================

	//=========================================================================
	//================================ TYPELIST ===============================
	//=========================================================================
		
	//TODO: Las typelist las tengo que definir en un fichero que se llame Types y meter ahi todos los tipos de structs y typelists
	//TYPE_LIST
	//

	/*
	* @brief struct that encapsulates a list of Types
	* @tparam Ts... Types that will be encapsulated in the TypeList
	*/
	template <typename... Ts>
	struct TypeList
	{
		/*
		* @brief Returns the number of elements in the Typelist
		*/
		consteval static std::uint8_t size() noexcept { return sizeof...(Ts); }

		/*
		* @brief Method that checks if the List has the specified type
		* @tparam T type to be checked if it is in the list
		*/
		template<typename T>
		consteval static bool contains() noexcept 
		{
			return (false || ... || is_same_v<T, Ts>);
		}

		/*
		* @brief Method that returns the position of a specified type inside the typelist
		* @tparam T Type to know its position
		*/
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

	/*
	* @brief Metafunction to replace a list of types with the specified type
	* @tparam NewT new type to encapsulate the types
	* @tparam List TypeList of types
	* Example: replace_t<td::tuple, CMPS> == std::tulpe<PhysCMP, RendCMP, AICMP>
	*/
	template<template<typename...> class NewT, typename List> //We need to specify that it now is a class
	struct replace {};

	template<template<typename...> class NewT, typename...Ts>
	struct replace<NewT, TypeList<Ts...>> : type_struct<NewT<Ts...>> {};

	//Helper
	template<template<typename...> class NewT, typename List>
	using replace_t = typename replace<NewT, List>::type; //We need to specify that is a typename

	//FORALL INSERT (El de Fran)
	//

	/*
	* @brief Metafunction to insert a certain type to each type of the list
	* @tparam T type to be inserted
	* @tparam List TypeList of types that will now be encapsulated by the specified type
	* Example: forall_insert_template_t<SlotMap, CMPS> == TypeList<SlotMap<PhysCMP>, SlotMap<RendCMP>, SlotMap<AICMP>>
	*/
	template<template <typename...> class T, typename List>
	struct forall_insert_template{};

	template<template <typename...> class T, typename...Ts>
	struct forall_insert_template<T, TypeList<Ts...>> : type_struct<TypeList<T<Ts>...>>{};

	//Helper
	template<template<class...> class T, typename List>
	using forall_insert_template_t = typename forall_insert_template<T, List>::type;

	//============================================================================

	//CONTAINER REPLACE
	//

	/*
	* @brief Metafunction to replace a container for another
	* @tparam NewCont New container to encapsulate the elements
	* @tparam OldCont Old container of elemenst
	* Example: replace_container_t<std::list, std::vector<int, float>> == std::list<int, float>
	*/
	template<template<typename...> class NewCont, class OldCont>
	struct replace_container{};

	template<template<typename...> class NewCont, template<typename...> class OldCont, typename...Ts>
	struct replace_container<NewCont, OldCont<Ts...>> : type_struct<NewCont<Ts...>>{};

	//Helper
	template<template<typename...> class NewCont, class OldCont>
	using replace_container_t = typename replace_container<NewCont, OldCont>::type;

	//REPLACE ALL LIST
	//

	/*
	* @brief Metafunction to encapsulate a list of types inside a container and apply another container to each list type
	* @tparam T1 Outer Container
	* @tparam T2 Inner Container
	* Example:
	* using CMPs = TypeList<PhyCmp, InputCmp, RenderCmp>
	* replace_all_list_t<std::tuple, std::vector, CMPs> == std::tuple<std::vector<PhyCmp>, std::vector<InputCmp>...>
	*/

	template<template<typename...> class T1, template<typename...> class T2, typename List>
	struct replace_all_list{};

	template<template<typename...> class T1, template<typename...> class T2, typename... Ts>
	struct replace_all_list<T1, T2, TypeList<Ts...>> : type_struct<T1<T2<Ts>...>>{};

	//Helper
	template<template<typename...> class T1, template<typename...> class T2, typename List>
	using replace_all_list_t = replace_all_list<T1, T2, List>::type;

	//ADD TYPES TO LIST
	//

	/*
	* @brief Metafunction to add new types to a list of types
	* @tparam List TypeList of types
	* @tparam List of types to be added to the List
	* Example: add_types_to_list_t<CMPS, HealthCMP, InventoryCMP> == TypeList<PhysCMP, RendCMP, AICMP, HealthCMP, InventoryCMP>
	*/
	template<typename List, typename...Ts>
	struct add_types_to_list{};

	template<typename... Ts, typename...Ns>
	struct add_types_to_list<TypeList<Ts...>, Ns...> : type_struct<TypeList<Ts..., Ns...>>{};

	//Helper
	template<typename List, typename...Ts>
	using add_types_to_list_t = typename add_types_to_list<List, Ts...>::type;

	//COMBINE TYPELISTS
	//

	/*
	* @brief Metafunction to combien two TypeLists into one
	* @tparam List1 first TypeList
	* @tparam List2 second TypeList
	* Example: 
	* List1 = TypeList<PhysCMP, RendCMP, AICMP>
	* List2 = TypeList<HealthCMP, InventoryCMP>
	* combine_typelists_t<List1, List2> == TypeList<PhysCMP, RendCMP, AICMP, HealthCMP, InventoryCMP>
	*/
	template<typename List1, typename List2>
	struct combine_typelists{};

	template<typename... Ts1, typename... Ts2>
	struct combine_typelists<TypeList<Ts1...>, TypeList<Ts2...>> : type_struct<TypeList<Ts1..., Ts2...>>{};

	//Helper
	template<typename List1, typename List2>
	using combine_typelists_t = typename combine_typelists<List1, List2>::type;

	//ERASE FIRST TYPE
	//

	/*
	* @brief Metafucntion to erase the first type of a TypeList
	* @tparam List TypeList of types
	* Example: 
	* CMPS = TypeList<PhysCMP, RendCMP, AICMP>
	* erase_first_type_t = TypeList<RendCMP, AICMP>
	*/
	template<typename List>
	struct erase_first_type{};

	template<typename T, typename... Ts>
	struct erase_first_type<TypeList<T, Ts...>> : type_struct<TypeList<Ts...>>{};

	//Helper
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

	/*
	* @brief struct that contains the traits of a TypeLists
	* @tparam TYPELIST TypeList with different types
	*/
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

		/*
		* @brief Method that returns the size of a List
		* @return uint8_t Size of the list
		*/
		consteval static std::uint8_t size() noexcept { return TYPELIST::size(); };

		/*
		* @brief Method that returns the id of an element inside of a list based on its position in it.
		* @tparam T type which we want the id
		* @return uint8_t id
		*/
		template <typename T>
		consteval static uint8_t id() noexcept { 
			static_assert(TYPELIST::template contains<T>()); //We need to explicitly signal that contains is a member template (not a member value nor a member function)
			return TYPELIST::template pos<T>(); 
		}

		/*
		* @brief Method that returns the bitmask of the specified types
		* @tparam Ts... parameter pack of types
		* @reutrn The bitmask of this types
		*/
		template<typename... Ts>
		consteval static mask_type mask() noexcept { return (0 | ... | (1 << id<Ts>())); }
	};

	//=========================================================================

	//TAG_TRAITS
	//

	/*
	* @brief Struct that contains the tag_traits of a TAGLIST
	* @tparam TAGLIST list of tags
	*/
	template<typename TAGLIST>
	struct tag_traits : common_traits<TAGLIST>
	{};

	//CMP_TRAITS
	//

	/*
	* @brief Struct that contains the component_traits of a CMPLIST
	* @tparam CMPLIST List of components
	*/
	template<typename CMPLIST>
	struct cmp_traits : tag_traits<CMPLIST>
	{};

	//=========================================================================
};
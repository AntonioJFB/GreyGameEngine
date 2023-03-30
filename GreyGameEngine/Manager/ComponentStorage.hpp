#pragma once

#include <tuple>
#include "../Utils/DataStructures/SlotMap.h"
#include "../Metaprogramming/Metaprogramming.hpp"

namespace GreyGameEngine
{
	template <typename CMPs, size_t Capacity>
	struct ComponentStorage
	{
		using cmp_info = MP::cmp_traits<CMPs>;

	public:
		/*
		*@brief Default constructor of ComponentStorage
		*/
		constexpr explicit ComponentStorage() = default;

		//Making this class non copiable
		//NON_COPIABLE_CLASS(ComponentStorage);

		/*
		*@brief Method to obtain the SlotMap that contains all the components of type CMP
		* @tparam CMP: type of the components you want to get
		* @return SlotMap refrence of components of type CMP
		*/
		template<typename CMP>
		constexpr auto& getComponents() noexcept;

		/*
		* @brief Method to obtain the SlotMap that contains all the components of type CMP
		* @tparam CMP: type of the components you want to get
		* @return SlotMap const refrence of components of type CMP
		*/
		template<typename CMP>
		constexpr auto const& getComponents() const noexcept;

		/*
		* @brief Method to obtain the component of type CMP using the key to access the SlotMap
		* @tparam CMP: type of the component you want to get
		* @param pKey: Key to access the SlotMap and obtain the component
		* @return CMP reference
		*/
		template<typename CMP>
		constexpr CMP& getComponent(auto const pKey) noexcept;

		/*
		* @brief Method to obtain the component of type CMP using the key to access the SlotMap
		* @tparam CMP: type of the component you want to get
		* @param pKey: Key to access the SlotMap and obtain the component
		* @return CMP const reference
		*/
		template<typename CMP>
		constexpr CMP const& getComponent(auto const pKey) const noexcept;

		/*
		* @brief Method to add a component to the ComponentStorage
		* @tparam CMP: type of the component to add
		* @param pCmp: L Reference to the component to add
		* @return Key to access the new component
		*/
		template <typename CMP>
		constexpr auto addComponent(CMP& pCmp) noexcept;

		/*
		* @brief Method to add a component to the ComponentStorage
		* @tparam CMP: type of the component to add
		* @param pCmp: Temporal Reference to the component to add
		* @return Key to access the new component
		*/
		template <typename CMP>
		constexpr auto addComponent(CMP&& pCmp) noexcept;

		/*
		* @brief Method to add a component to the ComponentStorage
		* @tparam CMP: type of the component to add, ParamterTypes...: Types of the parameter pack
		* @param parameters: Parameter pack with the necessary parameters to create the Component
		* @return Key to access the new component
		*/
		template <typename CMP, typename ...ParametersTypes>
		constexpr auto addComponent(ParametersTypes&&...parameters) noexcept;

		/*
		* @brief Method to remove a component from the ComponentStorage
		* @tparam CMP: type of the component to add
		* @param pKey: Key to access the component to remove
		* @return void
		*/
		template <typename CMP>
		constexpr void removeComponent(auto const pKey) noexcept;

		/*
		* @brief Method to remove all the components in ComponentStorage
		* @return void
		*/
		constexpr void clearStorage() noexcept;

		/*
		* @brief Method to remove all the components in a SlotMap of components  of type CMP
		* @tparam CMP: Type of the component
		* @return void
		*/
		template <typename CMP>
		constexpr void clearComponents() noexcept;

	private:
		
		using data_type = cmp_info::mask_type;

		template<typename T>
		using to_slotmap = SlotMap<T, data_type, Capacity>;

		template<typename List>
		using to_tuple = MP::replace_t<std::tuple, List>;

		using storage_t = to_tuple<MP::forall_insert_template_t<to_slotmap, CMPs>>;

		storage_t components_{};

		/*
		* @brief Implementation method to get the components when this is const or not
		* @tparam CMP: Type of the component
		* @param self: Pointer to this to know if it is const or not
		* @return const/or not const reference of SlotMap of components CMP
		*/
		template <typename CMP>
		static constexpr auto& getComponents_impl(auto* self) noexcept;

		/*
		* @brief Implementation method to get a component when this is const or not
		* @tparam CMP: Type of the component
		* @param self: Pointer to this to know if it is const or not
		* @param pKey: Key to access the component
		* @return const/or not const reference to the component
		*/
		template<typename CMP>
		[[nodiscard]] static constexpr auto& getComponent_impl(auto const pKey, auto* self) noexcept;
	};

}
#include "ComponentStorage.tpp"
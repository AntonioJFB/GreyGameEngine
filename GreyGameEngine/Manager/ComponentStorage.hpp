#pragma once

#include <tuple>
#include "../Utils/DataStructures/SlotMap.h"

template <typename CMP1, typename CMP2, typename CMP3, size_t Capacity>
class ComponentStorage
{
public:
	constexpr explicit ComponentStorage() = default;

	//Making this class non copiable
	//NON_COPIABLE_CLASS(ComponentStorage)

	//Funcion para coger un slotmap entero (const y no const --> hacer lo de la impl de Fran)
	template<typename CMP>
	constexpr auto& getComponents() noexcept;
	
	//Funcion para coger un slotmap entero const
	template<typename CMP>
	constexpr auto const& getComponents() const noexcept;

	//Funcion para coger un component
	template<typename CMP>
	constexpr CMP& getComponent(auto const pKey) noexcept;

	//Funcion para coger un component
	template<typename CMP>
	constexpr CMP const& getComponent(auto const pKey) const noexcept;

	//Funcion para añadir un componente pasando uno ya creado
	template <typename CMP>
	constexpr auto addComponent(CMP& pCmp) noexcept;

	//Funcion para añadir un componente pasando una referencia temporal
	template <typename CMP>
	constexpr auto addComponent(CMP&& pCmp) noexcept;

	//Funcion para añadir un componente pasando directamente los parametros
	template <typename CMP, typename ...ParametersTypes>
	constexpr auto addComponent(ParametersTypes&&...parameters) noexcept;

	//Funcion para borrar un component
	template <typename CMP>
	constexpr void removeComponent(auto const pKey) noexcept;

	//Funcion para limpiar el storage completo
	constexpr void clearStorage() noexcept;

	//Funcion para limpiar un solo SlotMap
	template <typename CMP>
	constexpr void clearComponents() noexcept;

private:
	//TODO: Esto es bastante guarro, cuando haga la lista de CMPs tengo que cambiar a hacer un parameter pack 
	//y con la CMPList de metaprogramming
	std::tuple
	<	SlotMap<CMP1, size_t, Capacity>
	,	SlotMap<CMP2, size_t, Capacity>
	,	SlotMap<CMP3, size_t, Capacity>
	> components_{};

	template <typename CMP>
	static constexpr auto& getComponents_impl(auto* self) noexcept;

	template<typename CMP>
	[[nodiscard]] static constexpr auto& getComponent_impl(auto const pKey, auto* self) noexcept;
};

#include "ComponentStorage.tpp"
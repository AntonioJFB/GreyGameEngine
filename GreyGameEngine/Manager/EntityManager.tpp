#include <cassert>
#include <cstdio>

template <typename CMP0, typename CMP1, typename CMP2, typename TAG0, typename TAG1, typename TAG2, size_t Capacity>
constexpr EntityManager<CMP0, CMP1, CMP2, TAG0, TAG1, TAG2, Capacity>::EntityManager(const size_t pCapacity) noexcept
{
	entities_.reserve(pCapacity);
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, typename TAG0, typename TAG1, typename TAG2, size_t Capacity>
constexpr typename EntityManager<CMP0, CMP1, CMP2, TAG0, TAG1, TAG2, Capacity>::Entity_t& 
EntityManager<CMP0, CMP1, CMP2, TAG0, TAG1, TAG2, Capacity>::createEntity() noexcept
{
	return entities_.emplace_back();
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, typename TAG0, typename TAG1, typename TAG2, size_t Capacity>
constexpr auto*
EntityManager<CMP0, CMP1, CMP2, TAG0, TAG1, TAG2, Capacity>::GetEntityByID_impl(auto* self, const auto pID) noexcept
{
	assert(("There aren't entities!", self->entities_.size()));

	decltype(&self->entities_[0]) entity {nullptr};

	for(auto& e : self->entities_)
	{
		if(e.getID() == pID)
			return &e;
	}

	return entity;
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, typename TAG0, typename TAG1, typename TAG2, size_t Capacity>
template <typename CMP>
constexpr CMP& 
EntityManager<CMP0, CMP1, CMP2, TAG0, TAG1, TAG2, Capacity>::addComponent(Entity_t& pEntity, CMP& pComponent) noexcept
{
	return addComponent<CMP>(pEntity, CMP{pComponent});
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, typename TAG0, typename TAG1, typename TAG2, size_t Capacity>
template <typename CMP, typename... ParamTypes>
constexpr CMP&
EntityManager<CMP0, CMP1, CMP2, TAG0, TAG1, TAG2, Capacity>::addComponent(Entity_t& pEntity, ParamTypes&&... pParams) noexcept
{
	return addComponent<CMP>(pEntity, CMP{ std::forward<ParamTypes>(pParams)... });

	//TODO: Otra opcion a parte podria ser hacer como abajo y pasar los params... para la funcion del componentStorage que tmb recibe los params asi
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, typename TAG0, typename TAG1, typename TAG2, size_t Capacity>
template <typename CMP>
constexpr CMP&
EntityManager<CMP0, CMP1, CMP2, TAG0, TAG1, TAG2, Capacity>::addComponent(Entity_t& pEntity, CMP&& pComponent) noexcept
{
	//1.- Deberia comprobar si la entidad ya tiene ese componente
	auto cmpMask = getMask<CMP>();

	if(pEntity.hasComponent(cmpMask))
	{
		//2.- Si lo tiene, deberia devolver el que tiene
		auto key = pEntity.getComponent<CMP>();
		return components_.getComponent<CMP>(key);
	}
	
	//3.- Si no lo tiene deberia crearlo y devolverlo
	auto key = components_.addComponent<CMP>(pComponent);

	pEntity.addComponent<CMP>(key, cmpMask);

	return components_.getComponent<CMP>(key);
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, typename TAG0, typename TAG1, typename TAG2, size_t Capacity>
template <typename CMP>
constexpr void
EntityManager<CMP0, CMP1, CMP2, TAG0, TAG1, TAG2, Capacity>::removeComponent(Entity_t& pEntity) noexcept
{
	auto cmpMask = getMask<CMP>();

	assert(("The entity doesn't have this component!", pEntity.hasComponent(cmpMask)));

	auto key = pEntity.removeComponent<CMP>(cmpMask);
	components_.removeComponent<CMP>(key);
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, typename TAG0, typename TAG1, typename TAG2, size_t Capacity>
template <typename... CMPs>
constexpr void
EntityManager<CMP0, CMP1, CMP2, TAG0, TAG1, TAG2, Capacity>::removeComponents(Entity_t& pEntity) noexcept
{
	(removeComponent<CMPs>(pEntity), ...);
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, typename TAG0, typename TAG1, typename TAG2, size_t Capacity>
template <typename CMP>
constexpr bool
EntityManager<CMP0, CMP1, CMP2, TAG0, TAG1, TAG2, Capacity>::hasComponent(Entity_t& pEntity) const noexcept
{
	auto cmpMask = getMask<CMP>();
	return pEntity.hasComponent(cmpMask);
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, typename TAG0, typename TAG1, typename TAG2, size_t Capacity>
template <typename CMP>
constexpr auto&
EntityManager<CMP0, CMP1, CMP2, TAG0, TAG1, TAG2, Capacity>::getComponent_impl(Entity_t const& pEntity, auto* self) noexcept
{
	auto key = pEntity.getComponent<CMP>();
	return self->components_.getComponent<CMP>(key);
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, typename TAG0, typename TAG1, typename TAG2, size_t Capacity>
template <typename CMP>
constexpr auto&
EntityManager<CMP0, CMP1, CMP2, TAG0, TAG1, TAG2, Capacity>::getComponents_impl(auto* self) noexcept
{
	return self->components_.getComponents<CMP>();
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, typename TAG0, typename TAG1, typename TAG2, size_t Capacity>
template <typename TAG>
constexpr void
EntityManager<CMP0, CMP1, CMP2, TAG0, TAG1, TAG2, Capacity>::addTag(Entity_t& pEntity) noexcept
{
	auto tagMask = getMask<TAG>(); //TODO: Esto cuando meta metaprogramming va a cambiar
	pEntity.addTag(tagMask);
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, typename TAG0, typename TAG1, typename TAG2, size_t Capacity>
template <typename TAG>
constexpr void
EntityManager<CMP0, CMP1, CMP2, TAG0, TAG1, TAG2, Capacity>::removeTag(Entity_t& pEntity) noexcept
{
	auto tagMask = getMask<TAG>();  //TODO: Esto cuando meta metaprogramming va a cambiar
	pEntity.removeTag(tagMask);
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, typename TAG0, typename TAG1, typename TAG2, size_t Capacity>
template <typename TAG>
constexpr bool
EntityManager<CMP0, CMP1, CMP2, TAG0, TAG1, TAG2, Capacity>::hasTag(Entity_t& pEntity) noexcept
{
	auto tagMask = getMask<TAG>();  //TODO: Esto cuando meta metaprogramming va a cambiar
	return pEntity.hasTag(tagMask);

}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, typename TAG0, typename TAG1, typename TAG2, size_t Capacity>
constexpr void
EntityManager<CMP0, CMP1, CMP2, TAG0, TAG1, TAG2, Capacity>::checkDeadEntities() noexcept
{
		bool thereAreDeadEntities {false};
	
		for(auto& e : entities_)
		{
			if (e.getID() == NON_VALID_ENTITY_ID)
			{
				thereAreDeadEntities = true;
				removeComponents<CMP0, CMP1, CMP2>(e); //!!!!!!! TODO: Aqui debo pasarle la lista de componentes que tiene la entidad. Puede ser que no tenga todos los componentes.
			}
		}

		if (thereAreDeadEntities)
			removeDeadEntities();
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, typename TAG0, typename TAG1, typename TAG2, size_t Capacity>
constexpr void
EntityManager<CMP0, CMP1, CMP2, TAG0, TAG1, TAG2, Capacity>::removeDeadEntities() noexcept
{
	entities_.erase(std::remove_if(	entities_.begin(), entities_.end(), 
									[](Entity_t const& e){
										return e.getID() == NON_VALID_ENTITY_ID;
									}), 
									entities_.end());
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, typename TAG0, typename TAG1, typename TAG2, size_t Capacity>
constexpr void 
EntityManager<CMP0, CMP1, CMP2, TAG0, TAG1, TAG2, Capacity>::forAll() noexcept
{
	//SI le pongo una captura, el lambda ya no es convertible a puntero a funcion.
	//Un lambda  es un struct con el operador llamada, si le pongo captura va a tener variables miembro.
	//Si no le pongo captura, el objeto solo tiene la funcion y eso si que es convertible a puntero a funcion.
	//Un struct que lleva variables miembro no lo puedo convertir a funcion. Pero un struct que solo lleva
	//una funcion si qu elo puedo convertir a funcion.

	//TODO: Esto es una implementacion de juguete
	forAll_impl([](Entity_t& e){
		std::cout << "Entity_" << e.getID() << "\n";
		std::cout << "Components: " << e.getComponentsMask() << "\n";
		std::cout << "Tags:" << e.getTagsMask() << "\n";
	});
}

//=============================================================================

template <typename CMP0, typename CMP1, typename CMP2, typename TAG0, typename TAG1, typename TAG2, size_t Capacity>
constexpr void
EntityManager<CMP0, CMP1, CMP2, TAG0, TAG1, TAG2, Capacity>::forAll_impl(TypeProcessFunc process) noexcept
{
	//TODO: Esto es una implementacion de juguete

	for (auto& e : entities_)
		process(e);
}
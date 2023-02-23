template <typename CMP1, typename CMP2, typename CMP3, size_t Capacity>
template<typename CMP>
constexpr auto& ComponentStorage<CMP1, CMP2, CMP3, Capacity>::getComponents() noexcept
{
	return getComponents_impl<CMP>(this);
}

//=============================================================================

template <typename CMP1, typename CMP2, typename CMP3, size_t Capacity>
template<typename CMP>
constexpr auto const& ComponentStorage<CMP1, CMP2, CMP3, Capacity>::getComponents() const noexcept
{
	return getComponents_impl<CMP>(this);
}

//=============================================================================

template <typename CMP1, typename CMP2, typename CMP3, size_t Capacity>
template<typename CMP>
static constexpr auto& ComponentStorage<CMP1, CMP2, CMP3, Capacity>::getComponents_impl(auto* self) noexcept
{
	return std::get<SlotMap<CMP, size_t, Capacity>>(self->components_);
}

//=============================================================================

template <typename CMP1, typename CMP2, typename CMP3, size_t Capacity>
template<typename CMP>
constexpr CMP& ComponentStorage<CMP1, CMP2, CMP3, Capacity>::getComponent(auto const pKey) noexcept
{
	return getComponent_impl<CMP>(pKey, this);

}

//=============================================================================

template <typename CMP1, typename CMP2, typename CMP3, size_t Capacity>
template<typename CMP>
constexpr CMP const& ComponentStorage<CMP1, CMP2, CMP3, Capacity>::getComponent(auto const pKey) const noexcept
{
	return getComponent_impl<CMP>(pKey, this);
}

//=============================================================================

template <typename CMP1, typename CMP2, typename CMP3, size_t Capacity>
template<typename CMP>
static constexpr auto& 
ComponentStorage<CMP1, CMP2, CMP3, Capacity>::getComponent_impl(auto const pKey, auto* self) noexcept
{
	auto& cmps = std::get<SlotMap<CMP, size_t, Capacity>>(self->components_);
	return cmps[pKey];
}

//=============================================================================

template <typename CMP1, typename CMP2, typename CMP3, size_t Capacity>
template<typename CMP>
constexpr auto ComponentStorage<CMP1, CMP2, CMP3, Capacity>::addComponent(CMP& pCMP) noexcept
{
	return addComponent<CMP>(CMP{pCMP});
}

//=============================================================================

template <typename CMP1, typename CMP2, typename CMP3, size_t Capacity>
template<typename CMP>
constexpr auto ComponentStorage<CMP1, CMP2, CMP3, Capacity>::addComponent(CMP&& pCMP) noexcept
{
	auto& cmps = std::get<SlotMap<CMP, size_t, Capacity>>(components_);
	return cmps.push_back(pCMP);
}

//=============================================================================

template <typename CMP1, typename CMP2, typename CMP3, size_t Capacity>
template<typename CMP, typename... ParametersTypes>
constexpr auto ComponentStorage<CMP1, CMP2, CMP3, Capacity>::addComponent(ParametersTypes&&...parameters) noexcept
{
	auto& cmps = std::get<SlotMap<CMP, size_t, Capacity>>(components_);
	return cmps.push_back(CMP{std::forward<ParametersTypes>(parameters)...});
}

//=============================================================================

template <typename CMP1, typename CMP2, typename CMP3, size_t Capacity>
template<typename CMP>
constexpr void ComponentStorage<CMP1, CMP2, CMP3, Capacity>::removeComponent(auto const pKey) noexcept
{
	std::get<SlotMap<CMP, size_t, Capacity>>(components_).erase(pKey);
}

//=============================================================================

template <typename CMP1, typename CMP2, typename CMP3, size_t Capacity>
constexpr void ComponentStorage<CMP1, CMP2, CMP3, Capacity>::clearStorage() noexcept
{ 
	//TODO: Esto es bastante guarro, cuando haga la lista de CMPs tengo que cambiar a hacer un parameter pack 
	//y con la CMPList de metaprogramming
	std::get<SlotMap<CMP1, size_t, Capacity>>(components_).clear();
	std::get<SlotMap<CMP2, size_t, Capacity>>(components_).clear();
	std::get<SlotMap<CMP3, size_t, Capacity>>(components_).clear();
}

//=============================================================================

template <typename CMP1, typename CMP2, typename CMP3, size_t Capacity>
template<typename CMP>
constexpr void ComponentStorage<CMP1, CMP2, CMP3, Capacity>::clearComponents() noexcept
{
	std::get<SlotMap<CMP, size_t, Capacity>>(components_).clear();
}
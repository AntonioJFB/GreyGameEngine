template <typename CMP1, typename CMP2, typename CMP3, size_t Capacity>
template<typename CMP>
constexpr auto& ComponentStorage<CMP1, CMP2, CMP3, Capacity>::getComponents() noexcept
{
	return std::get<SlotMap<CMP, size_t, Capacity>>(components_);
}

//=============================================================================

template <typename CMP1, typename CMP2, typename CMP3, size_t Capacity>
template<typename CMP>
constexpr CMP& ComponentStorage<CMP1, CMP2, CMP3, Capacity>::getComponent(auto pKey) noexcept
{
	auto const& cmps = std::get<SlotMap<CMP, size_t, Capacity>>(components_);
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

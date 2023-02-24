template<typename CMP0, typename CMP1, typename CMP2, size_t Capacity>
template<typename CMP>
constexpr auto Entity<CMP0, CMP1, CMP2, Capacity>::getComponent() const noexcept
{
	return std::get<key_type<CMP>>(key_storage);
}

template<typename CMP0, typename CMP1, typename CMP2, size_t Capacity>
template<typename CMP>
constexpr void Entity<CMP0, CMP1, CMP2, Capacity>::addComponent(auto pSlotKey, auto pCMPId) noexcept
{
	components_ |= pCMPId; //TODO: Cuando lo haga con metaprogramming esto deberia cambiar

	std::get<key_type<CMP>>(key_storage) = pSlotKey; //Aqui puedo usar decltype(pSlotKey) para el template parameter del get
}
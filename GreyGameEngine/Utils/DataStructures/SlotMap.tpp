#include <cassert>

template <typename CMP_T, typename DATA_T, size_t Capacity>
constexpr SlotMap<CMP_T, DATA_T, Capacity>::SlotMap() noexcept
{
	initialize();
}

//=============================================================================

template <typename CMP_T, typename DATA_T, size_t Capacity>
constexpr void SlotMap<CMP_T, DATA_T, Capacity>::initialize() noexcept
{
	freeList_ = 0;
	generation_ = 0;
	size_ = 0;

	for(auto i{0}; i<Capacity; ++i)
	{
		slots_[i].id_ = i + 1;
		slots_[i].gen_ = 0;
	}
}

//=============================================================================

template <typename CMP_T, typename DATA_T, size_t Capacity>
constexpr auto const
SlotMap<CMP_T, DATA_T, Capacity>::allocate() noexcept
{
	assert(("Wrong control of freeList_!", freeList_ < Capacity));

	auto reservedPos = freeList_;

	freeList_ = slots_[freeList_].id_;

	auto& slot = slots_[reservedPos];
	slot.id_ = size_;
	slot.gen_ = generation_;

	++generation_;
	++size_;

	return reservedPos;
}

//=============================================================================

template <typename CMP_T, typename DATA_T, size_t Capacity>
constexpr SlotMap<CMP_T, DATA_T, Capacity>::key_type const
SlotMap<CMP_T, DATA_T, Capacity>::push_back(CMP_T&& pCMP)
{
	auto const reservedPos = allocate();

	auto const& slot = slots_[reservedPos];

	data_[slot.id_] = std::move(pCMP);
	erase_[slot.id_] = reservedPos;

	auto key{slot};
	key.id_ = reservedPos;

	return key;
}

//=============================================================================

template <typename CMP_T, typename DATA_T, size_t Capacity>
constexpr SlotMap<CMP_T, DATA_T, Capacity>::key_type const
SlotMap<CMP_T, DATA_T, Capacity>::push_back(CMP_T const& pCMP)
{
	return push_back(CMP_T{ pCMP });
}

//=============================================================================

template <typename CMP_T, typename DATA_T, size_t Capacity>
constexpr CMP_T const& SlotMap<CMP_T, DATA_T, Capacity>::erase(auto const pKey) noexcept
{
	assert(("The key is not valid!", isValid(pKey)));

	auto& slot = slots_[pKey.id_];

	auto const& retCMP_T = data_[slot.id_];

	if(slot.id_ != size_-1)
		copyLast(slot.id_);

	slot.id_ = freeList_; //The slot.id_ points where the freeList pointed
	
	freeList_ = pKey.id_; //The freeList points to the slot that has been erased

	slot.gen_ = generation_; //Change the generation to invalidate the key

	++generation_;
	--size_;

	return retCMP_T;
}

//=============================================================================

template <typename CMP_T, typename DATA_T, size_t Capacity>
constexpr bool SlotMap<CMP_T, DATA_T, Capacity>::isValid(auto const pKey) noexcept
{
	return !((slots_[pKey.id_].gen_ != pKey.gen_) || (pKey.id_ >= Capacity));
}

//=============================================================================

template <typename CMP_T, typename DATA_T, size_t Capacity>
constexpr void SlotMap<CMP_T, DATA_T, Capacity>::copyLast(DATA_T const pID) noexcept
{
	data_[pID] = data_[size_ - 1];
	erase_[pID] = erase_[size_ - 1];
	slots_[erase_[pID]].id_ = pID;
}

//=============================================================================

template <typename CMP_T, typename DATA_T, size_t Capacity>
constexpr void SlotMap<CMP_T, DATA_T, Capacity>::clear() noexcept
{
	initialize();
}

//=============================================================================

template <typename CMP_T, typename DATA_T, size_t Capacity>
constexpr CMP_T const& SlotMap<CMP_T, DATA_T, Capacity>::operator[](auto pKey) const noexcept
{
	assert(("The key is not valid!", isValid(pKey)));
	
	return data_[slots_[pKey.id_].id_];
}

//=============================================================================

template <typename CMP_T, typename DATA_T, size_t Capacity>
constexpr CMP_T& SlotMap<CMP_T, DATA_T, Capacity>::operator[](auto pKey) noexcept
{
	assert(("The key is not valid!", isValid(pKey)));

	return data_[slots_[pKey.id_].id_];
}

//=============================================================================

template <typename CMP_T, typename DATA_T, size_t Capacity>
constexpr typename SlotMap<CMP_T, DATA_T, Capacity>::iterator 
SlotMap<CMP_T, DATA_T, Capacity>::begin() noexcept
{
	return data_.begin();
}

//=============================================================================

template <typename CMP_T, typename DATA_T, size_t Capacity>
constexpr typename SlotMap<CMP_T, DATA_T, Capacity>::citerator
SlotMap<CMP_T, DATA_T, Capacity>::begin() const noexcept
{
	return data_.begin();
}

//=============================================================================

template <typename CMP_T, typename DATA_T, size_t Capacity>
constexpr typename SlotMap<CMP_T, DATA_T, Capacity>::citerator
SlotMap<CMP_T, DATA_T, Capacity>::cbegin() const noexcept
{
	return data_.cbegin();
}

template <typename CMP_T, typename DATA_T, size_t Capacity>
constexpr typename SlotMap<CMP_T, DATA_T, Capacity>::iterator
SlotMap<CMP_T, DATA_T, Capacity>::end() noexcept
{
	return data_.begin() + size_;
}

//=============================================================================

template <typename CMP_T, typename DATA_T, size_t Capacity>
constexpr typename SlotMap<CMP_T, DATA_T, Capacity>::citerator
SlotMap<CMP_T, DATA_T, Capacity>::end() const noexcept
{
	return data_.begin() + size_;
}

//=============================================================================

template <typename CMP_T, typename DATA_T, size_t Capacity>
constexpr typename SlotMap<CMP_T, DATA_T, Capacity>::citerator
SlotMap<CMP_T, DATA_T, Capacity>::cend() const noexcept
{
	return data_.cbegin() + size_;
}


namespace LibraryShared
{
	template <typename TKey>
	uint32_t HashFunctor<TKey>::operator()(const TKey& key) const
	{
		uint32_t sumOfBytes = 0u;
		const uint8_t* arrayOfBytes = reinterpret_cast<const uint8_t*>(&key);
		for (uint32_t i = 0; i < static_cast<uint32_t>(sizeof(TKey)); ++i)
		{
			sumOfBytes += static_cast<uint32_t>(arrayOfBytes[i]);
		}
		return sumOfBytes;
	}
}
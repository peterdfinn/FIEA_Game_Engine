namespace UnitTest
{
	template <typename TKey>
	uint32_t BadHashFunctor<TKey>::operator()(const TKey& key) const
	{
		key;
		
		return 0xfffffff0;
	}
}
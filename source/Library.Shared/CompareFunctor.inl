namespace LibraryShared
{
	template <typename TKey>
	bool CompareFunctor<TKey>::operator()(const TKey& key1, const TKey& key2) const
	{
		return (key1 == key2);
	}
}
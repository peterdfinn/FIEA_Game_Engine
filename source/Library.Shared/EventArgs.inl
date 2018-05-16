namespace LibraryShared
{
	template <typename T>
	RTTI_DEFINITIONS(EventArgs<T>);

	template <typename T>
	EventArgs<T>::EventArgs(const T& rawMessage) : mRawMessage(rawMessage)
	{}
}
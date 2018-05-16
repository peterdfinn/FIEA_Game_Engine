#pragma once
/// <summary>
/// Author:	Peter Finn
/// Date:	3-14-2018
/// This file contains the header for the templated class Factory.
/// </summary>

#include "HashMap.h"

namespace LibraryShared
{
	/// <summary>
	/// The templated class Factory is the abstract base class from which any
	/// number of concrete factory classes may inherit. It is the top of the
	/// hierarchy in the Abstract Factory design pattern.
	/// </summary>
	template <typename T>
	class Factory
	{
	public:
		/// <summary>
		/// The class Factory::Iterator is an iterator within the templated
		/// class Factory.
		/// </summary>
		class Iterator
		{
			friend class Factory;
		public:
			/// <summary>
			/// This is the default constructor for the Factory::Iterator. It
			/// default-constructs its only data member, a HashMap::Iterator.
			/// </summary>
			Iterator() = default;

			/// <summary>
			/// This is the destructor for the Factory::Iterator. It has nothing
			/// out-of-the-ordinary that it needs to do, so it has been
			/// defaulted.
			/// </summary>
			virtual ~Iterator() = default;

			/// <summary>
			/// This is the copy constructor for the Factory::Iterator. It has
			/// been defaulted; the only thing it needs to do is call the copy
			/// constructor of this class's one and only member.
			/// </summary>
			/// <param name="other">- The Iterator to copy.</param>
			Iterator(const Iterator& other) = default;

			/// <summary>
			/// This is the move constructor for the Factory::Iterator. It has
			/// been defaulted; all that needs to be done is to call the move
			/// constructor of this class's one and only member.
			/// </summary>
			/// <param name="other">- The Iterator to move.</param>
			Iterator(Iterator&& other) = default;

			/// <summary>
			/// This is the copy assignment operator for the Factory::Iterator.
			/// It simply calls the copy assignment operator of this class's
			/// only member, a HashMap::Iterator.
			/// </summary>
			/// <param name="other">- The Iterator to copy.</param>
			/// <returns>A reference to this Iterator, after it has been copied
			/// to.</returns>
			Iterator& operator=(const Iterator& other);

			/// <summary>
			/// This is the move assignment operator for the Factory::Iterator.
			/// It simply calls the move assignment operator of this class's
			/// only member, a HashMap::Iterator.
			/// </summary>
			/// <param name="other">- The Iterator to copy.</param>
			/// <returns>A reference to this Iterator, after it has been moved
			/// into.</returns>
			Iterator& operator=(Iterator&& other);

			/// <summary>
			/// This is an overloading of the prefix increment operator. It
			/// increments the HashMap Iterator this object contains.
			/// </summary>
			/// <returns>A reference to this iterator, after it has been
			/// incremented.</returns>
			Iterator& operator++();

			/// <summary>
			/// This is an overloading of the postfix increment operator. It
			/// increments the HashMap Iterator this object contains.
			/// </summary>
			/// <returns>A reference to this iterator, before it has been
			/// incremented.</returns>
			Iterator operator++(int);

			/// <summary>
			/// This overloading of the dereference operator returns a reference
			/// to a concrete Factory, the one pointed to by the pointer in the
			/// pair in the HashMap that this Iterator refers to. If this
			/// Iterator is the end iterator, an exception is thrown. There is
			/// also a const version of this method that returns a const
			/// reference.
			/// </summary>
			/// <returns>A reference to the Factory pointed to by the HashMap
			/// pair this Iterator refers to.</returns>
			Factory& operator*();

			/// <summary>
			/// See non-const version of this method.
			/// </summary>
			/// <returns></returns>
			const Factory& operator*() const;

			/// <summary>
			/// This overloading of the equality operator returns true if both
			/// iterators refer to the same string-Factory pair in the static
			/// HashMap, or if they are both uninitialized or the end iterator.
			/// </summary>
			/// <param name="other">- The iterator to which to compare this one.
			/// </param>
			/// <returns>True if both iterators are uninitialized, the end
			/// iterator, or referring to the same string-Factory pair; false
			/// otherwise.</returns>
			bool operator==(const Iterator& other) const;

			/// <summary>
			/// This overloading of the inequality operator returns false if both
			/// iterators refer to the same string-Factory pair in the static
			/// HashMap, or if they are both uninitialized or the end iterator.
			/// </summary>
			/// <param name="other">- The iterator to which to compare this one.
			/// </param>
			/// <returns>False if both iterators are uninitialized, the end
			/// iterator, or referring to the same string-Factory pair; true
			/// otherwise.</returns>
			bool operator!=(const Iterator& other) const;

		private:
			explicit Iterator(typename HashMap<std::string, Factory*>::Iterator hashMapIterator);
			typename HashMap<std::string, Factory*>::Iterator mHashMapIterator;
		};

		/// <summary>
		/// This method returns the beginning iterator of a Factory's list of
		/// concrete Factories.
		/// </summary>
		/// <returns>The beginning iterator of this Factory's list of concrete
		/// Factories.</returns>
		static Iterator begin();
		
		/// <summary>
		/// This method returns the end iterator of a Factory's list of concrete
		/// Factories.
		/// </summary>
		/// <returns>The end iterator of this Factory's list of concrete
		/// Factories.</returns>
		static Iterator end();

		/// <summary>
		/// This method attempts to add a concrete factory to this Abstract
		/// Factory. The addition will fail if a concrete factory with the same
		/// type name (same return value of ClassName()) already exists in the
		/// static HashMap.
		/// </summary>
		/// <param name="factoryToAdd">- A reference to the concrete factory to
		/// add to the abstract factory.</param>
		/// <returns>True if the addition succeeded, meaning a concrete factory
		/// of factoryToAdd's type does not already exist in the abstract
		/// factory; false otherwise.</returns>
		static bool Add(Factory& factoryToAdd);

		/// <summary>
		/// This method attempts to remove a concrete factory from the abstract
		/// factory. If a concrete factory with the same type name (i.e., the
		/// same return value of ClassName()) as the input argument exists in
		/// the abstract factory, that concrete factory is removed and this
		/// method returns true; otherwise, nothing happens to the abstract
		/// factory and this method returns false.
		/// </summary>
		/// <param name="factoryToRemove"></param>
		/// <returns></returns>
		static bool Remove(Factory& factoryToRemove);

		/// <summary>
		/// This method returns a pointer to the concrete factory in the
		/// abstract factory's container of concrete factories that has a class
		/// name that matches the input string. If no such concrete factory
		/// exists, nullptr is returned.
		/// </summary>
		/// <param name="className">- The class name of the concrete factory to
		/// find.</param>
		/// <returns>If there is a concrete factory with class name 'className'
		/// in the container of concrete factory, a pointer to it is returned;
		/// otherwise, nullptr is returned.</returns>
		static Factory* Find(const std::string& className);
		
		/// <summary>
		/// This method returns a pointer to a new dynamically allocated object
		/// with the class name 'className', if a factory for that type exists
		/// in this class's static hash map; otherwise, nullptr is returned.
		/// IMPORTANT: if a non-nullptr pointer is returned, it is the
		/// responsibility of the user to then ensure that pointer is deleted.
		/// </summary>
		/// <param name="className">- The class name of the object that should
		/// be created.</param>
		/// <returns>If a factory of the desired class exists in this class's
		/// static HashMap, then a pointer to a newly dynamically allocated
		/// object of that class is returned; otherwise, nullptr is returned.
		/// </returns>
		static T* Create(const std::string& className);

		/// <summary>
		/// This method returns the name of the class that this concrete factory
		/// creates.
		/// </summary>
		/// <returns>The name of the class this concerete factory creates.</returns>
		virtual std::string ClassName() const = 0;

		/// <summary>
		/// This method dynamically allocates memory for a new object of the
		/// class this concrete factory creates. IMPORTANT: if anything other
		/// than nullptr is returned, then the returned pointer must be deleted
		/// at some point. This is the user's responsibility.
		/// </summary>
		/// <returns></returns>
		virtual T* const Create() const = 0;
		
	private:
		static HashMap<std::string, Factory*> mConcreteFactories;
	};

	#define ConcreteFactory(ConcreteProductType, AbstractProductType)							\
	class ConcreteProductType##Factory final : public LibraryShared::Factory<AbstractProductType>	\
	{																							\
	public:																						\
		virtual std::string ClassName() const override									\
		{return #ConcreteProductType;}															\
		virtual AbstractProductType* const Create() const override								\
		{return new ConcreteProductType;}														\
	};
}

#include "Factory.inl"
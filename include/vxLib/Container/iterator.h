#ifndef __VX_ITERATOR_H
#define __VX_ITERATOR_H
#pragma once

#include <iterator>
#include <vxLib/types.h>

namespace vx
{
	template<class Container>
	class array_const_iterator
	{
	public:
		enum { SIZE = Container::SIZE };
		using iterator_category = std::random_access_iterator_tag;

		using _MyContainer = Container;

		using value_type = typename _MyContainer::value_type;
		using reference = typename _MyContainer::const_reference;
		using difference_type = typename _MyContainer::difference_type;
		using pointer = typename _MyContainer::const_pointer;
		using tpointer = typename _MyContainer::pointer;

		using _Unchecked_type = pointer;

	private:
		tpointer m_pObject;
		const Container *m_pCon;

		inline void _compat(const array_const_iterator &other) const
		{
			VX_ASSERT(this->m_pCon == other.m_pCon, "Different containers");
		}

		inline void checkPtr() const
		{
			// lower bound
			VX_ASSERT(m_pCon->data() <= m_pObject &&
				// upper bound
				m_pObject < m_pCon->data() + SIZE,
				"Iterator out of bounds");
		}

	public:
		array_const_iterator()
			:m_pObject(nullptr),
			m_pCon(nullptr)
		{
		}

		array_const_iterator(tpointer ptr, const _MyContainer *pCon)
			:m_pObject(ptr),
			m_pCon(pCon)
		{
		}

		typedef pointer _Unchecked_type;

		array_const_iterator& _Rechecked(_Unchecked_type _Right)
		{	// reset from unchecked iterator
			this->m_pObject = (tpointer)_Right;
			return (*this);
		}

		_Unchecked_type _Unchecked() const
		{	// make an unchecked iterator
			return (_Unchecked_type(this->m_pObject));
		}

		pointer operator->() const
		{
			VX_ASSERT(m_pObject);
			checkPtr();

			return m_pObject;
		}

		reference operator*() const
		{
			VX_ASSERT(m_pObject);
			checkPtr();

			return *m_pObject;
		}

		array_const_iterator& operator++()
		{
			VX_ASSERT(m_pObject < (m_pCon->data() + SIZE), "Iterator out of bounds");

			++this->m_pObject;
			return (*this);
		}

		array_const_iterator operator++(int)
		{
			auto _Tmp = *this;
			++*this;
			return (_Tmp);
		}

		array_const_iterator& operator--()
		{
			VX_ASSERT(m_pObject > m_pCon->data(), "Iterator out of bounds");

			--this->m_pObject;
			return (*this);
		}

		array_const_iterator operator--(int)
		{
			auto _Tmp = *this;
			--*this;
			return (_Tmp);
		}

		array_const_iterator& operator+=(difference_type _Off)
		{
			auto p = m_pObject + _Off;
			VX_ASSERT(p < (m_pCon->data() + SIZE), "Iterator out of bounds");

			m_pObject = p;
			return (*this);
		}

		array_const_iterator& operator-=(difference_type _Off)
		{
			auto p = m_pObject - _Off;
			VX_ASSERT(p >= m_pCon->data(), "Iterator out of bounds");

			m_pObject = p;
			return (*this);
		}

		friend array_const_iterator operator+(const array_const_iterator &lhs, difference_type _Off)
		{	// return this + integer
			auto _Tmp = lhs;
			return (_Tmp += _Off);
		}

		friend array_const_iterator operator-(const array_const_iterator &lhs, difference_type _Off)
		{	// return this + integer
			auto _Tmp = lhs;
			return (_Tmp -= _Off);
		}

		friend difference_type operator-(const array_const_iterator& lhs, const array_const_iterator& rhs)
		{	// return difference of iterators
			lhs._compat(rhs);
			return (lhs.m_pObject - rhs.m_pObject);
		}

		friend difference_type operator+(const array_const_iterator& lhs, const array_const_iterator& rhs)
		{	// return difference of iterators
			lhs._compat(rhs);
			return (lhs.m_pObject + rhs.m_pObject);
		}

		friend bool operator==(const array_const_iterator &lhs, const array_const_iterator &rhs)
		{
			lhs._compat(rhs);
			return (lhs.m_pObject == rhs.m_pObject);
		}

		friend bool operator!=(const array_const_iterator &lhs, const array_const_iterator &rhs)
		{
			lhs._compat(rhs);
			return !(lhs == rhs);
		}

		friend bool operator<(const array_const_iterator &lhs, const array_const_iterator &rhs)
		{
			lhs._compat(rhs);
			return (lhs.m_pObject < rhs.m_pObject);
		}

		friend bool operator<=(const array_const_iterator &lhs, const array_const_iterator &rhs)
		{
			lhs._compat(rhs);
			return (lhs.m_pObject <= rhs.m_pObject);
		}

		friend bool operator>(const array_const_iterator &lhs, const array_const_iterator &rhs)
		{
			lhs._compat(rhs);
			return (lhs.m_pObject > rhs.m_pObject);
		}

		friend bool operator>=(const array_const_iterator &lhs, const array_const_iterator &rhs)
		{
			lhs._compat(rhs);
			return (lhs.m_pObject >= rhs.m_pObject);
		}
	};

	template<class Container>
	class array_iterator : public array_const_iterator < Container >
	{
		typedef array_const_iterator<Container>		_MyBase;

	public:
		typedef typename _MyContainer::value_type		value_type;
		typedef typename _MyContainer::difference_type	difference_type;
		typedef typename _MyContainer::reference		reference;
		typedef typename _MyContainer::pointer			pointer;

	private:
		typedef array_iterator<Container>			_MyIter;

	public:
		array_iterator()
			:_MyBase()
		{
		}

		array_iterator(pointer obj, const _MyContainer *pPool)
			:_MyBase(obj, pPool)
		{
		}

		typedef pointer _Unchecked_type;

		_MyIter& _Rechecked(_Unchecked_type _Right)
		{	// reset from unchecked iterator
			this->m_pObject = _Right;
			return (*this);
		}

		_Unchecked_type _Unchecked() const
		{	// make an unchecked iterator
			return (_Unchecked_type(this->m_pObject));
		}

		pointer operator->() const
		{
			return (_STD pointer_traits<pointer>::pointer_to(**this));
		}

		reference operator*() const
		{
			return _MyBase::operator*();
		}

		_MyIter& operator++()
		{
			++*(_MyBase *)this;
			return (*this);
		}

		_MyIter operator++(int)
		{
			auto _Tmp = *this;
			++*this;
			return (_Tmp);
		}

		_MyIter& operator--()
		{
			--*(_MyBase *)this;
			return (*this);
		}

		_MyIter operator--(int)
		{
			auto _Tmp = *this;
			--*this;
			return (_Tmp);
		}

		_MyIter& operator+=(difference_type _Off)
		{
			*(_MyBase *)this += _Off;
			return (*this);
		}

		_MyIter& operator-=(difference_type _Off)
		{
			*(_MyBase *)this -= _Off;
			return (*this);
		}

		friend _MyIter operator+(const _MyIter &lhs, difference_type _Off)
		{	// return this + integer
			auto _Tmp = lhs;
			return (_Tmp += _Off);
		}

		friend _MyIter operator-(const _MyIter &lhs, difference_type _Off)
		{	// return this + integer
			auto _Tmp = lhs;
			return (_Tmp -= _Off);
		}
	};

	template<class Container>
	class vector_const_iterator
	{
	public:
		using iterator_category = std::random_access_iterator_tag;

		using _MyContainer = Container;

		using value_type = typename _MyContainer::value_type;
		using reference = typename _MyContainer::const_reference;
		using difference_type = typename _MyContainer::difference_type;
		using pointer = typename _MyContainer::const_pointer;
		using tpointer = typename _MyContainer::pointer;

		using _Unchecked_type = pointer;

		using _MyIter = vector_const_iterator<Container>;

	private:
		friend _MyContainer;

		tpointer m_pObject;
		const Container *m_pCon;

		inline void _compat(const _MyIter &other) const
		{
			VX_ASSERT(this->m_pCon == other.m_pCon, "Different containers");
		}

		inline void validate() const
		{
			VX_ASSERT((this->m_pCon->begin() <= *this) && (*this < this->m_pCon->end()), "Iterator invalid");
		}

	public:
		vector_const_iterator()
			:m_pObject(nullptr),
			m_pCon(nullptr)
		{
		}

		vector_const_iterator(tpointer ptr, const _MyContainer *pCon)
			:m_pObject(ptr),
			m_pCon(pCon)
		{
		}

		typedef pointer _Unchecked_type;

		_MyIter& _Rechecked(_Unchecked_type _Right)
		{	// reset from unchecked iterator
			this->m_pObject = (tpointer)_Right;
			return (*this);
		}

		_Unchecked_type _Unchecked() const
		{	// make an unchecked iterator
			return (_Unchecked_type(this->m_pObject));
		}

		pointer operator->() const
		{
			VX_ASSERT(m_pObject, "No object");
			validate();

			return m_pObject;
		}

		reference operator*() const
		{
			VX_ASSERT(m_pObject, "No object");
			validate();

			return *m_pObject;
		}

		vector_const_iterator& operator++()
		{
			++this->m_pObject;
			return (*this);
		}

		vector_const_iterator operator++(int)
		{
			auto _Tmp = *this;
			++*this;
			return (_Tmp);
		}

		_MyIter& operator--()
		{
			--this->m_pObject;
			return (*this);
		}

		_MyIter operator--(int)
		{
			auto _Tmp = *this;
			--*this;
			return (_Tmp);
		}

		_MyIter& operator+=(difference_type _Off)
		{
			auto p = m_pObject + _Off;

			m_pObject = p;
			return (*this);
		}

		_MyIter& operator-=(difference_type _Off)
		{
			auto p = m_pObject - _Off;

			m_pObject = p;
			return (*this);
		}

		friend _MyIter operator+(const _MyIter &lhs, difference_type _Off)
		{	// return this + integer
			auto _Tmp = lhs;
			return (_Tmp += _Off);
		}

		friend _MyIter operator-(const _MyIter &lhs, difference_type _Off)
		{	// return this + integer
			auto _Tmp = lhs;
			return (_Tmp -= _Off);
		}

		friend difference_type operator-(const _MyIter& lhs, const _MyIter& rhs)
		{	// return difference of iterators
			lhs._compat(rhs);
			return (lhs.m_pObject - rhs.m_pObject);
		}

		friend difference_type operator+(const _MyIter& lhs, const _MyIter& rhs)
		{	// return difference of iterators
			lhs._compat(rhs);
			return (lhs.m_pObject + rhs.m_pObject);
		}

		friend bool operator==(const _MyIter &lhs, const _MyIter &rhs)
		{
			lhs._compat(rhs);
			return (lhs.m_pObject == rhs.m_pObject);
		}

		friend bool operator!=(const _MyIter &lhs, const _MyIter &rhs)
		{
			lhs._compat(rhs);
			return !(lhs == rhs);
		}

		friend bool operator<(const _MyIter &lhs, const _MyIter &rhs)
		{
			lhs._compat(rhs);
			return (lhs.m_pObject < rhs.m_pObject);
		}

		friend bool operator<=(const _MyIter &lhs, const _MyIter &rhs)
		{
			lhs._compat(rhs);
			return (lhs.m_pObject <= rhs.m_pObject);
		}

		friend bool operator>(const _MyIter &lhs, const _MyIter &rhs)
		{
			lhs._compat(rhs);
			return (lhs.m_pObject > rhs.m_pObject);
		}

		friend bool operator>=(const _MyIter &lhs, const _MyIter &rhs)
		{
			lhs._compat(rhs);
			return (lhs.m_pObject >= rhs.m_pObject);
		}
	};

	template<class Container>
	class vector_iterator : public vector_const_iterator < Container >
	{
		typedef vector_const_iterator<Container>		_MyBase;

	public:
		typedef typename _MyContainer::value_type		value_type;
		typedef typename _MyContainer::difference_type	difference_type;
		typedef typename _MyContainer::reference		reference;
		typedef typename _MyContainer::pointer			pointer;

	private:
		typedef vector_iterator<Container>			_MyIter;

	public:
		vector_iterator()
			:_MyBase()
		{
		}

		vector_iterator(pointer obj, const _MyContainer *pPool)
			:_MyBase(obj, pPool)
		{
		}

		typedef pointer _Unchecked_type;

		_MyIter& _Rechecked(_Unchecked_type _Right)
		{	// reset from unchecked iterator
			this->m_pObject = _Right;
			return (*this);
		}

		_Unchecked_type _Unchecked() const
		{	// make an unchecked iterator
			return (_Unchecked_type(this->m_pObject));
		}

		pointer operator->() const
		{
			return (_STD pointer_traits<pointer>::pointer_to(**this));
		}

		reference operator*() const
		{
			return _MyBase::operator*();
		}

		_MyIter& operator++()
		{
			++*(_MyBase *)this;
			return (*this);
		}

		_MyIter operator++(int)
		{
			auto _Tmp = *this;
			++*this;
			return (_Tmp);
		}

		_MyIter& operator--()
		{
			--*(_MyBase *)this;
			return (*this);
		}

		_MyIter operator--(int)
		{
			auto _Tmp = *this;
			--*this;
			return (_Tmp);
		}

		_MyIter& operator+=(difference_type _Off)
		{
			*(_MyBase *)this += _Off;
			return (*this);
		}

		_MyIter& operator-=(difference_type _Off)
		{
			*(_MyBase *)this -= _Off;
			return (*this);
		}

		friend _MyIter operator+(const _MyIter &lhs, difference_type _Off)
		{	// return this + integer
			auto _Tmp = lhs;
			return (_Tmp += _Off);
		}

		friend _MyIter operator-(const _MyIter &lhs, difference_type _Off)
		{	// return this + integer
			auto _Tmp = lhs;
			return (_Tmp -= _Off);
		}
	};
}
#endif
#pragma once
/*
The MIT License (MIT)

Copyright (c) 2015 Dennis Wandschura

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <iterator>
#include <vxLib/types.h>

namespace vx
{
	template<typename Container>
	class array_const_iterator
	{
	public:
		enum { SIZE = Container::SIZE };
		typedef std::random_access_iterator_tag iterator_category;

		typedef Container _MyContainer;

		typedef typename _MyContainer::value_type value_type;
		typedef typename _MyContainer::const_reference reference;
		typedef typename _MyContainer::difference_type difference_type;
		typedef typename _MyContainer::const_pointer pointer;
		typedef typename _MyContainer::pointer tpointer;

		typedef pointer _Unchecked_type;

	protected:
		tpointer m_pObject;
		const Container *m_pCon;

		inline void _compat(const array_const_iterator &other) const
		{
			VX_ASSERT(this->m_pCon == other.m_pCon);
		}

		inline void checkPtr() const
		{
			// lower bound
			VX_ASSERT(m_pCon->data() <= m_pObject &&
				// upper bound
				m_pObject < m_pCon->data() + SIZE);
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
			VX_ASSERT(m_pObject < (m_pCon->data() + SIZE));

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
			VX_ASSERT(m_pObject > m_pCon->data());

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
			VX_ASSERT(p < (m_pCon->data() + SIZE));

			m_pObject = p;
			return (*this);
		}

		array_const_iterator& operator-=(difference_type _Off)
		{
			auto p = m_pObject - _Off;
			VX_ASSERT(p >= m_pCon->data());

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

	template<typename Container>
	class array_iterator : public array_const_iterator < Container >
	{
		typedef array_const_iterator<Container>		_MyBase;

	public:
		typedef typename Container::value_type		value_type;
		typedef typename Container::difference_type	difference_type;
		typedef typename Container::reference		reference;
		typedef typename Container::pointer			pointer;

	private:
		typedef array_iterator<Container>			_MyIter;

	public:
		array_iterator()
			:_MyBase()
		{
		}

		array_iterator(pointer obj, const Container *pPool)
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
			return (pointer)this->m_pObject;
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

	template<typename Container>
	class vector_const_iterator
	{
	public:
		typedef std::random_access_iterator_tag iterator_category;

		typedef Container _MyContainer;

		typedef typename _MyContainer::value_type value_type;
		typedef typename _MyContainer::const_reference reference;
		typedef typename _MyContainer::difference_type difference_type;
		typedef typename _MyContainer::const_pointer pointer;
		typedef typename _MyContainer::pointer  tpointer;

		typedef  pointer _Unchecked_type;

		typedef vector_const_iterator<Container> _MyIter;

	protected:
		friend Container;

		tpointer m_pObject;
		const Container *m_pCon;

		inline void _compat(const _MyIter &other) const
		{
			VX_ASSERT(this->m_pCon == other.m_pCon);
		}

		inline void validate() const
		{
			VX_ASSERT((this->m_pCon->begin() <= *this) && (*this < this->m_pCon->end()));
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
			VX_ASSERT(m_pObject);
			validate();

			return m_pObject;
		}

		reference operator*() const
		{
			VX_ASSERT(m_pObject);
			validate();

			return (*this->m_pObject);
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
		typedef typename _MyBase::_MyContainer _MyContainer;

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
			return (this->m_pObject);
		}

		reference operator*() const
		{
			return *(this->m_pObject);
		}

		_MyIter& operator++()
		{
			++(*(_MyBase*)this);
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
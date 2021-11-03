/*
fneotensor - Tensor manipulation program
Copyright (C) 2021 lieu02tard

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "tensor_unitdim_dec.h"
#include <stdlib.h>
#include <string.h>
#ifndef LIBTENSOR_UNIT_DIMENSION_IMPLEMENTATION
#define LIBTENSOR_UNIT_DIMENSION_IMPLEMENTATION
namespace libtensor_namespace
{
	
	template<typename T>
	void tensor<1,T>::free_all()
	{
		if (this->_is_init_yet == true)
			if (this->p != nullptr)
				free(this->p);
	}
	
	
	template<typename T>
	void tensor<1,T>::tensor_init_empty()
	{
		free_all();
		lock_usable();
	}

	template<typename T>
	void tensor<1,T>::tensor_init_null()
	{
		tensor_init_empty();
		this->_is_null		= true;
	}

	template<typename T>
	bool tensor<1,T>::_inner_alloc(int n)
	{
		if ( n > 0 )
		{
			this->self_size 	= n;
			this->p			= (T*) malloc( sizeof(T) * n );
			turn_usable();
			return p != nullptr;
		}
		else
		{
			tensor_init_empty();
			return false;
		}

	}

	template<typename T>
	bool tensor<1,T>::_content_inner_alloc(T* values, int size)
	{
		if (size != this->self_size)
		{
			free_all();
			_inner_alloc(size);
		}
		if (this->p == nullptr)
		{
			lock_usable();
			return false;
		}	
		memcpy(this->p, values, sizeof(T)*size);
		if (this->p != nullptr)
			return true;
		else
			return false;
	}

	template<typename T>
	bool tensor<1,T>::init(T* values,int* size)
	{
		tensor_init_empty();
		bool _result = _content_inner_alloc(values, *size);
		turn_usable();
		return _result;
	}

	template<typename T>
	bool tensor<1,T>::init(int* size)
	{
		tensor_init_empty();
		bool _result =  _inner_alloc(size);
		turn_usable();
		return _result;
	}

	template<typename T>
	bool tensor<1,T>::init_vector(int size)
	{
		tensor_init_empty();
		bool _result = _inner_alloc(size);
		turn_usable();
		return _result;
	}

	template<typename T>
	bool tensor<1,T>::init_vector(T* values, int size)
	{
		tensor_init_empty();
		bool _result =  _content_inner_alloc(values, size);
		turn_usable();
		return _result;
	}

	template<typename T>
	bool tensor<1,T>::ft_empty_init()
	{
		this->self_size 	= 0;
		this->p			= nullptr;
		this->_is_init_yet	= false;
		this->_is_null		= false;
		this->_is_child 	= false;
		return true;
	}

	template<typename T>
	bool tensor<1,T>::ft_init(T* values, int n)
	{
		ft_empty_init();
		bool _result = _content_inner_alloc(values, n);
		turn_usable();
		return _result;
	}

	template<typename T>
	bool tensor<1,T>::ft_init(int* pn)
	{
		ft_empty_init();
		bool _result = _inner_alloc(*pn);
		turn_usable();
		return _result;
	}

	template<typename T>
	bool tensor<1,T>::_subtensor_copy(const tensor<1,T>& crt)
	{
		for (int i = 0; i < this->self_size; i++)
			this->p[i] = crt.p[i];
		return true;
	}

	template<typename T>
	bool tensor<1,T>::size_compare(int size)
	{
		return size == this->self_size;
	}

	template<typename T>
	bool tensor<1,T>::size_compare(int* psize)
	{
		return size_compare(*psize); 
	}

	template<typename T>
	void tensor<1,T>::turn_usable()
	{
		this->_is_init_yet	= true;
		this->_is_null		= false;
		this->_is_child		= false;
	}

	template<typename T>
	void tensor<1,T>::lock_usable()
	{
		this->_is_init_yet	= false;
		this->_is_null		= false;
		this->_is_child		= false;
	}

	
	template<typename T>
	bool tensor<1,T>::copy_size(int psize)
	{
		this->self_size = psize;
		return true;
	}

	template<typename T>
	tensor<1,T>::tensor ( int n ) 
	{
		ft_empty_init();
	}

	//Not recommend
	template<typename T>
	tensor<1,T>::tensor(T* values, int n) 
	{
		ft_init(values, n);
	}

	template<typename T>
	tensor<1,T>::tensor(int n , ...)
	{
		ft_empty_init();
		va_list ap;
		va_start(ap, n);
		_inner_alloc(n);
		for (int i = 0; i < n; i++)
			 this->p[i] = va_arg(ap, T);
	}

	template<typename T>
	tensor<1,T>::tensor( const tensor<1,T>& rt )
	{
		if (this == &rt)
			return (*this);
		ft_empty_init();
		if (_inner_alloc(rt.self_size) == false)
		{
			//Tag error
			return;
		}
		if (_subtensor_copy(rt) == false)
		{
			//Raise error or somewhat
			tensor_init_empty();
		}
		turn_usable();
	}

	template<typename T>
	tensor<1,T>& tensor<1,T>::operator= (const tensor<1,T>& crt)
	{
		if (this == &crt)
			return (*this);
		if (size_compare(crt.self_size) == false)
			_finner_alloc(crt.self_size);
		_content_inner_alloc(crt.p);
		return *this;
	}
	
	template<typename T>
	tensor<1,T>::tensor( tensor<1,T>&& t )
	{
		if (this == &rrt)
			return (*this);
		swap(t, this);
		t.ft_empty_init();
	}

	template<typename T>
	tensor<1,T>& tensor<1,T>::operator= (tensor<1,T>&& rrt)
	{
		if (this == &rrt)
			return (*this);
		swap(rrt, this);
		return *this;
	}

	template<typename T>
	char tensor<1,T>::resize( int n )
	{
		if (this->_is_child == false)
		{
			this->p = (T*) realloc(p, sizeof(T)*n);
			if (p != nullptr)
				return 0;
			else 
				return -1;
		}
		else
			return 0;
	}

	template<typename T>
	T& tensor<1,T>::at(int n)
	{
		if ((n<0) || (n>=self_size))
		{
			T temp = {};
			return temp;
		}
		return this->p[n];
	}

	template<typename T>
	T& tensor<1,T>::operator [] ( int n )
	{
		return at(n);
	}

	template<typename T>
	tensor<1,T> tensor<1,T>::operator + 
	( const tensor<1,T>& v )
	{
		if ( size_compare(v.self_size) == false )
		{
			tensor<1,T> temp = {};
			return temp;
		}
		tensor<1,T> _result {*this};
		for (int i = 0; i < this->self_size; i++)
			_result[i] += v[i];
		return _result;
		
	}

	template<typename T>
	tensor<1,T>& tensor<1,T>::operator += 
	( const tensor<1,T>& v )
	{
		if ( size_compare(v.self_size) == false )
		{
			tensor<1,T> temp = {};
			return temp;
		}
		for ( int i = 0; i < this->self_size; i++)
			this->at(i) += v[i];
		return *this;
	}

	template<typename T>
	tensor<1,T> tensor<1,T>::operator- ()
	{
		tensor<1,T> _result{ this->self_size };
		for ( int i = 0; i < this->size; i++ )
			_result[i] = - this->at(i);
		return _result;
	}

	template<typename T>
	tensor<1,T> tensor<1,T>::operator - 
	( const tensor<1,T>& v )
	{
		if (size_compare(v.self_size) == false)
		{
			tensor<1,T> temp = {};
			return temp;
		}
		tensor<1,T> res{*this};
		for (int i = 0; i < this->self_size; i++)
			res[i] += v[i];
		return res;
	}

	template<typename T>
	tensor<1,T>& tensor<1,T>::operator -=
	( const tensor<1,T>& v )
	{
		if (size_compare(v.self_size) == false)
		{
			tensor<1,T> temp = {};
			return temp;
		}
		for ( int i =0; i < this->self_size; i++ )
			this->at(i) -= v[i];
		return *this;
	}

	template<typename T>
	bool tensor<1,T>::operator ==
	(const tensor<1,T>& v)
	{
		if (size_compare(v.self_size) == false)
			return false;
		for (int i = 0; i < this->self_size; i++)
			if (v.p[i] != this->p[i])
				return false;
		return true;
	}

	template<typename T>
	bool tensor<1,T>::operator!=
	(const tensor<1,T>& v)
	{
		if (size_compare(v.self_size) == false)
			return true;
		for (int i = 0; i < this->self_size; i++)
			if (v.p[i] != this->p[i])
				return true;
		return false;
	}

	template<typename T>
	int tensor<1,T>::get_size(){ return this->size; }
	
	template<typename T>
	T* tensor<1,T>::get_data(){return this->p;}

	template<typename T>
	tensor<1,T>::operator bool ()
	{return is_usable();}
#ifdef LIBTENSOR_TENSOR_ITERATOR
	//Remember to remove tensor<1,T>::tensor_iterator with
	//"tensor<1,T>::tensor_iterator"
	template<typename T>
	tensor<1,T>::tensor_iterator::tensor_iterator()
	{this->p = nullptr;}

	template<typename T>
	tensor<1,T>::tensor_iterator::tensor_iterator(const tensor_iterator& cri)
	{this->p = cri.p;}

	template<typename T>
	typename tensor<1,T>::tensor_iterator& tensor<1,T>::tensor_iterator::operator= (const tensor_iterator& cri)
	{this->p = cri.p; return *this;}

	template<typename T>
	tensor<1,T>::tensor_iterator::tensor_iterator(tensor_iterator&& rri)
	{swap(rri, this);rri.clear();}
	
	template<typename T>
	typename tensor<1,T>::tensor_iterator& tensor<1,T>::tensor_iterator::operator= (tensor_iterator&& rri)
	{swap(rri, this); rri.clear(); return *this;}

	template<typename T>
	tensor<1,T>::tensor_iterator::tensor_iterator(T* pd)
	{this->p = pd;}
	
	template<typename T>
	tensor<1,T>::tensor_iterator::tensor_iterator(T& pd)
	{this->p = &pd;}
#endif

	template<typename T> 
	void tensor<1,T>::clear()
	{
		tensor_init_empty();
	}

	template<typename T> 
	tensor<1,T>::~tensor()
	{
		clear();
	}	
}
#endif	

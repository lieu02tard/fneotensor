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

#include "tensor_declare.h"
#include <string.h>
#include "swap.h"
#ifndef LIBTENSOR_TENSOR_HEAD_IMP
#define LIBTENSOR_TENSOR_HEAD_IMP
//Remeber to replace the [tmp] flag
//to "template<size_t dimension, typename T>"
namespace libtensor_namespace
{
	//Virtually deinitialize
	template<size_t dimension, typename T>
	void tensor<dimension, T>::tensor_init_empty()
	{
		if (this->_is_init_yet == true)
			for (size_t i = 0; i < *(this->self_size); i++)
				this->p[i].tensor_init_empty();
		free_all();
		lock_usable();
	}
	template<size_t dimension, typename T >
	void tensor<dimension, T>::tensor_init_null()
	{
		tensor_init_empty();
		this->_is_null 		= false;
	}
	//This is not to turn the object usable
	//Just to modding the inner tensor
	//Refer to this only after you has done something meaningful
	template<size_t dimension, typename T>
	void tensor<dimension, T>::turn_usable()
	{
		this->_is_init_yet	= true;
		this->_is_null		= false;
	}
	
	template<size_t dimension, typename T>
	void tensor<dimension, T>::lock_usable()
	{
		this->_is_init_yet	= false;
		this->_is_null		= false;
	}
	//This should be call after you have deinitialize all  of array and subarray
	template<size_t dimension, typename T>
	void tensor<dimension, T>::free_all()
	{
		if (this->_is_init_yet == true)
			if (this->p != nullptr)
				free(this->p);
	}
	
	template<size_t dimension, typename T>
	bool tensor<dimension, T>::_inner_alloc(size_t d)
	{
		if (d < 0)
			return false;
		this->p			=  (tensor<dimension - 1, T>*) calloc
					(sizeof(tensor<dimension - 1, T>), d);
		for (size_t i = 0; i < d; i++)
			p->set_subtensor();

		if (this->p == nullptr)
			return false;
		this->self_size		= (size_t*) calloc(sizeof(int), dimension);
		if (this->self_size == nullptr)
			return false;
		*self_size		= d;
		this->_is_init_yet	= true;
		this->_is_null		= false;
		return true;
	}
	//Make sure the self_size and p posize_ters are freed before call this
	//Or memory leak may happen
	template<size_t dimension, typename T>
	bool tensor<dimension, T>::_inner_alloc(size_t* size)
	{
		
		this->p			= (tensor<dimension - 1, T>*) calloc
					(sizeof(tensor<dimension - 1, T>), *size);
		if (this->p ==nullptr)
			return false;
		//this->self_size		= (size_t*) calloc (sizeof(int), dimension); [DEPCRATED]
		copy_size(size);
		for (size_t i = 0; i < *size; i++)
		{
			if (this->p[i]->ft_init(size + 1) == false)
				return false;
			this->p[i]->set_subtensor();
		}
		return true;
	}

	//this is the most with bugs potential
	template<size_t dimension, typename T>
	bool tensor<dimension, T>::_content_inner_alloc(T* values, size_t* size)
	{
		this->p	= (tensor<dimension -1, T>*) calloc(sizeof(tensor<dimension - 1, T>), *size);
		if (this->p == nullptr)
			return false;
		//this->self_size		= (size_t*) calloc(sizeof(int), dimension); [DEPCRATED]
		memcpy(this->self_size, size, sizeof(size_t)*dimension);
		for (size_t i = 0; i < *size; i++)
		{
			this->p[i]->set_subtensor();
			if (this->p[i]->ft_child_init( *(values + i), size + 1) == false)
				return false;
		}
		return true;
	}

	template<size_t dimension, typename T>
	bool tensor<dimension, T>::_content_inner_alloc(tensor<dimension - 1, T>* p_subs, size_t* psize, bool set_child)
	{
		this->p = (tensor<dimension - 1, T>*) calloc(sizeof(tensor<dimension - 1,T>), *psize);
		if (this->p == nullptr)
			return false;
		copy_size(psize);
		//this->self_size		= (size_t*) calloc(sizeof(int), dimension); [DEPCRATED]
		/*memcpy(this->self_size, psize, sizeof(size_t) * dimension);
		memcpy(this->p, p_subs, *psize * sizeof(tensor<dimension - 1, T>));*/
		for(size_t i = 0; i < *psize; i++)
		{
			this->p[i].set_subtensor();
			if (set_child == true)
			{
				if ((this->p[i].ft_child_init(p_subs[i], psize + 1)) == false)
				{
					tensor_init_null();
					return false;
				}
			}
			else
			{
				if ((this->p[i].ft_init(p_subs[i], psize + 1)) == false)
				{
					tensor_init_null();
					return false;
				}
			}

		}

		if (set_child == true)
			for (size_t i = 0; i < *psize; i++)
				this->p[i].set_subtensor();
		if (this->p == nullptr)
			return false;
		return true;
	}

	template<size_t dimension, typename T>
	bool tensor<dimension, T>::_content_inner_alloc(tensor<dimension, T>& subs, size_t* psize, bool set_child)
	{
		this->p = (tensor<dimension - 1, T>*) calloc(sizeof(tensor<dimension - 1,T>), *psize);
		if (this->p == nullptr)
			return false;
		copy_size(psize);
		for (size_t i = 0; i < *psize; i++)
		{
			this->p[i].set_subtensor();
			if (set_child == true)
			{
				if ((this->p[i].ft_child_init(subs[i], psize + 1, set_child)) == false)
				{
					tensor_init_null();
					return false;
				}
			}
			else
			{
				if ((this->p[i].ft_init(subs[i], psize + 1, set_child)) == false)
				{
					tensor_init_null();
					return false;
				}
			}
		}
	}
	
	template<size_t dimension, typename T>
	bool tensor<dimension, T>::init(T* values, size_t* pn)
	{
		tensor_init_empty();
		bool _result =  _content_inner_alloc(values, pn);
		turn_usable();
		return _result;
	}
	
	template<size_t dimension, typename T>
	bool tensor<dimension, T>::init(size_t* pn)
	{
		tensor_init_empty();
		bool _result =  _inner_alloc(pn);
		turn_usable();
		return _result;
	}
	
	template<size_t dimension, typename T>
	bool tensor<dimension, T>::init(tensor<dimension - 1,T>* p_subs, size_t* psize, bool set_child)
	{
		tensor_init_empty();
		bool _result =  _content_inner_alloc(p_subs, psize, set_child);
		turn_usable();
		return _result;
	}

	template<size_t dimension, typename T>
	bool tensor<dimension, T>::ft_empty_init()
	{
		this->_is_init_yet	= false;
		this->self_size		= (size_t*) calloc(sizeof(int), dimension);
		this->p			= nullptr;
		this->_is_null		= false;
		this->_is_child		= false;
		if (this->self_size == nullptr)
			return false;
		for (size_t i = 0; i < dimension; i++)
			this->self_size[i] = 0;
		return true;
	}

	template<size_t dimension, typename T>
	bool tensor<dimension, T>::ft_init(T* values, size_t* pn)
	{
		ft_empty_init();
		bool _result =  content_inner_alloc(values, pn);
		turn_usable();
		return _result;
	}

	template<size_t dimension, typename T>
	bool tensor<dimension, T>::ft_init(size_t* pn)
	{
		ft_empty_init();
		bool _result = _inner_alloc(pn);
		turn_usable();
		return _result;
	}

	template<size_t dimension, typename T>
	bool tensor<dimension, T>::ft_init(tensor<dimension - 1, T>* p_subs, size_t* psize, bool set_child)
	{
		ft_empty_init();
		bool _result = _content_inner_alloc(p_subs, psize, set_child);
		turn_usable();
		return _result;
	}

	template<size_t dimension, typename T>
	bool tensor<dimension, T>::ft_init(tensor<dimension, T>& subs, size_t* psize, bool set_child)
	{
		ft_empty_init();
		bool _result = _content_inner_alloc(subs, psize, set_child);
		turn_usable();
		return _result;
	}

	template<size_t dimension, typename T>
	tensor<dimension, T>::tensor()
	{
		ft_empty_init();
	}
	//This is stupid, i know
	template<size_t dimension, typename T>
	tensor< dimension, T >::tensor( size_t n, ... )
	{
		va_list ap;
		va_start( ap, n );
		//size_t k = 0; //unused
		size_t _iterator = 1;//because we already have n
		size_t* temp_size = (int*) calloc(sizeof(int), dimension);
		while(_iterator < dimension)
		{
			temp_size[_iterator] = va_arg(ap, size_t);
			_iterator++ ;
		}
		ft_init(temp_size);
		free(temp_size);
	}

	template<size_t dimension, typename T>
	tensor<dimension, T>::tensor(size_t* pn)
	{
		ft_init(pn);
	}
	//If the size provided and 'values' doesn't match
	//May lead to unpredicted behaviour
	//Be careful
	template<size_t dimension, typename T>
	tensor<dimension, T>::tensor(T* values, ...)
	{
		va_list ap;
		va_start(ap, values);

		size_t k		= 0;
		size_t _iterator	= 0;
		size_t* temp_size	= (int*) calloc(sizeof(int), dimension);
		while(_iterator < dimension)
		{
			this->self_size[_iterator] = va_arg(values, size_t);
			_iterator ++ ;
		}
		ft_init(values, temp_size);
	}

	template<size_t dimension, typename T>
	tensor<dimension, T>::tensor(T* values, size_t* pn)
	{
		ft_init(values, pn);
	}

	template<size_t dimension, typename T>
	tensor<dimension, T>::tensor(tensor<dimension - 1, T>* p_subs, size_t* psize, bool set_child)
	{
		ft_init(p_subs, psize, set_child);
	}
	//Call after clear memory
	//Hard copy
	//Copy for all sub-tensor to copy
	//must match in size
	//ortherwise will led to memeory leak
	//Well, i donot like recursive like this 
	//And it may turn fatal suome time
	template<size_t dimension, typename T>
	bool tensor<dimension, T>::_subtensor_copy(tensor<dimension, T>& crt)
	{
		for (size_t i = 0; i < *(this->self_size); i ++)
			if(this->p[i]->_subtensor_copy(crt[i]) == false)
				return false;
	}

	template<size_t dimension, typename T>
	tensor<dimension, T>::tensor(const tensor<dimension, T>& rt)
	{
		if (this == &rt)	//Self assignment
			return (*this);	
		ft_empty_init();
		if (_inner_alloc(rt.self_size) == false)
		{
			fprintf(stderr,"Copy constructor: malformed self_size array");
			return;
		}
		if (_sub_tensor_copy( rt.p ) == false)
		{
			tensor_init_empty();
			fprintf(stderr, "Failed to allocate and create tensor");
			return;
		}
		if (rt.usable())
			turn_usable();
	}

	template<size_t dimension, typename T>
	tensor<dimension, T>::tensor(tensor<dimension, T>&& rrt)
	{
		if (this == &rrt)
			return (*this);
		swap(this, rrt);
		rrt.ft_empty_init();
	}
	
	template<size_t dimension, typename T>
	tensor<dimension - 1, T>& tensor<dimension, T>::operator[] (size_t pos)
	{
		return at(pos);
	}

	//Equivalance to operator []
	//For when in implementation and I didn't want to use
	//operator[](size_t)
	//that sound just to weird
	template<size_t dimension, typename T> tensor<dimension - 1, T>& 
        tensor<dimension, T>::at (size_t pos)   //Return element at _pos_ position
	{
		if (is_usable())
			return this->p[pos];
		else
		{
			tensor<dimension - 1, T> temp;
			return temp;
		}
	}

	template<size_t dimension, typename T>  bool 
    tensor<dimension, T>::size_compare(size_t* src_cmp) //Compare two array  _src_cmp_ and this->self_size
	{
		for (size_t i = 0; i < dimension; i++)
			if (this->self_/size[i] != src_cmp[i])
				return false;
		return true;
	}
	template<size_t dimension, typename T>
	tensor<dimension, T>& tensor<dimension, T>::operator =
		(const tensor<dimension, T>& crt)
	{
		//If have not-matching size, clear the object
		//And allocate again
		//Which is more efficient than reallocate (i guess)

		if (this == &crt)
			return (*this);
		if (size_compare(crt.self_size) == false)
		{
			tensor_init_empty();
			init(crt.self_size);
		}
		//Copying
		for (size_t i = 0; i < *(crt.self_size); i++)
		{
			if( (this->p[i]->_subtensor_copy(crt[i])) == false)
			{
				tensor_init_null();
				break;
			}
		}
		return *this;
	}

	template<size_t dimension, typename T>
	tensor<dimension, T>& tensor<dimension, T>::operator =
		(tensor<dimension, T>&& rrt)
	{
		if (this == &crt)
			return (*this);
		swap(rrt, this);
		return *this;
	}

	template<size_t dimension, typename T>
	tensor<dimension, T> tensor<dimension, T>::operator +
		(const tensor<dimension, T>& crt)
	{
		if (size_compare(crt.self_size) == false)
		{
			tensor<dimension, T> result {};
			return result;
		}
		tensor<dimension, T> result (this->self_size);
		for (size_t  i = 0; i < *(this->self_size); i++)
			result[i] = this->p[i] + crt.p[i];
		return result;
	}
	template<size_t dimension, typename T>
	tensor<dimension, T>& tensor<dimension, T>::operator +=
		(const tensor<dimension, T>& crt)
	{
		if (size_compare(crt.self_size) == false)
		{
			tensor<dimension, T> result{};
			return result;
		}
		for (size_t i = 0; i < *(this->self_size); i++)
			this->at(i) +=  crt.p[i];
		return *this;
	}
	
	template<size_t dimension, typename T>
	tensor<dimension, T> tensor<dimension,T>::operator - ()
	{
		tensor<dimension, T> result(this->size);
		for (size_t i = 0; i < *(this->self_size); i++)
			this->at(i) = - this->at(i);
	}
	
	template<size_t dimension, typename T>
	tensor<dimension, T> tensor<dimension, T>::operator - 
		(const tensor<dimension, T>& crt)
	{
		if (size_compare(crt.self_size) == false)
		{
			tensor<dimension, T> result {};
			return result;
		}
		tensor<dimension, T> result (this->self_size);
		for (size_t i = 0; i < *(this->self_size); i++)
			result[i] = this->p[i] - crt.p[i];
		return result;
	}

	template<size_t dimension, typename T>
	tensor<dimension, T>& tensor<dimension, T>::operator -=
		(const tensor<dimension, T>& crt)
	{
		if (size_compare(crt.self_size) == false)
		{
			tensor<dimension, T> result {};
			return result;
		}
		for (size_t i = 0; i < *(this->self_size); i++)
			this->at(i) += crt.p[i];
		return *this;
	}

	template<size_t dimension, typename T>
	bool operator == (const tensor<dimension, T> crt1, const tensor<dimension, T>& crt2)
	{
		/*
		if (crt1.compare_size(crt2.self_size) == false)
			return false;*/
		for (size_t i = 0; i < dimension; i++)
			if (crt1.self_size[i] != crt2.self_size[i])
				return false;
		//size_t process_stage = 0;
		for(size_t i = 0; i < crt1.self_size[0]; i++)
			if (crt1.p[i] != crt2.p[i])
				return false;
		return true;
	}

	//For the sake of efficency, i decided not to use the above function
	//But write a new one
	template<size_t dimension, typename T>
	bool operator != (const tensor<dimension, T> crt1, const tensor<dimension, T>& crt2)
	{
		return ~( crt1  == crt2 );
	}
	template<size_t dimension, typename T>
	size_t tensor<dimension, T>::get_size(size_t stage )
	{
		return this->self_size[stage];
	}

	template<size_t dimension, typename T>
	tensor<dimension, T>::operator bool()
	{
		return turn_usable();
	}

	template<size_t dimension, typename T>
	bool tensor<dimension, T>::is_usable()
	{
		return ( this-> _is_null && (this->p != nullptr) && (this->self_size != nullptr) && (this->_is_init_yet) );
	}

	template<size_t dimension, typename T>
	bool tensor<dimension, T>::copy_size(size_t* psize)
	{
		if (this->self_size == nullptr)
		{
			if (this->self_size != nullptr)
				free(this->self_size);
			this->self_size = (size_t*) calloc(sizeof(int), dimension);
		}
		memcpy(this->self_size, psize, sizeof(size_t)*dimension);
		if (this->self_size != nullptr)
			return true;
		else
			return false;
	}
	//suggest to use this instead of the variadic ones
	//We suggest create a new object instead of resize
	//Which is potential of bugs
	template<size_t dimension, typename T>
	char tensor<dimension, T>::resize(size_t * psize, bool clear)
	{
		if (this->_is_child == true)
			return -1;
		if (clear == false)
		{
			//Expanding
			if (*psize > *(this->self_size))
			{
				this->p = (tensor<dimension - 1, T>*) realloc(p,
						sizeof(tensor<dimension - 1, T>) * (*psize));
				return 0x01;
				
			}
				
			else if (*psize < *(this->self_size))
			{
				for (size_t i = *psize; i < *(this->self_size); i++)
					this->p[i]->clear();//practically remove
				this->p = (tensor<dimension - 1, T>*) realloc(p, 
						sizeof(tensor<dimension - 1, T>) * (*psize));
				copy_size(psize);
				return 0x02;
			}

			else 
			{
				//Do nothing
				return 0x03;
			}
		}
		else
		{
			tensor_init_empty();
			init(*psize);
			return 0x04;
		}
	}

	template<size_t dimension, typename T>
	char tensor<dimension, T>::resize(bool clear, ...)
	{
		va_list ap;
		va_start(ap, clear);
		size_t temp_size[dimension] ;
		for (size_t i = 0; i < dimension; i++)
			temp_size[i] = va_arg(ap, size_t);
		return resize(temp_size, clear);
	}
	//Quick reminder:
	//replace "tensor<dimension>, typename T = double>" with "tensor<dimension, T>::tensor_iterator"
#ifdef LIBTENSOR_TENSOR_ITERATOR
	template<size_t dimension, typename T>
	tensor<dimension, T>::tensor_iterator::tensor_iterator()
	{ this->p 		= nullptr; }

	template<size_t dimension, typename T>
	tensor<dimension, T>::tensor_iterator::tensor_iterator(const tensor_iterator& cri)
	{ this->p 		= cri.p;}

	template<size_t dimension, typename T>
	typename tensor<dimension, T>::tensor_iterator& tensor<dimension, T>::tensor_iterator::operator= (const tensor_iterator& cri)
	{
		this->p		= cri.p;
		return *this;
	}

	//just swap values of 2 posize_ter
	template<size_t dimension, typename T>
	tensor<dimension, T>::tensor_iterator::tensor_iterator(tensor_iterator&& rri)
	{ swap(this->p, rri.p); }

	template<size_t dimension, typename T>
	typename tensor<dimension, T>::tensor_iterator& tensor<dimension, T>::tensor_iterator::operator= (tensor_iterator&& rri)
	{
		swap(this->p, rri.p); 
		return *this;
	}

	template<size_t dimension, typename T>
	tensor<dimension, T>::tensor_iterator::tensor_iterator(tensor<dimension - 1, T>* pt)
	{ this->p 		= pt; }
	
	template<size_t dimension, typename T>
	tensor<dimension, T>::tensor_iterator::tensor_iterator(tensor<dimension - 1, T>& rt)
	{ this->p		= &rt; }
	
	template<size_t dimension, typename T>
	bool tensor<dimension, T>::tensor_iterator::operator== (const tensor<dimension, T>::tensor_iterator& crt)
	{
		if ( (this->p == nullptr) || (crt.p == nullptr) )
			return false;
		return this->p == crt.p;
	}
	
	template<size_t dimension, typename T>
	bool tensor<dimension, T>::tensor_iterator::operator!= (const tensor<dimension, T>::tensor_iterator& crt)
	{
		if ( (this->p == nullptr) || (crt.p == nullptr) )
			return true;
		return this->p != crt.p;
	}

	template<size_t dimension, typename T>
	tensor<dimension - 1, T>& tensor<dimension, T>::tensor_iterator::operator *()
	{
		return *p;
	}
#endif

	//Practically call tensor_init_empty
	template<size_t dimension, typename T>
	void tensor<dimension, T>::clear()
	{
		tensor_init_empty();
	}

	//Practically just tensor_init_empty
	template<size_t dimension, typename T>
	tensor<dimension, T>::~tensor()
	{
		clear();
	}
#ifdef LIBTENSOR_TENSOR_ITERATOR
	template<size_t dimension, typename T>
	typename tensor<dimension, T>::tensor_iterator tensor<dimension, T>::begin()
	{
		if (this->p != nullptr)
			return tensor_iterator(this->p[0]);
		else
		{
			tensor_iterator temp();
			return temp;
		}
	}
	
	template<size_t dimension, typename T>
	typename tensor<dimension, T>::tensor_iterator tensor<dimension, T>::end()
	{
		if (this->p != nullptr)
			return tensor_iterator(this->p[*(this->self_size) -1]);
		else
		{
			tensor_iterator temp();
			return temp;
		}
	}
#endif
			
}
#endif

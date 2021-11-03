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
#ifndef LIBTENSOR_UNIT_DIMENSION_DECLARE
#define LIBTENSOR_UNIT_DIMENSION_DECLARE
namespace libtensor_namespace
{
template<typename T>
class tensor<1,T>
{
	private:
		T* p;
		int self_size;
		bool _is_init_yet;
		bool _is_null;
		bool _is_child;
		
		//A safe version of _inner_alloc
		void _tensor_init(); //This is for a fresh one
		bool _finner_alloc(int n)
		{tensor_init_empty(); return _inner_alloc(n);}
		bool _inner_alloc(int);
		bool _content_inner_alloc(T*, int);

		void free_all();
		bool size_compare(int*);
		bool size_compare(int);
		void turn_usable();//Allow access permission
		void lock_usable();//Denied access permission
		bool copy_size(int);
	public:
		void tensor_init_empty();//This is for clear an already existing one
		void tensor_init_null();

		tensor();
		//Allocate memory for certain size
		//After this, the vector is usable
		//Handle with care and remember your position
		tensor(int);
		
		//Allocate memory with actual content
		tensor(T*, int);
		tensor(int, ...);
		
		//These 2 are for the sake of backward compability
		bool init(T*, int*);
		bool init(int*);
		
		//Exclusive for tensor<1>
		bool init_vector (int);
		bool init_vector (T*, int);

		bool _child_init(void* values, int* psize)
		{bool _result = init(values, psize); this->_is_child = true; return _result;}
		bool _child_init(int* psize)
		{bool _result = init(psize); this->_is_child = true; return _result;}
		
		bool ft_empty_init();
		bool ft_init(T* values, int* pn)
		{return ft_init(values, *pn);}
		bool ft_init(T*, int);
		bool ft_init(int*);
		bool ft_init(tensor<1,T>& values, int *pn, bool = true)
		{return _content_inner_alloc(values.get_data(), *pn);}
		//bool ft_init(tensor<dimension - 1, T>*, int*, bool = true);
		bool ft_child_init(T* values, int* psize)
		{bool _result = ft_init(values, psize); this->_is_child = true; return _result;}
		bool ft_child_init(int* psize)
		{bool _result = ft_init(psize); this->_is_child = true; return _result;}
		bool ft_child_init(tensor<1,T>& values, int* pn)
		{bool _result = ft_init(values, pn); this->_is_child = true; return _result;}

		//Not actually copy subtensor
		//Just copy the values it manage
		bool _subtensor_copy(const tensor<1, T>&);
		
		T& operator[] (int);
		T& at(int);
		tensor(const tensor<1, T>&);
		tensor<1, T>& operator= (const tensor<1, T>&);
		
		tensor(tensor<1,T>&&);
		tensor<1, T>& operator= (tensor<1,T>&&);
		
		tensor<1,T> operator + (const tensor<1,T>& );
		tensor<1,T>& operator += (const tensor<1,T>& );

		tensor<1,T> operator -();
		tensor<1,T> operator - (const tensor<1,T>& );
		tensor<1,T>& operator -= (const tensor<1,T>& );
		
		bool operator == (const tensor<1,T>&);
		bool operator != (const tensor<1,T>&);

		void set_subtensor()
		{this->_is_child = true;}
		void set_maintensor()
		{this->_is_child = false;}
		int get_size();
		//Exmtremely dangerous
		T* get_data();
		operator bool();
		bool is_usable();

		char resize(int);
#ifdef LIBTENSOR_TENSOR_ITERATOR	
		class tensor_iterator : public std::iterator<std::random_access_iterator_tag, int, T*, T&>
		{
			private:
				T* p;
			public:
				tensor_iterator();

				tensor_iterator(const tensor_iterator&);
				tensor_iterator& operator= (const tensor_iterator&);
				tensor_iterator(tensor_iterator&&);
				tensor_iterator& operator= (tensor_iterator&&);
				tensor_iterator(T*);
				tensor_iterator(T&);

				bool operator == (const tensor_iterator&);
				bool operator != (const tensor_iterator&);
				T& operator * ();
		};
		tensor_iterator begin();
		tensor_iterator end();
#endif

		void clear();
		~tensor();
};

	template<typename T>
	using vect = tensor<1,T>;
	using real_vect = tensor<1,double>;
}
#endif

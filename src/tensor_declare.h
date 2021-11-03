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
#ifndef LIBTENSOR_TENSOR_DECL_H
#define LIBTENSOR_TENSOR_DECL_H
//#include <stdarg.h>
#include "tensor_config.h"  //This is the place to put "#define" pp

#ifdef LIBTENSOR_STDARG
    #include <stdarg.h>
#endif  /*NOT LIBTENSOR_STDARG*/

#include <stdlib.h>
#include <stdio.h>
#ifdef LIBTENSOR_TENSOR_ITERATOR
	#include <iterator>
#endif  /*NOT LIBTENSOR_TENSOR_ITERATOR*/

//Please dont enter any 'int' variable with negative
namespace libtensor_namespace
{
    typedef uint8_t tensor_stt;
    template<size_t dimension,typename T = double>
    class tensor
    {
    private:
        //----------------Assets-----------------------
        size_t* self_size;
        tensor<dimension-1,T>* p;
        tensor_stt state; 

        //----------------Assets-access----------------
        size_t _get_size(int stage){return (stage < dimension) ? this->self_size[stage] : -1;}
        void turn_useable();
        void lock_useable();
        bool copy_size(size_t);
        
        //----------------Initialize function----------
	    //A safe version of _inner_alloc
	    bool _finner_alloc(size_t* pn)
	    {tensor_init_empty(); return _inner_alloc(pn);}
	    //Allocate empty array of p
	    bool _inner_alloc(size_t);
	    //Allocate and initialize all
	    bool _inner_alloc(size_t*);
	    // By default set all subtensor to child
        bool _content_inner_alloc(T*, size_t*);
	    bool _content_inner_alloc(tensor<dimension - 1, T>*, size_t*, bool = true);
	    bool _content_inner_alloc(tensor<dimension, T>&, size_t*, bool = true);

        void _tensor_init();


	//void free_all();
	//bool size_compare(int*); [PUBLICIZED]
	//void turn_usable();
	//void lock_usable();
	//bool copy_size(size_t*);
	//Initialize
	//void _tensor_init();

    public:

	bool size_compare(size_t*);
	//practically deinitialze
	void tensor_init_empty();
	//like tensor_init_empty but _is_null is true
	void tensor_init_null();
    
	//copy from array to size
	//bool copy_size(int*); [privatized]
	
	    //----------------Initialize public function---
	    bool init(T*, size_t*);
    	bool init(size_t*);
	    bool init(tensor<dimension - 1, T>*, size_t*, bool = true);
	    bool init(tensor<dimension, T>&, size_t*, bool = true);
	    bool _child_init(T* values, size_t* psize)//only use for sub-tensor
	        {bool _result = init(values, psize); this->_is_child = true;return _result;}
	    bool _child_init(size_t* psize)
	        {bool _result = init(psize); this->_is_child = true;return _result;}
	    bool _child_init(tensor<dimension - 1,T>* values, size_t* psize, bool set_child = true)
	        {bool _result = init(values,psize,set_child); this->is_child = true; return _result;}
	    bool child_init(tensor<dimension, T>& values, size_t* psize, bool set_child = true)
	        {bool _result = init(values, psize, set_child); this->is_child = true; return _result;}
	
	//First time initialization
	//Declared since the malloc, calloc and realloc doesn't call the constructor
	    bool ft_empty_init();
	    bool ft_init(T*, size_t*);
	    bool ft_init(size_t*);
	    bool ft_init(tensor<dimension - 1,T>*,size_t*,bool = true);
	    bool ft_init(tensor<dimension, T>&, size_t*, bool = true);
	    bool ft_child_init(T* values, size_t* psize)
	        {bool _result = ft_init(values, psize); this->_is_child = true; return _result;}
	    bool ft_child_init(size_t* psize)
	        {bool _result = ft_init(psize); this->_is_child = true;return _result;}
	    bool ft_child_init(tensor<dimension - 1,T>* values, size_t* psize, bool set_child = true)
	        {bool _result = ft_init(values, psize, set_child); this->is_child = true; return _result;}
	    bool ft_child_init(tensor<dimension, T>& values, size_t* psize, bool set_child = true)
	        {bool _result = ft_init(values, psize, set_child); this->is_child = true; return _result;}

	    bool _subtensor_copy(tensor<dimension, T>&);

        //----Contructor---------------------------
        tensor();
#ifdef LIBTENSOR_STDARG
        tensor(size_t ...);
        tensor(T*, size_t ...);
#endif  /*NOT LIBTENSOR_STDARG*/

        tensor(size_t *);
        tensor(T*, size_t*);
        tensor(tensor<T, dimension - 1>*, size_t*, bool = true);
        tensor(tensor<T, dimension - 1>*, size_t , bool = true);

        // ---------Assignment--------------
        tensor(const tensor<dimension,T>&);
	    tensor(tensor<dimension, T>&&);
	    tensor<dimension, T>& operator = (const tensor<dimension, T>&);
	    tensor<dimension, T>& operator = (tensor<dimension, T>&&);
	    
        //----------Numeric operator
        tensor<dimension,T> operator +
        (const tensor<dimension,T>&);
        tensor<dimension, T>& operator +=(const tensor<dimension,T>&);

        tensor<dimension,T> operator- ();
        tensor<dimension,T> operator -
        (const tensor<dimension,T>&);

        tensor<dimension, T>& operator -=(const tensor<dimension,T>&);
	
	bool operator == (const tensor<dimension, T>&);
	bool operator != (const tensor<dimension, T>&);
	
	//Return size for certain stage
        size_t get_size(size_t stage);

	//Return the pointer of p
       	//Extremely dangerous
	tensor<dimension - 1, T>* get_data();

       	// Return object's usuability 
        operator bool();
	//Clone of operator bool
	bool is_usable();


        //Resize tensor to certain size
        char resize(size_t*, bool = false);
	//Resize tensor to certain size
	char resize(bool = false,...);
	
	void set_subtensor() { this->_is_child = true; }
	void set_maintensor() { this->is_child = false; }

//---------------tensor_iterator-------------------
#ifdef LIBTENSOR_TENSOR_ITERATOR
	class tensor_iterator : public std::iterator<std::random_access_iterator_tag, tensor<dimension - 1, T>, size_t, tensor<dimension - 1, T>*, tensor<dimension - 1, T>&>
	{
		private:
			tensor<dimension - 1, T>* p;
		public:
			tensor_iterator();

			tensor_iterator(const tensor_iterator&);
			tensor_iterator(tensor_iterator&&);
			tensor_iterator& operator = (const tensor_iterator&);
			tensor_iterator& operator = (tensor_iterator&&);

			tensor_iterator(tensor<dimension - 1, T>*);
			tensor_iterator(tensor<dimension - 1, T>&);

			tensor_iterator& operator ++ ();
			tensor_iterator& operator ++ (size_t);

			bool operator == (const tensor_iterator&);
			bool operator != (const tensor_iterator&);
			tensor<dimension - 1, T>& operator *();
			~tensor_iterator(){p = nullptr;}
	};

	typename tensor_iterator begin();
	typename tensor_iterator end();
#endif

        void clear();
        ~tensor();
    };
}
#endif

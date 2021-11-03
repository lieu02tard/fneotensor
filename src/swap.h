#ifndef LIBTENSOR_SWAP_MODULE
#define LIBTENSOR_SWAP_MODULE
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
//Make sure to distinguish between 
//Reference and pointer
//If swap pointer and pointer, only the value of those pointer will be changed
//Which mean they will be changed like normal variable
//If swap ref and ref, the reference itself will ref to another variable
//If swap pointer and ref,the pointer will point to the address of the reference
//And the reference will ref value in which to the pointer point to
template<typename T>
void swap(T& a,T& b);
/*template<typename T>
void swap(T** a, T** b);*/
template<typename T>
void swap(T* a, T& b);
template<typename T>
void swap(T& a, T* b)
{ swap(b, a); }

template<typename T>
void swap(T& a,T& b)
{
    T temp{static_cast<T&&>(a)};
    a=static_cast<T&&>(b);
    b=static_cast<T&&>(temp);
}
/*
template<typename T>
void swap (T** a, T**b)
{
	T* c	= a;
	*a	= *b;
	*b	= c;
}*/
//Pretty crappy swap
template<typename T>
void swap (T* a, T& b)
{
	T temp 	= {static_cast<T&&>(b)};
	b	= static_cast<T&&>(*a);
	*a	= static_cast<T&&>(temp);
}
#endif

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
//This file is heavily emphasis on linear algebra and calculation
//Feel free to just use tensor.h for you normal use

#include "tensor_declare.h"
#include <string.h>
#ifndef LIBTENSOR_MATRIX_DECLARATION
#define LIBTENSOR_MATRIX_DECLARATION
namespace libtensor_namespace
{
typedef int rowid;
typedef int colid;
template<typename T>
class matrix : public tensor<2, T>
{
	private:
		bool is_RREF_already;
		int* d;
		int* f;
		int r;
		void free_d(); void free_f();
		void free_a(){free(d); free(f);}
		T m_unit ; 		//Unit variable, such as 1 
	public:
		void set_unit(T unit) { m_unit = unit ; }
		T& at(rowid, colid);
		bool is_square();
		bool is_unit();
		bool is_invertible();

		matrix<T> transpose();
		matrix<T> operator *(const matrix<T>&);

		void rowop_Swap(rowid, rowid);
		void rowop_AddScalar(rowid passive, rowid active, T scalar);
		void rowop_MultiplyScalar(rowid, T);

		//passive row is being change
		//active is the key row
		//k is the column of the leading 1 at i where j equals 0
		void third_RO(rowid passive, rowid active, colid key);
		void second_RO(rowid, rowid);

		void RREF();
		bool isRREF();

		void del_RREF_state();
		void clear_zero();
		
		int transF(int*);
		int transD(int*);

		void printD(); void printF();
};
template<typename T>
matrix<T>& attach(matrix<T>*, tensor<1, T>*);

template<class Iter, class val>
Iter find(Iter first, Iter last, val value)
{
	Iter i = first;
	while (*i != value)
	{
		if (i == last)
			return Iter();
		i++;
	}
	return i;
}
}
#endif

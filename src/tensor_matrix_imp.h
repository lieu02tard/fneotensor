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
#include "tensor_matrix_dec.h"
#ifndef LIBTENSOR_MATRIX_IMPLEMENTATION
#define LIBTENSOR_MATRIX_IMPLEMENTATION
#define LIBTENSOR_MATRIX_ROFFSET 0
//Remember to replace the [tmp] flag with
//"template<typename T>"
namespace libtensor_namespace
{
template<typename T>
T& matrix<T>::at (rowid i, colid j)
{
	return this->p[i][j];
}

template<typename T>
bool matrix<T>::is_square()
{
	return this->_get_size(0) == this->get_size(1);
}

template<typename T>
bool matrix<T>::is_unit()
{
	if (this->is_square() == false)
		return false;
	if (this->_get_size(0) <= 1)
		return true;
	T temp = this->at(0)[0];
	for (int i = 1; i < this->_get_size(0); i++)
		if ( this->at(i)[i] != temp )
			return false;
	return true;
}


//Note that i and j as we call 
//Should count from zero
//But if you want to start from 1
//define LIBTENSOR_MATRIX_ROFFSET 1
template<typename T>
void matrix<T>::rowop_Swap(rowid i, rowid j)
{
	if (i == j)
		return;
	swap(this->p[i + LIBTENSOR_MATRIX_ROFFSET], this->p[j + LIBTENSOR_MATRIX_ROFFSET]);
}

template<typename T>
void matrix<T>::rowop_MultiplyScalar(rowid i, T scalar)
{
	this->p[i + LIBTENSOR_MATRIX_ROFFSET] = this->p[i + LIBTENSOR_MATRIX_ROFFSET] * scalar;
}

template<typename T>
void matrix<T>::rowop_AddScalar(rowid passive, rowid active, T scalar)
{
	this->p[passive + LIBTENSOR_MATRIX_ROFFSET] += this->p[active + LIBTENSOR_MATRIX_ROFFSET] * scalar;
}

//Rowop on row r
//To make entry (r,j) value 1 (or unit)
template<typename T>
void matrix<T>::second_RO(rowid r, colid j)
{
	rowop_MultiplyScalar(r , m_unit / at(r,j));
}



//Make the value at (passive,k) 0
template<typename T>
void matrix<T>::third_RO(rowid passive, rowid active, colid key)
{
	rowop_AddScalar(passive, active, - at(passive, key) / at(active, key) );
}

//RREF checking algorithm
//Step 0: Setup variable
//d is an array storing the value of the leading-1 column(s)
//di denote the number of the most recent checked leading 1
//r iterate the rows
//j iterate the columns
//l and h mark two nextby leading-1
//
//Step 1: Increase r by 1. If r equal m, terminate and return true
//Step 2: Increase j by 1
//Step 3: Check the entry at (r,j). If equals 1, goto step 4
//If not equals 0 , terminate and return false
//If equals 0, goto step 2
//Step 4: If j equal n, check the rows from r + 1 to the rest
//If there is any of those that is non-zero, terminate and return false
//If all of those are zero vector, terminate and return true
//If j is not equal to n, goto step 5
//
//Step 5: If (r,j) equal 1, iterate x from 1 to m
//When x != r, if (x,j) = 0 then terminate and return false
////This is to ensure that there are the column j are all zero except at row r
//Goto step 6
//
//Step 6
//Set i = 0 //To iterate from 1 to di
////l is the second recent leading 1 column
////h is the recent leading 1 column
//Goto step 7
//Step 7: If di less than di, goto step 8
//If not, goto step 9
//Step 8: Check all number from l to h to look for non-zero
//If there are, terminate and return false
//If not, set l = d[i], h = d[++i] and goto step 7
//Step 9: Check all from (r,h) to (r, j -1)  (denote by z) to look for non-zero
//If there are, terminate and return 0
//If not, increase z by 1 and continue checking
//If z = j goto step 10
//Step 10:  
//d[++di] = j
//Goto step 1
//In this case from l to h
//
template<typename T>
bool matrix<T>::isRREF()
{
	if (is_RREF_already)
		return true;
	else
	{
		int j		= -1;		//Iterate throught columns
		int r		= -1;		//Iterate though rows
		int di		= 0;		//Mark the number of d
		int m 		= this->_get_size(0);
		int n		= this->_get_size(1);
		int *d		= (int*) calloc(sizeof(int), n);	//Store d
CheckF:
		while( ++r != m)
		{
			while(++j != n)
			{
				if (at(r,j) == 1) break;
				if (at(r,j) != 1)
					if (at(r,j) != 0)
						goto DeadEnd;
			}
			//This is to check whether break condition is j iterate to the value of _get_size(1)
			//Or at(r,j) == 1
			if (j == n)
			{
				for(int y = r+1; y <= m; y++)
					if (isZero(at(y)) == false)
						goto DeadEnd;
				//Begin a new iterate of r
				//goto CheckF ; [DEPCRATED]
				continue;
				
			}

			if (at(r,j) == 1)
			{
				//Check from 1 to m to ensure the column is zero 
				//Except at row r
				for (int x = 1; x <= m; x++)
					if (x != r)
						if (at(x,j) != 0)
							goto DeadEnd;

				int i 		= 0;
				int l 		= 0;
				int z		= 0;
				int h		= *d;

				while( i < di)
				{
					z = l + 1;
					while (z < h)
					{
						if (at(r,z) != 0)
							goto DeadEnd;
						z++;
					}
					
					l = d[i];
					h = d[++i];
				}
				
				while (z != j)
				{
					if (at(r,z) == 0)
						z++;
					else
						goto DeadEnd;
				}
					d[++di] = j;
			}
		}
VictoryEnd:	
		free(d);
		return true;
	}
DeadEnd:
	free(d);
	return false;
}
//RREF algorithm
//Step 0: Setup variable
//j = 0, to iterate through column
//r = 0, to iterate through row
//dx is an array of number denote leading 1
//di denote the current processing dx
//Step 1: Increase j by 1. If j equal n, terminate
//Else goto step 2
//Step 2: assign k = r + 1. 
//Step 3: If k <=m goto step 4
//else goto step 1
//Step 4: If (k,j) != 0 goto step 5. Else goto step 6
//Step 5:Set i = k. increase r by 1
//Swap row i and r
// second rowop to row r to make entry (r,j) 1
//third RO all the other row except r at column j to make the column j all 0 except at row r
//Register d[di++] = j
//set k = m + 1
//Goto step 3
//Step 6: Increase k by 1
//
template<typename T>
void matrix<T>::RREF()
{
	//Algorithm section
	int j		= -1;
	int r		= -1;
	int* dx		= (int*) calloc(sizeof(int), this->_get_size(1));
	int di		= 0;
	int x;
	int n		= this->_get_size(1);
	int m 		= this->_get_size(0);
	while(++j != n )
	{
		int k = r + 1;
		while (k < m)
		{
			if (at(k,j) != 0)
			{
				int i = k;
				r++;
				rowop_Swap(i, r);
				second_RO(r, j); // Convert (r,j) to 1

				//Convert every other entry of column j except r to be 0
				for( x = 1; x<=m; x++)
					if (x != r)
						third_RO(r, x, j);
				dx[di++] = j;
				//k = _get_size(0); //(Why ?). Maybe i just want to break
				break;
			}
			else k++;
		}
	}

	//Enregister section
	this->r 		= r + 1; //r in local scope are numinator, which start from 0
	// this->r denote the number of non-zero rows
	this->is_RREF_already	= true;
	this->d			= (int*) calloc(sizeof(int), r);
	this->f 		= (int*) calloc(sizeof(int), n - r);
	memcpy(this->d, dx, sizeof(int) * r);

	int i 			= 0;
	int l 			= 0;
	int fi 			= 1;
	int h			= *dx;

	while (i <= r)
	{
		x		= l + 1;
		//Copy every column number except those of d into f
		while (x < h)
			f[fi++] = x++;
		l 		= dx[i];
		h		= dx[++i];
	}
	x= d[r - 1] + 1;
	while (x < n)
		this->f[fi ++] = x++;
	
	free(d);
}
/*template<typename T>
void matrix<T>::print_d()
{
	if (this->is_RREF_already ==  true)
	{
		printf("\n");
		for (int i = 0; i < r; i++)
			printf("%d \t", this->d[i]);
		printf("\n");
	}
	//Enable if you really want an error 
	//else fprintf(stderr, "Call print_d while not RREF yet\n");
}*/

/*template<typename T>
void matrix<T>::print_f()
{
	if (this->is_RREF_already == true)
	{
		printf("\n");
		for (int i = 0; i < r; i++)
			printf("%d \t", this->d[i]);
		printf("\n");
	}
	//else fprintf(stderr, "Call print_f while not RREF yet\n");
}*/

//Copy f array
//Make sure f is freed and deinitialized before call
//Because there will be allocation
template<typename T>
int matrix<T>::transF(int* varf)
{
	int n 		= this->_get_size(1);
	int r 		= this->r;
	if (this->is_RREF_already == true)
	{
		varf = (int*) calloc(sizeof(int), n - r);
		memcpy(varf, this->f, sizeof(int) * (n-r));
		return n - r;
	}
	else return 0; //return false;
}

template<typename T>
int matrix<T>::transD(int* vard)
{
	if (this->is_RREF_already == true)
	{
		vard = (int*) calloc(sizeof(int), r);
		memcpy(vard, this->d, sizeof(int) * r);
		return this->r;
	}
	else return 0;
}

template<typename T>
matrix<T> matrix<T>::transpose()
{
	matrix<T> _result {this->_get_size(1), this->_get_size(0)};
	for (int i = 0; i < this->_get_size(0); i++)
		for( int j = 0; j < this->_get_size(1); j++)
			_result[j][i] = at(i, j);
	return _result;
}
}
#endif

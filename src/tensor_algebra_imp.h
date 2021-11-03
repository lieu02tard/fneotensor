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
#include <tensor_algebra_dec.h>
#ifndef LIBTENSOR_ALGEBRA_IMPLEMENTATION
#define LIBTENSOR_ALGEBRA_IMPLEMENTATION
namespace LIBTENSOR_ALGEBRA_MODULE
{
template<typename T>
void init_sol(solution<T>& s)
{
	s->free_vect 		= libtensor_namespace::vect<T>();
	s.result_set.clear();
	s.n 			= 0;
	s.r			= 0;
	s.f 			= nullptr;
	s.d			= nullptr;
	s.con			= false;
}

template<typename T>
bool isZero(libtensor_namespace::tensor<1,T> v)
{
	for (int i = 0; i < v.get_size(); i++)
		if (v[i] != 0)
			return false;
	return true;
}

template<typename T>
solve_engine<T>::solve_engine()
{
	this->coeff 		= libtensor_namespace::matrix<T>();
	this->constant		= libtensor_namespace::vect<T>();
	this->col		= 0;
	this->row 		= 0;
	init_sol(this->sol);
	this->is_solve_already	= false;
};

template<typename T>
solve_engine<T>::solve_engine(libtensor_namespace::matrix<T> A, libtensor_namespace::vect<T> b)
{
	if (A.get_size(1) == b.get_size())
	{
		if (b.get_size() > 0)
		{
			this->coeff		= A;
			this->constant		= b;
			this->col		= A.get_size(0);
			this->row		= b.get_size();
			init_sol(&(this->solution));
			this->is_solve_already	= false;
			return;
		}
		else if (b.get_size() == 0)
		{
DeadEnd:
			this->coeff		= libtensor_namespace::matrix<T>();
			this->constant 		= libtensor_namespace::vect<T>();
			this->col		= 0;
			this->row		= 0;
			init_sol(&(this->solution));
			this->is_solve_already	= false;
			return;
		}
	}
	else goto DeadEnd;
}

template<typename T>
solve_engine<T>::solve_engine(libtensor_namespace::matrix<T> m)
{
	this->col		= m.get_size(1) - 1;
	this->row		= m.get_size(0);
	this->coeff		= {this->row, this->col};
	this->constant		= {this->row};
	init_sol(this->_solutions);
	_solutions.n		= this->col;
	for (int i = 0; i < this->row; i++)
		for (int j = 1; j < this->col; j++)
			this->coeff[i][j] = m[i][j];
	for (int i = 1; i < this->row; i++)
		this->constant[i] = m[i][this->col + 1];
}

template<typename T>
solve_engine<T>::solve_engine(const solve_engine<T>& cst)
{
	if (this == &cst)
		return (*this);
	this->col		= cst.col;
	this->row		= cst.col;
	this->coeff		= cst.coeff;
	this->constant		= cst.constant;
	copy_sol(this->_solutions, cst._solutions);
}

template<typename T>
solve_engine<T>& solve_engine<T>::operator= (const solve_engine<T>& cst)
{
	if (this == &cst)
		return (*this);
	this->coeff		= cst.coeff;
	this->constant		= cst.constant;
	this->row		= cst.row;
	this->col		= cst.col;
	copy_sol(this->_solutions, cst._solutions);
}

template<typename T>
solve_engine<T>::solve_engine(solve_engine<T>&& rrt)
{
	if (this == &rrt)
		return (*this);
	swap(rrt, this);
	rrt.clear();
}

template<typename T>
solve_engine<T>& solve_engine<T>::operator= (solve_engine<T>&& rrt)
{
	if (this == &rrt)
		return (*this);
	swap(rrt, this);
	rrt.clear();
	return *this;
}

template<typename T>
bool solve_engine<T>::is_solve_yet()
{
	return this->is_solve_already;
}

template<typename T>
void solve_engine<T>::reCalibrate()
{
}

template<typename T>
bool solve_engine<T>::isHomogenous()
{
	return isZero(this->constant);
}

template<typename T>
void solve_engine<T>::print_ls()
{
	libtensor_namespace::matrix<T> m		= attach(this->coeff, this->constant);
	if (is_solve_already)
	{
		for (int j = 0; j <= m.get_size(0); j++)
			for (int i = 0; i <= m.get_size(1); i++)
				printf("%.8f\t", m[i][j]);
		printf("\n");
	}
	else fprintf(stderr, "Called print_ls while not solved yet\n");
}

//I can believe this is longer than the isRREF() or RREF() function
template<typename T>
solution<T> solve_engine<T>::solve()
{
	if (this->is_solve_already == false)
	{
		size_t row 	= this->coeff.row;
		size_t col 	= this->coeff.col + 1;
		libtensor_namespace::matrix<T> m 	= {row, col};
		for (size_t i = 0; i < row; i++)
		{
			for (int j = 0; j < col - 1; j++)
				m[i][j] = this->coeff[i][j];
			m[i][col - 1] = this->constant[i];
		}
		m.RREF();
		int r 		= m.non_zero();
		m.transD(this->_solutions.d);
		m.transF(this->_solutions.f);
		if (this->_solutions.d[r - 1] == col)
		{
			this->_solutions.con 	= false;
			this->is_solve_already	= false;
		}
		else
		{
			this->_solutions.con		= true;
			this->_solutions.resut_set.clear();
			this->_solutions.free_vect	= this->constant;
			this->_solutions.n		= this->coeff.col();
			this->_solutions.r		= r;
			this->_solutions.f[col - r - 1]	= col;
			int n 				= col - 1;
			int fi				= 0;		//Iterate through _solutions.f
			int di				= 0;		//Iterate through _solutions.d
			int fl				= this->_solutions.f[0];//Store most recent f
			int dl				= this->_solutions.d[0];//Store most recent d

			if (n == r)
			{
				libtensor_namespace::vect<T> v2	= {row};
				for (int i = 0; i < row; i++)
					v2[i] = m[i][col - 1];
				this->_solutions.free_vect 	= v2;
			}

			else
			{
				//Iterate through the non-zero rows (variable i)
				//Iterate though columns (returned variables) (variable j)
				//If i in f and i = f[j] then zi[j] = 1
				//If i in f and i!= f[j] then zi[j] = 0 for the independant variable 
				//Does not depend on other independant var
				//If i in d  and i = d[k] then zi[j] = -m[k][f[j]]
				for (int i = 1; i < n - r; i++)
				{
					libtensor_namespace::vect<T> v	= {n};
					for (int j = 0; j < n; j++)
					{
						if (j == this->_solutions.f[i])
							v[j] = 1;
						else
						{
							if (j == fl)
							{
								v[j] = 0;
								fl = this->_solutions.f[++fi];
							}
							if (j == dl)
							{
								v[j] = -m[di][this->_solutions.f[i]];
								dl = this->_solutions.d[++di];
							}
						}
					}
					this->_solutions.result_set.insert(std::pair<int, 
					libtensor_namespace::vect<T>>(this->_solutions.f[i], v));	
				}
				this->is_solve_already		= false;
				libtensor_namespace::vect<T> v2	= {n};
				for (int i = 0; i < n; i++)
				{
					fi 		= 1;
					fl		= this->_solutions.f[0];
					if (i == fl)
					{
						v2[i]	= 0;
						fl 	= this->_solutions.f[++fi];
					}
					else
						v2[i] = m.at(i, col);
				}
				this->_solutions.free_vect = v2;
			}
		}
	}
	return this->_solutions;
}

template<typename T>
libtensor_namespace::matrix<T> attach(libtensor_namespace::matrix<T> A, libtensor_namespace::vect<T> b)
{
	libtensor_namespace::matrix<T> _result;
	if (A.get_size(1) == b.get_size)
	{
		_result		= {A.get_size(0), A.get_size(1) + 1};
		for (int i = 0; i < A.get_size(0); i++)
		{
			for (int j = 0; j < A.get_size(1) - 1; j++)
				_result[i][j] = A[i][j];
			_result[i][A.get_size(1) - 1] = b[i];
		}
	}
	return _result;
}

template<typename T>
void solve_engine<T>::clear()
{
	this->coeff.clear();
	this->constant.clear();
	this->col		= 0;
	this->row		= 0;
	this->is_solve_already	= false;
	deinit(this->_solutions);
}


template<typename T>
solve_engine<T>::~solve_engine()
{
	this->clear();
}
}
#endif

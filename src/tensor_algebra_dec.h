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
#include <tensor.h>
#include <tensor_matrix.h>
#include <map>
//#include <typetraits>
#ifndef LIBTENSOR_ALGEBRA_DECLARATION
#define LIBTENSOR_ALGEBRA_DECLARATION
namespace LIBTENSOR_ALGEBRA_MODULE
{
template<typename T>
struct solution
{
	//Basically the [x1, x2,... xn] vector is the sum of free vector
	//And arbitrary linear combination of vectors in result_set
	//For example we have 4 variable
	//With f = {1,3} and d = {2,4}
	//Free vect {1,2,3,4}
	//result set = z1 = {1,6,0,2} z2= {0, -3, 1, 2}
	//Then x1 = x1
	//x2 = 6 x1 - 3 x2
	//x3 = x3
	//x4 = 2 x1 + 2 x4
	//Or [x1, x2, x3, x4] = x1 * [1, 6, 0, 2] + x3 * [0, -3, 1, 2]
	//It is easy to say that 
	//zj[i] = 1 if i in F and i = f[j]
	//zj[i] = 0 if i in F and i != f[j]
	//xj[i] = -m[k, f[j]] if i in D and i = d[k]
	//
	std::map<int, libtensor_namespace::vect<T>> result_set;	//n-r vectors 
	libtensor_namespace::vect<T> free_vect;	//Free vector 
	int n;			//Number of variables
	int r;			//Number of leading 1 s, or number of dependant variables
	int* f;			//Array of free variables
	int* d;			//Array of dependant variables
	bool con;		//(Why ?)
};
template<typename T>
void init_sol(solution<T>&);
template<typename T>
bool isZero(libtensor_namespace::tensor<1,T>);
template<typename T>
class solve_engine
{
	private:
		libtensor_namespace::matrix<T> coeff;
		libtensor_namespace::vect<T> constant;
		int col;
		int row;
		bool is_solve_already;
		solution<T> _solutions;
	public:
		solve_engine();
		solve_engine(libtensor_namespace::matrix<T>, libtensor_namespace::vect<T>);
		solve_engine(libtensor_namespace::matrix<T>);

		solve_engine(const solve_engine&);
		solve_engine& operator= (const solve_engine&);
		solve_engine(solve_engine&&);
		solve_engine& operator= (solve_engine&&);

		bool is_solve_yet();
		void reCalibrate();//(?why)
		bool isHomogenous();
		void print_ls();

		solution<T> solve();

		void clear();
		~solve_engine();
};
template<typename T>
libtensor_namespace::matrix<T> attach (libtensor_namespace::matrix<T> A, libtensor_namespace::vect<T> b);
}

#endif

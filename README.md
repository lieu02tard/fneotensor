
# fneotensor
A tensor (and linear algebra) manipulation library written in C++ . Written for studying purpose

## Features
* Generate and manipulate tensor fairly simple
* Perform generic tensor/matrix operation

## Quick example
```cpp
#include <tensor.h>
using namespace lsf_namespace;
int main()
{
	size_t m_size[] = {2, 2};
	tensor<double, 2> m0 = {m_size};
}
```
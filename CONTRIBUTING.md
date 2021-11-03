# Coding convention
## Git branch startegy

* **master** - points to latest public release
* **develop** - points to the development of the next release, contains tested and reviewed code
* **feature_**[name] - points to a branches with a new feature, one which is candidate for merge in to develop ( subject to rebase )
* **hotfix_**[name] - points to a branches with a fix for a particalar issue ID

## Git commit messages

* Use the present tense ("Add features ... " not "Added features ...")
* Use the imperative mood  ("Move cursors to..." not "Moves cursors to...")
* Limit the first line to 72 characters or less
* Reference issues and pull request libreally
* If your pull request fixes an existing issue, add "Fixes #ISSUENUMBER" to your pull request description

## Coding style

### Naming convention
`lowerNameCase`	
For names made of only one word, the first letter should be lowercase
For names made of multiple concatenated words, the first letter of the whole is lowercase, and the first letter for each subsequent word is capitalized

Use prefix `m_` for member variables

Use prefix `p_` for member function

### Includes
```cpp
//Class includes
#include "tensor_dec.h"

//Implement includes
#include "tensor_imp.h"

//Global includes
#include <stdio.h>
```

### Classes
```cpp
template<typename T>
class matman : public dp_matrix<T>
{
private:
	//Object
	mat_object
public:
	//Initialization
	matman();

signal:
	setunit();

request:
	int getsize();
}
```

### Pointers / References
```cpp
int* count;
const matman<T>& t_mat;
```

### Braces
```cpp
if (condition)
{
	doSomething();
	doAnotherThing();
}		
if (condition)
	doTwiceThings();
else
	doSomethingElse();
```

### Switch statement
```cpp
//Note : avoid declaring variables in a switch statement
switch (a) 
{
case 1 :
{
	doSomething();
	break;
}

default 
{
	doSomthingElse();
	break;
}
}
```


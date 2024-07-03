/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <iostream>

#include <MyClassA.h>

int main()
{
    std::cout << "Hello World";
    MyClassA a;
#ifdef WIN32
    return 0;
}
#endif

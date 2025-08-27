#ifndef MY_CLASS_H
#define MY_CLASS_H

#ifdef MYSHAREDANDSTATICLIB_STATIC
#define MyLib_EXPORT
#else
    #if defined(_WIN32) || defined(_WIN64)
        #ifdef MYSHAREDANDSTATICLIB_LIB
            #define MyLib_EXPORT __declspec(dllexport)
        #else
            #define MyLib_EXPORT __declspec(dllimport)
        #endif
    #else
        #ifdef MYSHAREDANDSTATICLIB_LIB
            #define MyLib_EXPORT __attribute__((visibility("default")))
        #else
            #define MyLib_EXPORT
        #endif
    #endif
#endif

class MyLib_EXPORT MyClass
{
public:
	MyClass();
private:
	int _myint;
};

#endif // !MY_CLASS_H

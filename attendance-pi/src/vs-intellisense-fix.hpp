// in nmake projects, Visual Studio's IntelliSense doesn't know what C++ version we're on
// so we make sure it does after checking if IntelliSense is currently parsing

#if defined(__INTELLISENSE__)
#undef __cplusplus
#define __cplusplus 201103L
#endif
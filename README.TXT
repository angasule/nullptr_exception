CATCHING NULL POINTER EXCEPTIONS WITH GCC ON LINUX
--------------------------------------------------

Recently I had to deal with some code that was compiled on MSVC and also on GCC on Linux, and unfortunately it was full of catch(...) which meant some NULL pointer exceptions were being ignored on Windows, but not on Linux.

MSVC has an option to trap NULL pointer exceptions and throw them in your program, which unfortunately tends to be abused. On Linux you get a SIGSEGV (segmentation fault), and the default handler core dumps.

The ideal solution would be to fix the code and have no bugs, but good luck with that :-) Especially with a large code base this is a big problem, it's better to get consistent, if erroneous, behaviour.

The way to get a NULL pointer exception on Linux was not very complicated, the two ingredients are an option for GCC, non-call-exceptions, and a signal handler for SIGSEGV. Since SIGSEGV is a synchronous signal this is perfectly safe, and it will look like a NULL dereference is causing the exception.

The code is quite short and easy to follow, as you can see I added two cases, the first try-catch will cause an NPE, which will be caught, while the second will cause a non-NULL segmentation fault, which our signal handler will handle the default way (a core dump). You could convert all segmentation faults to exceptions, but a non-NULL one is usually an indication of a much worse error.

Remember to compile this with the non-call-exceptions flag:
g++ main.cpp -g -lpthread -Wall -fnon-call-exceptions -O2 -o main


ORIGINAL SOURCE
---------------

https://gcc.gnu.org/ml/gcc-help/2011-08/msg00261.html

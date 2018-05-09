This is a tool library for quickly building C++ programs.

1. is currently version 0.1.1 and supports the following features:

     1.1 Use C++11 standard;
     1.2 On the basis of log4cplus has been further packaged to simplify the log function;
     1.3 To provide a simple universal finite state machine implementation;

2. Compile:

     2.1 Dependent library:
         2.1.1. c++11;
         2.1.2 log4cplus;

     2.2 Build:
         2.2.1. use cmake system;
         2.2.2. Currently compiled Debug version and Release version:
             a. create a new build directory in the source code directory;
             b. Debug compilation: Enter the build directory and execute cmake -DCMAKE_BUILD_TYPE=Debug ../;
             c. Release compilation: Go to the build directory and execute cmake -DCMAKE_BUILD_TYPE=Release ../;
             2.2.4 Executing the make Command
         2.2.3. Test;
             a. provides the corresponding test instance in the examples directory;

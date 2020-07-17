[中文说明](https://github.com/DavidLiuXh/kuafu/blob/master/README.cn.md)

This is a tool library for quickly building C++ programs.

A. is currently version 0.1.1 and supports the following features:

     1. Use C++11 standard;
     2. On the basis of log4cplus has been further packaged to simplify the log function;
     3. To provide a simple universal finite state machine implementation;

B. Compile:

     Dependent library:
         1.1 c++11;
         1.2 log4cplus 1.2.x;

     Build:
         2.1 use cmake system;
         2.2 Currently compiled Debug version and Release version:
             2.2.1 create a new build directory in the source code directory;
             2.2.2 Debug compilation: Enter the build directory and execute cmake -DCMAKE_BUILD_TYPE=Debug ../;
             2.2.3 Release compilation: Go to the build directory and execute cmake -DCMAKE_BUILD_TYPE=Release ../;
             2.2.4 Executing the make Command
         2.3 Test;
             2.3.1 provides the corresponding test instance in the examples directory;

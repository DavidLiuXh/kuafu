#! /bin/bash

gcc test_fsm.cc -I../../ -lstdc++ -L/usr/local/lib -llog4cplus -L../../lib/debug -lkuafu -Wl,-rpath ../../lib/debug -Wl,-rpath /usr/local/lib -o test_fsm

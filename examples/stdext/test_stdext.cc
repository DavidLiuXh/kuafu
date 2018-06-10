#include <string>
#include <iostream>
#include <functional> 

#include "stdext/apply.h"

int TestFunc(int a, const std::string& s) {
    std::cout << "a: " << a << " | str: " << s << std::endl;
    return 100;
}

int main(int argc, char* argv[]) {
    auto pars = std::make_tuple(100, "hello");
    std::function<int(int, const std::string&)> func = TestFunc;

    auto value = kuafu::stdext::Apply(func, pars);
    std::cout << "value: " << value << std::endl;

    return 0;
}

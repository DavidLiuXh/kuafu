
这是一个工具库,用于快速构建C++程序.

一.目前是版本0.1.1，支持以下特性：

    1.使用C++11标准;
    2.在log4cplus基础上作了进一步封装,简化了日志功能;
    3.提供一个简单的通用有限状态机的实现;

二.编译：

    依赖的库：
        1.1 c++11;
        1.2 log4cplus 1.2.x;

    构建：
        2.1 使用cmake系统;
        2.2 目前编译了Debug版本和Release版本：
            2.2.1 在源代码目录下新建 build 目录;
            2.2.2 Debug编译：进入 build 目录，执行 cmake -DCMAKE_BUILD_TYPE=Debug ../；
            2.2.3 Release编译：进入 build 目录，执行 cmake -DCMAKE_BUILD_TYPE=Release ../；
            2.2.4 执行 make 命令
        2.3 测试;
            2.3.1 在 examples 目录下提供了相应的测试实例;
            


# Introduction

This is a port from an old application that was original written in a mix of C++14, Java and C++/CX.

Originaly the goal was to use a simple implementation of the A* search algorithm as basis for
experimenting with writing a portable C++ based application application between
Android and Windows (UWP) mobile platforms, as learning process for what it would entail to make it
without using relying on third party libraries.

Now with the desmise of C++/CX and my focus on Windows development, I have decided to port the application from
C++/CX into C++/WinRT.

With the goal being to learn about how to write proper C++/WinRT, the application remains as bare bones as in the
previous learning experience, and got thiner as I removed the Android related code.

There are thousands of fancy demo samples with tile engines and A*, this is not one of them.

# Code Structure

The code is written in C++/WinRT, targeting Visual 2019 or later, composed of the following projects:

AStarDemo - The Windows UWP applications using C++/WinRT;

AStarDemoLib - The A* search algorithm implemented in portable modern C++ code;

AStarDemoLibTests - The unit tests for the A* library written with help of Google Tests testing framework.

# Building

It is only required to open the project solution located at *AStarDemo/AStarDemo.sln* and do a full build.

# References

The wonderful [A* tutorials](http://theory.stanford.edu/~amitp/GameProgramming/) from Amit Patel.

# License

This example application is under GPL2 (except for the wizard generated code for Windows lifecycle management).
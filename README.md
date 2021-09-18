# modern.cpp.logger
Modern C++ logger classes for logging functions (thread-safe) in most native and modern C++17.

## Features
- Log a message to /dev/null, stdout, file and file as xml.
- Log a message with severity of Verbose, Debug, Info, Warning, Error and Fatal.
- Log thread-safe from whole application.
- Use compile flag to avoid level below a specified level. Default is LOGGINGINFO.
- Log automatically file, line and function name from logging position (except Visual Studio builds)

## Build
```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE:STRING=Debug|Release ../modern.cpp.logger
make -j`nproc`
```

## Classes
- **FileLogger** - Loggin to a file.
- **Logger** - General definition and logging to /dev/null.
- **LoggerFactory** - Loggin to all possible types, as configured.
- **StdLogger** - Loggin to stdout.
- **XmlFileLogger** - Loggin to a file as xml.

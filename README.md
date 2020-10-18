# modern.cpp.logger
Modern C++ logger classes for logging functions in most native and modern C++17.

## Build
```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug|Release ../modern.cpp.logger
make -j`nproc`
```

## Classes
- **FileLogger** - Loggin to a file.
- **Logger** - General definition and logging to /dev/null.
- **LoggerFactory** - Loggin to all possible types, as configured.
- **StdLogger** - Loggin to stdout.
- **XmlFileLogger** - Loggin to a file as xml.

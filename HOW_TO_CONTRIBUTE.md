# How to contribute/build
## Requirements
### Windows
* Microsoft Visual Studio 2015 compiler
* CMake
* libssh2  
    * Configured with CMake
    * Built with Visual Studio 2015 compiler


    cd [root_of_libssh2]  
    mkdir bin  
    cd bin  
    cmake ..  
    cmake --build .  


### Linux (Only Ubuntu is tested)
* C/C++ compiler
* cmake


    sudo apt-get install cmake


* OpenSSL development library


    sudo apt-get install libssl-dev

* libssh2: build instructions is the same as in Windows


## Build FlushSSH
    cd [root_of_FlushSSH]
    mkdir build
    cd build
    cmake ..
    cmake --build .

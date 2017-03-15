# FlushSSH
A tool helping to run CLI commands from files on multiple servers  
[![Build status](https://ci.appveyor.com/api/projects/status/m232719gvcg0m1qc?svg=true)](https://ci.appveyor.com/project/DavyVan/flushssh)
[![Build Status](https://travis-ci.org/DavyVan/FlushSSH.svg?branch=master)](https://travis-ci.org/DavyVan/FlushSSH)  


# Installation
Windwos: Use the setup.exe  
Linux: For now, you must compile from source, please see CONTRIBUTING.md  


# Usage & Featheres
    FlushSSH {hosts_file_path} {cmd_file_path}  
**hosts_file_path**: A space separated file which contains all of the hosts you want to connect, please use the following format.  

    +--------------+----------+-----+-----------------------------+   
    |hostname (IP) | username | P/K | {password | key_file_paths} |
    +--------------+----------+-----+-----------------------------+

* To find a example, please see hosts_file_example.txt
* "hostname" can only be IP address, for now.
* "P/K" (Capital): "P" if this host use password to authenticate and follows the password after a space, or "K" if it uses public/private key pair and follows two file path (public key & private key, only separated files supported) after a space.

**cmd_file_path**: Contains all of the commands you want to execute on remote host(s). The format is quite simple with one command per line. e.g.

    cd ~
    mkdir myfolder

For now, some limitations:
* all of the hosts share the only one command file.  
* Only support authentication with password.  
* Connect and execute one by one.
* Store all of the echo in files, one host one file, named after hosts' hostname(IP).  
* Only support port 22.
* Only with IPv4

Will be better in future versions.  
# Update history
## V1.6 @ 2017.x.xx (current release)
* 


## V1.0 @ 2017.2.26 (1st release)
* Add Travis-CI & Appveyor test
* Add SIGINT handling
* Add installer for Windows Vista+


## V0.8 @ 2017.2.22 (Initial)

# FlushSSH
A tool helping to run CLI commands from files on multiple servers  
[![Build status](https://ci.appveyor.com/api/projects/status/m232719gvcg0m1qc?svg=true)](https://ci.appveyor.com/project/DavyVan/flushssh)
# Usage & Feathers
    FlushSSH {hosts_file_path} {cmd_file_path}  
**hosts_file_path**: contains all of the hosts you want to connect, please use the following format.  

    +---------------+----------+----------+   
    |hostsname (IP) | username | password |
    +---------------+----------+----------+

**cmd_file_path**: Contains all of the commands you want to execute on remote host(s). The format is quite simple with one command per line. e.g.

    cd ~
    mkdir myfolder

For now, some limitations:
* all of the hosts share the only one command file.  
* Only support authentication with password.  
* Connect and execute one by one.
* Store all of the echo in files, one host one file, named after hosts' hostname(IP).  
* Only support port 22.
* Only on Windows, only Windows 10 is tested.

Will be better in future versions.  
## Update history
### V0.8 @ 2017.2.22 (Initial)

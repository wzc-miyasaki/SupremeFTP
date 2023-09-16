# SupremeFTP

EE542 Lab4  Fast, Reliable File Transfer

&nbsp;
&nbsp;

#### Some useful Commands

Generate a file filled with random bites:

```
dd if=/dev/urandom of=data.bin bs=1M count=1024
```

Check OS memory

```
vim /proc/meminfo
```

To push the current branch and set the remote as upstream, use

```git
git checkout <your_local_branch_Name>
git push --set-upstream origin <your_local_branch_Name>
```

Check && Change the MTU of interface on Linux

```bash
sudo up link show dev <interface_Name>
cat  /sys/class/net/<interface_name>/mtu //or

sudo ip link set dev <interface_Name> mtu 1500 // change MTU
```

&nbsp;
&nbsp;

#### Install CMake

```bash
sudo apt update && sudo apt upgrade
sudo apt install cmake
```

&nbsp;
&nbsp;

#### Build the project

Create a directory named `build` at root directory `SupremeFTP/`.  Then `cd build/`. To build the project using CMake, use the following command

```bash
cmake ..
make
```

To build for debug in CMake, run cmake like this:

```bash
cmake -DCMAKE_BUILD_TYPE=Debug <path>
```

If successfully build up the project, the executable files `client` and `server` will be created at the path `SupremeFTP/build/test/` 

&nbsp;

#### Running program

To start the server, it takes two parameter

```bash
./server <port number> <window size>
```

- window size = Bandwidth delay product / packet size

&nbsp;

To start the client, it takes 2 parameters

```bash
./client  <server ip>  <port number> 
```

After the client has been executed, the user will be asked to provide the file name that the client expect to download from the server. 

&nbsp;

**Modify the Buffer size and RTT**

1.  The buffer size define the size of the payload inside *Packet* structure. It can be manually modified through the macro `BUFFER_SIZE` located at the header file `SupremeFTP/include/NetworkAnalyzer/NetworkAnalyzer.h`.  

    > Note:  the buffer size should be **< MTU** to ensure that the data can be inserted into only one packet. 

2. Similarly, to modify the RTT, we need to manually change the value of the macro `RTT` located at `SupremeFTP/include/NetworkAnalyzer/NetworkAnalyzer.h` , which is same place where the BUFFER_SIZE is defined. 





&nbsp;

A good CMake tutorial video link:  [here](https://youtu.be/AJRGU_XgVMQ)

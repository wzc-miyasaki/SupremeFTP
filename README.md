# SupremeFTP

EE542 Lab4  Fast, Reliable File Transfer


#### Some useful Commands
Generate a file filled with random bites:
```
dd if=/dev/urandom of=data.bin bs=1M count=1024
```


#### Install CMake

```bash
sudo apt update && sudo apt upgrade
sudo apt install cmake
```



#### Build the project

Create a directory named `build` at root directory `SupremeFTP/`.  Then `cd build/`. To build the project using CMake, us the following command

```bash
cmake ..
```

 之后在 build 目录下运行 makefile

```bash
make
```
这样整个程序就被安装在 build 目录下，如果安装成功，`build/test/` 目录下应该有一个 `main` executable file，该文件用于测试，可返回当前 hostname.

To build for debug in CMake, run cmake like this:
```bash
cmake -DCMAKE_BUILD_TYPE=Debug <path>
```

A good CMake tutorial video link:  [here](https://youtu.be/AJRGU_XgVMQ)

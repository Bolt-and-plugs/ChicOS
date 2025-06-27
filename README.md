# ChicOS(S)

This is a college Operating Systems class project, meanwhile a playground for us to taste the feeling of being a kernel programmer (roughly). It represents a Operating System Simulator with the following features:
- Round Robin Scheduler
- Process API (create, interrupt, kill and so on)
- Disk and Printer Simulation (with SSTF and FIFO politics)
- Memory API (alloc and dealloc) inside a fixed memory pool
- Semaphore simulation for synthetic programs
- UI for better visualization
- Single User handling (with password auth)
- Adaptative UI (Ui adapt to screen)
- Built-in Logger

![image](https://github.com/user-attachments/assets/63cdfe62-b072-4660-9e18-0b31182a6a8f)


# Testing the project

To test the project for yourself:

> you will need ncurses and cmake, install based on your linux distro/package manager

debian/ubuntu
```sh
sudo apt-get install libncurses5-dev libncursesw5-dev cmake libsodium-dev

```

arch based
```sh 
sudo pacman -S ncurses cmake libsodium

```

windows

> just install [cmake](https://cmake.org/download/) and follow the rest

---

Then, compile the code with cmake:


```sh

# for debug mode
cmake -DCMAKE_BUILD_TYPE=DEBUG -S . -B target/ 

# for release mode
cmake -DCMAKE_BUILD_TYPE=RELEASE  -S . -B target/ 

cmake --build target/

./target/ChicOS

```

- To get help type
```sh
    ./target/ChicOS -h
```

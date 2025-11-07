<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->
<a id="readme-top"></a>

<!-- PROJECT LOGO -->
<br />
<div align="center">

<h3 align="center">RISCV Kernel</h3>

  <p align="center">
    A fully functional implementation of a multi-threaded preemptive kernel built for RISCV architecture.
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
    </li>
    <li><a href="#getting-started">Getting Started</a></li>
    <li><a href="#usage">Usage</a></li>
    <li>
        <a href="#components">Components</a>
        <ul>
            <li><a href="#memory-allocator">Memory Allocator</a></li>
            <li><a href="#thread">Thread</a></li>
            <li><a href="#scheduler">Scheduler</a></li>
            <li><a href="#semaphore">Semaphore</a></li>
            <li><a href="#console">Console</a></li>
        </ul>
    </li>
    <li>
        <a href="#api">API</a>
        <ul>
            <li><a href="#c-api">C API</a></li>
            <li><a href="#cpp-api">C++ API</a></li>
        </ul>
    </li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

A fully functional implementation of a multi-threaded preemptive kernel built for the RISCV architecture.
It supports multiple threads executing on a single core with asynchronous context changes happening on every N clock pulses.
For a more detailed explanation of all the components refer to <a href="#Components">Components</a>.
For an explanation of the API which you can use to write code on top of the kernel refer to <a href="API">API</a> section.

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- GETTING STARTED -->
## Getting Started

1. Install prerequisites
* shell
    ```shell
    
    ```

2. Clone the repo
* shell
    ```shell
    git clone https://github.com/dusanveljkovic/RISCV-Kernel 
    ```

3. Build and run the kernel in qemu using `make`
* shell
    ```shell
    make qemu
    ```

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- USAGE EXAMPLES -->
## Usage

To run your own code on top of the kernel put it inside the `kmain(void*)` function in the `src/main.cpp` file, then run the command 
to build and run the kernel `make qemu`.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- Components -->
## Components

### Memory Allocator
Memory allocation is handled by a singleton class named `MemoryAllocator` which uses First-Fit continual allocation to allocate the required memory.
It must first be initialized, and then the `mem_alloc(size_t)` and `mem_free(void*)` functions can be called to allocated and free the memory.
The list of free blocks of memory is represented as a linked list whose head is stored in the class. 

### Thread
Thread is a represented by a struct named `TCB` (Thread Control Block) and stores information about the stack location, the function which the thread is executing 
and its argument, its state as in is it running, blocked, ready, sleeping or finished. It also includes a `TrapFrame` which is a struct that holds the values of 
all of the registers at the time of a context change.
A thread can be created, dispatched, exited or put to sleep.

### Scheduler
Scheduler is a class whose job it is to keep track of all the ready threads, put new ones and get the next thread ready for execution. Threads are oranized 
in a linked list where the head is stored for easy getting of the next thread, and the tail is stored for easy putting of a thread.

### Semaphore
Semaphores are used to allow synchronization of threads by limiting the number of threads that can execute a section of a code concurrently. Semaphore can be 
created and other threads can than `wait` on the semaphore or `signal` that other threads can execute the code and stop waiting. When a thread is waiting it 
is put in a linked list of blocked threads and is not put in a list of ready threads as to not waste cpu time on doing nothing. When another thread signals, 
the threads waiting is taken out of the list of blocked threads and put into the list of ready ones to execute.

### Console
Console can be used to write stuff to the user and also get the users input using `putc(char)` and `getc()`. Printing of the characters is handled by a 
special thread that is executed when the kernel starts.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- LICENSE -->
## License

Distributed under the "AS-IS" lincese. See `LICENSE` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- CONTACT -->
## Contact

Dusan Veljkovic - dusanveljkovickv@gmail.com

Project Link: [https://github.com/dusanveljkovic/RISCV-Kernel](https://github.com/dusanveljkovic/RISCV-Kernel)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

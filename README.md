<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->
<a id="readme-top"></a>
<!--
*** Thanks for checking out the Best-README-Template. If you have a suggestion
*** that would make this better, please fork the repo and create a pull request
*** or simply open an issue with the tag "enhancement".
*** Don't forget to give the project a star!
*** Thanks again! Now go create something AMAZING! :D
-->



<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![project_license][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]



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
        <li>
            <a href="#memory-allocator">Memory Allocator</a>
            <a href="#thread">Thread</a>
            <a href="#scheduler">Scheduler</a>
            <a href="#semaphore">Semaphore</a>
            <a href="#console">Console</a>
        </li>
    </li>
    <li>
        <a href="#api">API</a>
        <li>
            <a href="#c-api">C API</a>
            <a href="#cpp-api">C++ API</a>
        </li>
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
* bash
    ```
    
    ```

2. Clone the repo
* bash
    ```
    git clone https://github.com/dusanveljkovic/RISCV-Kernel 
    ```

3. Build and run the kernel in qemu using `make`
* bash
    ```
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



<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Top contributors:

<a href="https://github.com/github_username/repo_name/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=github_username/repo_name" alt="contrib.rocks image" />
</a>



<!-- LICENSE -->
## License

Distributed under the "AS-IS" lincese. See `LICENSE` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- CONTACT -->
## Contact

Dusan Veljkovic - dusanveljkovic@gmail.com

Project Link: [https://github.com/dusanveljkovic/RISCV-Kernel](https://github.com/dusanveljkovic/RISCV-Kernel)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

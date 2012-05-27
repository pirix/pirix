# πrix

This is an attempt to create a clean and compact operating system for
the ARM architecture. πrix is POSIX based and will target the
Raspberry Pi as first supported physical device.

## Features

So far, the following features are more or less stable:

    - memory management with paging
    - interrupt handling
    - multitasking
    - filesystem

## Building

In order to build πrix you have to install the system-specific
toolchain. A Makefile with the necessary patches is available on
https://github.com/7h0ma5/pirix-toolchain/. To compile and install the
toolchain, switch to the pirix-toolchain directory and run the
following commands:

    make binutils-make
    sudo make binutils-install

    make gcc-make
    sudo make gcc-install

    make newlib-make
    make newlib-install

Now you are ready to compile the kernel. πrix uses cmake and it's
better to run and out-of-source build to keep the source directory
clean. To compile the system run the following commands:

    mkdir build && cd build
    cmake path/to/pirix/source
    make

If everything went well and you have qemu-system-arm installed you can
just type `make qemu` to give πrix a test run.

## License

Copyright (C) 2012 Thomas Gatzweiler

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
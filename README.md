# πrix

Pirix is an attempt to create a microkernel-based, POSIX-compatible
operating system.

## Features

So far, the following features are more or less stable:

    - memory management with paging
    - interrupt handling
    - multitasking

The following features are currently under development:

    - inter-process communication

## Supported Platforms

    - i386
    - ARM
        - QEMU Integrator/CP
        - Raspberry Pi

## Building

In order to build Pirix you have to install the system-specific
toolchain. A script to build the toolchain is available on
https://github.com/pirix/pirix-toolchain/. To compile and install the
toolchain, switch to the pirix-toolchain directory and run the
following commands:

    $ ./prepare.sh
    $ ./build.sh
    # ./install.sh

Now you are ready to compile the kernel. Pirix uses
[SCons](http://scons.org) which is wrapped by a Makefile. To start the
build process simply type `make` in the source directory.

If everything went well and you have *qemu* and *grub2* installed you
can just type `make qemu` to give Pirix a test run.

## License

Copyright (C) 2012-2013 Thomas Gatzweiler

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
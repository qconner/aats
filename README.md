aats
====

This C++ / boost program is a multi-threaded Ascii Art Telnet Server.
It "plays back" ANSI or VT100 escape sequence files at 9600 baud or
other reduced speed (simulated baud rate).  Users would run telnet
to access this server.

It requires boost and has been tested only on MacOSX, to date.

Usage
-----
./vt100_server tcp_port_number baud_rate file_to_send

Building for MacOSX
-----
./BUILD.sh



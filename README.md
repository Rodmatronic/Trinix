# Trinix

Trinix is my hobby Unix-like kernel that aims to be loosely compatible with Linux. It has a modified MUSL userspace at the moment and can run some simple programs, and requires at least 128 MB of ram to be functional (Though will run with less.) The name is Tri Unix since this kernel my third attempt at making a Unix-like based on XV6.

To build an executable for Trinix, it must have a small entrypoint (0x10000 - 0x15000) and please bear in mind that regular Linux executables will not run natively.

Any contributions are greatly appreciated.


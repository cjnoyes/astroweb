astroweb
========

Astrology engine in C++

Originally written in Visual C++ as a part of a larger astrology program Astro For Windows

Requires g++ compiler from the gcc compiler collection and make. 
Compiles on windows using Mingw

Takes input and configuration and writes output data to and from .ini style files

To Install, create a directory, copy the contents of the data directory into a subdirectory data. 
Create a subdirectory session

astroweb --type <type> --job <key> [--home <dir>]

If not provided home directory is assumed to be the directory from where astroweb is running

--job is the key for job or session.

--type is one of natal, compat, compos, trans, progr

From the session directory...

configuration settings comes from <key>.conf

Input parameters come from <key>.in

output of program is written to <key>.dat


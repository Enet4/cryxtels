# Crystal Pixels - modern version

Crystal Pixels (Cryxtels in short) is a simulator game where you live in a dark and cold microcosm. It was imagined and brought to life by Alessandro Ghignola, who is mostly known for developing the space simulator game Noctis. You can fly to small spaces called pixels, with several other features, such as walking around them and creating pixels and objects.

Cryxtels was initially an MS-DOS game. Later on, a port was developed in order to be playable in modern systems such as Linux. This repository contains the latest development versions of this port.

At the moment, this version was only tested to work on Windows 8 and Linux.

## Building

Run *configure* to generate the Makefile's, then run *make*.
Alternatively, use the custom Makefile under the name *Makefile.bak*.

SDL 2.0.3 must be installed in the system. C++11 must be supported by the compiler.

Other building procedures for more operating systems could be available eventually.

## Installing

Place the compiled executable in the bin directory. Symlinks for running cryxtels in the console can be made manually.

## Running

Before you start playing, please read *crystal pixels.txt* and *readme new.txt* to understand how to play.
To run the game, simply execute cryxtels. Make sure that PIXELS.DEF and all .ATM files are in the same directory. Simple Directmedia Library version 2 must be installed in the system.

## License

GNU Public License v3

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

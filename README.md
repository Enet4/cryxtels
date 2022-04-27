# Crystal Pixels - modern version

![Screenshot from Crystal Pixels](screenshot.png)

[![CMake](https://github.com/Enet4/cryxtels/actions/workflows/ci.yml/badge.svg)](https://github.com/Enet4/cryxtels/actions/workflows/ci.yml)

Crystal Pixels (Cryxtels in short) is a simulator game where you live in a dark and cold microcosm. It was imagined and brought to life by Alessandro Ghignola, who is mostly known for developing the space simulator game Noctis. You can fly to small spaces called pixels, with several other features, such as walking around them and creating pixels and objects.

Cryxtels was initially an MS-DOS game. Later on, a port was developed in order to be playable in modern systems such as Linux. This repository contains the latest development versions of this port.

At the moment, this version was only tested to work on Windows 8 and Linux.

## Building

SDL 2.0.3 must be installed in the system.

CMake 3.10 or greater is required.
Run `cmake` to generate the project scaffold
(usually GNU Makefiles on Linux platforms),
then build the project.

Example on Linux:

```sh
mkdir Release
cd Release
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

The output is the executable file `cryxtels`.

The compiler must support standard C++11.

## Installing

Place the compiled executable in the bin directory. Symlinks for running cryxtels in the console can be made manually.

## Running

Before you start playing, please read *crystal pixels.txt* and *readme new.txt* to understand how to play.
To run the game, execute `cryxtels` while ensuring that
PIXELS.DEF and all .ATM files are in your current working directory
(these are all available here in the [bin](bin) folder).
Simple Directmedia Library version 2 must be installed in the system.

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

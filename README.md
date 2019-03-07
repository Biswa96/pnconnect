# pnconnect

Experiments with 9P file server in Windows 10 for fun

## How to build

Clone this repository. Open the solution (.sln) or project (.vcxproj) file
in Visual Studio and build it. Alternatively, run Visual Studio developer
command prompt, go to the cloned folder and run this command: `msbuild.exe`.
You can also build with mingw-w64 toolchain using `make` command. 

### Build p9np export libraries:

1. Generate DEF file: `gendef /c/Windows/System32/p9np.dll` 
2. For MSVC: `lib /DEF:p9np.def /MACHINE:X64` 
3. For GCC: `dlltool -D /c/Windows/System32/p9np.dll -d p9np.def -l libp9np.a` 

## License

This project is licensed under [GPLv3](LICENSE). 

    pnconnect -- Copyright (C) 2019 Biswapriyo Nath

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

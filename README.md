# Satellite Pass Prediction Software
This program will display visible pass times for a given satellite (>10 degrees elevation). Uses the given TLE in a file or queries celestrak.com for an updated TLE given a NORAD ID. Can be configured to output the generated information to a file for any number of days.

# Pre-requisites
__For Windows, Mingw-w64 is required to compile and / or run the program.__

To install Mingw-w64, follow steps 3 and 4 under the 'Prerequisites' section of the following document, which are also listed below:  
https://code.visualstudio.com/docs/cpp/config-mingw  

1. Proceed to https://www.msys2.org/.
2. Follow the instructions under the red "Installation" header beginning with downloading the MSYS2 installer. For clarity, some of those instructions have been mirrored below.
3. Once the MSYS2 terminal has opened automatically, run the following command:  
   `pacman -Syu`  
   _Note that this may take a minute..._

4. Once the MSYS2 terminal closes, use the Windows Start menu to search for and open "MSYS2 MSYS".
5. Once the MSYS2 terminal has opened, run the following command:  
   `pacman -Su`  
    _Note that this may take a minute..._   
6. To install mingw-w64, run the following command:  
   `pacman -S --needed base-devel mingw-w64-x86_64-toolchain`  
   _Note that this may take a minute..._   
7. Now that Mingw-w64 is installed, it must be added to the windows `PATH` environment variable. To do so, begin by making note of the path to Mingw-w64's `bin` folder. By default it is `C:\msys64\mingw64\bin`.
8. Search "Edit the system environment variables" in the Windows Start menu and select the entry of the same name.
9. Click the "Environment Variables..." option.
10. Find the variable labeled "Path", click the entry, and then click the "Edit..." button.
11. Click the "New" button and paste in the Mingw-w64 path found in step 7.
12. Select OK to confirm. Close and reopen any active console windows.
  
# Compilation
Linux Terminal:  
Run `make` in the directory.  
  
Windows:  
Double click `make.bat`.
  
# Use

Valid usages (REQuired, OPTional):  
1. `./predict.out`  
2. `./predict.out {Five-Digit NORAD ID or Input File}`  
3. `./predict.out {Five-Digit NORAD ID or Input File} {Output File}`  
4. `./predict.out {Five-Digit NORAD ID or Input File} {Output File} {Days to Predict}`  

First, ensure your two line element is pasted into an appropriate file. Two .tle files are given as examples.

Next, run one of the following commands.

Linux Terminal:  
`./predict.out {TLE File} {Days to Generate}`  
Will print to terminal {Days to Generate} days worth of pass predictions for the object whose TLE is is {TLE File}.
  
`./predict.out {TLE File} {Days to Generate} {Output File}`  
Will print to the file passed as {Output File} and to the terminal {Days to Generate} days worth of pass predictions for the object whose TLE is is {TLE File}.

Windows Command Prompt:  
`predict.out {TLE File} {Days to Generate}`  
Will print to terminal {Days to Generate} days worth of pass predictions for the object whose TLE is is {TLE File}.
  
`predict.out {TLE File} {Days to Generate} {Output File}`  
Will print to the file passed as {Output File} and to the terminal {Days to Generate} days worth of pass predictions for the object whose TLE is is {TLE File}.

# Licensing

    Copyright (C) 2021  Mit Bailey

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
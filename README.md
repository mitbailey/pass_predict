# Satellite Pass Prediction Software
This program will display visible pass times for a given satellite (>10 degrees elevation). Uses the given TLE in a file or queries celestrak.com for an updated TLE given a NORAD ID. Can be configured to output the generated information to a file for any number of days.

# Pre-requisites
__Mingw-w64 must be installed via MSYS2__  
Please follow steps 3 and 4 under the 'Prerequisites' section of the following document:  
https://code.visualstudio.com/docs/cpp/config-mingw
  
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
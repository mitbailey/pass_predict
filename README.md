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

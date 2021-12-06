# Satellite Pass Prediction Software
This program will display visible pass times for a given satellite.

No Args
- Prompt the user to input a NORADID or filename
One Arg
- Check if user entered NORADID or filename
- NORADID
    - Try to grab TLE from online
        - Can't connect, fail.
- Filename
    - Try to grab TLE from file
        - No or invalid TLE, fail.

## Pre-requisites
Steps 3 and 4 under the 'Prerequisites' section of the following document:  
https://code.visualstudio.com/docs/cpp/config-mingw
  
## Compilation
Linux Terminal:  
Run `make` in the directory.  
  
Windows:  
Double click `make.bat`.
  
## Use
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

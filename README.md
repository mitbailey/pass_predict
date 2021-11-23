# Satellite Pass Prediction Software
This program will display visible pass times for a given satellite.

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

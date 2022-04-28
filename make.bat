@ECHO OFF
REM Variables relevant to the compilation of the program.
SET CXX=g++
SET CPPOBJS=src/predict.cpp SGP4/src/CoordGeodetic.cpp SGP4/src/CoordTopocentric.cpp SGP4/src/DateTime.cpp SGP4/src/DecayedException.cpp SGP4/src/Eci.cpp SGP4/src/Globals.cpp SGP4/src/Observer.cpp SGP4/src/OrbitalElements.cpp SGP4/src/SatelliteException.cpp SGP4/src/SGP4.cpp SGP4/src/SolarPosition.cpp SGP4/src/TimeSpan.cpp SGP4/src/Tle.cpp SGP4/src/TleException.cpp SGP4/src/Util.cpp SGP4/src/Vector.cpp
SET EDCXXFLAGS=-I ./ -I ./include/ -I ./SGP4/src/ -I ./SGP4/include/ -I ./SGP4/passpredict/ -I ./SGP4/sattrack/ -Wall
SET EDLDFLAGS=-lurlmon
SET TARGET=predict.exe

REM This run a command-prompt command to compile using G++.
ECHO Compiling %CPPOBJS% into %TARGET% using %CXX% with %EDCXXFLAGS% flag(s)...
CMD /c "%CXX% %EDCXXFLAGS% %CPPOBJS% -o %TARGET% %EDLDFLAGS%"

REM This will run the executable automatically.
@REM START %TARGET%
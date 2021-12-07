@ECHO OFF
REM Variables relevant to the compilation of the program.
SET CXX=g++
SET CPPOBJS=src/predict.cpp SGP4/libsgp4/CoordGeodetic.cc SGP4/libsgp4/CoordTopocentric.cc SGP4/libsgp4/DateTime.cc SGP4/libsgp4/DecayedException.cc SGP4/libsgp4/Eci.cc SGP4/libsgp4/Globals.cc SGP4/libsgp4/Observer.cc SGP4/libsgp4/OrbitalElements.cc SGP4/libsgp4/SatelliteException.cc SGP4/libsgp4/SGP4.cc SGP4/libsgp4/SolarPosition.cc SGP4/libsgp4/TimeSpan.cc SGP4/libsgp4/Tle.cc SGP4/libsgp4/TleException.cc SGP4/libsgp4/Util.cc SGP4/libsgp4/Vector.cc
SET EDCXXFLAGS=-I ./ -I ./include/ -I ./SGP4/libsgp4/ -I ./SGP4/passpredict/ -I ./SGP4/sattrack/ -Wall
SET EDLDFLAGS=-lurlmon
SET TARGET=predict.exe

REM This run a command-prompt command to compile using G++.
ECHO Compiling %CPPOBJS% into %TARGET% using %CXX% with %EDCXXFLAGS% flag(s)...
CMD /c "%CXX% %EDCXXFLAGS% %CPPOBJS% -o %TARGET% %EDLDFLAGS%"

REM This will run the executable automatically.
@REM START %TARGET%
CXX = g++
CC = gcc
CPPOBJS = src/predict.o SGP4/src/CoordGeodetic.o SGP4/src/CoordTopocentric.o SGP4/src/DateTime.o SGP4/src/DecayedException.o SGP4/src/Eci.o SGP4/src/Globals.o SGP4/src/Observer.o SGP4/src/OrbitalElements.o SGP4/src/SatelliteException.o SGP4/src/SGP4.o SGP4/src/SolarPosition.o SGP4/src/TimeSpan.o SGP4/src/Tle.o SGP4/src/TleException.o SGP4/src/Util.o SGP4/src/Vector.o
COBJS =
EDCFLAGS := -std=gnu11 -O2 $(CFLAGS)
EDCXXFLAGS = -I ./ -I ./include/ -I ./SGP4/src/ -I ./SGP4/include/ -Wall -pthread $(CXXFLAGS)
EDLDFLAGS := -lpthread -lm $(LDFLAGS)
TARGET = predict.out

all: $(COBJS) $(CPPOBJS)
	$(CXX) $(EDCXXFLAGS) $(COBJS) $(CPPOBJS) -o $(TARGET) $(EDLDFLAGS)

%.o: %.cpp
	$(CXX) $(EDCXXFLAGS) -o $@ -c $<

%.o: %.c
	$(CC) $(EDCFLAGS) -o $@ -c $<

.PHONY: clean

clean:
	$(RM) *.out
	$(RM) *.o
	$(RM) src/*.o
	$(RM) SGP4/*.o
	$(RM) SGP4/src/*.o

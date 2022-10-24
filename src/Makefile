AR       := ar
ARFLAGS  := cr
CXX      := g++
CPPFLAGS := -O2 -I../lib

all : ../bin/regionquery
	@echo -n ""

clean :
	rm *.o ../bin/regionquery

../bin/regionquery: main.o polygon.o polygon1.o polygon2.o polygon3.o polygon4.o polygon5.o 
	$(CXX) $^ -o $@

main.o : main.cpp polygon.h
	$(CXX) $(CPPFLAGS) -c $< -o $@

polygon.o : polygon.cpp polygon1.cpp polygon2.cpp polygon3.cpp polygon4.cpp polygon5.cpp polygon.h 
	$(CXX) $(CPPFLAGS) -c $< -o $@

polygon1.o : polygon1.cpp polygon.h 
	$(CXX) $(CPPFLAGS) -c $< -o $@

polygon2.o : polygon2.cpp polygon.h 
	$(CXX) $(CPPFLAGS) -c $< -o $@

polygon3.o : polygon3.cpp polygon.h 
	$(CXX) $(CPPFLAGS) -c $< -o $@

polygon4.o : polygon4.cpp polygon.h 
	$(CXX) $(CPPFLAGS) -c $< -o $@

polygon5.o : polygon5.cpp polygon.h 
	$(CXX) $(CPPFLAGS) -c $< -o $@


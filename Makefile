main: main.cpp tester/tester.cpp
	g++ -o main.o main.cpp tester/tester.cpp
clean:
	rm *.txt *.o *.out
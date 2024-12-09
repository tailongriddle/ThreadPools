
test: tPool.o poolTester.o
	g++ tPool.o poolTester.o -std=c++20 -pthread -lm -o test

tPool.o: tPool.cc tPool.h
	g++ tPool.cc -std=c++20 -pthread -c

poolTester.o: poolTester.cc tPool.h
	g++ poolTester.cc -std=c++20 -pthread -c

clean:
	rm -f *.o 
	rm -f test
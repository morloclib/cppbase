all:
	g++ -Wall -std=c++11 -o test-core.out test-core.cpp core.hpp

.PHONY: test
test:
	g++ -Wall -std=c++11 -o test-core.out test-core.cpp core.hpp
	./test-core.out > .test-core-obs.txt
	diff .test-core-obs.txt .test-core-exp.txt


.PHONY: clean
clean:
	rm -f *gch *.out

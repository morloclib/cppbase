all:
	g++ -Wall -o test-core.out test-core.cpp core.hpp
	g++ -Wall -o test-serial.out test-serial.cpp serial.hpp

.PHONY: test
test:
	g++ -Wall -o test-core.out test-core.cpp core.hpp
	g++ -Wall -o test-serial.out test-serial.cpp serial.hpp
	./test-core.out > .test-core-obs.txt
	diff .test-core-obs.txt .test-core-exp.txt
	./test-serial.out > .test-serial-obs.txt
	diff .test-serial-obs.txt .test-serial-exp.txt


.PHONY: clean
clean:
	rm -f *gch *.out

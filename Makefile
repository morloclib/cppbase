all:
	g++ -Wall -std=c++11 -o test-core.out test-core.cpp core.hpp
	g++ -Wall -std=c++11 -o test-serial.out test-serial.cpp serial.hpp

.PHONY: test
test:
	# g++ -Wall -std=c++11 -o test-core.out test-core.cpp core.hpp
	# g++ -Wall -std=c++11 -o test-serial.out test-serial.cpp serial.hpp
	# ./test-core.out > .test-core-obs.txt
	# diff .test-core-obs.txt .test-core-exp.txt
	# ./test-serial.out > .test-serial-obs.txt
	# diff .test-serial-obs.txt .test-serial-exp.txt
	# # TEMP TEST
	g++ -Wall -std=c++11 -o test-serial.out test-serial.cpp serial.hpp
	./test-serial.out


.PHONY: clean
clean:
	rm -f *gch *.out

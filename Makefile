all:
	g++ -Wall -o test-core.out test-core.cpp core.hpp
	g++ -Wall -o test-serial.out test-serializers.cpp serializers.hpp

clean:
	rm -f *gch *.out

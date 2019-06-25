all:
	g++ -Wall serializers.cpp serializers.hpp

clean:
	rm -f *gch a.out

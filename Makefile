strip: WS2801.cpp WS2801.h strip.cpp
	g++ *.cpp -O3 -o strip

clean:
	rm -f strip


all: dirs
	cmake -S . -B build -H.
	cmake --build build 

dirs:
	mkdir -p build
	
clean:
	rm -rf build/*
	rm -f tests

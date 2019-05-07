.SUFFIXES: # disable default rules
EXECUTABLE = cloth 

.PHONY: all
all: build/bin/$(EXECUTABLE)

.PHONY: run
run: build/bin/$(EXECUTABLE)
	build/bin/$(EXECUTABLE)

.PHONY: clean
clean:
	rm -rf build

build:
	mkdir build

.PHONY: build/Makefile
build/Makefile: CMakeLists.txt | build
	(cd build; cmake -DCMAKE_BUILD_TYPE=Release ..)

.PHONY: build/bin/$(EXECUTABLE)
build/bin/$(EXECUTABLE): build/Makefile
	(cd build; make -j8)


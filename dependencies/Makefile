all: build-libvmcu unicorn/build/libunicorn.so

build-libvmcu: libvmcu-Virtual-MCU-Library
	cd libvmcu-Virtual-MCU-Library/build-release; make -f build.mk; cd ../..

unicorn/build/libunicorn.so: unicorn
	# i tried to have this be a cmake ExternalProject_Add but did not get far
	cd unicorn; mkdir build; cd build; cmake .. -DCMAKE_BUILD_TYPE=Release -DUNICORN_ARCH="x86" -DUNICORN_INSTALL=True -DCMAKE_POLICY_VERSION_MINIMUM=3.5; make
	# -DUNICORN_HAS_X86=True

libvmcu-Virtual-MCU-Library:
	git clone https://github.com/Milo-D/libvmcu-Virtual-MCU-Library.git

unicorn:
	git clone https://github.com/unicorn-engine/unicorn.git
	cd unicorn; git reset --hard d4b9248


COMMON = \
	`find src/ -type f -name "*.cc"` \
	-Isrc/ \
	-O2 \
	-std=c++11 \
	-Wall \
	-Werror \
	-Weverything \
	-Wextra \
	-Wno-covered-switch-default \
	-Wno-c++98-compat \
	-Wno-signed-enum-bitfield \
	-lsnappy \

all:
	rm -rf bin/
	mkdir -p bin/
	clang++ src/test.cpp $(COMMON) -o bin/test


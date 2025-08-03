CC=g++ -g -Wall -std=c++17

UNAME := $(shell uname -s)
ifeq ($(UNAME),Darwin)
    CC+=-D_XOPEN_SOURCE -Wno-deprecated-declarations
    LIBCPU=libcpu_macos.o
else
    LIBCPU=libcpu.o
endif

# List of source files for your thread library
THREAD_SOURCES=cpu.cpp cv.cpp mutex.cpp thread.cpp

# Generate the names of the thread library's object files
THREAD_OBJS=${THREAD_SOURCES:.cpp=.o}

all: libthread.o test5 test6 test110 test111 test205 test206 test210 test211 test212 test213 test214 test701 test702 test703 test704

# Compile the thread library and tag this compilation
libthread.o: ${THREAD_OBJS}
	./autotag.sh push
	ld -r -o $@ ${THREAD_OBJS}

# Compile an application program

# Generic Tests
test5: test5.cpp libthread.o ${LIBCPU}
	${CC} -o $@ $^ -ldl -pthread

test6: test6.cpp libthread.o ${LIBCPU}
	${CC} -o $@ $^ -ldl -pthread

# Mutex Tests
test110: test110.cpp libthread.o ${LIBCPU}
	${CC} -o $@ $^ -ldl -pthread

test111: test111.cpp libthread.o ${LIBCPU}
	${CC} -o $@ $^ -ldl -pthread

# CV Tests
test205: test205.cpp libthread.o ${LIBCPU}
	${CC} -o $@ $^ -ldl -pthread

test206: test206.cpp libthread.o ${LIBCPU}
	${CC} -o $@ $^ -ldl -pthread

test210: test210.cpp libthread.o ${LIBCPU}
	${CC} -o $@ $^ -ldl -pthread

test211: test211.cpp libthread.o ${LIBCPU}
	${CC} -o $@ $^ -ldl -pthread

test212: test212.cpp libthread.o ${LIBCPU}
	${CC} -o $@ $^ -ldl -pthread

test213: test213.cpp libthread.o ${LIBCPU}
	${CC} -o $@ $^ -ldl -pthread

test214: test214.cpp libthread.o ${LIBCPU}
	${CC} -o $@ $^ -ldl -pthread

# Extra join tests
test701: test701.cpp libthread.o ${LIBCPU}
	${CC} -o $@ $^ -ldl -pthread

test702: test702.cpp libthread.o ${LIBCPU}
	${CC} -o $@ $^ -ldl -pthread

test703: test703.cpp libthread.o ${LIBCPU}
	${CC} -o $@ $^ -ldl -pthread

test704: test704.cpp libthread.o ${LIBCPU}
	${CC} -o $@ $^ -ldl -pthread

# Generic rules for compiling a source file to an object file
%.o: %.cpp
	${CC} -c $<
%.o: %.cc
	${CC} -c $<

clean:
	rm -f ${THREAD_OBJS} libthread.o

	rm -f test0 test1 test2 test3 test4 test5 test6 test7 test8 test9 test10
	rm -rf test0.dSYM test1.dSYM test2.dSYM test3.dSYM test4.dSYM test5.dSYM test6.dSYM test7.dSYM test8.dSYM test9.dSYM test10.dSYM

	rm -f test50 
	rm -rf test50.dSYM

	rm -f test101 test102 test103 test104 test105 test106 test107 test108 test109 test110 test111
	rm -rf test101.dSYM test102.dSYM test103.dSYM test104.dSYM test105.dSYM test106.dSYM test107.dSYM test108.dSYM test109.dSYM test110.dSYM test111.dSYM
	
	rm -f test201 test202 test203 test204 test205 test206 test207 test208 test209 test210 test211 test212 test213 test214
	rm -rf test201.dSYM test202.dSYM test203.dSYM test204.dSYM test205.dSYM test206.dSYM test207.dSYM test208.dSYM test209.dSYM test210.dSYM test211.dSYM test212.dSYM test213.dSYM test214.dSYM

	rm -f test301 test302
	rm -rf test301.dSYM test302.dSYM

	rm -f test701 test702 test703 test704
	rm -rf test701.dSYM test702.dSYM test703.dSYM test704.dSYM

	rm -f test901 test902 test903 test904 test905 test906 test907 test908 test909 test910 test911
	rm -rf test901.dSYM test902.dSYM test903.dSYM test904.dSYM test905.dSYM test906.dSYM test907.dSYM test908.dSYM test909.dSYM test910.dSYM test911.dSYM
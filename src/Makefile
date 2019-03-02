TARGET=Job




SRCS=PPJob.cpp PPJobExecutor.cpp PPJobManager.cpp PPUtils.cpp  main.cpp



OBJS =  $(SRCS:.cpp=.o)


HDRS = $(SRCS:.cpp=.h)


CFLAGS = -g -std=c++11 -Wall -O3  -m64 -pthread -D_POSIX_PTHREAD_SEMANTICS
CINCLUDE=-I. -I.. -DLINUX
CC=/usr/bin/clang


LIBS=     -lstdc++ -lpthread   -lstdc++ -lrt -lXext  -lnsl -lm 

# ---------------------------------------------------
all::		start

all::		$(OBJS)

all::		$(TARGET)


start:
	rm -f  $(TARGET)


clean::
	rm     -f *.o    *.a  core $(TARGET)



# ---------------------------------------------------
# Compilation
.cpp.o:
	$(CC)   $(CFLAGS)  $(CINCLUDE) -c $*.cpp



$(TARGET):
	rm -f $(TARGET)
	$(CC) -o $(TARGET) $(OBJS)  $(LIBS) 




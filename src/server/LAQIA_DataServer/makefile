#
# makefile 
#

TARGET  = LAQIA_DataServer 
OBJS   = main.o message.o net_init.o utils.o clist.o encrypt.o

LIBS = -lmysqlclient -lz -lm 

INC     = -I/usr/include/mysql

LIB     = -L/usr/lib/mysql

CC      = g++
CXX     = g++
CFLAGS  = -g -c -pipe -Wall
LDFLAGS = $(LIB)

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

clean :
	rm -rf $(OBJS) $(TARGET)


SRC_C_FILE   = $(@F:.o=.c)
SRC_CPP_FILE = $(@F:.o=.cpp)

.SUFFIXES:
.SUFFIXES : .o .c .cc .cpp

.c.o:
	$(CC) $(CFLAGS) $(INC) $(SRC_C_FILE)
.cc.o:
	$(CXX) $(CFLAGS) $(INC) $(SRC_CPP_FILE)
.cpp.o:
	$(CXX) $(CFLAGS) $(INC) $(SRC_CPP_FILE)

clist.o: clist.cpp #def.h extern.h clist.h
main.o: main.cpp #def.h extern.h
encrypt.o: encrypt.cpp
message.o: message.cpp def.h extern.h message.h
net_init.o: net_init.cpp #def.h extern.h net_init.h
utils.o: utils.cpp# def.h extern.h

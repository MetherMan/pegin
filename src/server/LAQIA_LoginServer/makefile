#
# makefile 
#

TARGET  = LAQIA_LoginServer 
SRCS=admin_server.cpp message.cpp net_init.cpp client.cpp main.cpp utils.cpp encrypt.cpp DaumGameAuth.cpp DaumGameCrypt.cpp

OBJS=$(SRCS:.cpp=.o)

LIBS = -lmysqlclient -lz -lm 

INC     = -I/usr/include/mysql \
          -D__CHINA_VER__	     \

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


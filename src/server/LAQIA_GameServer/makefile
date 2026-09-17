# asdf  aazz
# makefile 
#

TARGET  = LAQIA_GameServer
OBJS    = main.o message.o monster.o net_init.o \
	  player.o utils.o world.o fight.o clist.o skill.o dataserver.o\
	  item.o admin.o itemdb.o party.o shop.o mercenary.o admin_server.o\
	  effect.o questdb.o quest.o house.o npc.o guild.o friends.o billing.o\
	  pet.o fishing.o war.o encrypt.o

LIBS = -lmysqlclient -lz -lm  

INC     = -I/usr/include/mysql \
	-D__CHINA_VER__ \
	-DdUSE_TEST_ACCOUNT \
	-D__DONT_SELECT__ -DdUSE_ITEMLOG -DdUSE_LOGIN_LOG -DITEM_EVENT_040321 \
	-DdNOT_USE_PET -DUSE_GUILD_WAR -DUSE_ITEM_WEIGHT -DdUSE_DEAD_GHOST -DUSE_GARBAGE_MONEY -DUSE_ITEM_TAX \
	-DUSE_ITEM_SERIAL -DdDAUM_BILLING -DBILLITEM_LOG -DUSE_NEW_ITEMDROP \
	-DTEAMMAY_IS_ADMIN\
	-D_dUMENOKOJI_ \
	-DIS_ARCHER_RANGE_TEST\
	-D_dUMENOKOJI_GHOST_GUILD_\
	-DdLIMIT_WORD\
	-D_dUMENOKOJI_SECOND_LEVEL_\
	-DNO_TIMELIMIT\
	-DDEBUG_RECVBUFF\
	-DDEBUG_SENDBUFF\
	-D_JamesVer2
	
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
main.o: main.cpp def.h extern.h 
message.o: message.cpp def.h extern.h message.h
monster.o: monster.cpp def.h extern.h monster.h
net_init.o: net_init.cpp def.h extern.h net_init.h
player.o: player.cpp def.h extern.h player.h
utils.o: utils.cpp def.h extern.h 
world.o: world.cpp def.h extern.h world.h
fight.o: fight.cpp def.h extern.h fight.h
clist.o: clist.cpp def.h extern.h clist.h
skill.o: skill.cpp def.h extern.h skill.h
item.o: item.cpp def.h extern.h item.h
dataserver.o: dataserver.cpp def.h extern.h dataserver.h
admin.o: admin.cpp def.h admin.h
itemdb.o: itemdb.cpp itemdb.h def.h itemdb.h
party.o: party.cpp party.h def.h 
shop.o: shop.cpp shop.h def.h
mercenary.o : mercenary.cpp mercenary.h
admin_server.o : admin_server.cpp admin_server.h def.h
effect.o: effect.cpp effect.h def.h
quest.o: quest.cpp quest.h def.h
questdb.o: questdb.cpp questdb.h def.h
house.o: house.cpp house.h def.h
npc.o: npc.cpp npc.h def.h
guild.o: guild.cpp
friends.o: friends.cpp
billing.o: billing.cpp
pet.o: pet.cpp
fishing.o: fishing.cpp
war.o: war.cpp

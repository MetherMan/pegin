#include "def.h"
#include "structs.h"
#include "world.h"
int main(){printf("%ld\n",(long)(sizeof(*gWORLD[0])));printf("%ld\n",(long)((long)&((sPWORLD)0)->mobMapAttr));printf("%ld\n",(long)((long)&((sPDESC_DATA)0)->ch2.gotoMapNum));printf("%ld\n",(long)((long)&((sPDESC_DATA)0)->ch2.gotoPosMapKind));}

#ifndef LOCAL_GM_PARSER_H
#define LOCAL_GM_PARSER_H
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "local_level_limits.h"
#include "local_rates.h"
enum GMKind { GM_HELP, GM_MONEY, GM_LEVEL, GM_CARD, GM_HORSE, GM_ITEM, GM_INFO, GM_HEAL, GM_CATALOG, GM_CASH, GM_EXP_RATE, GM_CARD_RATE, GM_ENHANCE, GM_UNKNOWN };
struct GMRequest { GMKind kind; int value, subtype; char target[64]; GMRequest():kind(GM_UNKNOWN),value(0),subtype(0){target[0]=0;} };
static bool gmEqual(const char* a,const char* b){return strcmp(a,b)==0;}
static int gmEquipSlot(const char* s){
 if(gmEqual(s,"무기")||gmEqual(s,"강화")||gmEqual(s,"enchant"))return -1;
 if(gmEqual(s,"활"))return 1;
 if(gmEqual(s,"손")||gmEqual(s,"장갑"))return 2;
 if(gmEqual(s,"검"))return 3;
 if(gmEqual(s,"머리")||gmEqual(s,"투구"))return 4;
 if(gmEqual(s,"옷")||gmEqual(s,"상의")||gmEqual(s,"갑옷"))return 5;
 if(gmEqual(s,"허리")||gmEqual(s,"벨트"))return 6;
 if(gmEqual(s,"하의")||gmEqual(s,"바지"))return 7;
 if(gmEqual(s,"귀걸이"))return 8;
 if(gmEqual(s,"반지"))return 9;
 if(gmEqual(s,"방패"))return 10;
 if(gmEqual(s,"신발")||gmEqual(s,"발"))return 11;
 if(gmEqual(s,"목걸이"))return 0;
 return -2;
}
static GMKind gmKind(const char* s){
 if(gmEquipSlot(s)!=-2)return GM_ENHANCE;
 if(gmEqual(s,"창고")||gmEqual(s,"catalog")) return GM_CATALOG;
 if(gmEqual(s,"help")||gmEqual(s,"도움말")) return GM_HELP;
 if(gmEqual(s,"돈")||gmEqual(s,"money")) return GM_MONEY;
 if(gmEqual(s,"캐시")||gmEqual(s,"cash")) return GM_CASH;
 if(gmEqual(s,"경험치배수")||gmEqual(s,"exprate")) return GM_EXP_RATE;
 if(gmEqual(s,"카드확률")||gmEqual(s,"cardrate")) return GM_CARD_RATE;
 if(gmEqual(s,"레벨")||gmEqual(s,"level")) return GM_LEVEL;
 if(gmEqual(s,"카드")||gmEqual(s,"card")) return GM_CARD;
 if(gmEqual(s,"말")||gmEqual(s,"horse")) return GM_HORSE;
 if(gmEqual(s,"아이템")||gmEqual(s,"item")) return GM_ITEM;
 if(gmEqual(s,"정보")||gmEqual(s,"info")) return GM_INFO;
 if(gmEqual(s,"회복")||gmEqual(s,"heal")) return GM_HEAL;
 return GM_UNKNOWN;
}
static bool gmNumber(const char* s,int lo,int hi,int& out){
 if(!s||!*s) return false;
 for(const char* p=s;*p;p++) if(*p<'0'||*p>'9') return false;
 errno=0;char* end=0;long n=strtol(s,&end,10);
 if(errno||*end||n<lo||n>hi) return false;out=(int)n;return true;
}
static int gmSkill(const char* s){
 if(gmEqual(s,"근접")||gmEqual(s,"근거리")||gmEqual(s,"close"))return 0;
 if(gmEqual(s,"원거리")||gmEqual(s,"range"))return 1;
 if(gmEqual(s,"흑마법")||gmEqual(s,"dark"))return 2;
 if(gmEqual(s,"백마법")||gmEqual(s,"white"))return 3;
 if(gmEqual(s,"전체")||gmEqual(s,"all"))return 4;return -1;
}
static bool gmParse(const char* input,GMRequest& r){
 if(strlen(input)>=512)return false;
 char buffer[512];strcpy(buffer,input);char* t[8];int n=0;
 for(char* s=strtok(buffer," \t\r\n");s;s=strtok(0," \t\r\n")){if(n==8)return false;t[n++]=s;}
 if(n==0||(!gmEqual(t[0],"/gm")&&!gmEqual(t[0],"/재뽕")))return false;
 if(n==1){r.kind=GM_HELP;return true;}
 if(gmEqual(t[1],"테스트")){if(n!=3||!gmEqual(t[2],"1"))return false;r.kind=GM_ITEM;r.subtype=19000;r.value=1;return true;}
 r.kind=gmKind(t[1]);int target=-1;
 switch(r.kind){
 case GM_EXP_RATE:case GM_CARD_RATE:
  if(n==2){r.value=-1;return true;}
  return n==3&&gmNumber(t[2],0,LOCAL_RATE_MAX,r.value);
 case GM_HELP:
  if(n==3){r.subtype=(int)gmKind(t[2]);return r.subtype!=GM_UNKNOWN;}
  r.subtype=GM_HELP;return n==2;
 case GM_ENHANCE:
  r.subtype=gmEquipSlot(t[1]);
  if(n<3||n>4||!gmNumber(t[2],0,20,r.value))return false;target=3;break;
 case GM_MONEY:case GM_CASH:
  if(n<3||n>4||!gmNumber(t[2],1,2000000000,r.value))return false;target=3;break;
 case GM_LEVEL:{
  if(n<3)return false;int s=gmSkill(t[2]);int p=s<0?2:3;r.subtype=s;
  if(n<p+1||n>p+2||!gmNumber(t[p],0,LOCAL_MAX_SKILL_LEVEL,r.value))return false;target=p+1;break;}
 case GM_CARD:
  if(n<4||n>5)return false;
  if(gmEqual(t[2],"weapon")||gmEqual(t[2],"무기"))r.subtype=1;
  else if(gmEqual(t[2],"armor")||gmEqual(t[2],"방어구"))r.subtype=2;
  else if(!gmNumber(t[2],1,2,r.subtype))return false;
  if(!gmNumber(t[3],1,1000,r.value))return false;target=4;break;
 case GM_HORSE:
  r.value=1;r.subtype=1;if(n==2)return true;if(n>4)return false;
  if(gmEqual(t[2],"갈색")||gmEqual(t[2],"brown"))r.subtype=1;
  else if(gmEqual(t[2],"흑마")||gmEqual(t[2],"black"))r.subtype=2;
  else if(gmEqual(t[2],"백마")||gmEqual(t[2],"white"))r.subtype=3;
  else if(!gmNumber(t[2],1,3,r.subtype))return false;target=3;break;
 case GM_ITEM:
  if(n<4||n>5||!gmNumber(t[2],1,65535,r.subtype)||!gmNumber(t[3],1,1000,r.value))return false;target=4;break;
 case GM_CATALOG:
  r.value=1;
  if(n<2||n>4)return false;
  if(n==2)return true;
  if(n==3&&gmNumber(t[2],1,10000,r.value))return true;
  if(t[2][0]=='-'||(t[2][0]>='0'&&t[2][0]<='9'))return false;
  if(strlen(t[2])>=sizeof(r.target))return false;strcpy(r.target,t[2]);
  if(n==4&&!gmNumber(t[3],1,10000,r.value))return false;
  return true;
 case GM_INFO:case GM_HEAL:
  if(n>3)return false;target=2;break;
 default:return false;
 }
 if(target<n){if(strlen(t[target])>=sizeof(r.target))return false;strcpy(r.target,t[target]);}
 return true;
}
#endif

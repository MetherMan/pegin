#include "def.h"
#include "structs.h"
#include "extern.h"
#include "message.h"
#include "player.h"
#include "item.h"
#include "itemdb.h"
#include "skill.h"
#include "admin.h"
#include "gm_parser.h"
#include "world.h"
#include "billing.h"
#include "gm_maps.h"
#include <limits.h>
#include <map>
#include <vector>
#include "gm_catalog_data.h"
#include "gm_enhance.h"
static void gmCatalogHelp(sPDESC_DATA p);
extern BOOL (*MessageFuncsList[])(sPDESC_DATA);
static BOOL (*gmOriginalLogin)(sPDESC_DATA)=0;
static BOOL gmLogin(sPDESC_DATA p) {
 // Repeated login packets must not insert one descriptor into the ID list twice.
 if(p->isInIdList)return 0;
 int pos=p->recvPos;
 unsigned int n=GetWord(p->TemprecvBuff,pos);
 if(n==0||n>=50)return 0;
 char id[50];memcpy(id,p->TemprecvBuff+pos,n);id[n]=0;
 if(FindPlayerIdList(id)) {
  // Reject the newcomer without kicking the person who is actually playing.
  g_nPos=2;PutWord(g_Packet,dPACKET_ALREADY_CONNECTED,g_nPos);
  PutSize(g_Packet,g_nPos);SendData(p,g_Packet,g_nPos);return 1;
 }
 return gmOriginalLogin(p);
}
static BOOL (*gmOriginalChat)(sPDESC_DATA)=0;
static const char* gmSkillNames[]={"근접","원거리","흑마법","백마법","전체"};

static bool gmPrefix(const char* s,unsigned int len,const char* prefix){
 unsigned int n=strlen(prefix);return len>=n&&!memcmp(s,prefix,n)&&(len==n||s[n]==' '||s[n]=='\t');
}
static void gmMoveHelp(sPDESC_DATA p){
 SendSystemMsg(p,"[이동 안내] 홈링 없이 이동합니다. 번호 또는 맵 이름 입력");
 SendSystemMsg(p,"사용법: /이동 맵번호 또는 /이동 맵이름");
 SendSystemMsg(p,"예시: /이동 3 | /이동 베일 | /이동 집 (베일 귀환)");
 char line[240]="";int count=0;
 for(unsigned int i=0;i<sizeof(gmDestinations)/sizeof(gmDestinations[0]);i++){
  const GMDestination& d=gmDestinations[i];if(!IsOpenMap(d.id))continue;
  char entry[90];snprintf(entry,sizeof(entry),"%d=%s  ",d.id,d.name);
  if(strlen(line)+strlen(entry)>=sizeof(line)-1){SendSystemMsg(p,line);line[0]=0;count=0;}
  strcat(line,entry);if(++count==3){SendSystemMsg(p,line);line[0]=0;count=0;}
 }
 if(line[0])SendSystemMsg(p,line);
}
static void gmMove(sPDESC_DATA p,char* input){
 char* cmd=strtok(input," \t\r\n");char* arg=strtok(0," \t\r\n");char* extra=strtok(0," \t\r\n");
 if(!arg||(!extra&&(!strcmp(arg,"help")||!strcmp(arg,"목록")||!strcmp(arg,"도움말")))){gmMoveHelp(p);return;}
 int id=-1;gmNumber(arg,1,65535,id);if(!strcmp(arg,"집")||!strcmp(arg,"귀환")||!strcmp(arg,"home"))id=3;
 const GMDestination* dest=0;
 int matches=0;
 if(!extra)for(unsigned int i=0;i<sizeof(gmDestinations)/sizeof(gmDestinations[0]);i++)if((id>0&&gmDestinations[i].id==id)||(id<0&&!strcmp(gmDestinations[i].name,arg))){dest=&gmDestinations[i];matches++;}
 if(matches>1){SendSystemMsg(p,"[GM 오류] 같은 이름의 맵이 여러 개입니다. 맵 번호를 입력하세요.");gmMoveHelp(p);return;}
 if(!dest||!IsOpenMap(dest->id)){SendSystemMsg(p,"[GM 오류] 이동할 맵을 확인하세요.");gmMoveHelp(p);return;}
 if(GET_HP(p)<=0){SendSystemMsg(p,"[GM 오류] 부활 후 이동하세요. 예시: /이동 집");return;}
 int x=dest->x,y=dest->y;
 // gmDestinations is validated against the shipped CLIENT player collision map.
 // The legacy mob mask is not a player-walkability predicate (towns differ).
 if(!IS_MAP_RANGE(x,y)){SendSystemMsg(p,"[GM 오류] 도착 좌표를 사용할 수 없습니다.");gmMoveHelp(p);return;}
 SendSystemMsg(p,(char*)"[이동] %s(%d)로 이동합니다.",(char*)dest->name,dest->id);
 log_file((char*)"gm_commands.log",(char*)"MOVE actor=%s from=%d to=%d x=%d y=%d\n",GET_ID(p),GET_MAP_NUM(p),dest->id,x,y);
 if(GET_MAP_KIND(p)==dMAP_KIND_INDOOR)PlayerFromIndoor(p);else PlayerFromMap(p);
 p->ch2.gotoMapNum=dest->id;p->ch2.gotoPosX=x;p->ch2.gotoPosY=y;p->ch2.gotoPosMapKind=dest->kind;
 SendMapLoading(p);
}

static void gmUsage(sPDESC_DATA p,GMKind k){
 switch(k){
 case GM_EXP_RATE:case GM_CARD_RATE:SendSystemMsg(p,"사용법: /재뽕 경험치배수 30 | /재뽕 카드확률 30");SendSystemMsg(p,"서버 전체 설정: 0~1000배, 1=기본, 0=중지, 숫자 생략=조회");SendSystemMsg(p,"즉시 적용하고 재시작 후에도 유지합니다. 카드 확률은 최대 100%.");break;
 case GM_ENHANCE:SendSystemMsg(p,"사용법: /재뽕 부위 수치 [이름] (0~20, 0=기본형)");SendSystemMsg(p,"예시: /재뽕 무기 10 | /재뽕 옷 10 | /재뽕 손 10");SendSystemMsg(p,"부위: 무기, 활, 머리, 옷, 손, 하의, 신발, 방패, 허리, 목걸이, 반지");SendSystemMsg(p,"착용 장비만 변경. 이름 생략=자신. 해당 장비에 존재하는 단계만 가능.");break;
 case GM_CATALOG:gmCatalogHelp(p);break;
 case GM_MONEY:SendSystemMsg(p,"사용법: /재뽕 돈 수량 [캐릭터이름] (1~20억 지급)");SendSystemMsg(p,"예시: /재뽕 돈 1000000 아버지");break;
 case GM_CASH:SendSystemMsg(p,"사용법: /재뽕 캐시 수량 [캐릭터이름] (1~20억 지급)");SendSystemMsg(p,"예시: /재뽕 캐시 9999999 (이름 생략=자신)");break;
 case GM_LEVEL:SendSystemMsg(p,"사용법: /재뽕 레벨 [계열] 값 [이름] (각 0~200)");SendSystemMsg(p,"계열: 근거리(근접) / 원거리 / 흑마법 / 백마법 / 전체");SendSystemMsg(p,"예시: /재뽕 레벨 근거리 100 아버지");SendSystemMsg(p,"/재뽕 레벨 100 = 대상이 현재 사용하는 계열만 100");SendSystemMsg(p,"/재뽕 레벨 전체 200 = 네 계열 각각 200, 총레벨 800");break;
 case GM_CARD:SendSystemMsg(p,"사용법: /재뽕 카드 종류 수량 [이름] (1~1000장)");SendSystemMsg(p,"종류: 1=무기, 2=방어구. 예시: /재뽕 카드 1 20 아버지");break;
 case GM_HORSE:SendSystemMsg(p,"사용법: /재뽕 말 [종류] [이름] (소유증서 1개)");SendSystemMsg(p,"1=갈색, 2=흑마, 3=백마. 예시: /재뽕 말 2 아버지");break;
 case GM_ITEM:SendSystemMsg(p,"사용법: /재뽕 아이템 아이템번호 수량 [이름]");SendSystemMsg(p,"예시: /재뽕 아이템 10193 10 아버지");break;
 case GM_INFO:SendSystemMsg(p,"사용법: /재뽕 정보 [이름]. 예시: /재뽕 정보 아버지");break;
 case GM_HEAL:SendSystemMsg(p,"사용법: /재뽕 회복 [이름]. 예시: /재뽕 회복 아버지");break;
 default:
  SendSystemMsg(p,"[GM 도움말] 일반 채팅에서 입력. 이름 생략=자신");
  SendSystemMsg(p,"/재뽕 테스트 2 = 푸른달의 트와일라잇 양손검 지급 (Rank 9)");
  SendSystemMsg(p,"/재뽕 테스트 3 = 푸른달의 트와일라잇 롱보우 지급 (Rank 9)");
  SendSystemMsg(p,"/재뽕 테스트 4 = 푸른달의 트와일라잇 스태프 지급 (Rank 9)");
  SendSystemMsg(p,"/재뽕 테스트 1 = 블러드나이트스피어 지급 (Rank 15, 착용 제한 없음)");
  SendSystemMsg(p,"/재뽕 돈 1000000 | /재뽕 레벨 근접 100");
  SendSystemMsg(p,"/재뽕 캐시 9999999 [이름] (이름 생략=자신)");
  SendSystemMsg(p,"/재뽕 경험치배수 30 | /재뽕 카드확률 30 (서버 전체)");
  SendSystemMsg(p,"/재뽕 레벨 전체 100 | /재뽕 카드 1 20");
  SendSystemMsg(p,"/재뽕 말 2 | /재뽕 아이템 10194 10");
  SendSystemMsg(p,"/재뽕 정보 [이름] | /재뽕 회복 [이름]");
  SendSystemMsg(p,"/재뽕 창고 = 기본형 아이템 목록 (더블클릭 지급)");
  SendSystemMsg(p,"/재뽕 무기 10 | /재뽕 옷 10 | /재뽕 손 10 (착용 장비 강화)");
  SendSystemMsg(p,"/이동 = 맵 목록 | /이동 집 = 베일 귀환");
  SendSystemMsg(p,"상세 예시: /재뽕 help 레벨 또는 /재뽕 help 카드");break;
 }
}
static bool gmReady(sPDESC_DATA p){return p&&(GET_STATE(p)==dDESC_STATE_PLAYING||GET_STATE(p)==dDESC_STATE_FIGHT);}
static void gmInfo(sPDESC_DATA actor,sPDESC_DATA p){
 SendSystemMsg(actor,(char*)"[GM] %s: 실드 %d / 인벤 빈칸 %d",GET_NAME(p),GET_MONEY(p),CountEmptyInven(p));
 SendSystemMsg(actor,(char*)"캐시 %d (계정 잔액)",GET_CASHMONEY(p));
 SendSystemMsg(actor,(char*)"[서버 설정] 경험치 %d배 / 카드 확률 %d배",LocalExpRate(),LocalCardRate());
 SendSystemMsg(actor,(char*)"근접 %d / 원거리 %d / 흑마법 %d / 백마법 %d / 합계 %d",GET_SATTACK(p),GET_LATTACK(p),GET_BMAGIC(p),GET_WMAGIC(p),GET_ALL_SKILL(p));
 SendSystemMsg(actor,(char*)"HP %d/%d / MP %d/%d / 힘 %d 민첩 %d 지능 %d",GET_HP(p),GET_MAX_HP(p)+GET_HPADD(p),GET_MP(p),GET_MAX_MP(p)+GET_MPADD(p),GET_STR_TOTAL(p),GET_DEX_TOTAL(p),GET_INT_TOTAL(p));
}
static void gmHealth(sPDESC_DATA p){
 GET_HP(p)=GET_MAX_HP(p)+GET_HPADD(p);GET_MP(p)=GET_MAX_MP(p)+GET_MPADD(p);
 SendPlayerMaxHPMP(p);SendPlayerHP(p);SendPlayerMP(p);DATASERV_SendUpdateHp(p);DATASERV_SendUpdateMp(p);
}
static void gmLevel(sPDESC_DATA actor,sPDESC_DATA p,const GMRequest& r){
 int selected=r.subtype;
 if(selected<0){
  int item=GET_USE_SKILL(p);
  if(item>0&&item<dMAX_ITEMINFO&&g_ItemInfo[item]){
   int index=g_ItemInfo[item]->skillIdx;
   if(index>=0&&index<dMAX_SKILLS&&g_SKILL[index]){
    sPSKILL_DATA skill=g_SKILL[index];
    if(IS_SATTACK(skill))selected=0;else if(IS_LATTACK(skill))selected=1;
    else if(IS_BMAGIC(skill))selected=2;else if(IS_WMAGIC(skill))selected=3;
   }
  }
 }
 if(selected<0||selected>4){SendSystemMsg(actor,"[GM 오류] 현재 계열을 확인할 수 없습니다. 근거리/원거리/흑마법/백마법을 지정하세요.");return;}
 const int h[]={g_STATE_MPHP_TABLE[0].SAttack,g_STATE_MPHP_TABLE[0].LAttack,g_STATE_MPHP_TABLE[0].BMagic,g_STATE_MPHP_TABLE[0].WMagic};
 const int m[]={g_STATE_MPHP_TABLE[1].SAttack,g_STATE_MPHP_TABLE[1].LAttack,g_STATE_MPHP_TABLE[1].BMagic,g_STATE_MPHP_TABLE[1].WMagic};
 int dh=0,dm=0;int old[4];for(int i=0;i<4;i++){old[i]=p->ch.skill[i];if(selected==4||selected==i){dh+=(r.value-old[i])*h[i];dm+=(r.value-old[i])*m[i];}}
 long long hp=(long long)GET_MAX_HP(p)+dh,mp=(long long)GET_MAX_MP(p)+dm;
 if(hp<1||mp<1||hp>INT_MAX-GET_HPADD(p)||mp>INT_MAX-GET_MPADD(p)){SendSystemMsg(actor,"[GM 오류] 이 값은 HP/MP 범위를 벗어납니다.");gmUsage(actor,GM_LEVEL);return;}
 for(int i=0;i<4;i++)if(selected==4||selected==i){p->ch.skill[i]=(WORD)r.value;p->ch.skill_exp[i]=0;}
 GET_MAX_HP(p)=(int)hp;GET_MAX_MP(p)=(int)mp;
 // Each progress bar uses its own branch level.
 for(int i=0;i<4;i++){
  g_nPos=2;PutWord(g_Packet,dPACKET_SKILL_UP,g_nPos);PutByte(g_Packet,(BYTE)i,g_nPos);PutWord(g_Packet,p->ch.skill[i],g_nPos);
  PutInteger(g_Packet,get_skill_exp(p->ch.skill[i],(BYTE)i),g_nPos);PutInteger(g_Packet,p->ch.skill_exp[i],g_nPos);PutSize(g_Packet,g_nPos);SendData(p,g_Packet,g_nPos);
  if(selected==4||selected==i){DATASERV_SendUpdateSkill(p,(BYTE)i);DATASERV_SendUpdateSkillExp(p,(BYTE)i);}
 }
 gmHealth(p);DATASERV_SAVECHA(p);
 SendSystemMsg(actor,(char*)"[GM 완료] %s: %s 레벨을 %d로 설정했습니다.",GET_NAME(p),(char*)gmSkillNames[selected],r.value);
 SendSystemMsg(actor,"힘/민첩/지능은 별도 능력치이므로 유지됩니다.");
 if(actor!=p)SendSystemMsg(p,(char*)"[GM] %s 레벨이 %d로 설정되었습니다.",(char*)gmSkillNames[selected],r.value);
 log_file((char*)"gm_commands.log",(char*)"LEVEL actor=%s target=%s before=%d,%d,%d,%d after=%d,%d,%d,%d\n",GET_ID(actor),GET_NAME(p),old[0],old[1],old[2],old[3],p->ch.skill[0],p->ch.skill[1],p->ch.skill[2],p->ch.skill[3]);
}
static void gmCash(sPDESC_DATA actor,sPDESC_DATA p,int amount){
 int old=GET_CASHMONEY(p);
 if(old<0||amount>INT_MAX-old){SendSystemMsg(actor,"[GM 오류] 캐시 잔액 최대치(21억)를 넘습니다.");return;}
 char escaped[128],query[512];
 mysql_escape_string(escaped,GET_ID(p),strlen(GET_ID(p)));
 snprintf(query,sizeof(query),"select idx from UserCashMoney where ownerID='%s' limit 1",escaped);
 if(!ITEMDB_Query(query)){SendSystemMsg(actor,"[GM 오류] 캐시 저장소를 읽지 못했습니다. 지급하지 않았습니다.");return;}
 MYSQL_RES* result=mysql_store_result(g_MYSQL);
 if(!result){SendSystemMsg(actor,"[GM 오류] 캐시 잔액을 확인하지 못했습니다. 지급하지 않았습니다.");return;}
 bool exists=mysql_num_rows(result)>0;mysql_free_result(result);
 if(exists)snprintf(query,sizeof(query),"update UserCashMoney set amount=%d where ownerID='%s' limit 1",old+amount,escaped);
 else snprintf(query,sizeof(query),"insert into UserCashMoney(ownerID,amount) values('%s',%d)",escaped,old+amount);
 if(!ITEMDB_Query(query)){SendSystemMsg(actor,"[GM 오류] 캐시를 저장하지 못했습니다. 지급하지 않았습니다.");return;}
 GET_CASHMONEY(p)=old+amount;p->useCashMoney=1;
 DATASERV_SendUpdateCash(p);UpdateCash(p);
 SendSystemMsg(actor,(char*)"[GM 완료] %s에게 %d캐시 지급. 현재 %d캐시",GET_NAME(p),amount,GET_CASHMONEY(p));
 if(actor!=p)SendSystemMsg(p,(char*)"[GM] %d캐시를 받았습니다. 현재 %d캐시",amount,GET_CASHMONEY(p));
 log_file((char*)"gm_commands.log",(char*)"CASH actor=%s target=%s amount=%d balance=%d\n",GET_ID(actor),GET_NAME(p),amount,GET_CASHMONEY(p));
}
static void gmGrant(sPDESC_DATA actor,sPDESC_DATA p,const GMRequest& r){
 int item=r.kind==GM_CARD?(r.subtype==1?dENCHANT_CARD_WEAPON:dENCHANT_CARD_ARMOR):(r.kind==GM_HORSE?10187+r.subtype:r.subtype);
 if(item>=dMAX_ITEMINFO||!IS_VALID_ITEM(item)){SendSystemMsg(actor,"[GM 오류] 존재하지 않는 아이템 번호입니다.");gmUsage(actor,r.kind);return;}
 int empty=CountEmptyInven(p),limit=LaqiaStackLimit(GET_ITEM_TYPE2(item),item),capacity=empty*limit;
 if(limit>1)for(sPITEM_DATA existing=p->inven;existing;existing=existing->i_next)
  if(existing->itemNum==item&&GetItemStackCount(existing)<limit)capacity+=limit-GetItemStackCount(existing);
 if(capacity<r.value){SendSystemMsg(actor,(char*)"[GM 오류] 인벤에 %d개만 더 담을 수 있습니다. 지급하지 않았습니다.",capacity);gmUsage(actor,r.kind);return;}
 int done=0;
 if(limit>1)for(sPITEM_DATA existing=p->inven;existing&&done<r.value;existing=existing->i_next){
  if(existing->itemNum!=item)continue;
  int room=limit-GetItemStackCount(existing),amount=r.value-done;
  if(room<amount)amount=room;if(amount<=0)continue;
  existing->exVal[0]=GetItemStackCount(existing)+amount;done+=amount;SendUpdatePotionCnt(p,existing);
 }
 for(;done<r.value;){
  sCHECK_INVEN slot;if(!CheckEmptyInven(p,&slot))break;
  sPITEM_DATA itemData=CreateItem(item,(char*)"LocalGM",(char*)__FILE__,__LINE__);if(!itemData)break;
  int amount=r.value-done;if(amount>limit)amount=limit;
  if(limit>1)itemData->exVal[0]=amount;
  if(!ItemToInventory(p,itemData,&slot))break;
  done+=amount;
 }
 SendSystemMsg(actor,(char*)"[GM 지급] %s: %s %d/%d개",GET_NAME(p),g_ItemInfo[item]->hName,done,r.value);
 if(actor!=p)SendSystemMsg(p,(char*)"[GM] %s %d개를 받았습니다.",g_ItemInfo[item]->hName,done);
 log_file((char*)"gm_commands.log",(char*)"ITEM actor=%s target=%s item=%d count=%d requested=%d\n",GET_ID(actor),GET_NAME(p),item,done,r.value);
}

static const int gmShelfShop=250,gmShelfNav=20460,gmShelfPageSize=40,gmShelfCategories=5;
static const char* gmShelfNames[]={"세트","무기","옷","스킬","기타"};
struct GMShelfCell {int item,slot;GMShelfCell(int n,int s):item(n),slot(s){}};
struct GMShelfPage {std::vector<GMShelfCell> cells;int group;GMShelfPage(int g=-1):group(g){}};
struct GMShelf {std::vector<GMShelfPage> pages[5];int page[5];int selected;char account[64];GMShelf():selected(0){account[0]=0;for(int i=0;i<5;i++)page[i]=0;}};
static std::map<sPDESC_DATA,GMShelf> gmShelves;
static BOOL (*gmOriginalBuy)(sPDESC_DATA)=0;
static BOOL (*gmOriginalShopRequest)(sPDESC_DATA)=0;
static int gmShelfPages(const GMShelf& s,int c){return (int)s.pages[c].size();}
static void gmCatalogHelp(sPDESC_DATA p){
 SendSystemMsg(p,"[운영자 창고] 세트 / 무기 / 옷 / 스킬 / 기타. 더블클릭 = 1개 무료 복사");
 SendSystemMsg(p,"세트: 무기 2줄, 전사/궁수/마법사 옷 각 1줄. 스킬: 검/활/흑마법/백마법.");
 SendSystemMsg(p,"맨 아래 이전/다음 카드로 해당 탭의 페이지를 넘깁니다.");
 SendSystemMsg(p,"사용법: /재뽕 창고 [검색어] [묶음번호]");
 SendSystemMsg(p,"예시: /재뽕 창고 | /재뽕 창고 검 | /재뽕 창고 2");
}
static bool gmShelfAllowed(sPDESC_DATA p){return gmReady(p)&&p->isAdmin&&p->adminLevel>=dADMIN_GOD;}
static void gmShelfSend(sPDESC_DATA p,GMShelf& s){
 int total=1+gmShelfCategories*2;
 for(int c=0;c<gmShelfCategories;c++)total+=(int)s.pages[c][s.page[c]].cells.size();
 if(total>255)return;
 const GMShelfPage& selected=s.pages[s.selected][s.page[s.selected]];
 SendSystemMsg(p,(char*)"[창고 %s] %s / 페이지 %d/%d",gmShelfNames[s.selected],selected.group<0?gmShelfNames[s.selected]:gmCatalogGroups[selected.group],s.page[s.selected]+1,gmShelfPages(s,s.selected));
 if(s.selected==0)SendSystemMsg(p,"위에서부터 무기 2줄 / 전사 옷 / 궁수 옷 / 마법사 옷. 맨 아래 이전/다음.");
 g_nPos=2;PutWord(g_Packet,dPACKET_SHOPITEM_LIST,g_nPos);PutByte(g_Packet,(BYTE)total,g_nPos);
 PutInteger(g_Packet,-1700000001,g_nPos);PutByte(g_Packet,(BYTE)s.selected,g_nPos);PutByte(g_Packet,0,g_nPos);PutByte(g_Packet,0,g_nPos);
 for(int c=0;c<gmShelfCategories;c++){
  const GMShelfPage& page=s.pages[c][s.page[c]];
  for(unsigned int i=0;i<page.cells.size();i++){
   const GMShelfCell& cell=page.cells[i];PutInteger(g_Packet,cell.item,g_nPos);PutByte(g_Packet,(BYTE)c,g_nPos);PutByte(g_Packet,(BYTE)(cell.slot%5),g_nPos);PutByte(g_Packet,(BYTE)(cell.slot/5),g_nPos);
  }
  for(int i=0;i<2;i++){PutInteger(g_Packet,gmShelfNav+c*2+i,g_nPos);PutByte(g_Packet,(BYTE)c,g_nPos);PutByte(g_Packet,(BYTE)(i?4:0),g_nPos);PutByte(g_Packet,8,g_nPos);}
 }
 PutSize(g_Packet,g_nPos);SendData(p,g_Packet,g_nPos);
}
static void gmCatalog(sPDESC_DATA p,const GMRequest& r){
 if(!strcmp(r.target,"help")||!strcmp(r.target,"도움말")){gmCatalogHelp(p);return;}
 if(p->ch2.dealVict||p->ch2.dealItems.GetSize()){SendSystemMsg(p,"[GM 오류] 거래를 마친 뒤 창고를 여세요.");gmCatalogHelp(p);return;}
 GMShelf next;snprintf(next.account,sizeof(next.account),"%s",GET_ID(p));int total=0,pages=1;
 for(unsigned int i=0;i<sizeof(gmCatalogEntries)/sizeof(gmCatalogEntries[0]);i++){
  const GMCatalogEntry& e=gmCatalogEntries[i];
  if(!IS_VALID_ITEM(e.id)||GET_ITEM_UNIQ2(e.id)!=0)continue;
  if(r.target[0]&&!strstr(e.name,r.target)&&!strstr(g_ItemInfo[e.id]->hName,r.target)&&(e.group<0||!strstr(gmCatalogGroups[e.group],r.target)))continue;
  std::vector<GMShelfPage>& list=next.pages[e.category];
  if(list.empty()||(e.group>=0?list.back().group!=e.group:list.back().cells.size()>=gmShelfPageSize))list.push_back(GMShelfPage(e.group));
  GMShelfPage& page=list.back();page.cells.push_back(GMShelfCell(e.id,e.group>=0?e.slot:(int)page.cells.size()));total++;
 }
 if(!total){SendSystemMsg(p,"[GM 오류] 검색 결과가 없습니다.");gmCatalogHelp(p);return;}
 for(int c=0;c<gmShelfCategories;c++){if(next.pages[c].empty())next.pages[c].push_back(GMShelfPage());if(gmShelfPages(next,c)>pages)pages=gmShelfPages(next,c);}
 if(r.value<1||r.value>pages){SendSystemMsg(p,(char*)"[GM 오류] 묶음 번호는 1~%d입니다.",pages);gmCatalogHelp(p);return;}
 for(int c=0;c<gmShelfCategories;c++)next.page[c]=r.value>gmShelfPages(next,c)?gmShelfPages(next,c)-1:r.value-1;
 for(int c=0;c<gmShelfCategories;c++)if(!next.pages[c][next.page[c]].cells.empty()){next.selected=c;break;}
 gmShelves[p]=next;gmShelfSend(p,gmShelves[p]);
}
static BOOL gmBuyFromShelf(sPDESC_DATA p){
 int pos=p->recvPos;unsigned short size=0,shop=0;memcpy(&size,p->TemprecvBuff,2);
 if(pos<4||pos+2>size||size>sizeof(p->TemprecvBuff))return 0;
 memcpy(&shop,p->TemprecvBuff+pos,2);if(shop!=gmShelfShop)return gmOriginalBuy(p);
 if(!gmShelfAllowed(p)){SendSystemMsg(p,"[GM 오류] 운영자 전용 창고입니다.");return 1;}
 if(pos+8!=size&&pos+7!=size){SendSystemMsg(p,"[GM 오류] 잘못된 창고 요청입니다.");return 1;}
 std::map<sPDESC_DATA,GMShelf>::iterator it=gmShelves.find(p);
 if(it==gmShelves.end()||strcmp(it->second.account,GET_ID(p))){SendSystemMsg(p,"[GM 오류] /재뽕 창고 명령으로 목록을 먼저 여세요.");return 1;}
 int item=0;memcpy(&item,p->TemprecvBuff+pos+2,4);GMShelf& shelf=it->second;
 if(item>=gmShelfNav&&item<gmShelfNav+gmShelfCategories*2){
  int c=(item-gmShelfNav)/2,step=(item-gmShelfNav)%2,next=shelf.page[c]+(step?1:-1);
  if(next<0||next>=gmShelfPages(shelf,c)){SendSystemMsg(p,step?"[창고] 마지막 목록입니다.":"[창고] 첫 목록입니다.");return 1;}
  shelf.page[c]=next;shelf.selected=c;gmShelfSend(p,shelf);return 1;
 }
 bool listed=false;
 for(int c=0;c<gmShelfCategories;c++){
  const std::vector<GMShelfCell>& cells=shelf.pages[c][shelf.page[c]].cells;
  for(unsigned int i=0;i<cells.size();i++)if(cells[i].item==item){listed=true;break;}
 }
 if(!listed){SendSystemMsg(p,"[GM 오류] 현재 창고 목록의 아이템을 선택하세요.");return 1;}
 if(p->ch2.dealVict||p->ch2.dealItems.GetSize()){SendSystemMsg(p,"[GM 오류] 거래 중에는 지급할 수 없습니다.");return 1;}
 GMRequest grant;grant.kind=GM_ITEM;grant.subtype=item;grant.value=1;gmGrant(p,p,grant);return 1;
}
static BOOL gmRequestShop(sPDESC_DATA p){gmShelves.erase(p);return gmOriginalShopRequest(p);}

static BOOL gmChat(sPDESC_DATA actor){
 int pos=actor->recvPos;unsigned int packetSize=0,len=0;
 memcpy(&packetSize,actor->TemprecvBuff,2);
 if(pos<4||pos+2>(int)packetSize||packetSize>sizeof(actor->TemprecvBuff))return 0;
 memcpy(&len,actor->TemprecvBuff+pos,2);
 if(len>packetSize-pos-2)return 0;
 const char* raw=actor->TemprecvBuff+pos+2;
 if(!gmPrefix(raw,len,"/재뽕")&&!gmPrefix(raw,len,"/gm")&&!gmPrefix(raw,len,"/이동"))return gmOriginalChat(actor);
 if(!gmReady(actor))return 1;
 if(!actor->isAdmin||actor->adminLevel<dADMIN_GOD){SendSystemMsg(actor,"[GM 오류] 운영자 전용 명령어입니다.");return 1;}
 if(len>=512||memchr(raw,0,len)){SendSystemMsg(actor,"[GM 오류] 명령어가 너무 길거나 잘못되었습니다.");gmUsage(actor,GM_HELP);return 1;}
 char input[512];memcpy(input,raw,len);input[len]=0;GMRequest r;
 if(gmPrefix(raw,len,"/이동")){gmMove(actor,input);return 1;}
 if(!gmParse(input,r)){SendSystemMsg(actor,"[GM 오류] 명령어, 숫자 범위, 인수 개수를 확인하세요.");gmUsage(actor,r.kind);return 1;}
 if(r.kind==GM_HELP){gmUsage(actor,(GMKind)r.subtype);return 1;}
 if(r.kind==GM_EXP_RATE||r.kind==GM_CARD_RATE){
  int exp=LocalExpRate(),cards=LocalCardRate();
  if(r.value>=0){
   if(r.kind==GM_EXP_RATE)exp=r.value;else cards=r.value;
   if(!LocalSetRates(exp,cards)){SendSystemMsg(actor,"[GM 오류] 배수 설정을 저장하지 못했습니다. 기존 설정을 유지합니다.");return 1;}
   log_file((char*)"gm_commands.log",(char*)"RATES actor=%s exp=%d cards=%d\n",GET_ID(actor),exp,cards);
  }
  SendSystemMsg(actor,(char*)"[서버 설정] 경험치 %d배 / 카드 확률 %d배 (즉시 적용, 저장됨)",exp,cards);return 1;
 }
 if(r.kind==GM_CATALOG){gmCatalog(actor,r);return 1;}
 sPDESC_DATA p=actor;
 if(r.target[0]){
  if(strlen(r.target)>=dNAME_LEN){SendSystemMsg(actor,"[GM 오류] 캐릭터 이름이 너무 깁니다.");gmUsage(actor,r.kind);return 1;}
  p=FindPlayerNameList(r.target);
 }
 if(!gmReady(p)){SendSystemMsg(actor,"[GM 오류] 접속 중인 캐릭터 이름을 입력하세요. 계정 아이디가 아닙니다.");gmUsage(actor,r.kind);return 1;}
 switch(r.kind){
 case GM_ENHANCE:gmEnhance(actor,p,r);break;
 case GM_CASH:gmCash(actor,p,r.value);break;
 case GM_MONEY:
  if(GET_MONEY(p)<0||r.value>dMAX_HAVE_MONEY-GET_MONEY(p)){SendSystemMsg(actor,"[GM 오류] 소지금 최대치(21억)를 넘습니다.");gmUsage(actor,r.kind);break;}
  GET_MONEY(p)+=r.value;DATASERV_SendUpdateMoney(p);UpdateMoney(p);DATASERV_SAVECHA(p);
  SendSystemMsg(actor,(char*)"[GM 완료] %s에게 %d실드 지급. 현재 %d실드",GET_NAME(p),r.value,GET_MONEY(p));
  if(actor!=p)SendSystemMsg(p,(char*)"[GM] %d실드를 받았습니다.",r.value);
  log_file((char*)"gm_commands.log",(char*)"MONEY actor=%s target=%s amount=%d balance=%d\n",GET_ID(actor),GET_NAME(p),r.value,GET_MONEY(p));break;
 case GM_LEVEL:gmLevel(actor,p,r);break;
 case GM_CARD:case GM_HORSE:case GM_ITEM:gmGrant(actor,p,r);break;
 case GM_INFO:gmInfo(actor,p);break;
 case GM_HEAL:gmHealth(p);SendSystemMsg(actor,(char*)"[GM 완료] %s의 HP/MP를 회복했습니다.",GET_NAME(p));break;
 default:gmUsage(actor,GM_HELP);break;
 }
 return 1;
}
static BOOL (*gmOriginalMapEnd)(sPDESC_DATA)=0;
static BOOL gmEndMapLoading( sPDESC_DATA pPlayer )
{
 if(!pPlayer->isAdmin||pPlayer->adminLevel<dADMIN_GOD||
    (GET_GOTO_MAPNUM(pPlayer)!=dPRIMIUM_ZONE1&&GET_GOTO_MAPNUM(pPlayer)!=dPRIMIUM_ZONE2&&GET_GOTO_MAPNUM(pPlayer)!=dPRIMIUM_ZONE3))return gmOriginalMapEnd(pPlayer);
 BYTE btIsFirstLoad = pPlayer->ch2.btFirstLoad;

	// 게임 속으로
	if( GET_STATE( pPlayer ) != dDESC_STATE_LOADING )
		return 1;

	if( pPlayer->ch2.gotoPosMapKind == dMAP_KIND_INDOOR )
	{
		PlayerIntoIndoor( pPlayer,
			pPlayer->ch2.gotoMapNum, 
			pPlayer->ch2.gotoPosX, 
			pPlayer->ch2.gotoPosY,
			pPlayer->ch2.gotoPosMapKind );			
	}
	else
	{
		PlayerIntoMap( pPlayer, 
			pPlayer->ch2.gotoMapNum, 
			pPlayer->ch2.gotoPosX, 
			pPlayer->ch2.gotoPosY,
			pPlayer->ch2.gotoPosMapKind );
	}
	GET_STATE( pPlayer ) = dDESC_STATE_PLAYING;

	// 공성중인 맵인 경우
	// 공성중인 맵에 처음 접속한 경우 
	// 마을로 보낸다 
#ifdef USE_GUILD_WAR
	if( btIsFirstLoad )
	{
		if( gWORLD[GET_MAP_NUM( pPlayer )]->underAttack )
		{
			sPWORLD pWorld = gWORLD[GET_MAP_NUM( pPlayer )];
			
			if( GET_GUILD( pPlayer ) == pWorld->warCristal.allowGuild[0] )
			{				
				pPlayer->ch2.gotoMapNum = GET_MAP_NUM( pPlayer );
				pPlayer->ch2.gotoPosX = pWorld->warCristal.posX + number( 2, 3 );
				pPlayer->ch2.gotoPosY = pWorld->warCristal.posY + number( 2, 3 );
				
				PlayerFromMap( pPlayer );
				SendMapLoading( pPlayer );
				return 1;
			}
			else 
			{
				PlayerFromMap( pPlayer );
				PlayerToStartPoint( pPlayer ); 
				SendMapLoading( pPlayer );
				return 1;
			}
		}
	}
#endif
	// END
	if( GET_MAP_NUM( pPlayer ) == dPRIMIUM_ZONE1 ||
		GET_MAP_NUM( pPlayer ) == dPRIMIUM_ZONE2 ||
		GET_MAP_NUM( pPlayer ) == dPRIMIUM_ZONE3 )
	{
		if( !(pPlayer->isAdmin&&pPlayer->adminLevel>=dADMIN_GOD) && !pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT5] &&
			!pPlayer->ch2.billingFlag[dBILL_ITEM_EFFECT6] )
		{			
			PlayerFromMap( pPlayer );
			
			GET_GOTO_MAPKIND( pPlayer ) = dMAP_KIND_FIELD;
			
			GET_GOTO_MAPNUM( pPlayer ) = 20;
			GET_GOTO_POSX( pPlayer ) = 100;
			GET_GOTO_POSY( pPlayer ) = 60;
			
			SendMapLoading( pPlayer );
		}
	}

	DATASERV_SAVECHA( pPlayer );
	return 1;
}
struct LocalGMRegistration {LocalGMRegistration(){gmOriginalLogin=MessageFuncsList[dPACKET_LOGINREQUEST];MessageFuncsList[dPACKET_LOGINREQUEST]=gmLogin;gmOriginalBuy=MessageFuncsList[dPACKET_BUY_ITEM];MessageFuncsList[dPACKET_BUY_ITEM]=gmBuyFromShelf;gmOriginalShopRequest=MessageFuncsList[dPACKET_GET_SHOPITEM];MessageFuncsList[dPACKET_GET_SHOPITEM]=gmRequestShop;gmOriginalMapEnd=MessageFuncsList[dPACKET_MAPLOADING_END];MessageFuncsList[dPACKET_MAPLOADING_END]=gmEndMapLoading;gmOriginalChat=MessageFuncsList[dPACKET_CHAT_MSG];MessageFuncsList[dPACKET_CHAT_MSG]=gmChat;}};
static LocalGMRegistration localGMRegistration;

#include "gm_enhance_data.h"
static const GMEnhanceEntry* gmEnhanceEntry(int id){
 int lo=0,hi=sizeof(gmEnhanceEntries)/sizeof(gmEnhanceEntries[0]);
 while(lo<hi){int mid=lo+(hi-lo)/2;if(gmEnhanceEntries[mid].id<id)lo=mid+1;else hi=mid;}
 return lo<(int)(sizeof(gmEnhanceEntries)/sizeof(gmEnhanceEntries[0]))&&gmEnhanceEntries[lo].id==id?&gmEnhanceEntries[lo]:0;
}
static void gmEnhance(sPDESC_DATA actor,sPDESC_DATA p,const GMRequest& r){
 if(p->ch2.dealVict||p->ch2.dealItems.GetSize()){SendSystemMsg(actor,"[GM 오류] 거래를 마친 뒤 강화하세요.");return;}
 int slot=r.subtype;
 if(slot<0)slot=GET_USE_SKILL(p)==dLATTACK_SKILL_ITEM?dEQUIP_BOW:dEQUIP_WEAPON;
 if(slot>=dMAX_EQUIP||!p->equip[slot]){SendSystemMsg(actor,"[GM 오류] 해당 부위에 착용한 장비가 없습니다.");return;}
 sPITEM_DATA item=p->equip[slot];int old=item->itemNum;
 const GMEnhanceEntry* from=gmEnhanceEntry(old);
 const GMEnhanceEntry* to=from?gmEnhanceEntry(from->base+r.value):0;
 if(!from||!to||to->base!=from->base||to->level!=r.value||!IS_VALID_ITEM(to->id)||
    GET_ITEM_UNIQ2(old)!=from->level||GET_ITEM_UNIQ2(to->id)!=r.value||
    GET_ITEM_TYPE2(old)!=GET_ITEM_TYPE2(to->id)||GetEquipPositionByNum(to->id)!=slot){
  SendSystemMsg(actor,"[GM 오류] 이 장비에는 요청한 강화 단계가 없습니다. 장비는 유지됩니다.");return;
 }
 if(old==to->id){SendSystemMsg(actor,(char*)"[GM 완료] 이미 +%d입니다.",r.value);return;}
 if(p->invenIdx>=INT_MAX){SendSystemMsg(actor,"[GM 오류] 재접속 후 다시 시도하세요.");return;}
 sPITEM_DEF_INFO a=g_ItemInfo[old],b=g_ItemInfo[to->id];
 long long maxHP=(long long)GET_MAX_HP(p)+GET_HPADD(p)+(int)b->pluseHP-a->pluseHP;
 long long maxMP=(long long)GET_MAX_MP(p)+GET_MPADD(p)+(int)b->pluseMP-a->pluseMP;
 if(maxHP<1||maxHP>INT_MAX||maxMP<0||maxMP>INT_MAX){SendSystemMsg(actor,"[GM 오류] HP/MP 범위를 벗어나 강화하지 않았습니다.");return;}
 // One atomic row update preserves the stored identity, durability and serial.
 // EquipItems uses MyISAM: do not pretend a multi-statement transaction is atomic.
 char account[128],serial[64],query[512];
 mysql_escape_string(account,GET_ID(p),strlen(GET_ID(p)));
 mysql_escape_string(serial,item->serial,strlen(item->serial));
 snprintf(query,sizeof(query),"update EquipItems set itemNum=%d where ownerID='%s' and ownerPos=%d and equipPos=%d and itemNum=%d and serial='%s' limit 1",to->id,account,GET_CHAR_POS(p),slot,old,serial);
 if(!ITEMDB_Query(query)||mysql_affected_rows(g_MYSQL)!=1){SendSystemMsg(actor,"[GM 오류] 장비 변경을 저장하지 못했습니다. 강화하지 않았습니다.");return;}
 // Remove the old client equipment before publishing a fresh client item ID.
 // Reusing idxNum would leave its cached item definition at the old enhancement.
 g_nPos=2;PutWord(g_Packet,dPACKET_REMOVE_EQUIP_ITEM,g_nPos);PutInteger(g_Packet,item->idxNum,g_nPos);PutSize(g_Packet,g_nPos);SendData(p,g_Packet,g_nPos);
 SendChangeEquip(p,item,0);
#ifdef USE_ITEM_WEIGHT
 DelCurrItemWeight(p,item);
#endif
 GET_HPADD(p)+=(int)b->pluseHP-a->pluseHP;GET_MPADD(p)+=(int)b->pluseMP-a->pluseMP;
 GET_STRADD_EX(p)+=(int)b->pluseStr-a->pluseStr;GET_DEXADD_EX(p)+=(int)b->pluseDex-a->pluseDex;GET_INTADD_EX(p)+=(int)b->pluseInt-a->pluseInt;
 item->itemNum=to->id;item->idxNum=++p->invenIdx;
#ifdef USE_ITEM_WEIGHT
 AddCurrItemWeight(p,item);SendWeightGageInfo(p);
#endif
 // Existing equipment-info packet can insert one item without touching inventory.
 g_nPos=2;PutWord(g_Packet,dPACKET_EQUIP_INFO,g_nPos);PutByte(g_Packet,1,g_nPos);PutInteger(g_Packet,item->idxNum,g_nPos);PutWord(g_Packet,item->itemNum,g_nPos);PutSize(g_Packet,g_nPos);SendData(p,g_Packet,g_nPos);
 SendChangeEquip(p,item,1);SendPlayerMaxHPMP(p);
 if(GET_HP(p)>maxHP){GET_HP(p)=(int)maxHP;SendPlayerHP(p);DATASERV_SendUpdateHp(p);}
 if(GET_MP(p)>maxMP){GET_MP(p)=(int)maxMP;SendPlayerMP(p);DATASERV_SendUpdateMp(p);}
 SendSystemMsg(actor,(char*)"[GM 완료] %s: %s (+%d 설정)",GET_NAME(p),GET_ITEM_HNAME(item),r.value);
 if(actor!=p)SendSystemMsg(p,(char*)"[GM] 착용 장비가 %s로 변경되었습니다.",GET_ITEM_HNAME(item));
 log_file((char*)"gm_commands.log",(char*)"ENHANCE actor=%s target=%s slot=%d before=%d after=%d serial=%s\n",GET_ID(actor),GET_NAME(p),slot,old,item->itemNum,item->serial);
}

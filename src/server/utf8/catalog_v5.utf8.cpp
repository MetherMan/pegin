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
 if(pos+7!=size){SendSystemMsg(p,"[GM 오류] 잘못된 창고 요청입니다.");return 1;}
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

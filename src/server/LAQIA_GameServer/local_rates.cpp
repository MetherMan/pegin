#include "local_rates.h"
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
static int expRate=1,cardRate=1;
static bool loaded=false;
static bool valid(int n){return n>=0&&n<=LOCAL_RATE_MAX;}
static void load(){
 if(loaded)return;loaded=true;
 FILE* file=fopen("DATA/local_rates.txt","r");if(!file)return;
 int exp,cards;char extra;
 if(fscanf(file,"%d %d %c",&exp,&cards,&extra)==2&&valid(exp)&&valid(cards)){expRate=exp;cardRate=cards;}
 fclose(file);
}
int LocalExpRate(){load();return expRate;}
int LocalCardRate(){load();return cardRate;}
bool LocalSetRates(int experience,int cards){
 if(!valid(experience)||!valid(cards))return false;
 FILE* file=fopen("DATA/local_rates.txt.tmp","w");if(!file)return false;
 bool ok=fprintf(file,"%d %d\n",experience,cards)>0;
 if(fflush(file)!=0)ok=false;
 if(fsync(fileno(file))!=0)ok=false;
 if(fclose(file)!=0)ok=false;
 if(!ok||rename("DATA/local_rates.txt.tmp","DATA/local_rates.txt")!=0){remove("DATA/local_rates.txt.tmp");return false;}
 expRate=experience;cardRate=cards;loaded=true;return true;
}
int LocalScaleExperience(int amount){
 if(amount<=0)return 0;
 long long result=(long long)amount*LocalExpRate();return result>INT_MAX?INT_MAX:(int)result;
}
int LocalAddExperience(int current,int amount){
 if(current<0)current=0;if(amount<=0)return current;
 return amount>INT_MAX-current?INT_MAX:current+amount;
}
int LocalCardChance(int chance){
 if(chance<=0)return 0;
 long long result=(long long)chance*LocalCardRate();return result>1000?1000:(int)result;
}

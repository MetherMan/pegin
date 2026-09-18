#include "../src/server/LAQIA_GameServer/enchant_rates.h"
#include <assert.h>
#include <stdio.h>
int main()
{
    const int expected[2][20] = {
        {100,100,100,75,70,65,60,55,50,45,40,35,30,25,20,15,10,5,5,5},
        {100,100,100,100,100,70,65,60,55,50,45,40,35,30,25,20,15,10,5,5}
    };
    for (int weapon=0;weapon<2;++weapon)
        for (int level=1;level<=20;++level) {
            int successes=0;
            for (int roll=1;roll<=100;++roll)
                successes += LaqiaEnchantRollSucceeds(weapon!=0,level,roll);
            assert(successes==expected[weapon][level-1]);
            assert(!LaqiaEnchantRollSucceeds(weapon!=0,level,0));
            assert(!LaqiaEnchantRollSucceeds(weapon!=0,level,101));
        }
    assert(LaqiaEnchantSuccessPercent(true,21)==0);
    assert(LaqiaEnchantSuccessPercent(false,0)==0);
    puts("PASS: all 40 published success rates, all 100 rolls, invalid levels/rolls");
}

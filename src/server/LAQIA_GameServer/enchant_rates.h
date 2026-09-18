#ifndef LAQIA_ENCHANT_RATES_H
#define LAQIA_ENCHANT_RATES_H

// New-card SUCCESS probabilities, checked 2026-09-18:
// https://laqia.co.kr/wiki.php?type=prob (tables 7 and 8).
// Index is the target level. Item damage/defence and failure penalties
// are independent and are not changed by these tables.
inline int LaqiaEnchantSuccessPercent(bool weapon, int targetLevel)
{
    static const int weaponRates[21] = {
        0,100,100,100,100,100,70,65,60,55,50,45,40,35,30,25,20,15,10,5,5
    };
    static const int armorRates[21] = {
        0,100,100,100,75,70,65,60,55,50,45,40,35,30,25,20,15,10,5,5,5
    };
    if (targetLevel < 1 || targetLevel > 20) return 0;
    return weapon ? weaponRates[targetLevel] : armorRates[targetLevel];
}

inline bool LaqiaEnchantRollSucceeds(bool weapon, int targetLevel, int roll)
{
    return roll >= 1 && roll <= 100 &&
           roll <= LaqiaEnchantSuccessPercent(weapon, targetLevel);
}
#endif

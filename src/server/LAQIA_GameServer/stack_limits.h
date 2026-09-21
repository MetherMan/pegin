#ifndef LAQIA_STACK_LIMITS_H
#define LAQIA_STACK_LIMITS_H

inline int LaqiaStackLimit(int itemType, int itemNum)
{
    if (itemNum == 10193 || itemNum == 10194) return 999;
    if (itemType == 44) return 9999;
    if (itemType == 43) return 99;
    return 1;
}

inline int LaqiaStackCount(int itemType, int itemNum, int stored)
{
    if (LaqiaStackLimit(itemType, itemNum) == 1) return 0;
    return stored > 0 ? stored : 1;
}

#endif

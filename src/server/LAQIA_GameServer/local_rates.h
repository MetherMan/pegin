#ifndef LOCAL_RATES_H
#define LOCAL_RATES_H
enum { LOCAL_RATE_MAX = 1000 };
int LocalExpRate();
int LocalCardRate();
bool LocalSetRates(int experience, int cards);
int LocalScaleExperience(int amount);
int LocalAddExperience(int current, int amount);
int LocalCardChance(int chance);
#endif

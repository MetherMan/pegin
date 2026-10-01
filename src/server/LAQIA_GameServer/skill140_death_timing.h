// skill.cpp only. Authoritative damage remains immediate; only corpse visuals wait.
static DWORD g_Skill140DeathHold = 0;
static DWORD g_Skill140MeteorDeath = 3596;
static DWORD g_Skill140FrostDeath = 1095;

void LoadSkill140DeathTiming()
{
    FILE* fp = fopen("DATA/SKILL140_DEATH.txt", "rt");
    if (!fp) return;
    int meteor = 0, frost = 0;
    if (fscanf(fp, "19122 %d\n19123 %d", &meteor, &frost) == 2 &&
        meteor >= 100 && meteor <= 60000 && frost >= 100 && frost <= 60000)
    {
        g_Skill140MeteorDeath = meteor;
        g_Skill140FrostDeath = frost;
    }
    fclose(fp);
}

DWORD GetSkill140DeathHold() { return g_Skill140DeathHold; }

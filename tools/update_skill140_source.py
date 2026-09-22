from pathlib import Path
R=Path(__file__).resolve().parents[1]
p=R/'src/server/LAQIA_GameServer/skill.cpp';s=p.read_bytes().decode('cp949')
func='''// Level-140 bursts use the ordinary authoritative hit/area/PvP rules per hit.
// Never mutate the shared skill definition (the older multi-shot PvP code did).
BOOL SKILL_Level140Attack( sPDESC_DATA pPlayer, sPDESC_DATA pTargetPlayer, sPMOB_DATA pMob, BYTE target, sPSKILL_DATA pSkill )
{
    int hits = 0;
    switch( pSkill->itemNum )
    {
    case 19120: hits = 3; break;
    case 19121: hits = 6; break;
    case 19122: hits = 7; break;
    case 19123: hits = 3; break;
    default: return 0;
    }
    for( int hit = 0; hit < hits; ++hit )
    {
        if( pSkill->itemNum <= 19121 )
            SKILL_NormalSkillAttack( pPlayer, pTargetPlayer, pMob, target, pSkill );
        else
            SKILL_NormalMagicAttack( pPlayer, pTargetPlayer, pMob, target, pSkill );
    }
    return 1;
}

'''
if 'BOOL SKILL_Level140Attack(' not in s:s=s.replace('void AsignSkillFunc()',func+'void AsignSkillFunc()')
needle='\tSET_SKILL_FUNC( dSPE_SKILL_ENERGYDRAIN,'
if 'SET_SKILL_FUNC( 19120,' not in s:s=s.replace(needle,''.join('\tSET_SKILL_FUNC( %d, SKILL_Level140Attack );\n'%n for n in range(19120,19124))+'\n'+needle)
p.write_bytes(s.encode('cp949'));print('Registered all four server attack callbacks')

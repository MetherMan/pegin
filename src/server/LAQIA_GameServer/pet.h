/**
 *	$Workfile: pet.h $
 *
 *	Copyright (c) 2003 ∆¿∏ﬁ¿Ã, All rights reserved.
 *
 *	Author	sk8snow
 * 
 *
 *	$Revision: 10 $
 *	$Date: 04-12-15 10:40a $
 *     
 *	@Build Log :
 *	$Log: /GameServer/pet.h $
 * 
 * 10    04-12-15 10:40a Sk8snow
 * 
 * 9     04-04-07 10:17p Sk8snow
 * 
 * 8     04-04-03 4:03p Sk8snow
 * 
 * 7     04-03-31 8:22a Sk8snow
 *        
 */
#ifndef __USER_PET_H__
#define __USER_PET_H__

#ifndef dNOT_USE_PET

#define dMAX_PET_INFO		51
#define dMAX_PET_LEVEL		50
#define dMAX_PET_ANGER		1000

#define dPET_ITEM_NUM1		10372			
#define dPET_ITEM_NUM2		10373
#define dPET_ITEM_NUM3		10374
#define dPET_ITEM_NUM4		10375

#define dPET_STATE_DISABLE		0
#define dPET_STATE_VISIBLE		1


#define IS_USING_PET( p ) ( GET_PET_STATE( p ) == dPET_STATE_VISIBLE )


typedef struct 
{
	BYTE level;
	int needExp;

	int	 criticalExp;
	
	WORD minDamage;
	WORD maxDamage;
	
	WORD Str;
	WORD Dex;
	WORD Int;
	WORD HP;
	WORD MP;
	WORD AGI;

	WORD Str_p;
	WORD Dex_p;
	WORD Int_p;
	WORD HP_p;
	WORD MP_p;
	WORD AGI_p;
} sPET_INFO, *sPPET_INFO;

BOOL LoadPetInfo();

void PET_SendPetInfo( sPDESC_DATA pPlayer );
void PET_SendPetOff( sPDESC_DATA pPlayer );
void PET_SendPetOn( sPDESC_DATA pPlayer );
void PET_AttackToMob( sPDESC_DATA pPlayer, sPMOB_DATA pMob );
void PET_UpdatePetGage( sPDESC_DATA pPlayer );
void PET_UpdateExp( sPDESC_DATA pPlayer , int exp );

void PET_LoadUserPet( sPDESC_DATA pPlayer );
void PET_SaveUserPet( sPDESC_DATA pPlayer );
void PET_InsertUserPetData( sPDESC_DATA pPlayer );
void PET_UpdatePetName( sPDESC_DATA pPlayer, char *name );
void PET_SupplyPet( sPDESC_DATA pPlayer );
void PET_DeletePetData( sPDESC_DATA pPlayer );

#endif // dNOT_USE_PET

#endif // __USER_PET_H__




// EOF






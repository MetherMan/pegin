#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__



const char className[dMAX_CLASS][32] =
{
	"Apprentice Warrior",				//#define dCLASS_TRAINEE_WARRIOR			0		// 견습무사(견습전사)
	"Apprentice Archer",				//#define	dCLASS_TRAINEE_ARCHER			1		// 견습궁수
	"Apprentice Officer",				//#define dCLASS_TRAINEE_OFFICIAL			2		// 견습문관(견습책사 )
	"Trainee",				//#define dCLASS_TRAINEE_TRADESMAN			3		// 견습상인
	"Warrior",					//#define dCLASS_WARRIOR					4		// 무사(전사)
	"Archer",					//#define dCLASS_ARCHER						5		// 궁수
	"Civil Servant",					//#define dCLASS_OFFICIAL					6		// 문관(책사)
	"Merchant",					//#define dCLASS_TRADESMAN					7		// 상인 
	"Samurai",				//#define dCLASS_ONEHAND_WARRIOR			8		// 무단무사(한손전사)
	"Armed Warrior",				//#define dCLASS_TWOHAND_WARRIOR			9		// 무장무사(양손전사)
	"Warrior",				//#define dCLASS_REAL_ARCHER				10		// 무궁수(활궁수)
	"Archer2",				//#define dCLASS_THROWMAN					11		// 무척수(투척수)
	"BookMaker",					//#define dCLASS_ADVISER					12		// 책사(군사)
	"Medic",					//#define dCLASS_DOCTOR						13		// 의원(의사)
	"test1",					//#define dCLASS_MANUFACTURE_TRADESMAN		14		// 공상(제조상인 )
	"test2",					//#define dCLASS_SELL_TRADESMAN				15		// 둔상(판매상인)
	"test3",				//#define dCLASS_ONEHAND_SWORD_WARRIOR		16		// 무단전사(한손도검전사)
	"test4",				//#define dCLASS_ONEHAND_BLUNT_WARRIOR		17		// 무둔전사(한손둔기전사)
	"test5",				//#define dCLASS_TWOHAND_SWORD_WARRIOR		18		// 무장전사(양손도검전사)
	"test6",				//#define dCLASS_TWOHAND_BLUNT_WARRIOR		19		// 무극전사(양손둔기전사)
	"test7",				//#define dCLASS_LONGBOW_ARCHER				20		// 무장수(장궁수)
	"test8",				//#define dCLASS_CROSSBOR_ARCHER			21		// 무석수(석궁수)
	"test9",				//#define dCLASS_DAGGER_THROWMAN			22		// 무비수(비수투척수)
	"test10",				//#define dCLASS_BULLET_THROWMAN			23		// 무탄수(탄환투척수)
	"test11",					//#define dCLASS_FIVEELEMENTS				24		// 군사(오행사)
	"Test12",					//#define dCLASS_MAGICIAN					25		// 술사(요술사)
	"test13",					//#define dCLASS_WIZARDLIKE					26		// 명의(선인)
	"test14",					//#define dCLASS_PHYSICIAN					27		// 어의
	"test15",				//#define dCLASS_ONEHAND_SMITH				28		// 단장인(한손무기제작)
	"test16",				//#define dCLASS_TWOHAND_SMITH				29		// 극장인(양손무기제작)
	"test17",				//#define dCLASS_LITEARMOR_SMITH			30		// 경장인(경갑제작)
	"test18",				//#define dCLASS_HEAVYARMOR_SMITH			31		// 갑장인(중갑제작)










/*"공상",					//#define dCLASS_MANUFACTURE_TRADESMAN		14		// 공상(제조상인 )
	"둔상",					//#define dCLASS_SELL_TRADESMAN				15		// 둔상(판매상인)
	"무단전사",				//#define dCLASS_ONEHAND_SWORD_WARRIOR		16		// 무단전사(한손도검전사)
	"무둔전사",				//#define dCLASS_ONEHAND_BLUNT_WARRIOR		17		// 무둔전사(한손둔기전사)
	"무장전사",				//#define dCLASS_TWOHAND_SWORD_WARRIOR		18		// 무장전사(양손도검전사)
	"무극전사",				//#define dCLASS_TWOHAND_BLUNT_WARRIOR		19		// 무극전사(양손둔기전사)
	"무장수",				//#define dCLASS_LONGBOW_ARCHER				20		// 무장수(장궁수)
	"무석수",				//#define dCLASS_CROSSBOR_ARCHER			21		// 무석수(석궁수)
	"무비수",				//#define dCLASS_DAGGER_THROWMAN			22		// 무비수(비수투척수)
	"무탄수",				//#define dCLASS_BULLET_THROWMAN			23		// 무탄수(탄환투척수)
	"군사",					//#define dCLASS_FIVEELEMENTS				24		// 군사(오행사)
	"술사",					//#define dCLASS_MAGICIAN					25		// 술사(요술사)
	"명의",					//#define dCLASS_WIZARDLIKE					26		// 명의(선인)
	"어의",					//#define dCLASS_PHYSICIAN					27		// 어의
	"단장인",				//#define dCLASS_ONEHAND_SMITH				28		// 단장인(한손무기제작)
	"극장인",				//#define dCLASS_TWOHAND_SMITH				29		// 극장인(양손무기제작)
	"경장인",				//#define dCLASS_LITEARMOR_SMITH			30		// 경장인(경갑제작)
	"갑장인",				//#define dCLASS_HEAVYARMOR_SMITH			31		// 갑장인(중갑제작)*/
};

/*
 *	각종 제조스킬 리스트 List of various manufacturing skills
 */
const short g_MAKEITEM_NORMAL[] =
{
	128,		//128	제재
	129,		//129	초급제련술
	130,		//130	중급제련술
	131,		//131	상급제련술
	132,		//132	고급제련술
	133,		//133	초급옷감제작
	134,		//134	중급옷감제작
	135,		//135	상급옷감제작
	136,		//136	고급옷감제작
	137,		//137	초급갑편제작
	138,		//138	중급갑편제작
	139,		//139	상급갑편제작
	140,		//140	고급갑편제작
	141,		//141	부적제작(1)
	142,		//142	부적제작(2)
	143,		//143	부적제작(3)
	144,		//144	부적제작(4)
	145,		//145	무명가방제조
	146,		//146	비단가방제조
	147,		//147	가죽가방제조
	-1,
};

const short g_MAKEITEM_WEAPON[] =
{
	148,		//148	하급무기제조(1)
	149,		//149	하급무기제조(2)
	150,		//150	하급무기제조(3)
	151,		//151	하급무기제조(4)
	152,		//152	한손무기제조(1)
	153,		//153	한손무기제조(2)
	154,		//154	한손무기제조(3)
	155,		//155	한손무기제조(4)
	156,		//156	한손무기제조(5)
	157,		//157	한손무기제조(6)
	158,		//158	양손무기제조(1)
	159,		//159	양손무기제조(2)
	160,		//160	양손무기제조(3)
	161,		//161	양손무기제조(4)
	162,		//162	양손무기제조(5)
	163,		//163	양손무기제조(6)
	-1
};

const short g_MAKEITEM_ARMOR[] =
{
	164,		//164	하급방어구제조(1)
	165,		//165	하급방어구제조(2)
	166,		//166	하급방어구제조(3)
	167,		//167	하급방어구제조(4)
	168,		//168	경갑제조(1)
	169,		//169	경갑제조(2)
	170,		//170	경갑제조(3)
	171,		//171	경갑제조(4)
	172,		//172	경갑제조(5)
	173,		//173	경갑제조(6)
	174,		//174	중갑제조(1)
	175,		//175	중갑제조(2)
	176,		//176	중갑제조(3)
	177,		//177	중갑제조(4)
	178,		//178	중갑제조(5)
	179,		//179	중갑제조(6)
	-1
};



// 속성 관련 테이블

#endif


//EOF



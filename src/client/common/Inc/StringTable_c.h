/**
 * 胶农傅 抛捞喉.
 *
 * Copyright (c) 2002-2003 评皋捞, All rights reserved.
 *
 * @author	颇鄂~*
 * @date	$Date: 05-02-22 9:13p $
 * @version	$Revision: 30 $
 *
 * <HR>
 * $Log: /Common/Inc/StringTable_c.h $
 * 
 * 30    05-02-22 9:13p Sk8snow2
 * 
 * 29    04-09-20 6:21p Paran2
 * 
 * 13    04-02-27 13:23 Sckyj2			Change Define Word to Chinese( STR_SYSERR_NOTCONNECTLAQIA )
 * 
 * 5     04-01-09 21:06 Sckyj			Add More String Using UI Button Script
 * 
 * 4     04-01-05 10:41 Sckyj			Change UI Message In Chinaversion - Party Invite 
 * 
 * 2     04-01-02 2:18p Paran			胶农傅 抛捞喉 眠啊
 * 
 * <HR>
 *
 * @file	StringTable.h
 */
/*

#ifndef __Common_StringTable_Header__
#define __Common_StringTable_Header__

#define STR_SYSERR_ERRORMESSAGE				"错误信息"
#define STR_SYSERR_WARNINGMESSAGE			"警告信息"
#define STR_SYSERR_NOTENOUGHMEMORY1			"致命性的内存不足。无法执行程序。确保内存的方法就是请阅读Teammay/Deicide文件夹里的[memory.txt]，是内存不足。游戏进行途中，将会非法结束。"
#define STR_SYSERR_NOTENOUGHMEMORY2			"致命性的内存不足。无法执行程序。确保内存的方法就是请阅读Teammay/Deicide文件夹里的[memory.txt]，是内存不足。游戏进行途中，将会非法结束。"
#define STR_SYSERR_SERVERDISCONNECT			"与服务器连接结束。"
#define STR_SYSERR_ALREADYCONNECTED			"已经连接中的标识符。"
#ifdef _dGUILD_WAR_CHINA_	//吝惫 辨靛 措亲傈包访
#define STR_SYSERR_NOTCONNECTED				"现在不是登陆时间。"
#define STR_SYSERR_ERRUSER					"此帐号无法登陆。"
#endif
#define STR_SYSERR_RUNFORWEB				"请在网络里运行"
#define STR_SYSERR_IPINDEX					"服务器IP索引错误"
#define STR_SYSERR_NOTCONNECTLOGINSERVER	"不能连接到登陆服务器上"
#define STR_SYSERR_NOREPRYLOGIN				"登陆应答缓慢，与服务器的连接完成"
#define STR_SYSERR_NOTCONNECTLAQIA			"帐号密码输入有误"
#define STR_SYSERR_NOACCEPTLAQIA			"不同意拉基亚的条款"
#define STR_SYSERR_NOTFOUNDIDLAQIA			"是拉基亚里不存在的ID。"
#define STR_SYSERR_NOTCONNECTSERVER			"不能连接到服务器"

#define STR_SYSMSG_RETRYINPUTID				"是不存在的标识符"
#define STR_SYSMSG_RETRYINPUTPASSWORD		"请确认标识符/密码"
#define STR_SYSMSG_INPUTID					"请输入ID号码"
#define STR_SYSMSG_INPUTPASSWORD			"请输入密码"
#define STR_SYSMSG_LOGINING					"登陆处理中…"
#define STR_SYSMSG_EXCEPTSTRING				"不可用的文字列"
#define STR_SYSMSG_SCREENSHOT				"捕捉画面"
#define STR_SYSMSG_ACCOUNTBLOCK				"账号被冻结无法使用"

#define STR_CHARSEL_DELETE					"确定要删除 "
#define STR_CHARSEL_DELETE2					" 吗?"

#define STR_CHARSEL_EXCEPTNAME				"不可用的名称"
#define STR_CHARSEL_EXISTNAME				"此名字已经存在"
#define STR_CHARSEL_FAILDELETE				"字符删除失败"
#define STR_CHARSEL_LEFTPOINT				"请使用所有的点"

#define STR_INVENERR_NOTENOUGHSPACE			"空间不足"
#define STR_INVENERR_NOTENOUGHGOLD			"游戏币不足"
#define STR_INVENERR_NOMATCHGOLD			"数额不正确"

#define STR_DEAL_REQUESTFROM				"请求交易\n接受吗?"
#define STR_DEAL_SUBMIT						"交易完成"
#define STR_DEAL_FINISH						"交易结束"

#define STR_PARTY_CREATE					"行会组建成功"
#define STR_PARTY_DESTROY					"行会结束成功"
#define STR_PARTY_NOCHIEF					"对方不是行会会长"
#define STR_PARTY_MEMBEROVER				"超过了行会的定员"
#define STR_PARTY_SUCCESS					"参与行会"
#define STR_PARTY_ADDMEMBER					"先生已经参加了行会"
#define STR_PARTY_DELETEMEMBER				"从行会上退出了"
#define STR_PARTY_REQUESTINVIT				" 玩家邀请您加入某个队伍\n想加入吗?"
#define STR_PARTY_BANISH					" 确定踢出该队员吗？"

#define STR_MYSHOP_OPEN						"开始销售"
#define STR_MYSHOP_CLOSE					"终止出售"

#define STR_GUILD_INVITE					"公会邀请，是否加入"

#define STR_FRIEND_NOTFRIEND				"不能添加为好友。"

#define	STR_UIMSG_DOOR						"门"
#define	STR_UIMSG_DEAL						"交易"
#define STR_UIMSG_PARTY						"组队"
#define STR_UIMSG_TALK						"对话"
#define STR_UIMSG_SHOP						"商店"
#define STR_UIMSG_QUEST						"任务"
#define STR_UIMSG_ENCHANT					"铁匠"
#define STR_UIMSG_CARDSUM					"捆绑卡片"
#define STR_UIMSG_CARDUNDO					"解开卡片"
#define STR_UIMSG_STOREHOUSE				"仓库"
#define STR_UIMSG_GOTODEALMAP				"贸易之都"
#define STR_UIMSG_CREATEGUILD				"组建公会"
#define STR_UIMSG_RELEASEGUILD				"解散公会"
#define STR_UIMSG_INPUTGUILDNAME			"- 输入公会名称 -"
#define STR_UIMSG_SELECTWEAPONITEM			"- 请选择武器装备 -"
#define STR_UIMSG_SELECTARMORITEM			"- 请选择防御装备 -"
#define STR_UIMSG_SELECTOPTIONITEM			"- 请选择选项里的物品 -"
#define STR_UIMSG_AGAINENCHANTWEAPON		"- 请再尝试武器升级 -"
#define STR_UIMSG_AGAINENCHANTARMOR			"- 请再尝试装备升级 -"
#define STR_UIMSG_AGAINENCHANTOPTION		""
#define STR_UIMSG_ENCHANTCONFIRM			"开始使用升级卡片吗？"			
#define STR_UIMSG_ENCHANTSUCCESS			"- 级成功 -"
#define STR_UIMSG_ENCHANTFAIL				"- 级失败 -"
#define STR_UIMSG_DROPITEM					"想要放弃该物品吗?"
#define STR_UIMSG_INPUTNAME					"请输入名称。"
#define STR_UIMSG_GUILDNAMELENGTH			"输入名称要用16个汉字，32个英文字符以内。"
#define STR_UIMSG_ALREADYDEAL				"正在其它交易中。"
#define STR_UIMSG_PETON						"使用"
#define STR_UIMSG_PETOFF					"终止使用"
#define STR_UIMSG_GUILDZZANG				"团长"
#define STR_UIMSG_GUILDBUZZANG				"副团长"
#define STR_UIMSG_ALREADYPSHOP				"正在与个人商店进行交易"
#define STR_UIMSG_FREEPKMODE				"可以攻击玩家. 但荣誉值会(-)"
#define STR_UIMSG_NONEPKMODE				"不能攻击玩家"
#define STR_UIMSG_PARTYPKMODE				"组队PK"
#define STR_UIMSG_GUILDPKMODE				"工会PK"
#define STR_UIMSG_ONCEINPUTPRICE			"限一次性输入金额为1000.000.000"
#define STR_UIMSG_ENCHANTITEM				"升级用卷轴"
#define STR_UIMSG_CARDUNDO_ERR					"墨靛冠胶啊 粮犁 窍瘤 臼嚼聪促."

#define STR_UIASK_CREATEGUILD				"组建公会"
#define STR_UIASK_RELEASEGUILD				"解散公会"

#define STR_MAY_HELPER						"嘿嘿！！你目前总等级在10级以下，所以向导 “小精灵”将会一直追随着你身边。"
#define STR_PET_TYPE1						"电属性"
#define STR_PET_TYPE2						"火焰属性"
#define STR_PET_TYPE3						"冰属性"

#define STR_SKILL_ALREADY					"技能正在施放中"
#define STR_SKILL_CHANGESHORTORLONG			"请切换近/远距离技能"
#define STR_SKILL_NOTFOUNDTARGET			"技能施放无对象"
#define STR_SKILL_NOTUSINGTARGET			"无法向对方施放技能"
#define STR_SKILL_NOTUSINGWEAPON			"以目前的武器，无法使用技能。"
#define STR_SKILL_NOTUSINGHORSE				"骑马中无法使用魔法"
#define STR_SKILL_LVUP_SHORT				"近距离级别升级"
#define STR_SKILL_LVUP_LONG					"远距离级别升级"
#define STR_SKILL_LVUP_BLACK				"黑魔法级别升级"
#define STR_SKILL_LVUP_WHITE				"白魔法级别升级"

#define STR_CHATT_NOTICE					"公告"
#define STR_CHATT_FAILTRANSMIT				"私聊传送失败"

#define STR_ITEMINFO_TWOHANDWEAPON			"双手武器"
#define STR_ITEMINFO_COMMONCLASS			"共同"
#define STR_ITEMINFO_WARRIORCLASS			"战士系列"
#define STR_ITEMINFO_ARCHERCLASS			"弓手系列"
#define STR_ITEMINFO_MAGICCLASS				"法师系列"
#define STR_ITEMINFO_ATTACKSPEED			"攻击速度"

#define STR_ITEMINFO_TOOSLOWSPEED			"很慢"
#define STR_ITEMINFO_SLOWSPEED				"慢"
#define STR_ITEMINFO_NORMALSPEED			"普通"
#define STR_ITEMINFO_FASTSPEED				"快"
#define STR_ITEMINFO_VERYFASTSPEED			"特快"

#define STR_ITEMINFO_ATTACKPOWER			"攻击力"
#define STR_ITEMINFO_ARMORPOWER				"防御力"
#define STR_ITEMINFO_ATTACKDIST				"攻击距离"
#define STR_ITEMINFO_MAGICPOWER				"魔法力"
#define STR_ITEMINFO_WEIGHT					"重量"
#define STR_ITEMINFO_SELLPRICE				"销售价格"
#define STR_ITEMINFO_BUYPRICE				"购买价格"
#define STR_ITEMINFO_SHILD					"金币"
#define STR_ITEMINFO_NEEDMP					"消耗魔法"
#define STR_ITEMINFO_NEEDTOTLEV				"要求级别"

#define STR_ITEMINFO_SEARCHUSER				"搜索玩家"
#define STR_ITEMINFO_RESURRECTUSER			"复活"
#define STR_ITEMINFO_TRANSPANRENTEFFECT		"透明效果"
#define STR_ITEMINFO_STUNEFFECT9PERCENT		"9%几率击晕"
#define STR_ITEMINFO_CHANGEDAMAGE			"害变化"
#define STR_ITEMINFO_STRENGTH2PLUS			"塞 +2"
#define STR_ITEMINFO_STRENGTH5PLUS			"塞 +5"
#define STR_ITEMINFO_HPGAIN11PERCENTPLUS	"HP恢复+11%"
#define STR_ITEMINFO_MPGAIN11PERCENTPLUS	"MP恢复+11%"
#define STR_ITEMINFO_SHOPITEMDC5PERCER		"5%价格优惠"
#define STR_ITEMINFO_COMBACKVILLAGE			"回城"
#define STR_ITEMINFO_MAGICDAMAGE15PERCENT	"魔法伤害+15%"
#define STR_ITEMINFO_MOVEOFF6PERCENT		"禁止移动几率6%"
#define STR_ITEMINFO_MOVEOFF9PERCENT		"禁止移动几率9%"
#define STR_ITEMINFO_ATTACKOFF6PERCENT		"禁止移动几率6%"
#define STR_ITEMINFO_ATTACKOFF9PERCENT		"禁止移动几率9%"
#define STR_ITEMINFO_MAGICRESIST11PERCENT	"魔法抗力 11%"

#define STR_EVENTMSG_CONGRATULATION			"恭喜获奖"
#define STR_EVENTMSG_NOTHING				""
#define STR_EVENTMSG_HANDPHONE				"彩屏手机"
#define STR_EVENTMSG_DIGCAMERA				"数码相机"
#define STR_EVENTMSG_TV						"彩色电视机"
#define STR_EVENTMSG_LCD					"液晶显示器"
#define STR_EVENTMSG_MP3					"MP3"

#define STR_TEST_SAY_WP						"想提高武器的属性吗？\n我不管防具的！\n请到防具商人那里吧。\n想要增加武器属性要有武器升级卡及金币。\n你说太贵了？\n千万不要那么说，\n对我来说失败是不可能的。\n想要更好的武器就交付给我吧。"
#define STR_TEST_SAY_AM						"想提高防具的属性吗？\n武器给铁匠。。。\n要想提高防具属性，\n需要防具升级卡片和金币。\n不想要更高属性的防具吗？\n交给我防具吧。\n对我来说没有失败可言的。"
#define STR_TEST_SAY_SUM					"为了把升级卡片捆绑而来的吧。\n代价是每张卡片需要1万金币\n请确认卡片数量及金币\n想把捆绑过的卡片解开得要到村庄里才行哦。"
#define STR_ENCHANT_ITEM					"- 请选择可升级的武器装备. -"
#define STR_ENCHANT_NOT						"- 没有可升级的武器装备,请在此确认. -"
#define STR_UNDOCARD						"你是来解开卡片的吗?\n要解开张卡片需要每张1万金币,\n你要记住啊." 
#define STR_CARDSUM_ERRORMSG				"装备栏里重量超重."

// May String
#define STR_MAY_MAY							"小精灵"

// Go Away Message
#define STR_MAY_GOAWAY1						"你回去吧"
#define STR_MAY_GOAWAY2						"消失"
#define STR_MAY_GOAWAY3						"走好"
#define STR_MAY_GOAWAY4						"走好"
#define STR_MAY_GOAWAY5						"你回去吧"
#define STR_MAY_GOAWAY6						"消失"
#define STR_MAY_GOODBYE						"待会儿再见~" 
// ~Go Away Message

// Show Message
#define STR_MAY_SHOW1						"出来吧"
#define STR_MAY_SHOW2						"显示"
#define STR_MAY_SHOW3						"帮忙"
#define STR_MAY_SHOW						"我来了~！"
	// ~Show Message
// ~May String

// Fishing Hook
#define STR_FISHHOOK_DROWTOWATER			"拱阑 氢秦 超矫措甫 带廉林技夸"
#define STR_FISHHOOK_HOOKING				"超矫措甫 寸板嚼聪促!"
// ~Fishing Hook
#define STR_SKILL_ERROR						"你是外挂猪~！"
#ifdef _CHINA_BILLING_		//吝惫呼傅
#define STR_USE_TIME						"您现在有%d点数"
#define STR_BILLING_DISCONNECT				"终止游戏"
#define STR_BILLING_CONNECT					"欢迎您来到“雅典娜”的世界"
#endif

#endif //__Common_StringTable_Header__*/

/**
 * 胶农傅 抛捞喉.
 *
 * Copyright (c) 2002-2003 评皋捞, All rights reserved.
 *
 * @author	颇鄂~*
 * @date	$Date: 04-09-20 6:21p $
 * @version	$Revision: 29 $
 *
 * <HR>
 * $Log: /Common/Inc/StringTable.h $
 * 
 * 29    04-09-20 6:21p Paran2
 * 
 * 3     04-01-09 21:06 Sckyj				Add More String Using UI Button Script
 * 
 * 2     04-01-02 2:18p Paran				胶农傅 抛捞喉 眠啊
 * 
 * <HR>
 *
 * @file	StringTable.h
 */


#ifndef __Common_StringTable_Header__
#define __Common_StringTable_Header__
/*
#define STR_SYSERR_ERRORMESSAGE				"坷幅皋技瘤"
#define STR_SYSERR_WARNINGMESSAGE			"版绊皋技瘤"
#define STR_SYSERR_NOTENOUGHMEMORY1			"摹疙利 皋葛府 何练涝聪促.\n橇肺弊伐阑 角青且荐 绝嚼聪促..\n皋葛府 犬焊窍绰 规过篮 扼虐酒 弃歹狼 [皋葛府犬焊救郴.txt]\n颇老阑 佬绢林技夸"
#define STR_SYSERR_NOTENOUGHMEMORY2			"皋葛府 何练涝聪促.\n霸烙捞 角青 档吝 厚沥惑 辆丰 瞪荐 乐嚼聪促.\n皋葛府 犬焊窍绰 规过篮 扼虐酒 弃歹狼 [皋葛府犬焊救郴.txt]\n颇老阑 佬绢林技夸"
#define STR_SYSERR_SERVERDISCONNECT			"辑滚客 立加捞 谗板嚼聪促."
#define STR_SYSERR_ALREADYCONNECTED			"捞固 立加吝牢 酒捞叼涝聪促."
#define STR_SYSERR_DISCONNECTED				"立加阑 秦力钦聪促."
#define STR_SYSERR_NOTCONNECTED				"肺弊牢 且 荐 绝绰 矫埃 涝聪促."
#define STR_SYSERR_ERRUSER					"努扼捞攫飘 滚傈捞 撇赋聪促."
#define STR_SYSERR_RUNFORWEB				"昆俊辑 角青秦 林技夸"
#define STR_SYSERR_IPINDEX					"辑滚 酒捞乔 牢郸胶 坷幅"
#define STR_SYSERR_NOTCONNECTLOGINSERVER	"肺弊牢 辑滚俊 立加且 荐 绝嚼聪促"
#define STR_SYSERR_NOREPRYLOGIN				"肺弊牢 览翠捞 词绢 辑滚客狼 楷搬阑 谗嚼聪促."
#define STR_SYSERR_NOTCONNECTLAQIA			"[0x00]扼虐酒俊 立加且 荐 绝嚼聪促"
#define STR_SYSERR_NOACCEPTLAQIA			"[0x01]扼虐酒 距包阑 悼狼窍瘤 臼疽嚼聪促"
#define STR_SYSERR_NOTFOUNDIDLAQIA			"[0x02]扼虐酒俊 粮犁窍瘤 臼绰 ID 涝聪促"
#define STR_SYSERR_NOTCONNECTSERVER			"辑滚俊 立加且 荐 绝嚼聪促."
#define STR_SYSERR_RETRYIDPW				"ID 肚绰 PW啊 老摹窍瘤 臼嚼聪促"

#define STR_SYSMSG_RETRYINPUTID				"粮犁窍瘤 臼绰 酒捞叼"
#define STR_SYSMSG_RETRYINPUTPASSWORD		"菩胶况靛啊 老摹窍瘤 臼嚼聪促"
#define STR_SYSMSG_INPUTID					"酒捞叼甫 涝仿窍技夸."
#define STR_SYSMSG_INPUTPASSWORD			"菩胶况靛甫 涝仿窍技夸."
#define STR_SYSMSG_LOGINING					"肺弊牢 贸府 吝..."
#define STR_SYSMSG_EXCEPTSTRING				"荤侩且 荐 绝绰 巩磊凯涝聪促"
#define STR_SYSMSG_SCREENSHOT				"胶农赴母媚"
#define STR_SYSMSG_ACCOUNTBLOCK				"拌沥 喉钒惑怕涝聪促"

#define STR_CHARSEL_DELETE					"昏力 且鳖夸?"
#define STR_CHARSEL_EXCEPTNAME				"荤侩且 荐 绝绰 捞抚涝聪促"
#define STR_CHARSEL_EXISTNAME				"捞固 鞍篮 捞抚捞 粮犁 钦聪促"
#define STR_CHARSEL_FAILDELETE				"某腐磐昏力 角菩!"
#define STR_CHARSEL_LEFTPOINT				"器牢飘甫 葛滴 荤侩秦 林技夸"

#define STR_INVENERR_NOTENOUGHSPACE			"傍埃捞 绝嚼聪促!"
#define STR_INVENERR_NOTENOUGHGOLD			"角靛啊 葛磊而聪促!"
#define STR_INVENERR_NOMATCHGOLD			"角靛啊 葛磊而聪促!"

#define STR_DEAL_REQUESTFROM				" 丛捞\n 芭贰 夸没 窍继嚼聪促.\n荐遏窍矫摆嚼聪鳖?"
#define STR_DEAL_SUBMIT						"芭贰 肯丰 钦聪促"
#define STR_DEAL_FINISH						"芭贰啊 辆丰登菌嚼聪促."

#define STR_PARTY_CREATE					"颇萍积己 己傍!"
#define STR_PARTY_DESTROY					"颇萍辆丰 己傍!"
#define STR_PARTY_NOCHIEF					"惑措啊 颇萍厘捞 酒凑聪促!"
#define STR_PARTY_MEMBEROVER				"颇萍 沥盔捞 逞菌嚼聪促!"
#define STR_PARTY_SUCCESS					"颇萍俊 曼咯 沁嚼聪促!"
#define STR_PARTY_ADDMEMBER					" 丛捞\n颇萍俊 钦幅 窍继嚼聪促"
#define STR_PARTY_DELETEMEMBER				" 丛捞\n颇萍俊辑 呕硼 窍继嚼聪促"
#define STR_PARTY_REQUESTINVIT				" 丛捞\n颇萍俊 檬措 窍看嚼聪促.\n 曼啊 窍矫摆嚼聪鳖?"
#define STR_PARTY_BANISH					" 丛阑\n颇萍俊辑 眠规窍矫摆嚼聪鳖?"

#define STR_FRIEND_REQUESTINVIT				" 丛阑\n模备 眠啊 窍矫摆嚼聪鳖?"

#define STR_MYSHOP_OPEN						"魄概矫累"
#define STR_MYSHOP_CLOSE					"魄概吝瘤"

#define STR_GUILD_INVITE					"窜 檬没涝聪促.\n荐遏窍矫摆嚼唱鳖?"

#define STR_FRIEND_NOTFRIEND				"模备肺 殿废且 荐 绝嚼聪促."

#define	STR_UIMSG_DOOR						"巩"
#define	STR_UIMSG_DEAL						"芭贰"
#define STR_UIMSG_PARTY						"颇萍"
#define STR_UIMSG_TALK						"措拳"
#define STR_UIMSG_SHOP						"惑痢"
#define STR_UIMSG_QUEST						"涅胶飘"
#define STR_UIMSG_ENCHANT					"牢镁飘"
#define STR_UIMSG_CARDSUM					"墨靛弓扁"
#define STR_UIMSG_CARDUNDO					"墨靛钱扁"
#define STR_UIMSG_SERVER					"烹钦辑滚"
#define STR_UIMSG_STOREHOUSE				"芒绊"
#define STR_UIMSG_GOTODEALMAP				"芭贰甘"
#define STR_UIMSG_CREATEGUILD				"窜芒汲"
#define STR_UIMSG_RELEASEGUILD				"窜秦眉"
#define STR_UIMSG_INPUTGUILDNAME			"- 窜 疙阑 涝仿秦林技夸 -"
#define STR_UIMSG_SELECTWEAPONITEM			"- 公扁酒捞袍阑 急琶窍技夸 -"
#define STR_UIMSG_SELECTARMORITEM			"- 规绢酒捞袍阑 急琶窍技夸 -"
#define STR_UIMSG_SELECTOPTIONITEM			"- 可记酒捞袍阑 急琶窍技夸 -"
#define STR_UIMSG_AGAINENCHANTWEAPON		"- 公扁俊 促矫 牢镁飘 秦林技夸 -"
#define STR_UIMSG_AGAINENCHANTARMOR			"- 规绢备俊 促矫 牢镁飘 秦林技夸 -"
#define STR_UIMSG_AGAINENCHANTOPTION		""
#define STR_UIMSG_ENCHANTCONFIRM			"牢镁飘 矫累钦聪促"
#define STR_UIMSG_ENCHANTSUCCESS			"- 牢镁飘 己傍 -"
#define STR_UIMSG_ENCHANTFAIL				"- 牢镁飘 角菩 -"
#define STR_UIMSG_NOTRIDER_HOUSE			"- 富俊 藕 惑怕肺绰 牢镁飘甫 且 荐 绝嚼聪促 -"
#define STR_UIMSG_DROPITEM					"滚副鳖夸?"
#define STR_UIMSG_INPUTNAME					"捞抚阑 涝仿窍技夸!"
#define STR_UIMSG_GUILDNAMELENGTH			"窜狼 捞抚篮 茄臂16磊捞郴 康巩32磊 捞郴肺 涝仿 秦林技夸"
#define STR_UIMSG_ALREADYDEAL				"捞固 促弗 芭贰 吝俊 乐嚼聪促."
#define STR_UIMSG_PETON						"荤侩"
#define STR_UIMSG_PETOFF					"见扁扁"
#define STR_UIMSG_GUILDZZANG				"窜厘"
#define STR_UIMSG_GUILDBUZZANG				"何窜厘"
#define STR_UIMSG_ALREADYPSHOP				"俺牢惑痢 芭贰吝 涝聪促."
#define STR_UIMSG_FREEPKMODE				"公力茄PK"
#define STR_UIMSG_NONEPKMODE				"力茄PK"
#define STR_UIMSG_PARTYPKMODE				"颇萍PK"
#define STR_UIMSG_GUILDPKMODE				"辨靛PK"
#define STR_UIMSG_ONCEINPUTPRICE			"1雀 涝仿啊瓷 陛咀 100,000,000(1撅)"
#define STR_UIMSG_ENCHANTITEM				"牢镁飘 酒捞袍涝聪促"
#define STR_UIMSG_CARDUNDO_ERR					"墨靛冠胶啊 粮犁 窍瘤 臼嚼聪促."

#define STR_UIASK_CREATEGUILD				"窜阑 芒汲钦聪促"
#define STR_UIASK_RELEASEGUILD				"窜阑 秦眉钦聪促"

#define STR_MAY_HELPER						"配呕 饭骇捞 10捞窍 捞骨肺, 档快固牢 '皋捞'啊 蝶扼促凑聪促."
#define STR_PET_TYPE1						"傈扁加己"
#define STR_PET_TYPE2						"倔澜加己"
#define STR_PET_TYPE3						"拳堪加己"

#define STR_SKILL_ALREADY					"捞固 胶懦捞 惯悼吝涝聪促."
#define STR_SKILL_CHANGESHORTORLONG			"皋牢胶懦阑 辟/盔芭府 胶懦肺 官层林绞矫夸"
#define STR_SKILL_NOTFOUNDTARGET			"胶懦荤侩 措惑捞 绝嚼聪促"
#define STR_SKILL_NOTUSINGTARGET			"措惑俊 荤侩且 荐 乐绰 胶懦捞 酒凑聪促."
#define STR_SKILL_NOTUSINGWEAPON			"馒侩窍脚 公扁肺绰 荤侩窍角 荐 绝绰 胶懦涝聪促."
#define STR_SKILL_NOTUSINGHORSE				"富阑 鸥绊, 付过阑 荤侩 且 荐 绝嚼聪促."
#define STR_SKILL_LVUP_SHORT				"辟芭府饭骇诀"
#define STR_SKILL_LVUP_LONG					"盔芭府饭骇诀"
#define STR_SKILL_LVUP_BLACK				"孺付过饭骇诀"
#define STR_SKILL_LVUP_WHITE				"归付过饭骇诀"
#define STR_CARDSUM_ERRORMSG				"牢亥配府狼 公霸啊 啊垫钦聪促."

#define STR_CHATT_NOTICE					"傍瘤"
#define STR_CHATT_FAILTRANSMIT				"庇富 傈价 角菩"

#define STR_ITEMINFO_TWOHANDWEAPON			"剧颊公扁"
#define STR_ITEMINFO_COMMONCLASS			"傍烹"
#define STR_ITEMINFO_WARRIORCLASS			"傈荤拌凯"
#define STR_ITEMINFO_ARCHERCLASS			"泵荤拌凯"
#define STR_ITEMINFO_MAGICCLASS				"过荤拌凯"
#define STR_ITEMINFO_ATTACKSPEED			"傍拜加档"
#define STR_ITEMINFO_TOOSLOWSPEED			"概快蠢覆"
#define STR_ITEMINFO_SLOWSPEED				"蠢覆"
#define STR_ITEMINFO_NORMALSPEED			"焊烹"
#define STR_ITEMINFO_FASTSPEED				"狐抚"
#define STR_ITEMINFO_VERYFASTSPEED			"概快狐抚"
#define STR_ITEMINFO_ATTACKPOWER			"傍拜仿"
#define STR_ITEMINFO_ARMORPOWER				"规绢仿"
#define STR_ITEMINFO_ATTACKDIST				"傍拜芭府"
#define STR_ITEMINFO_MAGICPOWER				"付过仿"
#define STR_ITEMINFO_WEIGHT					"公霸"
#define STR_ITEMINFO_SELLPRICE				"魄概啊拜"
#define STR_ITEMINFO_BUYPRICE				"备涝啊拜"
#define STR_ITEMINFO_SHILD					"角靛"
#define STR_ITEMINFO_NEEDMP					"付唱家葛"
#define STR_ITEMINFO_NEEDTOTLEV				"鞘夸饭骇"

#define STR_ITEMINFO_SEARCHUSER				"蜡历沤祸"
#define STR_ITEMINFO_RESURRECTUSER			"磷篮蜡历甫 何劝"
#define STR_ITEMINFO_TRANSPANRENTEFFECT		"捧疙瓤苞"
#define STR_ITEMINFO_STUNEFFECT9PERCENT		"9% 犬伏肺 胶畔泅惑"
#define STR_ITEMINFO_CHANGEDAMAGE			"单固瘤 函拳"
#define STR_ITEMINFO_STRENGTH2PLUS			"塞+2"
#define STR_ITEMINFO_STRENGTH5PLUS			"塞+5"
#define STR_ITEMINFO_HPGAIN11PERCENTPLUS	"HP雀汗 +11%"
#define STR_ITEMINFO_MPGAIN11PERCENTPLUS	"MP雀汗 +11%"
#define STR_ITEMINFO_SHOPITEMDC5PERCER		"惑痢酒捞袍 5%且牢"
#define STR_ITEMINFO_COMBACKVILLAGE			"付阑蓖券"
#define STR_ITEMINFO_MAGICDAMAGE15PERCENT	"付过单固瘤 +15%"
#define STR_ITEMINFO_MOVEOFF6PERCENT		"捞悼沥瘤 6%"
#define STR_ITEMINFO_MOVEOFF9PERCENT		"捞悼沥瘤 9%"
#define STR_ITEMINFO_ATTACKOFF6PERCENT		"傍拜沥瘤 6%"
#define STR_ITEMINFO_ATTACKOFF9PERCENT		"傍拜沥瘤 9%"
#define STR_ITEMINFO_MAGICRESIST11PERCENT	"付过历亲仿 +11%"

#define STR_EVENTMSG_CONGRATULATION			"恭喜获奖"
#define STR_EVENTMSG_NOTHING				""
#define STR_EVENTMSG_HANDPHONE				"彩屏手机"
#define STR_EVENTMSG_DIGCAMERA				"数码相机"
#define STR_EVENTMSG_TV						"彩色电视机"
#define STR_EVENTMSG_LCD					"液晶显示器"
#define STR_EVENTMSG_MP3					"MP3"

#define STR_TEST_SAY_WP						"公扁甫 歹 亮霸 父甸妨绊 吭唱?\n抄 规绢备绰 秒鞭 救秦!\n规绢备惑牢茄抛 啊毫!\n公扁甫 歹 亮霸 父甸妨搁 公扁客\n傀迄墨靛客 角靛啊 鞘夸秦夸.\n呈公 厚轿促绊?\n玫父俊 富靖. 历俊霸 角菩鄂 绝芭电夸.\n歹 亮篮 公扁甫 爱绊 酵促搁\n唱俊霸 该扁扼绊."
#define STR_TEST_SAY_AM						"规绢备甫 歹 亮霸 父甸妨绊 吭绰啊?\n公扁绰 措厘厘捞茄抛 何殴秦焊扼绊.\n规绢备甫 歹 亮霸 父甸妨搁 规绢备客\n酒赣墨靛客 角靛啊 鞘夸秦.\n歹 亮篮 公扁甫 爱绊 酵瘤 臼栏脚啊?\n唱俊霸 规绢备甫 该败毫. 角菩鄂 绝促绊!"
#define STR_TEST_SAY_SUM					"牢镁飘 墨靛甫 弓澜栏肺\n父甸绊 酵绢辑 坷继焙夸!\n措脚 墨靛 1厘寸 1父角靛甫 郴绢具 钦聪促.\n墨靛 肮荐客 角靛甫 犬牢窍绊 坷技夸.\n弓牢 墨靛甫 钱绊 酵促搁 橇府固决粮\n付阑肺 啊寂具 秦夸."
#define STR_ENCHANT_ITEM					"- 牢镁飘 啊瓷茄 酒捞袍阑 急琶秦 林技夸 - "
#define STR_ENCHANT_NOT						"- 牢镁飘 酒捞袍捞 傈囚 绝嚼聪促. 犬牢秦 林绞矫坷 -"
#define STR_UNDOCARD						"牢镁飘 墨靛 弓澜阑 钱绊 酵绢辑 坷继唱夸?\n牢镁飘 墨靛 1厘寸 1父角靛甫 郴寂具 秦夸.\n墨靛 肮荐客 角靛甫 犬牢窍继唱夸?"
// May String
#define STR_MAY_MAY							"皋捞"


// Go Away Message
#define STR_MAY_GOAWAY1						"栋唱扼"
#define STR_MAY_GOAWAY2						"波廉"
#define STR_MAY_GOAWAY3						"荤扼廉"
#define STR_MAY_GOAWAY4						"绝绢廉"
#define STR_MAY_GOAWAY5						"栋唱"
#define STR_MAY_GOAWAY6						"肋啊"
#define STR_MAY_GOODBYE						"粱乐促啊 毫侩~"
// ~Go Away Message

// Show Message
#define STR_MAY_SHOW1						"唱客"
#define STR_MAY_SHOW2						"唱鸥唱"
#define STR_MAY_SHOW3						"档客拎"
#define STR_MAY_SHOW						"搂~"
// ~Show Message
// ~May String

// Fishing Hook
#define STR_FISHHOOK_DROWTOWATER			"拱阑 氢秦 超矫措甫 带廉林技夸"
#define STR_FISHHOOK_HOOKING				"超矫措甫 寸板嚼聪促!"
// ~Fishing Hook

#define STR_SKILL_ERROR						"棵官弗 胶懦捞 酒凑聪促. 橇肺弊伐阑 辆丰窍绞矫坷."
#define STR_PRIMIEM_ZONE					"橇府固决猎"

#define STR_BILLING_DISCONNECT				"终止游戏"
#define STR_BILLING_CONNECT					"欢迎您来到“雅典娜”的世界"
#define STR_CHARSEL_DELETE2					" 吗?"*/
 
#define STR_SYSERR_ERRORMESSAGE				"Error Message"
#define STR_SYSERR_WARNINGMESSAGE			"Warning Message"
#define STR_SYSERR_NOTENOUGHMEMORY1			"A fatal error - not enough memory.\nThere is not enough memory\navailable to run Deicide."
#define STR_SYSERR_NOTENOUGHMEMORY2			"Not enough memory.\nProgram may end abnormally"
#define STR_SYSERR_SERVERDISCONNECT			"Server connection disconnected"
#define STR_SYSERR_ALREADYCONNECTED			"This Id is already\nlogged on to sever." 
#define STR_SYSERR_NOTCONNECTED				"肺弊牢 且 荐 绝绰 矫埃 涝聪促."
#define STR_SYSERR_RETRYIDPW				"ID 肚绰 PW啊 老摹窍瘤 臼嚼聪促"
#define STR_SYSERR_ERRUSER					"Different client version"
#define STR_SYSERR_RUNFORWEB				"Please run at the website"
#define STR_SYSERR_IPINDEX					"Server IP index error"
#define STR_SYSERR_NOTCONNECTLOGINSERVER	"Cannot connect to Login sever"
#define STR_SYSERR_NOREPRYLOGIN				"Connection has been failed due to the time out with login sever"
#define STR_SYSERR_NOTCONNECTLAQIA			"[0x00]Can not log in Deicide"
#define STR_SYSERR_NOACCEPTLAQIA			"[0x01]Please agree with Deicide's terms of agreement"
#define STR_SYSERR_NOTFOUNDIDLAQIA			"[0x02]This Id does not exist in Deicide"
#define STR_SYSERR_NOTCONNECTSERVER			"Can't connect to Sever"

#define STR_SYSMSG_RETRYINPUTID				"This Id does not exist"
#define STR_SYSMSG_RETRYINPUTPASSWORD		"Incorrect Password"
#define STR_SYSMSG_INPUTID					"Enter your ID"
#define STR_SYSMSG_INPUTPASSWORD			"Enter your Password"
#define STR_SYSMSG_LOGINING					"Login process..."
#define STR_SYSMSG_EXCEPTSTRING				"You can't use these strings"
#define STR_SYSMSG_SCREENSHOT				"Screenshot"
#define STR_SYSMSG_ACCOUNTBLOCK				"Your account has been blocked"

#define STR_CHARSEL_DELETE					"Delete?"
#define STR_CHARSEL_DELETE2					"Do you really want to Delete?"

#define STR_CHARSEL_EXCEPTNAME				"This name cannot be used"
#define STR_CHARSEL_EXISTNAME				"Other user is already using this name"
#define STR_CHARSEL_FAILDELETE				"Fail to delete the character"
#define STR_CHARSEL_LEFTPOINT				"Please use all point available"

#define STR_INVENERR_NOTENOUGHSPACE			"Not enough inventory space!"
#define STR_INVENERR_NOTENOUGHGOLD			"Not enough Shild!"
#define STR_INVENERR_NOMATCHGOLD			"Not enough Shild!"

#define STR_DEAL_REQUESTFROM				" has\nasked you a trade.\nAccept the trade?\n"
#define STR_DEAL_SUBMIT						"Complete trade"
#define STR_DEAL_FINISH						"Trade ended"

#define STR_PARTY_CREATE					"Party created!"
#define STR_PARTY_DESTROY					"Party disbanded!"
#define STR_PARTY_NOCHIEF					"惑措啊 颇萍厘捞 酒凑聪促!"
#define STR_PARTY_MEMBEROVER				"The Party is full!"
#define STR_PARTY_SUCCESS					"You've joined the party!"
#define STR_PARTY_ADDMEMBER					" has\njoined the party"
#define STR_PARTY_DELETEMEMBER				" has\nleft the party."
#define STR_PARTY_REQUESTINVIT				" has asked \n you to join the party. Accept?"
#define STR_PARTY_BANISH					"\n remove this user from your party?"

#define STR_FRIEND_REQUESTINVIT				" has asked \n You to Join the Friend. Accept?"

#define STR_MYSHOP_OPEN						"Start"
#define STR_MYSHOP_CLOSE					"Stop"

#define STR_GUILD_INVITE					"has asked you \n to join the guild. Accept?"

#define STR_FRIEND_NOTFRIEND				"Cannot register to your friend list."

#define	STR_UIMSG_DOOR						"Door"
#define	STR_UIMSG_DEAL						"Trade"
#define STR_UIMSG_PARTY						"Party"
#define STR_UIMSG_TALK						"Chat"
#define STR_UIMSG_SHOP						"Shop"
#define STR_UIMSG_QUEST						"Quest"
#define STR_UIMSG_STOREHOUSE				"Warehouse"
#define STR_UIMSG_GOTODEALMAP				"Trade Map"
#define STR_UIMSG_CREATEGUILD				"StartGuild"
#define STR_UIMSG_RELEASEGUILD				"CloseGuild"
#define STR_UIMSG_INPUTGUILDNAME			"- Type your guild name -"
#define STR_UIMSG_SELECTWEAPONITEM			"- Select your weapon -"
#define STR_UIMSG_SELECTARMORITEM			"- Select your armor -"
#define STR_UIMSG_SELECTOPTIONITEM			"- Select your option item -"
#define STR_UIMSG_AGAINENCHANTWEAPON		"- Try to enchant your weapon again -"
#define STR_UIMSG_AGAINENCHANTARMOR			"- Try to enchant your Armor again -"
#define STR_UIMSG_AGAINENCHANTOPTION		""
#define STR_UIMSG_ENCHANTCONFIRM			"Start a enchantment"
#define STR_UIMSG_ENCHANTSUCCESS			"- Enchant succeeded -"
#define STR_UIMSG_ENCHANTFAIL				"- Enchant failed -"
#define STR_UIMSG_NOTRIDER_HOUSE			"- You can't enchant while riding a horse -"
#define STR_UIMSG_DROPITEM					"Do you want to drop this?"
#define STR_UIMSG_INPUTNAME					"Enter your name!"
#define STR_UIMSG_GUILDNAMELENGTH			"Guild name should be under 32 character"
#define STR_UIMSG_ALREADYDEAL				"This user is already in trade with other user."
#define STR_UIMSG_PETON						"Use"
#define STR_UIMSG_PETOFF					"Hide"
#define STR_UIMSG_GUILDZZANG				"Master"
#define STR_UIMSG_GUILDBUZZANG				"Vice Master"
#define STR_UIMSG_ALREADYPSHOP				"This user is inside usershop."
#define STR_UIMSG_FREEPKMODE				"Free PK"
#define STR_UIMSG_NONEPKMODE				"Non PK"
#define STR_UIMSG_PARTYPKMODE				"Party PK"
#define STR_UIMSG_GUILDPKMODE				"Guild PK"
#define STR_UIMSG_ONCEINPUTPRICE			"1雀 涝仿啊瓷 陛咀 100,000,000(1撅)"
#define STR_UIMSG_ENCHANTITEM				"This is a Enchanted Item."

#define STR_UIASK_CREATEGUILD				"Create Guild"
#define STR_UIASK_RELEASEGUILD				"Disband Guild"

#define STR_MAY_HELPER						"Guide 'MAY' will follow you, until you reached total level 10."
#define STR_PET_TYPE1						"Electrical element"
#define STR_PET_TYPE2						"Ice element"
#define STR_PET_TYPE3						"Flame element"

#define STR_SKILL_ALREADY					"This skill has activated."
#define STR_SKILL_CHANGESHORTORLONG			"Change your main skill into Close/Ranger attack skill."
#define STR_SKILL_NOTFOUNDTARGET			"This skill has not targeted"
#define STR_SKILL_NOTUSINGTARGET			"You can't use this skill to this target."
#define STR_SKILL_NOTUSINGWEAPON			"You can't use this skill with your weapon."
#define STR_SKILL_NOTUSINGHORSE				"You can't use magic skill while riding a horse."
#define STR_SKILL_LVUP_SHORT				"辟芭府饭骇诀"
#define STR_SKILL_LVUP_LONG					"盔芭府饭骇诀"
#define STR_SKILL_LVUP_BLACK				"孺付过饭骇诀"
#define STR_SKILL_LVUP_WHITE				"归付过饭骇诀"
#define STR_SKILL_LVUP_SHORT				"Close Attack level-up"
#define STR_SKILL_LVUP_LONG					"Ranger Attack level-up"
#define STR_SKILL_LVUP_BLACK				"Dark Magic level-up"
#define STR_SKILL_LVUP_WHITE				"White Magic level-up"

#define STR_CHATT_NOTICE					"Notice"
#define STR_CHATT_FAILTRANSMIT				"Whisper delivery failure"

#define STR_ITEMINFO_TWOHANDWEAPON			"Two-handed weapon"
#define STR_ITEMINFO_COMMONCLASS			"Normal"
#define STR_ITEMINFO_WARRIORCLASS			"Close Attack Class"
#define STR_ITEMINFO_ARCHERCLASS			"Range Attack Class"
#define STR_ITEMINFO_MAGICCLASS				"Magical Class"
#define STR_ITEMINFO_ATTACKSPEED			"Attack speed"
#define STR_ITEMINFO_TOOSLOWSPEED			"Very slow"
#define STR_ITEMINFO_SLOWSPEED				"Slow"
#define STR_ITEMINFO_NORMALSPEED			"Normal"
#define STR_ITEMINFO_FASTSPEED				"Fast"
#define STR_ITEMINFO_VERYFASTSPEED			"Very fast"
#define STR_ITEMINFO_ATTACKPOWER			"Attack :"
#define STR_ITEMINFO_ARMORPOWER				"Defense :"
#define STR_ITEMINFO_ATTACKDIST				"Attack Range :"
#define STR_ITEMINFO_MAGICPOWER				"Magic power :"
#define STR_ITEMINFO_WEIGHT					"Weight :"
#define STR_ITEMINFO_SELLPRICE				"Selling price"
#define STR_ITEMINFO_BUYPRICE				"Purchase price"
#define STR_ITEMINFO_SHILD					"Shild"
#define STR_ITEMINFO_NEEDMP					"Mana drain"
#define STR_ITEMINFO_NEEDTOTLEV				"Req. level"
#define STR_ITEMINFO_RANK					"Rank"

#define STR_ITEMINFO_SEARCHUSER				"Search user"
#define STR_ITEMINFO_RESURRECTUSER			"esurrect Dead user"
#define STR_ITEMINFO_TRANSPANRENTEFFECT		"Invisible effect"
#define STR_ITEMINFO_STUNEFFECT9PERCENT		"9% Chance Stun effect"
#define STR_ITEMINFO_CHANGEDAMAGE			"Damage Change"
#define STR_ITEMINFO_STRENGTH2PLUS			"Strength +2"
#define STR_ITEMINFO_STRENGTH5PLUS			"Strength +5"
#define STR_ITEMINFO_HPGAIN11PERCENTPLUS	"Hp Recovery +11%"
#define STR_ITEMINFO_MPGAIN11PERCENTPLUS	"MP Recovery +11%"
#define STR_ITEMINFO_SHOPITEMDC5PERCER		"5% discount shop-items"
#define STR_ITEMINFO_COMBACKVILLAGE			"Town Portal"
#define STR_ITEMINFO_MAGICDAMAGE15PERCENT	"Magic damage +15%"
#define STR_ITEMINFO_MOVEOFF6PERCENT		"6% Chance stop movement"
#define STR_ITEMINFO_MOVEOFF9PERCENT		"9% Chance stop movement"
#define STR_ITEMINFO_ATTACKOFF6PERCENT		"6% Chance stop attack"
#define STR_ITEMINFO_ATTACKOFF9PERCENT		"9% Chance stop attack"
#define STR_ITEMINFO_MAGICRESIST11PERCENT	"+11% Magic resistance"

#define STR_EVENTMSG_CONGRATULATION			"恭喜获奖"
#define STR_EVENTMSG_NOTHING				""
#define STR_EVENTMSG_HANDPHONE				"彩屏手机"
#define STR_EVENTMSG_DIGCAMERA				"数码相机"
#define STR_EVENTMSG_TV						"彩色电视机"
#define STR_EVENTMSG_LCD					"液晶显示器"
#define STR_EVENTMSG_MP3					"MP3"

// May String
#define STR_MAY_MAY							"MAY"


// Go Away Message
#define STR_MAY_GOAWAY1						"Leave"
#define STR_MAY_GOAWAY2						"Be gone"
#define STR_MAY_GOAWAY3						"Go away"
#define STR_MAY_GOAWAY4						"Leave me alone"
#define STR_MAY_GOAWAY5						"Just leave"
#define STR_MAY_GOAWAY6						"Good bye"
#define STR_MAY_GOODBYE						"See you soon~"
// ~Go Away Message

// Show Message
#define STR_MAY_SHOW1						"Come out"
#define STR_MAY_SHOW2						"Be here !!"
#define STR_MAY_SHOW3						"Help me out"
#define STR_MAY_SHOW						"Yahoo~"
// ~Show Message
// ~May String

// Fishing Hook
#define STR_FISHHOOK_DROWTOWATER			"Throw your fishing rod to the water."
#define STR_FISHHOOK_HOOKING				"Pull your fishig rod!"
// ~Fishing Hook

#define STR_SKILL_ERROR						"It not correct skill. Please close this program."
#define STR_PRIMIEM_ZONE					"Premium zone"

#define STR_TEST_SAY_WP						"Are you here to make your weapon better?\nI don't deal in Armor! Go to Armor dealer!\nYou need weapon enchant card\nand shild to make weapon better.\n Too expensive? No way.\nThe word fail is not in my dictionary.\nGive it to me if you want\nto get a better one."
#define STR_TEST_SAY_AM						"Are you here to make your Armor better?\nAsk for weapon to blacksmith.\nYou need Armor card and shild to make\nArmor better\nWhy don't you get a better one?\nGive it to me.\nThere is no fail at all."
#define STR_TEST_SAY_SUM					"You are here to make card to bind!\nFor that, you should pay\n10 thousand shild for one card.\nDid you check the number of\ncard and shild?\nYou have to go the Premium Zone\nto unbind card."

#define STR_UIMSG_ENCHANT					"Enchant"
#define STR_UIMSG_CARDSUM					"Bind card"
#define STR_UIMSG_CARDUNDO					"Unbind card"
#define STR_UIMSG_SERVER					"United server"

#define STR_CARDSUM_ERRORMSG				"Your inventory is full."
#define STR_UIMSG_CARDUNDO_ERR				"There is no Card Box"
#define STR_ENCHANT_ITEM					"- Select the item for a enchantment - "
#define STR_ENCHANT_NOT						"- There is no item for for a enchantment at all. -"
#define STR_UNDOCARD						"You came here to bind the book of encahnt-card?\nYou should pay 10 thousand shild for one enchant card.\nDid you check the number of card and shild?"

#endif //__Common_StringTable_Header__


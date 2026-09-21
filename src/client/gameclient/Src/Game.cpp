/**
 * ∞‘¿” «¡∑π¿” º“Ω∫ ∆ƒ¿œ.
 *
 * Copyright (c) 2002-2003 ∆¿∏ﬁ¿Ã, All rights reserved.
 *
 * @author	æ∆º£
 * @date	$Date: 05-02-18 7:38p $
 * @version	$Revision: 659 $
 *
 * <HR>
 * $Log: /GameClient/Src/Game.cpp $
 * 
 * 659   05-02-18 7:38p Sk8snow2
 * 
 * 658   04-12-09 6:28p Sk8snow2
 * 
 * 657   04-12-08 3:06a Sk8snow2
 * 
 * 656   04-09-24 4:45p Paran
 * 
 * 655   04-09-17 2:43p Paran
 * 
 * 654   04-09-17 12:10p Paran
 * 
 * 653   04-09-08 8:40p Sk8snow2
 * 
 * 652   04-09-08 8:37p Paran
 * 
 * 651   04-08-28 12:18p Paran
 * 
 * 650   04-08-28 3:04a Paran
 * 
 * 649   04-08-27 8:01p Paran
 * 
 * 648   04-08-27 12:23p Sk8snow2
 * 
 * 647   04-08-26 3:31p Paran
 * 
 * 646   04-08-24 6:06p Sk8snow2
 * 
 * 645   04-08-24 3:20p Sk8snow2
 * 
 * 644   04-08-23 8:40p Paran
 * 
 * 614   04-05-08 1:48a Sk8snow2		OnPacketGuildMemberList √ﬂ∞°
 * 
 * 604   04-04-09 1:37a Sk8snow2		∞≈∑°Ω√¿€ ∆–≈∂∞¸∑√ ºˆ¡§
 * 
 * 585   04-03-15 5:43p Sckyj			øÓøµ¿⁄ ¿€æ˜ 1¬˜øœ∑· - µø¿€∏∑±‚ æ»«‘
 * 
 * 584   04-03-12 6:42p Sckyj			øÓøµ¿⁄ ¿€æ˜¡ﬂ - CreatePlayer() ºˆ¡§
 * 
 * 583   04-03-10 8:37p Sckyj			CreatePlayer() : øÓøµ¿⁄ ƒ≥∏Ø≈Õ ª˝º∫Ω√ øÓøµ¿⁄æ∆¿Ã≈€π¯»£∑Œ ∞≠¡¶¿‘∑¬
 * 
 * 581   04-03-10 6:17p Paran			±‚¥…¡∂π∞ √ﬂ∞°
 * 
 * 578   04-02-27 6:02p Sk8snow2		Pet ∞¸∑√ «‘ºˆ ¥Ÿºˆ √ﬂ∞°
 * 
 * 574   04-02-06 4:13p Kjmgo			ƒ´∏ﬁ∂Û ¿˚øÎ
 * 
 * 567   04-01-31 10:37a Sk8snow2		OnPacketGuildAddmemberRequest √ﬂ∞°
 * 
 * 565   04-01-28 12:06a Sk8snow2		ƒ£±∏∞¸∑√ «‘ºˆ √ﬂ∞°
 * 
 * 561   04-01-15 2:40a Sk8snow2		OnPacketItemEvent √ﬂ∞° ( ¡ﬂ±π æ∆¿Ã≈€ ¿Ã∫•∆Æ ∞·∞˙ ∞¸∑√ )
 * 
 * 560   04-01-14 11:47p Kjmgo			∞¯º∫¿¸Ω√ √÷º“«¡∑π¿”¿Ø¡ˆø° « ø‰«— ƒ⁄µÂ √ﬂ∞°
 * 
 * 556   04-01-13 9:43p Kjmgo			ªÁæÁ º≥¡§ ¿˚øÎ
 * 
 * 554   04-01-12 11:08p Sk8snow2		OnPacketUpdateGuildInfo √ﬂ∞°
 * 
 * 549   04-01-06 1:58p Kjmgo			∞¯∞› ∏≈≈©∑Œ √ﬂ∞°
 * 
 * 547   04-01-06 10:40a Sk8snow2		OnPacketSetMovable √ﬂ∞°
 * 
 * 546   04-01-06 10:16a Kjmgo			º±≈√¿Ø¡ˆ
 * 
 * 545   04-01-06 12:01a Kjmgo			∞¯º” √ﬂ∞° 
 * 
 * 542   04-01-02 5:36p Paran			Ω∫≈≥ ¿˚øÎ¥ÎªÛ ºˆ¡§
 * 
 * 541   04-01-02 2:20p Paran			∏ﬁºº¡ˆ define √≥∏Æ
 * 
 * 540   04-01-02 7:18a Kjmgo			ø¿∏•¬  ≈¨∏ØΩ√ æ∆π´∞Õµµ ««≈∑µ«¡ˆ æ æ“¿ªãö¿« √≥∏Æ∏¶ ∫–∏Æ
 * 
 * 537   03-12-31 2:43p Paran			Ω∫≈≥
 * 
 * 533   03-12-26 11:00a Sk8snow2		OnPacketBeginSkill √ﬂ∞°
 * 
 * 532   03-12-26 10:33a Kjmgo			º±≈√¿Ø¡ˆ¡ﬂ, ∏ÛΩ∫≈Õµµ √ﬂ∞°
 * 
 * 531   03-12-24 11:02a Kjmgo			Ω∫≈≥, ≈◊Ω∫∆Æ ƒ⁄µÂ ª¿‘
 * 
 * 530   03-12-24 10:30a Kjmgo			Ω∫≈≥¿˚øÎ
 * 
 * 529   03-12-24 4:01a Kjmgo			ƒ≥∏Ø≈Õ ∏µ®¿ª ∏µŒ ∏ﬁ∏∏Æø° ≥÷æÓ ≥ı¥¬¥Ÿ.
 * 
 * 527   03-12-23 2:47p Kjmgo			∏ª¿ª ≈∏∞Ì ∏∂π˝ ªÁøÎΩ√, ¡÷¿«ªÁ«◊¿ª ¡÷∞Ì, ≈∏∞Ÿ √Îº“
 * 
 * 525   03-12-21 3:55a Kjmgo			πˆ±◊ ºˆ¡§
 * 
 * 523   03-12-20 9:08p Kjmgo			πˆ±◊ºˆ¡§ §–.§Ã
 * 
 * 520   03-12-20 6:18a Kjmgo			∏ﬁ∏∏Æ ∞¸∏Æ
 * 
 * 517   03-12-19 8:55a Kjmgo			MAY(∂Øªﬂ) ±‚¥… æ˜±◊∑π¿ÃµÂπ◊, ∞¯∞›Ω√¿€ ∆–≈∂¿ª ∫∏≥Ω»ƒ, ¿œ¡§Ω√∞£µøæ»
										Ω√¿€«œ∂Û¥¬ ∆–≈∂¿ª πﬁ¡ˆ ∏¯«œ∏È ¥ŸΩ√ ∫∏≥ª¥¬ ƒ⁄µÂ √ﬂ∞°
 * 
 * 516   03-12-19 4:13a Sk8snow2		OnPacketSystemMsg √ﬂ∞°
 * 
 * 510   03-12-17 10:04p Kjmgo			Alt≈∞∏¶ ¥©∏¶Ω√, ø•∫Ì∑≥ √‚∑¬
 * 
 * 509   03-12-17 9:22p Sckyj			m_pSound ¡¶∞≈
 * 
 * 508   03-12-17 8:04p Sckyj			~CGame()ø°º≠ m_pSound «ÿ¡¶«œ¥¬ ƒ⁄µÂ √ﬂ∞°
 * 
 * 507   03-12-17 4:15p Sckyj			Init() - m_pSound ª˝º∫ ƒ⁄µÂ √ﬂ∞°
 * 
 * 506   03-12-17 2:28p Kjmgo			May ∏ª∞…±‚ ºˆ¡§
 * 
 * 504   03-12-17 11:11a Sckyj			LoadMap¿ª «“∂ß πÃµæÓ«√∑π¿ÃæÓ∏¶ √ ±‚»≠«œµµ∑œ √ﬂ∞°
 * 
 * 503   03-12-17 10:31a Kjmgo			º±≈√ ¿Ø¡ˆ ºˆ¡§
 * 
 * 501   03-12-17 6:24a Paran			¡§∏∏: ¡§«ı¿Ã«¸ ¿€æ˜ πÈæ˜
 * 
 * 500   03-12-17 4:51a Sk8snow2		OnPacketPartyAskInvite √ﬂ∞° ( ∆ƒ∆º√ ¥Î ∞¸∑√ )
 * 
 * 497   03-12-16 2:46p Sk8snow2		OnPacketEnchantItemResult √ﬂ∞°
 * 
 * 496   03-12-16 2:13p Sk8snow2		OnUpdateCha √ﬂ∞° ( º∫«‚ ∫Ø∞Ê )
 * 
 * 494   03-12-16 12:41a Sckyj			5.1channel speaker º≥¡§∞¸∑√ «‘ºˆ ºˆ¡§
 * 
 * 492   03-12-15 4:23p Sckyj			LoadMap() -- ∏ ¿Ã ∑Œµ˘µ…∂ß ¡ˆ≥≠ ∏ ø°º≠¿Áª˝µ«¥¯ ¿Ωæ«¿ª ∏’¿˙ stopΩ√≈∞µµ∑œ ºˆ¡§
 * 
 * 491   03-12-15 11:41a Sckyj			ªÁøÓµÂ ∞¸∑√ «‘ºˆ ºˆ¡§
 * 
 * 490   03-12-14 6:10p Sk8snow2		OnPacketSoundEffect √ﬂ∞°
 * 
 * 489   03-12-14 6:10p Sckyj			BGM ¿Áª˝∞¸∑√ ƒ⁄µÂ ºˆ¡§
 * 
 * 488   03-12-14 3:11p Sckyj			BGM∞¸∑√ «‘ºˆ ºˆ¡§(¿€æ˜¡ﬂ)
 * 
 * 487   03-12-13 3:26p Sckyj			∏  ∑Œµ˘Ω√ ∑Œµ˘¿Ωæ« ø¨¡÷∏¶ ∏∑æ∆πˆ∏≤
 * 
 * 486   03-12-12 8:00p Kjmgo			««≈∑«“ãö ∑π¿ÃæÓ ¿˙¿Â
 * 
 * 484   03-12-10 10:56p Kjmgo			±Ê√£±‚, ««≈∑ 1¬˜ øœ∑·
 * 
 * 483   03-12-10 8:32p Kjmgo			PickingEffect∏¶ Character∑Œ ø≈±Ë
 * 
 * 482   03-12-10 2:20p Kjmgo			±Ê√£±‚ π◊, ««≈∑ ºˆ¡§¡ﬂ
 * 
 * 478   03-12-04 7:16p Kjmgo			¡÷¿Œ∞¯ ƒ≥∏Ø≈Õ ∞¯º” ºˆ¡§ ∞°¥…«œµµ∑œ «œ¥¬ ƒ⁄µÂ ª¿‘
 * 
 * 476   03-12-04 10:44a Kjmgo			«√∑π¿ÃæÓ¿« May¿« ¿¸ø™∆˜¿Œ≈Õ √ﬂ∞°
 * 
 * 475   03-12-03 5:15p Sckyj			PlayBgm()«‘ºˆ ºˆ¡§ - Area -> Map ∫Ø∞ÊΩ√ «◊ªÛ 0π¯ bgm¿∏∑Œ map¿Ωæ«¿ª ¿Áª˝«œ¥¯∞Õ¿ª ºˆ¡§
 * 
 * 474   03-12-03 12:25a Sckyj			SetMapInfo() «‘ºˆ Area √ﬂ∞°
 * 
 * 473   03-12-02 11:36p Sckyj			SetMapInfo() area ¿Ã∏ßµÈ ºˆ¡§
 * 
 * 471   03-12-02 3:43a Sk8snow2		≥Øææ ∫Ø∞Ê √≥∏Æ ∞¸∑√
 * 
 * 469   03-12-01 7:01p Sckyj			PlayBGM«‘ºˆ ºˆ¡§( µø¿œ«— ¡ˆø™ø°º≠ Area BGM¿Ã ∫πºˆ∞≥∞° ¿Áª˝µ…ºˆ ¿÷µµ∑œ ∫Ø∞Ê )
 *  
 * 467   03-11-28 10:46p Sckyj			BGM Looping ∞¸∑√ ƒ⁄µÂ ºˆ¡§
 * 
 * 466   03-11-28 5:10p Paran			±”º”∏ª ºˆ¡§
 * 
 * 465   03-11-28 10:14a Sk8snow2		OnPacketMemberChat √ﬂ∞°
 * 
 * 464   03-11-28 10:17a Sckyj			Map, Areaø° ¥Î«— Reverb Type º≥¡§≈‰∑œ ±‚¥…√ﬂ∞°
 * 
 * 463   03-11-28 9:31a Sk8snow2
 *  * void	OnPacketBroadcastChat();	// ¿¸√º √§∆√
 *  * void	OnPacketSellingChat();	// ∏≈∏≈ √§∆√
 *  * void	OnPacketGuildChat();	// ±ÊµÂ √§∆√
 *  * void	OnPacketPartyChat();	// ∆ƒ∆º √§∆√ 
 *  * √ﬂ∞°
 * 
 * 462   03-11-28 9:19a Sckyj			∏ , area ∫Ø∞Êø° µ˚∏• ªÁøÓµÂ ¿‹«‚√≥∏Æ ¿€æ˜¡ﬂ
 * 
 * 461   03-11-26 3:51p Sckyj			Ambient BGM Ω√∞£¥Î∫∞ ¿Áª˝±‚¥… √ﬂ∞°
 * 
 * 460   03-11-21 10:00a Paran			∏˜ æ∆¿Ã≈€ µÂ∑” ºˆ¡§
 * 
 * 459   03-11-15 9:41p Paran			PK ∫Œ∫– 1¬˜ ºˆ¡§
 * 
 * 457   03-11-12 5:45p Kjmgo			¿”Ω√∑Œ Ω∫≈≥ ∆ƒ¿œ ¿–¡ˆ æ µµ∑œ ºˆ¡§
 * 
 * 456   03-11-10 5:53p Kjmgo			Fov∞™¿ª ø¯ªÛ≈¬∑Œ ∫π±∏
 * 
 * 455   03-11-10 4:18a Sk8snow2		SET_FUNC( dPACKET_DAMAGE_PVP, OnPacketDamagePvP	); √ﬂ∞°
 * 
 * 454   03-11-08 12:02p Sckyj			Ambient BGM¿ª Crossplayµ«µµ∑œ ±‚¥… √ﬂ∞°
 * 
 * 453   03-11-05 11:36a Sckyj			Ambient BGM play √ﬂ∞°
 * 
 * 451   03-10-27 3:25p Kjmgo			∏ ∑Œµ˘Ω√ ƒ´∏ﬁ∑Œ √ ±‚º¶¿ª ∫œ¬ ¿ª «‚«œµµ∑œ
 * 
 * 449   03-10-23 10:04p Paran			∞¯∞›»ø∞˙∫Œ∫– ºˆ¡§.
 * 
 * 448   03-10-21 2:45p Kjmgo			LoadMapΩ√, ∞∞¿∫ ∏ ¿œ∂ßµµ, ≈©∏Æ√≥¿« πŸøÓµÂ π⁄Ω∫∏¶ ∏±∏Æ¡Ó«œ¥¯∞Õ¿ª... ¥Ÿ∏•
										∏ ¿œ∂ß∏∏ «œµµ∑œ ºˆ¡§( æÔººΩ∫ πŸ¿Ãø√∑π¿Ãº« πÆ¡¶ )
 * 
 * 447   03-10-20 10:16p Paran			∏ÛΩ∫≈Õ µ•πÃ¡ˆ ∫Œ∫– ºˆ¡§.
 * 
 * 445   03-10-17 10:09p Kjmgo			∑Œ±◊ «„π˙≥™∞‘ ≥≤±‚±‚+_+
 * 
 * 444   03-10-17 3:48p Sckyj			BGM Replay ƒ⁄µÂ ºˆ¡§(bgm_offªÛ≈¬ø°º≠µµ 5∫–µ⁄ replayµ«¥¬∞Õ πÊ¡ˆ)
 * 
 * 443   03-10-15 11:14a Kjmgo			∆–≈∂ √≥∏Æ∏¶ «‘ºˆ ∆˜¿Œ≈Õ ªÁøÎ
 * 
 * 441   03-10-11 1:32a Kjmgo			∏ÛΩ∫≈Õ ∏µ®∞˙, πŸøÓµÂ π⁄Ω∫∏¶ ∏  ∑Œµ˘Ω√ ∏µŒ ∏±∏Æ¡Ó «œµµ∑œ ºˆ¡§
 * 
 * 435   03-10-06 7:19p Kjmgo			Send Fail. Bug. 
 * 
 * 430   03-10-03 11:49p Kjmgo			∏ÛΩ∫≈Õ ««≈∑ ∞≈∏Æ 15πÃ≈Õø°º≠ 25πÃ≈Õ∑Œ ºˆ¡§, √¢, ∫¿, µø¿€¿ª ¡ˆ∆Œ¿Ã∑Œ
										¥Î√º, ∏ÛΩ∫≈Õ ∞¯∞›∞≈∏Æ∏¶ ¡æ¿¸∞≈∏Æ∑Œ ¿Áºˆ¡§
 * 
 * 426   03-10-02 11:20p Kjmgo			ƒ≥∏Ø≈Õ State ±««— ¿Ã¿¸ : øœ∑· 
 * 
 * 425   03-10-02 10:33p Kjmgo			ƒ≥∏Ø≈Õ State ±««— ¿Ã¿¸ ¿€æ˜ : 1¬˜ πÈæ˜
 * 
 * 423   03-10-02 5:49p Kjmgo			m_dwPickDoor∏¶ 0¿∏∑Œ ºº∆√
 * 
 * 422   03-10-02 4:30p Kjmgo			m_pPlayer∏¶ g_pPlayer∑Œ ∞ªΩ≈«œµµ∑œ ºˆ¡§( «√∑π¿ÃæÓ∞° ¡◊¿ªΩ√ æÔººΩ∫
										πŸ¿Ãø√∑π¿Ãº« ≥ª¥¬ πˆ±◊ ºˆ¡§ )
 * 
 * 421   03-10-02 2:41p Kjmgo			∏ÛΩ∫≈Õ ∞¯∞› ∞≈∏Æ∏¶ ∏ÛΩ∫≈Õ¿« Radius∏∏≈≠¿« ø©¿Ø∏¶ ¡‹
 * 
 * 420   03-10-02 11:47a Kjmgo			««≈∑ øÏº±º¯¿ß∏¶ π›¥Î∑Œ ¿˚øÎ
 * 
 * 418   03-10-01 11:08p Kjmgo			≈©∏Æ√≥µÈ∞˙¿« √Êµπ√≥∏Æ ¡¯«‡
 * 
 * 416   03-10-01 6:40p Kjmgo			Skill¿˚øÎ
 * 
 * 413   03-09-30 5:29p Kjmgo			AddCharacterø°º≠ ±‚¡∏ø° ¿÷¥¯ ∞Õ¿Ã∏È π´Ω√«ﬂ¥¯∞Õ¿ª ¡ˆøÏ∞Ì ªı∑Œ ª˝º∫«œ∞‘ ºˆ¡§
 * 
 * 411   03-09-29 5:07p Kjmgo			Item∞˙ Monster¿« ∞ÊøÏ, ¿œ¡§ ∞≈∏Æ∫∏¥Ÿ ∏÷∂ß π›¬¶ ∞≈∏Æ¥¬ πˆ±◊ ºˆ¡§
 * 
 * 410   03-09-29 5:05p Kjmgo			PickingøÎ Interface Pointer∏¶ CGame ≥ª∫Œø°º≠ ¿¸ø™¿∏∑Œ ºˆ¡§
 * 
 * 409   03-09-29 4:06p Kjmgo			Picking ºˆ¡§.( «—«¡∑π¿”¥Á «—∞≥¿« Level∏∏ ∞ÀªÁ«œµµ∑œ. ( ≥™∏”¡ˆ¥¬ Skip ) )
 * 
 * 408   03-09-29 3:28p Kjmgo			m_pWorld->GetCharacter( m_dwPlayer ); ±∏πÆ¿« ≥≠π´∏¶ m_pPlayer∑Œ ¥Î√º
 * 
 * 399   03-09-17 11:00a Kjmgo			∏∂π˝ Ω√¿¸ ∫Œ∫– æ‡∞£ ºˆ¡§( µø¿€¿Ã ¡ﬂ∫πµ«º≠ ≥™∞°¥¬∞≈ ºˆ¡§ )
 * 
 * 389   03-09-08 8:20p Kjmgo			µø¿€~ ¿€æ˜
 * 
 * 382   03-08-31 10:19p Kjmgo			Chracter ¿Ãµø∞˙ ∞¯∞› ºˆ¡§ 
 * 
 * 360   03-08-14 10:03a Kjmgo			∏∂π˝Ω∫≈©∏≥∆Æ µŒ∞≥∑Œ ∫–∏Æ
 * 
 * 357   03-08-10 6:59p Admin			∏ª ≈∏∞Ì≥ª∏Æ±‚ ¿˚øÎ
 * 
 * 302   03-06-24 7:50p Paran			∏  π¯»£ ºˆ¡§.
 * 
 * 297   03-06-23 5:53p Paran			πŸøÓµÂ π⁄Ω∫ √≥∏Æ.
 * 
 * 293   03-06-19 8:31p Paran			∏  ¡ˆø™ ∫–∏Æ
 * 
 * 289   03-06-14 11:03a Paran			Skill zzang~
 * 
 * 287   03-06-09 10:17p Kjmgo			««≈∑ ∞¸∑√ ºˆ¡§
 * 
 * 283   03-05-29 3:09p Paran			√º¿Œ¡ˆ «⁄µÂ æ∆¿Ã≈€ ∆–≈∂ ºˆ¡§
 * 
 * 282   03-05-29 2:28p Paran			æ∆¿Ã≈€ º’πŸ≤Ÿ±‚ ∆–≈∂ ºˆ¡§.
 * 
 * 279   03-05-27 8:41p Paran			»∞∞¯∞›, ∆˙¥ı ¡§∏Æ
 * 
 * 277   03-05-24 9:28p Paran			««≈∑ ∞≈∏Æ ºˆ¡§
 * 
 * 276   03-05-23 3:43p Paran			¿Œ≈Õ∆–¿ÃΩ∫ ∞¸∑√ ºˆ¡§.
 * 
 * 275   03-05-23 4:05a Paran			∞¯∞›∞°¥…∞≈∏Æ√ﬂ∞°.
 * 
 * 271   03-05-17 11:13a Ahastudio		Print Screen ≈∞∏¶ ¥©∏¶ ∞ÊøÏ, ∞‘¿” »≠∏È ¿˙¿Â ±‚¥… √ﬂ∞°.
 *
 * 270   03-05-16 8:09p Paran			NPC Ω∫≈©∏≥∆Æ ºˆ¡§.
 *
 * 256   03-05-11 11:03a Ahastudio		πŸ¥⁄ ««≈∑ ¿Ã∆Â∆Æ∏¶ π›∫π«œµµ∑œ ºˆ¡§.
 *
 * 253   03-05-10 5:19p Ahastudio		«√∑π¿ÃæÓ ªÛ≈¬ø° µ˚∂Û µø¿€¿ª ¡¶«—«—¥Ÿ.
 *
 * 251   03-05-10 4:33p Ahastudio		ƒ≥∏Ø≈Õ, ∏ÛΩ∫≈Õ, NPCø° SetLighting() ¿˚øÎ.
 *
 * 249   03-05-09 11:30p Paran			æ∆¿Ã≈€ ªÁøÎ ∆–≈∂ √≥∏Æ.
 *
 * 246   03-05-08 1:30a Paran			∆¡ ¡§∫∏ √≥∏Æ.
 *
 * 242   03-05-06 6:18p Paran			ªÛ¡° ±‚¥… √ﬂ∞°.
 *
 * 240   03-05-05 1:40p Ahastudio		AddPlayer -> CreatePlayer()
 *
 * 238   03-04-30 7:10p Paran			Ω∫≈≥ º±≈√ ∫Œ∫– ºˆ¡§.
 *
 * 221   03-04-15 1:25a Paran			NPC √ﬂ∞°.
 *
 * 219   03-04-11 5:29p Paran			ƒ≥∏Ø≈Õ ¿Â¬¯ ∫Œ∫– √ﬂ∞°.
 *
 * 212   03-04-03 3:58a Ahastudio		¿Ãµø ∆˜¿Œ≈Õ «•Ω√ √ﬂ∞°.
 *
 * 209   03-03-29 2:09p Paran			Ω∫≈≥πŸ ºˆ¡§.
 *
 * 207   03-03-28 1:17a Paran			Ω∫≈≥πŸ √ﬂ∞°, ƒ≥∏Ø≈Õ ºø∑∫∆Æ ∫Œ∫– ºˆ¡§.
 *
 * 206   03-03-27 11:19a Paran			¿Œ≈Õ∆‰¿ÃΩ∫ æÛ±º ∫Œ∫– ºˆ¡§.
 *
 * 205   03-03-27 11:03a Ahastudio		Heart Beat √ﬂ∞°.
 *
 * 203   03-03-26 9:18p Paran			¿Œ∫•≈‰∏Æ ∞¯∞£ æ¯¿∏∏È æ∆¿Ã≈€¿ª ¡›¡ˆ ∏¯«—¥Ÿ.
 *
 * 198   03-03-26 6:22a Paran			æ∆¿Ã≈€ º≠πˆ ø¨∞·.
 *
 * 196   03-03-25 8:39p Paran			æ∆¿Ã≈€ ∞¸∑√ «‘ºˆ √ﬂ∞°.
 *
 * 193   03-03-25 3:36a Paran			¿Œ∫•≈‰∏Æ æ∆¿Ã≈€ º≠πˆøÕ ø¨∞·.
 *
 * 192   03-03-22 5:28a Ahastudio		æ∆¿Ã≈€ ¡§∫∏ ∞¸∏Æ¿⁄∏¶ World∑Œ ¿Ãµø.
 *
 * 190   03-03-21 5:43a Ahastudio		º˝¿⁄∞° ¥≠∏Æ∏È Ω∫≈≥ ∫Ø∞Ê.
 *
 * 188   03-03-18 12:03p Ahastudio		«√∑π¿ÃæÓ √ﬂ∞° ¿Ã∆Â∆Æ ºˆ¡§.
 *
 * 184   03-03-15 1:05p Ahastudio		ªÁ∏¡ ∞¯¡ˆ √≥∏Æ ∫Ø∞Ê.
 *
 * 183   03-03-14 2:30p Ahastudio		ProcessRecvPacket π◊ OnPacket¿ª GameProcessPacket.cpp∑Œ ¿Ãµø.
 *
 * 182   03-03-14 3:40a Ahastudio		∏  ∑Œµ˘ ∫Œ∫– ºˆ¡§.
 *
 * 181   03-03-14 3:07a Ahastudio		æ∆¿Ã≈€ ∞¸∑√ OnPacket «‘ºˆ √ﬂ∞°.
 *
 * 180   03-03-12 9:19p Ahastudio		Frame Init∏¶ Returnø°º≠ øπø‹∑Œ ∫Ø∞Ê.
 *
 * 179   03-03-08 11:23a Ahastudio		∫π¿‚«— Function ∫–∏Æ.
 *
 * 177   03-02-22 10:11a Ahastudio		Camera∏¶ Engine¿∏∑Œ ø≈±‚∞Ì, Camera ƒƒ∆˜≥Õ∆Æ ªÁøÎ.
 *
 * 176   03-02-22 9:41a Ahastudio		Camera ªÁøÎ¿ª ∆˜¿Œ≈Õ∑Œ ∫Ø∞Ê. ¬˜»ƒø° Camera¥¬ Engine¿∏∑Œ ø≈±Ê øπ¡§.
 * <HR>
 *
 * @file	Game.cpp
 */

// /D "_NOWEBLOGIN"
#include "Global.h"
#include "Game.h"
#include "BodyMgr.h"
#include <io.h>
#include "Encrypt.h"

#define ATTACKLIMIT_TIME	2000

IMPLEMENT_FRAME( CGame, Game );

void (*g_apPacketProcess[dPACKET_MAX_VALUE])();

/**
 * ª˝º∫¿⁄.
 */
CGame::CGame()
:	m_bQuit			( FALSE ),
	m_bExitFrame	( FALSE ),
	m_bPlaying		( FALSE ),
	m_bMoveAttack	( FALSE ),
	m_bAdvanceMove	( FALSE ),
	m_bPick			( FALSE ),
	m_bSendAttackPacket( FALSE ),
	m_pCamera		( NULL ),
	m_pWorld		( NULL ),
	m_pPlayer		( NULL ),
	m_wMap			( 0xFFFF ),		
	m_dwPlayer		( g_dwPlayerId ),
	m_btMapType		( 0 ),
	m_bCheckLMB		( 0 ),
	m_dwAttackLimit ( 0 ),
	m_nSkillItem	( 0 ),
	m_nPickLevel	( 0 ),
	m_dwPickDoor	( 0 ),
	m_dwViewDelay	( 0 ),
	m_nZoomMode		( ZOOM_SHORT ),
	m_fCameraOffset ( 0.0f ),
	m_bNoMove		( FALSE ),
	m_bMiniGaming	( FALSE ),
	bCheck1			( FALSE ),
	bCheck			( FALSE ),
	m_btIsTest		( FALSE ),
	m_dwTickss		( 0 ),
	m_btIsFriendList( FALSE )
{	
	g_pPickCharacter = NULL;
	g_pPickMonster	 = NULL;
	g_pPickNPC		 = NULL;
	g_pPickItem		 = NULL;
	g_pFuncEntity	 = NULL;
} //CGame::CGame

/**
 * º“∏Í¿⁄.
 */
CGame::~CGame()
{
	Assert( g_pClient );

	g_pClient->Disconnect();

	g_BodyMgr.Cleanup();
	SAFE_DELETE( g_pSelectMgr );
	SAFE_RELEASE( m_pWorld );
	SAFE_RELEASE( m_pCamera );

	
	ReleaseMonsterModel();
	ReleaseCreatureBoundBox();
	
	g_mgrSkill.Destory();
} //CGame::~CGame

/**
 * ∞‘¿” «¡∑π¿”¿ª √ ±‚»≠«—¥Ÿ.
 * ∞‘¿” ø˘µÂøÕ ¿Ø¿˙ ¿Œ≈Õ∆‰¿ÃΩ∫∏¶ √ ±‚»≠«—¥Ÿ.
 */
BOOL CGame::Init()
{
	Log( "== CGame Start ==" );

	Assert( g_pScene );

	DEBUG_OUT( "Reset the game...." );
	
	m_pCamera = MAKE_COMPONENT( Camera );
	Assert( m_pCamera );
	CHECK_PTR_RETURN( m_pCamera );
	m_pCamera->SetFov( 55 );
 

	m_pWorld = MAKE_COMPONENT( World );
	Assert( m_pWorld );
	CHECK_PTR_RETURN( m_pWorld );

	// ƒ˜∏Æ∆º º≥¡§ 
	m_pWorld->SetMapQuality( g_nMapQuality );

	Log( "== World Init ==" );
	m_pWorld->Init( g_pScene, m_pCamera );

	Log( "== InitGUI ==" );
	InitGUI();

	Log( "== InitFuncArray ==" );
	InitFuncArray();



	Log( "== Skill Init ==" );
	g_mgrSkill.Init( m_pWorld );
	
	// SKILL 
/*	if( !g_mgrSkill.Load( "skill.wsd" ) )
	{
		Assert( !"Ω∫≈≥∆ƒ¿œ¿ª ¿–æÓ ø¿¥¬µ•ø° Ω«∆– «œø¥Ω¿¥œ¥Ÿ." ); 
		return FALSE;
	} //if*/
	//g_mgrSkill

	g_BodyMgr.Init();

	m_pPlayer = m_pWorld->GetCharacter( g_dwPlayerId );
	g_pPlayer = m_pPlayer;

	if( !m_pPlayer )
	{
//		Assert( !"«√∑π¿ÃæÓ ¡§∫∏∞° ¡∏¿Á «œ¡ˆ æ Ω¿¥œ¥Ÿ." );
		return FALSE;
	} //if

	Log( "-=-=- ∞‘¿” √ ±‚»≠ º∫∞¯ -=-=-" );	



	return TRUE;
} //CGame::Init

/**
 * ∞‘¿” GUI∏¶ √ ±‚»≠«—¥Ÿ.
 */
void CGame::InitGUI()
{
	Assert( g_pGUI );

	g_pGUI->SetModeState( "[GAME_MODE]" );
	g_pGUI->InitDlg( m_pCamera );

	Log( "-=-=- ∞‘¿” √ ±‚»≠ : GUI ª˝º∫ º∫∞¯ -=-=-" );
} //CGame::InitGUI

/**
 * «‘ºˆ ∆˜¿Œ≈Õ πËø≠¿« √ ±‚»≠
 */
void CGame::InitFuncArray()
{
	for( int i = 0 ; i < dPACKET_MAX_VALUE ; i++ )
	{
		m_apPacketProcess[i] = OnPacketNull;
	} //for

#define SET_FUNC( s, p ) m_apPacketProcess[s] = p

	SET_FUNC( dPACKET_MAPLOADING,			OnPacketMapLoading				);
	SET_FUNC( dPACKET_CHARACTER_INSERT,		OnPacketCharacterInsert			);
	SET_FUNC( dPACKET_CHARACTER_REMOVE,		OnPacketCharacterRemove			);
	SET_FUNC( dPACKET_CHARACTER_MOVE,		OnPacketCharacterMove			);
	SET_FUNC( dPACKET_CHARACTER_DIRECTION,	OnPacketCharacterDirection		);
	SET_FUNC( dPACKET_MONSTER_INSERT,		OnPacketMonsterInsert			);
	SET_FUNC( dPACKET_MONSTER_REMOVE,		OnPacketMonsterRemove			);
	SET_FUNC( dPACKET_MONSTER_MOVE,			OnPacketMonsterMove				);
	SET_FUNC( dPACKET_CHAT_MSG,				OnPacketChatMsg					);
	SET_FUNC( dPACKET_ATTACK_USERMOB,		OnPacketNull					);
	SET_FUNC( dPACKET_ATTACK_MOBUSER,		OnPacketNull					);
	SET_FUNC( dPACKET_MOB_HPINFO,			OnPacketMobHpInfo				);
	SET_FUNC( dPACKET_DAMAGE_USER,			OnPacketDamageUser				);
	SET_FUNC( dPACKET_DAMAGE_MOB,			OnPacketDamageMob				);
	SET_FUNC( dPACKET_DIE_USER,				OnPacketDieUser					);
	SET_FUNC( dPACKET_DIE_MOB,				OnPacketDieMob					);
	SET_FUNC( dPACKET_RESTART,				OnPacketNull					);
	SET_FUNC( dPACKET_CHAT_WHISPER,			OnPacketChatWhisper				);
	SET_FUNC( dPACKET_CHAT_WHISPER_RES,		OnPacketChatWhisperResult		);
	SET_FUNC( dPACKET_SKILL_UP,				OnPacketSkillUp					);
	SET_FUNC( dPACKET_GET_EXP,				OnPacketGetExp					);
	SET_FUNC( dPACKET_SKILLUP_EVENT,		OnPacketSkillUpEvent			);
	SET_FUNC( dPACKET_HPGAIN_USER,			OnPacketHpGainUser				);
	SET_FUNC( dPACKET_MPGAIN_USER,			OnPacketMpGainUser				);
	SET_FUNC( dPACKET_HPGAIN_MOB,			OnPacketHpGainMob				);
	SET_FUNC( dPACKET_INSERT_MAPITEM,		OnPacketInsertMapItem			);
	SET_FUNC( dPACKET_REMOVE_MAPITEM,		OnPacketRemoveMapItem			);
	SET_FUNC( dPACKET_INSERT_INVENTORY,		OnPacketInsertInventory			);
	SET_FUNC( dPACKET_REMOVE_INVENTORY,		OnPacketRemoveInventory			);
	SET_FUNC( dPACKET_MOVE_ITEM,			OnPacketNull					);
	SET_FUNC( dPACKET_DROP_ITEM,			OnPacketNull					);
	SET_FUNC( dPACKET_GET_ITEM,				OnPacketNull					);
	SET_FUNC( dPACKET_INSERT_EQUIP_ITEM,	OnPacketInsertEquipItem			);
	SET_FUNC( dPACKET_REMOVE_EQUIP_ITEM,	OnPacketRemoveEquipItem			);
	SET_FUNC( dPACKET_EQUIP_ITEM,			OnPacketNull					);
	SET_FUNC( dPACKET_CHANGE_HAND_ITEM,		OnPacketNull					);
	SET_FUNC( dPACKET_CHANGE_MAIN_SKILL,	OnPacketChangeMainSkill			);
	SET_FUNC( dPACKET_ITEM_FROM_HAND,		OnPacketItemFromHand			);
	SET_FUNC( dPACKET_HOLDING_ITEM,			OnPacketNull					);
	SET_FUNC( dPACKET_SET_INVENXY,			OnPacketNull					);
	SET_FUNC( dPACKET_ALREADY_CONNECTED,	OnPacketAlreadyConnected		);
	SET_FUNC( dPACKET_PLAYERS_ITEM_INFO,	OnPacketItemPlayerItemInfo		);
	SET_FUNC( dPACKET_CHANGE_EQUIP,			OnPacketChangeEquip				);
	SET_FUNC( dPACKET_REMOVE_SPELLBOOK,		OnPacketRemoveSpellBook			);
	SET_FUNC( dPACKET_MOVETO_SPELLBOOK,		OnPacketNull					);
	SET_FUNC( dPACKET_CHANGE_TIME,			OnPacketChangeTime				);
	SET_FUNC( dPACKET_UPDATE_MONEY,			OnPacketUpdateMoney				);
	SET_FUNC( dPACKET_GET_SHOPITEM,			OnPacketNull					);
	SET_FUNC( dPACKET_SHOPITEM_LIST,		OnPacketShopItemList			);
	SET_FUNC( dPACKET_SELL_ITEM,			OnPacketNull					);
	SET_FUNC( dPACKET_BUY_ITEM,				OnPacketNull					);
	SET_FUNC( dPACKET_SHOP_RESULT,			OnPacketShopResult				);
	SET_FUNC( dPACKET_BEGIN_PLAYER_ATTACK,	OnPacketBeginPlayerAttack		);
	SET_FUNC( dPACKET_END_PLAYER_ATTACK,	OnPacketNull					);
	SET_FUNC( dPACKET_CAST_ITEM,			OnPacketNull					);
	SET_FUNC( dPACKET_DEAL_BEGIN,			OnPacketDealBegin				);
	SET_FUNC( dPACKET_DEAL_INSERT_ITEM,		OnPacketDealInsertItem			);
	SET_FUNC( dPACKET_DEAL_REMOVE_ITEM,		OnPacketDealRemoveItem			);
	SET_FUNC( dPACKET_DEAL_UPDATEMONEY,		OnPacketDealUpdateMoney			);
	SET_FUNC( dPACKET_DEAL_REQUEST_SUBMIT,	OnPacketDealRequestSubmit		);
	SET_FUNC( dPACKET_DEAL_FINISH,			OnPacketDealFinish				);
	SET_FUNC( dPACKET_PARTY_CREATE_OK,		OnPacketPartyCreateOk			);
	SET_FUNC( dPACKET_PARTY_DESTROYED,		OnPacketPartyDestroyed			);
	SET_FUNC( dPACKET_PARTY_JOIN_RESULT,	OnPacketPartyJoinResult			);
	SET_FUNC( dPACKET_PARTY_ADDMEMBER,		OnPacketPartyAddMember			);
	SET_FUNC( dPACKET_PARTY_DELMEMBER,		OnPacketPartyDeleteMember		);
	SET_FUNC( dPACKET_PARTY_MEMBER_HPMP,	OnPacketPartyMemberHPMP			);
	SET_FUNC( dPACKET_PARTY_MEMBER_MAPINFO,	OnPacketPartyMemberMapInfo		);
	SET_FUNC( dPACKET_UPDATE_POINT,			OnPacketUpdatePoint				);
	SET_FUNC( dPACKET_UPDATE_LEVEL,			OnPacketUpdateLevel				);
	SET_FUNC( dPACKET_SET_ATTACKRANGE,		OnPacketAttackRange				);
	SET_FUNC( dPACKET_ADD_HERO,				OnPacketAddHero					);
	SET_FUNC( dPACKET_REMOVE_HERO,			OnPacketRemoveHero				);
	SET_FUNC( dPACKET_EQUIP_INFO,			OnPacketEquipInfo				);
	SET_FUNC( dPACKET_MAGIC_INFO,			OnPacketMagicInfo				);
	SET_FUNC( dPACKET_MAGIC_CAST,			OnPacketNull					);
	SET_FUNC( dPACKET_MAGIC_CAST_USER,		OnPacketMagicCastUser			);
	SET_FUNC( dPACKET_ADD_EFFECT,			OnPacketAddEffect				);
	SET_FUNC( dPACKET_REMOVE_EFFECT,		OnPacketRemoveEffect			);
	SET_FUNC( dPACKET_UPDATE_MAXHPMP,		OnPacketUpdateMaxHPMP			);
	SET_FUNC( dPACKET_SET_SKILLEXP,			OnPacketSetSkillExp				);
	SET_FUNC( dPACKET_ITEM_DBCLICK,			OnPacketNull					);
	SET_FUNC( dPACKET_ADD_SKILL,			OnPacketAddSkill				);
	SET_FUNC( dPACKET_SKILL_LIST,			OnPacketSkillList				);
	SET_FUNC( dPACKET_UPDATE_POTIONCNT,		OnPacketUpdatePotionCnt			);
	SET_FUNC( dPACKET_QUEST_LIST,			OnPacketQuestList				);
	SET_FUNC( dPACKET_QUEST_RESULT,			OnPacketQuestResult				);
	SET_FUNC( dPACKET_OPEN_GARBAGE,			OnPacketNull					);
	SET_FUNC( dPACKET_GARBAGE_ITEMLIST,		OnPacketGarbageItemList			);
	SET_FUNC( dPACKET_INSERT_TO_GARBAGE,	OnPacketInsertToGarbage			);
	SET_FUNC( dPACKET_REMOVE_FROM_GARBAGE,	OnPacketRemoveFromGarbage		);
	SET_FUNC( dPACKET_INPUT_GARBAGE_ITEM,	OnPacketNull					);
	SET_FUNC( dPACKET_OUTPUT_GARBAGE_ITEM,	OnPacketNull					);
	SET_FUNC( dPACKET_RIDE_INFO,			OnPacketRideInfo				);
	SET_FUNC( dPACKET_BEGIN_CASTMAGIC2,		OnPacketBeginCastMagic2			);
	SET_FUNC( dPACKET_NPC_INFO,				OnPacketNpcInfo					);
	SET_FUNC( dPACKET_MOB_STATE,			OnPacketMobState				);
	SET_FUNC( dPACKET_GHOST_STATE,			OnPacketGhostState				);
	SET_FUNC( dPACKET_PARTYBOARD_LIST,		OnPacketPartyBoardList			);
	SET_FUNC( dPACKET_DAMAGE_PVP,			OnPacketDamagePvP				);
	SET_FUNC( dPACKET_BROADCAST_CHAT,		OnPacketBroadcastChat			);
	SET_FUNC( dPACKET_SELLING_CHAT,			OnPacketSellingChat				);
	SET_FUNC( dPACKET_GUILD_CHAT,			OnPacketGuildChat				);
	SET_FUNC( dPACKET_PARTY_CHAT,			OnPacketPartyChat				);			
	SET_FUNC( dPACKET_MEMBER_CHAT,			OnPacketMemberChat				);
	SET_FUNC( dPACKET_CHANGE_WEATHER,		OnPacketChangeWeather			);
	SET_FUNC( dPACKET_SOUND_EFFECT,			OnPacketSoundEffect				);
	SET_FUNC( dPACKET_UPDATE_CHA,			OnPacketUpdateCha				);
	SET_FUNC( dPACKET_ENCHANT_ITEM_RES,		OnPacketEnchantItemResult		);
	SET_FUNC( dPACKET_PARTY_ASKINVITE,		OnPacketPartyAskInvite			);
	SET_FUNC( dPACKET_UPDATE_CLASS,			OnPacketUpdateClass				);
	SET_FUNC( dPACKET_SYSTEM_MSG,			OnPacketSystemMsg				);
	SET_FUNC( dPACKET_BEGIN_SKILL,			OnPacketBeginSkill				);
	SET_FUNC( dPACKET_SET_VISIBLE,			OnPacketSetVisible				);
	SET_FUNC( dPACKET_SET_MOVABLE,			OnPacketSetMovable				);
	SET_FUNC( dPACKET_UPDATE_GUILDINFO,		OnPacketUpdateGuildInfo			);
	SET_FUNC( dPACKET_ITEM_EVENT,			OnPacketItemEvent				);
	SET_FUNC( dPACKET_GET_FRIENDLIST,		OnPacketFriendList				);
	SET_FUNC( dPACKET_FRIEND_ADDED,			OnPacketFriendAdded				);
	SET_FUNC( dPACKET_FRIEND_REMOVED,		OnPacketFriendRemoved			);
	SET_FUNC( dPACKET_FRIEND_MEMO,			OnPacketFriendMemo				);

	SET_FUNC( dPACKET_GUILD_ADDMEMBER_REQUEST, OnPacketGuildAddmemberRequest );
	SET_FUNC( dPACKET_PET_INFO,				OnPacketPetInfo					);		
	SET_FUNC( dPACKET_PET_ON,				OnPacketPetON					);
	SET_FUNC( dPACKET_PET_OFF,				OnPacketPetOFF					);	
	SET_FUNC( dPACKET_UPDATE_PET_GAGE,		OnPacketUpdatePetGage			);
	SET_FUNC( dPACKET_UPDATE_PET_INFO,		OnPacketUpdatePetInfo			);
	SET_FUNC( dPACKET_PET_ATTACK,			OnPacketPetAttack				);
	SET_FUNC( dPACKET_ATTACK_TO_PET,		OnPacketAttackToPet				);
	SET_FUNC( dPACKET_FISHING_STATUS,		OnPacketFisingStatus			);
	SET_FUNC( dPACKET_FISHING_PATTERN,		OnPacketFishingPattern			);	
	SET_FUNC( dPACKET_FISHING_POS,			OnPacketFishingPos				);
	SET_FUNC( dPACKET_PET_UPDATENAME,		OnPacketPetUpdateName			);
	SET_FUNC( dPACKET_PET_UPDATEEXP,		OnPacketPetUpdateExp			);
	SET_FUNC( dPACKET_DEAL_BEGINOK,			OnPacketDealBeginOk				);
	SET_FUNC( dPACKET_FRIEND_LOGIN,			OnPacketFriendLogin				);
	SET_FUNC( dPACKET_FRIEND_LOGOUT,		OnPacketFriendLogout			);
	SET_FUNC( dPACKET_MOB_POPEFFECT,		OnPacketMobPopEffect			);
	SET_FUNC( dPACKET_PLAYER_POPEFFECT,		OnPacketPlayerPopEffect			);
	SET_FUNC( dPACKET_CHANGE_STATUS,		OnPacketChangeStatus			);
	SET_FUNC( dPACKET_GUILD_MEMBERLIST,		OnPacketGuildMemberList			);
	SET_FUNC( dPACKET_MYSHOP_OPENRESULT,	OnPacketMyShopOpenResult		);
	SET_FUNC( dPACKET_MYSHOP_TITLECHANGED,	OnPacketMyShopChangeTitle		);
	SET_FUNC( dPACKET_MYSHOP_ITEMADDED,		OnPacketMyShopAddItem			);
	SET_FUNC( dPACKET_MYSHOP_ITEMREMOVED,	OnPacketMyShopRemoveItem		);
	SET_FUNC( dPACKET_MYSHOP_SELLBEGIN_RESULT, OnPacketMyShopSellBegin		);
	SET_FUNC( dPACKET_MYSHOP_SELLFINISH_OK, OnPacketMyShopSellFinish		);
	SET_FUNC( dPACKET_MYSHOP_SENDITEMLIST,	OnPacketMyShopItemList			);
	SET_FUNC( dPACKET_MYSHOP_UPDATEINFO,	OnPacketMyShopUpdateInfo		);
	SET_FUNC( dPACKET_MYSHOP_REMOVELISTITEM, OnPacketMyShopRemoveListItem	);
	SET_FUNC( dPACKET_MYSHOP_UPDATECASH,	OnPacketMyshopUpdateCash		);
	SET_FUNC( dPACKET_WORLD_CHAT,			OnPacketWorldChat				);
	SET_FUNC( dPACKET_INSERT_OBJMOB,		OnPacketInsertObjMob			);
	SET_FUNC( dPACKET_UPDATE_WEIGHTGAGE,	OnPacketUpdateWeightGage		);	
	SET_FUNC( dPACKET_CHANGE_NAMECOLOR,		OnPacketChangeNameColor			);	
	SET_FUNC( dPACKET_UPDATE_WARCAMP,		OnPacketUpdateWarCamp			);	
	SET_FUNC( dPACKET_UPDATE_WARINFO,		OnPacketUpdateWarinfo			);	
	SET_FUNC( dPACKET_UPDATE_GARBAGEMONEY,	OnPacketUpdateGarbageMoney		);
	SET_FUNC( dPACKET_UPDATE_PVP_MODE,		OnPacketUpdatePvPMode			);
	SET_FUNC( dAPCKET_SEND_CASTLELIST,		OnPacketSendCastleList			);
	SET_FUNC( dPACKET_SEND_WARSTATE,		OnPacketWarState				);
	SET_FUNC( dPACKET_OPEN_TAXWINDOW,		OnPacketOpenTaxWindow			);
	SET_FUNC( dPACKET_UPDATE_TAX,			OnPacketUpdateTax				);
	SET_FUNC( dPACKET_BILLING_ITEM_INFO,	OnPacketBillingItemInfo			);
	SET_FUNC( dPACKET_REMOVE_BILLING_ITEM,	OnPacketRemoveBillingItem		);
	SET_FUNC( dPACKET_MAP_SHOUT,			OnPacketMapShout				);
	SET_FUNC( dPACKET_ENEMY_DIVISION,		OnPacketEnemyDivision			);
	SET_FUNC( dPACKET_SHOW_ITEMTIMER,		OnPacketShowItemTimer			);
	//SET_FUNC( dPACKET_SELCHAR_SYSTEMMSG,	OnPacketSelCharSystemMsg		);
	SET_FUNC( dPACKET_SELCHAR_SYSTEMMSG,	OnPacketNull		);

	SET_FUNC( dPACKET_SMITH_QUIT,			OnPacketSmithFinish				);
	SET_FUNC( dPACKET_SMITH_ITEMADDED,		OnPacketSmithAddItem			);
	SET_FUNC( dPACKET_SMITH_ITEMDELED,		OnPacketSmithRemoveItem			);
	SET_FUNC( dPACKET_UPDATE_CARDCNT,		OnPacketUpdateCardCnt		    );

	SET_FUNC( dPACKET_FRIEND_MSG,			OnPacketFriendLoginMsg			);
	SET_FUNC( dPACKET_UPDATE_CASH_MONEY,	OnPacketUpdateCash				);

	SET_FUNC( dPACKET_FRIEND_ASKYESNO	,	OnPacketFriendDOUWANT			);
	SET_FUNC( dPACKET_FRIEND_JOIN_YES	,	OnPacketNull					);
	SET_FUNC( dPACKET_FRIEND_JOIN_NO	,	OnPacketNull					);
#undef SET_FUNC

	Log( "-=-=- ∞‘¿” √ ±‚»≠ : Function ª˝º∫ º∫∞¯ -=-=-" );

} //CGame::InitFuncArray

/**
 * Update.
 *
 * @return	æ÷«√∏Æƒ…¿Ãº« Ω««‡ ø©∫Œ
 */
BOOL CGame::Update(
	const DWORD		dwTick,		///< [IN] ∞Ê∞˙ tick
	string*			psName		///< [OUT] ¥Ÿ∏• ∞‘¿” ªÛ≈¬ «¡∑π¿”¿∏∑Œ ≥—±Ê ∞ÊøÏ,
								///<       ≥—æÓ∞• «¡∑π¿” ¿Ã∏ß.
	)

{
	Assert( psName );
	Assert( g_pMediaPlayer );
	Assert( g_pInput );
	Assert( g_pScene );
	Assert( g_pGUI );
	Assert( m_pWorld );

#ifdef _DEBUG
	if( g_bAttackMacro )
	{
		g_pPickMonster = NULL;
	} //if

	if( g_bSelfAttack )
		g_pPickCharacter = g_pPlayer;
#endif

	static DWORD s_dwTicks = 0;
	s_dwTicks += dwTick;

	if( s_dwTicks >= m_dwViewDelay )
	{
		// 2∫– ∏∂¥Ÿ ∞ªΩ≈.
		s_dwTicks = 0;
		CalculateShowLevel();
	} //if

	if( g_bBGM_On )
	{
		g_pMediaPlayer->Update( dwTick, m_pWorld->GetTime() );
	} //if	

	UpdateInput( dwTick );

	g_mgrSkill.Update( dwTick );

	UpdateNetwork( dwTick );

	
	//m_pWorld->Update( dwTick );

//	UpdateInput( dwTick );

	MovePlayer( dwTick );

	m_pWorld->Update( dwTick );
	//g_mgrSkill.Update( dwTick );

	//UpdateInput( dwTick );

//	m_bMiniGaming = m_mgFishHook.Update( dwTick );
	m_pCamera->Update( dwTick );

	

	if( m_bQuit )
	{	// ∞‘¿” ¡æ∑·.
		return FALSE;
	} //if

	if( m_bExitFrame )
	{
		*psName = "SelectCharacter";
		return TRUE;
	} //if

	Render( dwTick );

	return TRUE;
} //CGame::Update

/**
 * Render.
 *
 * @param	dwTick		[IN] ∞Ê∞˙ Tick
 */
void CGame::Render( const DWORD dwTick )
{
	Assert( m_pCamera );
	Assert( m_pWorld );
	Assert( g_pScene );
	Assert( g_pGUI );

	if( !m_bPlaying )					return;

	if( !g_pScene->BeginFrame() )		return;

	m_pCamera->Execute();
	m_pWorld->Draw();

	if( g_pSelectMgr )
		g_pSelectMgr->Draw( dwTick );

	g_pGUI->Draw();

//	m_mgFishHook.Render();

	g_pScene->EndFrame();	
} //CGame::Render

/**
 *
 */
void CGame::CalculateShowLevel()
{	
#define LIMIT	1.5
	g_fFPS = g_pScene->GetFPS();

	g_btShowLevel;

	float fFrame = g_btShowLevel * 2 + g_btShowLevel;

	if( g_fFPS <= fFrame + ( g_btShowLevel * LIMIT ) &&
		g_fFPS >= fFrame - ( g_btShowLevel * LIMIT ) )
	{
		// ¿˚¡§ «¡∑π¿” æ»¿Ã∂Û∏È ±◊≥… π´Ω√«—¥Ÿ.
		m_dwViewDelay = 0;
		return ;
	} //if

	if( g_fFPS > fFrame + ( g_btShowLevel + ( LIMIT + 2.2 )  ) )
	{
		// ¿˚¡§ √÷∞Ì «¡∑π¿”∫∏¥Ÿ ≈©¥Ÿ∏È,
		// ∑π∫ß¿ª ø√∏∞¥Ÿ.
		m_dwViewDelay = 2000;
		g_btShowLevel++;
	}
	else if( g_fFPS < fFrame - ( g_btShowLevel * LIMIT ) )
	{
		// ¿˚¡§ √÷«œ «¡∑π¿”∫∏¥Ÿ ¿€¥Ÿ∏È,
		// ∑π∫ß¿ª ≥ª∏∞¥Ÿ.
		g_btShowLevel--;
		m_dwViewDelay = 3000;//2 * 60 * 1000;
	} //if..else..

	g_btShowLevel = MAX( 0, g_btShowLevel );
	g_btShowLevel = MIN( 5, g_btShowLevel );

	/*
		5√  ∏∂¥Ÿ ∞ªΩ≈«—¥Ÿ.
		¿Ã∂ß ¡÷¿« «ÿæﬂ «“∞Õ¿Ã...
		±◊ãö∏∂¥Ÿ ø‘¥Ÿ∏Æ ∞¨¥Ÿ∏Æ «œ¥¬µ• ¿Ã∞… ¿‚æ∆æﬂ «—¥Ÿ.
		¥Îæ» 1.
				«—π¯ ≥∑√ﬂ∞≈≥™ ø√∑»¥Ÿ∏È øÿ∏∏«ÿº± ±◊¥Î∑Œ ¿÷æÓæﬂ «—¥Ÿ.
				±◊∑≤∑¡∏È «œ≥™æø ¥Ÿ √≥∏Æ∏¶ «ÿ¡÷¥¬∞Õ¿Ã ¡¡¿ªµÌ.
				( ¡Ô∞¢¡Ô∞¢ Ω«Ω√∞£¿∏∑Œ √≥∏Æ∏¶ «ÿ¡Ÿºˆ ¿÷¡ˆ∏∏, º“Ω∫∞° ∏πæ∆¡¯¥Ÿ. )
		¥Îæ» 2.
				Limit∏¶ µ÷º≠, «—π¯ πŸ≤Ó∏È √÷º“«— æÛ∏∂ µøæ»¿∫ πŸ≤Ó¡ˆ æ µµ∑œ «—¥Ÿ.
				( ±◊∏Æ ¡¡¿∫ πÊπ˝¿∫ æ∆¥“µÌ.. ¡Ô∞¢¡Ô∞¢ ¥Î∫Ò«“ºˆ æ¯¥Ÿ. )
			
		¿⁄ æÀ¿œ¿Ã ¿‚æ∆º≠ «ÿ¡÷¥¬ ¥Îæ» 1¿Ã ∞°¿Â ¿˚¥Á«“∞≈ ∞∞¥Ÿ.

		∞¢∞¢¿« ¿˚¥Á«¡∑π¿”¿ª ±∏«œ¥¬ πÊπ˝¿∫ ¥Ÿ¿Ω∞˙ ∞∞¥Ÿ.

		«¡∑π¿” = «ˆ¿Á ∑π∫ß * 2 + «ˆ¿Á ∑π∫ß

		∞¢ «¡∑π¿”∞£ +, - «ˆ¡¶ ∑π∫ß 	

		«¡∑π¿”¿ª ∞®«“∂ß¥¬, µÙ∑π¿Ã∏¶ µ–¥Ÿ. ( 2∫– )
	*/
#undef LIMIT
} //CGame::CalculateShowLevel

/**
 * ªÁøÎ¿⁄ ¿‘∑¬¿ª √≥∏Æ«—¥Ÿ.
 *
 * @param	dwTick		[IN] ∞Ê∞˙ Tick
 */
void CGame::UpdateInput( const DWORD dwTick )
{
	if( !m_bPlaying )		return;

	Assert( g_pInput );
	Assert( g_pGUI );
	Assert( m_pCamera );

	g_pInput->Update();

	InputCamera( dwTick );

	if( g_pGUI->PopEvent( g_szUIName, &g_UIevent, &g_dwUIParam1, &g_dwUIParam2 ) )
	{
		switch( g_UIevent )
		{
		case EVENT_KEYDOWN:					OnEventKeyDown( g_dwUIParam1 );									break;
		case EVENT_KEYUP:					OnEventKeyUp( g_dwUIParam1 );									break;
		case EVENT_BUTTON:					OnEventButton( g_szUIName, g_dwUIParam2 );						break;
		case EVENT_RETURNKEY:				OnEventReturnKey( g_szUIName );									break;
		case EVENT_SHOWDLG:					OnEventShowDlg( g_szUIName, g_dwUIParam2 );						break;
		case EVENT_ITEM_INSERTINVENTORY:	OnEventInsertItemInInventory( g_dwUIParam1, g_dwUIParam2 );		break;
		case EVENT_ITEM_HOLDING:			OnEventHolding( g_dwUIParam1, g_dwUIParam2 );					break;
		case EVENT_ITEM_CHANGEHAND:			OnEventChangeHand( g_dwUIParam1, g_dwUIParam2 );				break;
		case EVENT_ITEM_MOVE:				OnEventItemMove( g_dwUIParam1, g_dwUIParam2 );					break;
		case EVENT_ITEM_DROP:				OnEventItemDrop( g_dwUIParam1, g_dwUIParam2 );					break;
		case EVENT_ITEM_INSERTEQUIP:		OnEventInsertEquip( g_dwUIParam2 );								break;
		case EVENT_ITEM_REMOVEQUIP:			OnEventRemoveEquip( g_dwUIParam2 );								break;
		case EVENT_ITEM_TOMAP:				OnEventItemToMap( g_dwUIParam1, g_dwUIParam2 );					break;
		case EVENT_SPELL_MOVE:				OnEventSpellMove( g_dwUIParam1, g_dwUIParam2 );					break;
		case EVENT_SETMAINSKILL:			OnEventSetMainSkill( g_dwUIParam1, g_dwUIParam2 );				break;
		case EVENT_ITEM_BUY:				OnEventBuyItem( g_dwUIParam1, g_dwUIParam2 );					break;
		case EVENT_ITEM_SELL:				OnEventSellItem( g_dwUIParam1, g_dwUIParam2 );					break;
		case EVENT_CAST_ITEM:				OnEventCastItem( g_dwUIParam1 );								break;
		case EVENT_SENDCHATTING:			OnEventSendChatting( g_dwUIParam2 );							break;
		case EVENT_DEAL_ADDMONEY:			OnEventDealAddGold( g_dwUIParam1 );								break;
		case EVENT_DEAL_ADDITEM:			OnEventDealAddItem( g_dwUIParam1, g_dwUIParam2 );				break;
		case EVENT_DEAL_DELETEITEM:			OnEventDealDeleteItem( g_dwUIParam1, g_dwUIParam2 );			break;
		case EVENT_DEAL_CANCEL:				OnEventDealCancel();											break;
		case EVENT_DEAL_CHECKIN:			OnEventDealCheckIn();											break;
		case EVENT_PARTY_CREATE:			OnEventPartyCreate();											break;
		case EVENT_PARTY_DESTROY:			OnEventPartyDestroy();											break;
		case EVENT_PARTY_INVIT:				OnEventPartyInvit( g_dwUIParam1 );								break;
		case EVENT_PARTY_OUT:				OnEventPartyOut();												break;
		case EVENT_PARTY_BANISH:			OnEventPartyBanish();											break;
		case EVENT_PARTY_BOARD:				/*OnEventPartyBoard();*/										break;
		case EVENT_PARTY_BOARDREGIST:		/*OnEventPartyBoardRegist();*/									break;
		case EVENT_PARTY_HEAL:				OnEventPartyHeal( g_dwUIParam1 );								break;
		case EVENT_DEAL_REQUEST:			OnEventDealRequest();											break;
		case EVENT_PARTY_REQUEST:			OnEventPartyRequest( g_dwUIParam1 );							break;
		case EVENT_TALK_REQUEST:			OnEventTalkRequest( g_dwUIParam1, g_dwUIParam2 );				break;
		case EVENT_SHOP_REQUEST:			OnEventShopRequest( g_dwUIParam1, g_dwUIParam2 );				break;
		case EVENT_QUEST_REQUEST:			OnEventQuestRequest( g_dwUIParam1, g_dwUIParam2 );				break;

		case EVENT_SMITH_REQUEST:			OnEventSmithRequest( g_dwUIParam1, g_dwUIParam2 );				break;
		case EVENT_SMITH_ADDITEM:			OnEventSmithAddItem( g_dwUIParam1 );							break;
		case EVENT_SMITH_REMOVEITEM:		OnEventSmithRemoveItem( g_dwUIParam1 );							break;
		case EVENT_SMITH_ENCHENT:			OnEventSmithUpdate( g_dwUIParam1, g_dwUIParam2 );				break;
		case EVENT_CARDSUM:					OnEventCardSum( g_dwUIParam1, g_dwUIParam2 );					break;
		case EVENT_CARDUNDONPC:				OnEventCardUndoNPC( g_dwUIParam1, g_dwUIParam2 );				break;
		case EVENT_CARDUNDO:				OnEventCardUndo( g_dwUIParam1 );								break;
		
		case EVENT_STORAGE_REQUEST:			OnEventStorageRequest();										break;
		case EVENT_STORAGE_ADDITEM:			OnEventStorageAddItem( g_dwUIParam1 );							break;
		case EVENT_STORAGE_REMOVEITEM:		OnEventStorageRemoveItem( g_dwUIParam1 );						break;
		case EVENT_STORAGE_SAVEGOLD:		OnEventStorageSaveGold( g_dwUIParam2 );							break;
		case EVENT_STORAGE_GETGOLD:			OnEventStorageGetGold( g_dwUIParam2 );							break;
		case EVENT_QUEST_ACCEPT:			OnEventQuestAccept( g_dwUIParam1, g_dwUIParam2 );				break;
		case EVENT_OPTION_SOUND:			OnEventSoundOption();											break;
		case EVENT_OPTION_BGMONOFF:			OnEventBGMOnOff( g_dwUIParam2 );								break;
		case EVENT_OPTION_EFFONOFF:			OnEventEffOnOff( g_dwUIParam2 );								break;
		case EVENT_OPTION_51CHONOFF:		OnEvent51ChOnOff( g_dwUIParam2 );								break;
		case EVENT_SOUND_VOLUME:			OnEventSoundVol( g_dwUIParam1, g_dwUIParam2 );					break;
		case EVENT_DBCLICK_ITEM:			OnEventDBClickItem( g_dwUIParam1, g_dwUIParam2 );				break;
		case EVENT_USEQUICKITEM:			OnEventQuickSlot( g_dwUIParam1 );								break;
		case EVENT_CAMERA:					OnEventCamera( g_dwUIParam1 );									break;
		case EVENT_DIE:						OnEventPlayerDie( g_dwUIParam1 );								break;
		case EVENT_TIMER:					OnEventTimer( g_szUIName, g_dwUIParam1 );						break;
		case EVENT_ITEM_AUTOPICKUP:			OnEventAutoItemPickUp();										break;
		case EVENT_ENCHANT:					OnEventEnchant( g_dwUIParam1, g_dwUIParam2 );					break;
		case EVENT_ENCHANTCARD_END:			OnEventEndEnchant();											break;
		case EVENT_INFO_POSITION:			OnEventInfoPosition();											break;
		case EVENT_SKILL_SPECIAL:			OnEventSkillSpecial( g_dwUIParam1 );							break;
		case EVENT_FRIEND_CUTOFF:			OnEventFriendCut();												break;
		case EVENT_CHATT_CUTOFF:			OnEventChattingCut();											break;
		case EVENT_WHISPER_CUTOFF:			OnEventWhisperCut();											break;
		case EVENT_DEAL_CUTOFF:				OnEventDealCut();												break;
		case EVENT_FRIEND_REGIST:			OnEventFriendInvit( g_dwUIParam1 );								break;//	OnEventFriendRegist();									break;
	    case EVENT_FRIEND_LIST:				OnEventFriendList();											break;
		case EVENT_FRIEND_DELETE:			OnEventFriendDelete();											break;
		case EVENT_GUILD_TAKEMEMBER:		OnEventGuildTakeMember();										break;
		case EVENT_GUILD_AID:				OnEventAid( g_dwUIParam1, g_dwUIParam2 );						break;
		case EVENT_GUILD_REMOVEMEMBER:		OnEventGuildRemoveMember();										break;
		case EVENT_GUILD_OUT:				OnEventGuildQuit();												break;
		case EVENT_GUILD_CREATE:			OnEventGuildCreate();											break;
		case EVENT_GUILD_RELEASE:			OnEventGuildRelease();											break;
		case EVENT_GUILD_PROMOTE:			OnEventGuildPromote();											break;
		case EVENT_GUILD_DEMOTE:			OnEventGuildDemote();											break;
		case EVENT_GUILD_WARINFO:			OnEvnetGuildWarInfo( g_dwUIParam1 );							break;
		case EVENT_GUILD_GETCASTLELIST:		OnEvnetGuildGetCastleList();									break;
		case EVENT_GOTODEALMAP	:			OnEventGotoDealMap();											break;
		case EVENT_PET_ONOFF:				OnEventPetOnOff();												break;
		case EVENT_PET_CHANGENAME:			OnEventChangePetName();											break;
		case EVENT_PKMODE:					OnEventPKMode( g_dwUIParam1, g_dwUIParam2 );					break;
		case EVENT_GUILD_ALLMEMBERLIST:		OnEventRequestGuildList();										break;
		case EVENT_MYSHOP_OPEN:				OnEventMyShopOpen();											break;
		case EVENT_MYSHOP_CHANGETITLE:		OnEventMyShopChangeTitle();										break;
		case EVENT_MYSHOP_ADDITEM:			OnEventMyShopAddItem( g_dwUIParam1, g_dwUIParam2 );				break;
		case EVENT_MYSHOP_REMOVEITEM:		OnEventMyShopRemoveItem( g_dwUIParam1 );						break;
		case EVENT_MYSHOP_SELLBEGIN:		OnEventMyShopSellBegin();										break;
		case EVENT_MYSHOP_SELLFINISH:		OnEventMyShopSellFinish();										break;
		case EVENT_MYSHOP_GETITEMLIST:		OnEventMyShopGetItemList( g_dwUIParam1 );						break;
		case EVENT_MYSHOP_BUYITEM:			OnEventMyShopBuyItem( g_dwUIParam1 );							break;
		case EVENT_TEX_GET:					OnEventGetTex();												break;
		case EVENT_TEX_CHANGE:				OnEventChangeTex( g_dwUIParam1 );								break;
		case EVENT_ENCHANT_CHANGE:			OnEventEnchantChange( g_dwUIParam1 );							break;
		case EVENT_ENCHANT_COLLECTION:		OnEventEnchantCollection( g_dwUIParam1 );						break;
		case EVENT_ITEMROOK:				OnEventItemRook();												break;
		case EVENT_LEVELROOK:				OnEventLevelRook();												break;
		case EVENT_USECASHITEM:				OnEventCashItemUse( g_dwUIParam1 );								break;
		case EVENT_GETBILLINGITEM:			OnEventGetBillingItem();										break;
		
		} //switch
	}

	if( !g_pGUI->IsIMEInput() )
	{
		OnEventHotKey( dwTick );
	} //if

	if( g_pGUI->Update( dwTick ) )
	{	// ∏∂øÏΩ∫∞° GUI ¿ßø° ¿÷¥Ÿ∏È «√∑π¿ÃæÓ ¡∂¿€¿ª «œ¡ˆ æ ¥¬¥Ÿ.
		return;
	} //if

	InputPlayer( dwTick );
} //CGame::UpdateInput

/**
 * ªÁøÎ¿⁄ ¿‘∑¬ø° µ˚∂Ûº≠ ƒ´∏ﬁ∂Û∏¶ øÚ¡˜¿Œ¥Ÿ.
 *
 * @param	dwTick		[IN] ∞Ê∞˙ Tick
 */
void CGame::InputCamera( const DWORD dwTick )
{
	Assert( m_pCamera );

	if( m_nZoomMode == ZOOM_LONG )
	{ //∞¯º∫¿¸Ω√.
		m_fCameraOffset = -15;
		//m_pCamera->SetMaxMapZoom();		
		m_pCamera->SetMaxZoom( 35.0F );
		m_pCamera->Zoom( m_fCameraOffset );
	}
	else if( m_nZoomMode == ZOOM_SHORT )
	{ //¿œπ›¿¸Ω√.
		if( m_pCamera->GetZoom() > m_pCamera->GetMaxMapZoom() )
		{
			m_fCameraOffset = 5;
		}
		else
		{
			m_fCameraOffset = 0.005F * g_pInput->GetMouseZ();
			m_pCamera->SetMaxMapZoom();
		//	m_pCamera->SetMaxZoom( 15.0F );
		} //if..else
		m_pCamera->Zoom( m_fCameraOffset );
	} //if..else

	if( g_pInput->GetMouseR() )
	{
		int		nMouseX = g_pInput->GetMouseX();
		int		nMouseY = g_pInput->GetMouseY();
		float	fAngle = 3.0F * dwTick * 30 / 1000;

		if( nMouseX < 4 )
		{
			m_pCamera->RotateZ( -fAngle );
		}
		else if( nMouseX >= g_nWidth - 4 )
		{
			m_pCamera->RotateZ( fAngle );
		} //if

		if( nMouseY < 4 )
		{
			m_pCamera->RotateX( fAngle );
		}
		else if( nMouseY >= g_nHeight - 4 )
		{
			m_pCamera->RotateX( -fAngle );
		} //if
	/*	fAngle = 0.2F * g_pInput->GetMouseMoveX() * dwTick * 30 / 1000;
		m_pCamera->RotateZ( fAngle );

		fAngle = -0.2F * g_pInput->GetMouseMoveY() * dwTick * 30 / 1000;
		m_pCamera->RotateX( fAngle );*/
	}
	else if( g_pInput->GetMouseM() )
	{
		//∏∂øÏΩ∫ Ω√¡°∫Ø∞Ê
		float fAngle = 0.2F * g_pInput->GetMouseMoveX() * dwTick * 30 / 1000;
		m_pCamera->RotateZ( fAngle );

		fAngle = -0.2F * g_pInput->GetMouseMoveY() * dwTick * 30 / 1000;
		m_pCamera->RotateX( fAngle );
	} //if

	g_pSoundMgr->SetPosition( m_pCamera->GetPosition(), m_pCamera->GetFront() );
} //CGame::InputCamera

/**
 * ¿‘∑¬ø° µ˚∏• «√∑π¿ÃæÓ «‡µø √≥∏Æ.
 *
 * @param	dwTick		[IN] ∞Ê∞˙ tick
 */
void CGame::InputPlayer( const DWORD dwTick )
{
	m_pPlayer = g_pPlayer;

	if( !m_pPlayer )
	{
		return;
	} //if

	if( m_pPlayer->GetState() == STATE_DEAD ||
		m_nZoomMode == ZOOM_LOCK )
	{	// «√∑π¿ÃæÓ∞° µÓ¿Â«œ¡ˆ æ æ“∞≈≥™ ¡◊¿∫ ªÛ≈¬¿Ãπ«∑Œ
		// æ∆π´∑± ¡∂¿€µµ ∞°¥…«œ¡ˆ æ ¥Ÿ.
		return;
	} //if

	int			nMouseX	= g_pInput->GetMouseX();
	int			nMouseY	= g_pInput->GetMouseY();

	static Vector3	vPos; //Vector3( 0.0F, 0.0F, 0.0F );
	vPos = m_pPlayer->GetPosition();

	int			nX = (int)vPos.x;
	int			nY = (int)vPos.y;

	m_pCamera->FullShot( vPos );
	m_pWorld->GetTerrain()->SetPosition( vPos );

	// BGM ¿Áª˝
	PlayAreaBGM( nX, nY );

	///////////////////////////////////////
	//	Check Picking Object
	///////////////////////////////////////

	if( !CheckPick( vPos, nMouseX, nMouseY ) )
	{
		g_pGUI->SetCursorMode( "CURSOR_NORMAL" );
	} //if..else..

	///////////////////////////////////////
	//	Check Click Mouse
	///////////////////////////////////////

	if( g_pInput->GetMouseLClick() &&
		!g_infoPlayer.btStun )				///< ∏∂øÏΩ∫ ≈¨∏Ø«œ∞Ì Ω∫≈œ ªÛ≈¬∞° æ∆¥œ∏È
	{	// ∏∂øÏΩ∫ øﬁ¬  πˆ∆∞ ≈¨∏Ø.
		float		fHeight	= m_pPlayer->GetPosition().z;
		m_pPlayer->SetLayer();

		if( nMouseX <= 0)// = g_pInput->GetMouseX();
			return;
		else if( nMouseX >= g_nWidth -1 )
			return;

		if( nMouseY <= 0 )
			return;
		else if( nMouseY >= g_nHeight - 1 )
			return;

		static Vector3	vPick( 0.0F, 0.0F, 0.0F );

		m_nPickedLayer = m_pWorld->PickTerrain( m_pPlayer->GetLayer(), fHeight, nMouseX, nMouseY, &vPick );
		
		OnMouseLClick( m_nPickedLayer, vPick );
	}
	else	if( g_pInput->GetMouseRClick() )
	{	// ∏∂øÏΩ∫ ø¿∏•¬  πˆ∆∞ ≈¨∏Ø.
		OnMouseRClick( TRUE, vPos );
	} //if
#ifdef _DEBUG
	else	if( g_bAttackMacro && g_pPickMonster )
	{
		PickedMonster_R();
		//InputPlayer
		//OnMouseRClick( TRUE, vPos );
	}
	else	if( g_bSelfAttack && g_pPickCharacter )
	{
		PickedCharacter_R();
	}
#endif

#ifdef _UMENOKOJI_MODE_
/*	if( g_pGUI->GetAttackMonster() == TRUE )//g_pGUI->GetCheck() == TRUE )
	{			
		OnMouseRClick( TRUE, vPos );
		if( m_dwTickss >= 1 )
		{
			g_pGUI->SetAttackMonster( FALSE );
			m_dwTickss=0;
		}
		m_dwTickss++;
	}*/
#endif //#ifdef _DEBUG
} //CGame::InputPlayer

/**
 *
 */
BOOL CGame::CheckPick( const Vector3& vPos, int nMouseX, int nMouseY )
{
/*	if( m_pPlayer->IsGhost() )
	{
		g_pPickCharacter	= NULL;
		g_pPickMonster		= NULL;
		g_pPickNPC			= NULL;
		g_pPickItem			= NULL;
		m_dwPickDoor		= 0;
//		return FALSE;
	} //if
*/
	if( m_nPickLevel == 0 )
	{		
	/*	g_pPickCharacter	= NULL;
		g_pPickMonster		= NULL;
		g_pPickNPC			= NULL;
		g_pPickItem			= NULL;
		m_dwPickDoor		= 0;*/
	} //if

	static Vector3		s_vOrig;
	static Vector3		s_vDir;
	g_pScene->GetPickRay( nMouseX, nMouseY, &s_vOrig, &s_vDir );

	switch( m_nPickLevel )
	{
	case 3:
		{	// øÏº± º¯¿ß±«¿∏∑Œ ƒ≥∏Ø≈Õ∏¶ ∏’¿˙ ø¨ªÍ«—¥Ÿ.
			CheckPickCharacter( vPos, s_vOrig, s_vDir, FALSE );
			CheckPickMonster( vPos, s_vOrig, s_vDir, TRUE );
			CheckPickNpc( vPos, s_vOrig, s_vDir, TRUE );
			CheckPickEtc( vPos, s_vOrig, s_vDir, TRUE );
		}
		break;
	case 2:
		{
			CheckPickMonster( vPos, s_vOrig, s_vDir, FALSE );
			CheckPickNpc( vPos, s_vOrig, s_vDir, TRUE );
			CheckPickEtc( vPos, s_vOrig, s_vDir, TRUE );
			CheckPickCharacter( vPos, s_vOrig, s_vDir, TRUE );
		} 
		break;
	case 1:
		{
			CheckPickNpc( vPos, s_vOrig, s_vDir, FALSE );
			CheckPickEtc( vPos, s_vOrig, s_vDir, TRUE );
			CheckPickCharacter( vPos, s_vOrig, s_vDir, TRUE );
			CheckPickMonster( vPos, s_vOrig, s_vDir, TRUE );
		} 
		break;
	case 0:
		{
			CheckPickEtc( vPos, s_vOrig, s_vDir, FALSE );
			CheckPickCharacter( vPos, s_vOrig, s_vDir, TRUE );
			CheckPickMonster( vPos, s_vOrig, s_vDir, TRUE );
			CheckPickNpc( vPos, s_vOrig, s_vDir, TRUE );
		} 
		break;
	}; //switch

/*	if( g_sTarget.IsEmpty() )
	{
		g_pGUI->SetAttackMonster( FALSE );
	}*/

	m_nPickLevel++;

	if( m_nPickLevel > 3 )
	{
		m_nPickLevel = 0;
	
		if( !m_bPick )
		{
			return FALSE;
		} //if
		m_bPick = FALSE;
	} //if

	return TRUE;
} //CGame::CheckPick

/**
 *
 */
BOOL CGame::CheckPickCharacter( const Vector3& vPos, Vector3& vOrig, Vector3& vDir, BOOL bSkip )
{
	if( !bSkip && !m_bPick )
	{
		g_pPickCharacter = NULL;
		if( g_pPickCharacter = m_pWorld->PickCharacter( vPos, vOrig, vDir ) )
		{	// ƒ≥∏Ø≈Õ∞° ««≈∑µ» ∞ÊøÏ
			g_pPickCharacter->SetLighting( FALSE );
			
			if( !CheckPlayerKill() )	g_pGUI->SetCursorMode( "CURSOR_NORMAL" );
			else						g_pGUI->SetCursorMode( "CURSOR_ATTACK" );
			
			m_bPick = TRUE;

			g_pPickMonster		= NULL;
			g_pPickNPC			= NULL;
			g_pPickItem			= NULL;
			g_pFuncEntity		= NULL;
			m_dwPickDoor		= 0;
			return TRUE;
		} //if
	}
	else
	{
		if( g_pPickCharacter )
		{	// ƒ≥∏Ø≈Õ∞° ««≈∑µ» ∞ÊøÏ
			g_pPickCharacter->SetLighting( FALSE );

			if( !CheckPlayerKill() )	g_pGUI->SetCursorMode( "CURSOR_NORMAL" );
			else						g_pGUI->SetCursorMode( "CURSOR_ATTACK" );

			return TRUE;
		} //if
	} //if..else..
	return FALSE;
} //CGame::CheckPickCharacter

/**
 *
 */
BOOL CGame::CheckPickMonster( const Vector3& vPos, Vector3& vOrig, Vector3& vDir, BOOL bSkip )
{
	if( !bSkip && !m_bPick )
	{
		if( g_pPickMonster = m_pWorld->PickMonster( vPos, vOrig, vDir ) )
		{	// ∏ÛΩ∫≈Õ∞° ««≈∑µ» ∞ÊøÏ
			g_pPickMonster->SetLighting( FALSE );
			g_pGUI->SetCursorMode( "CURSOR_ATTACK" );
			m_bPick = TRUE;

			g_pPickCharacter	= NULL;
			g_pPickNPC			= NULL;
			g_pPickItem			= NULL;
			g_pFuncEntity		= NULL;
			m_dwPickDoor		= 0;
			return TRUE;
		} //if
	}
	else
	{
		if( g_pPickMonster )
		{	// ∏ÛΩ∫≈Õ∞° ««≈∑µ» ∞ÊøÏ
			g_pPickMonster->SetLighting( FALSE ); 
			g_pGUI->SetCursorMode( "CURSOR_ATTACK" );
			return TRUE;
		} //if
	} //if..else..

	return FALSE;
} //CGame::CheckPickMonster

/**
 *
 */
BOOL CGame::CheckPickNpc( const Vector3& vPos, Vector3& vOrig, Vector3& vDir, BOOL bSkip )
{
	if( !bSkip && !m_bPick )
	{
		if( g_pPickNPC = m_pWorld->PickNPC( vPos, vOrig, vDir ) )
		{	// NPC ««≈∑µ» ∞ÊøÏ
			if( ( vPos - g_pPickNPC->GetPosition() ).Length() < 10.0F )
			{
				g_pPickNPC->SetLighting( FALSE );
				g_pGUI->SetCursorMode( "CURSOR_TALK" );
				m_bPick = TRUE;

				g_pPickCharacter	= NULL;
				g_pPickMonster		= NULL;
				g_pPickItem			= NULL;
				g_pFuncEntity		= NULL;
				m_dwPickDoor		= 0;
				return TRUE;
			}
			else
			{
				g_pPickNPC = NULL;
			} //if
		} //if..else..
	}
	else
	{
		if( g_pPickNPC )
		{	// æ∆¿Ã≈€¿Ã ««≈∑µ» ∞ÊøÏ
			g_pPickNPC->SetLighting( FALSE );
			g_pGUI->SetCursorMode( "CURSOR_TALK" );
			return TRUE;
		} //if..else..	
	} //if..else..

	return FALSE;
} //CGame::CheckPickNpc

/**
 *
 */
BOOL CGame::CheckPickEtc( const Vector3& vPos, Vector3& vOrig, Vector3& vDir, BOOL bSkip )
{
	if( !bSkip && !m_bPick )
	{
		if( g_pPickItem = m_pWorld->PickItem( vPos, vOrig, vDir ) )
		{	// æ∆¿Ã≈€¿Ã ««≈∑µ» ∞ÊøÏ
			if( ( vPos - g_pPickItem->GetPosition() ).Length() < 3.0F )
			{
				g_pPickItem ->SetLighting( FALSE );

				g_pGUI->SetCursorMode( "CURSOR_HAND" );
				m_bPick = TRUE;

				g_pPickCharacter	= NULL;
				g_pPickMonster		= NULL;
				g_pPickNPC			= NULL;
				g_pFuncEntity		= NULL;
				m_dwPickDoor		= 0;

				return TRUE;
			}
			else
			{
				g_pPickItem = NULL;
			} //if
		}
		else if( g_pFuncEntity = m_pWorld->PickFuncEntity( vPos, vOrig, vDir ) )
		{
			g_pFuncEntity ->SetLighting( FALSE );

			g_pGUI->SetCursorMode( "CURSOR_SELECT" );

			m_bPick				= TRUE;
			g_pPickItem			= NULL;
			g_pPickCharacter	= NULL;
			g_pPickMonster		= NULL;
			g_pPickNPC			= NULL;
			m_dwPickDoor		= 0;

			return TRUE;
		}
	/*	else if( m_dwPickDoor = m_pWorld->PickDoor( vPos, vOrig, vDir ) )
		{
			g_pGUI->SetTipText( STR_UIMSG_DOOR, 0xff0000c2 );
			g_pGUI->SetCursorMode( "CURSOR_DOOR1" );

			m_bPick				= TRUE;
			g_pPickCharacter	= NULL;
			g_pPickMonster		= NULL;
			g_pPickNPC			= NULL;
			g_pPickItem			= NULL;
			g_pFuncEntity		= NULL;

			return TRUE;
		} //if..else..*/
	}
	else
	{
		if( g_pPickItem )
		{	// æ∆¿Ã≈€¿Ã ««≈∑µ» ∞ÊøÏ
			g_pPickItem->SetLighting( FALSE );
			g_pGUI->SetCursorMode( "CURSOR_HAND" );
			return TRUE;
		}
		else if( g_pFuncEntity )
		{
			g_pFuncEntity->SetLighting( FALSE );
			g_pGUI->SetCursorMode( "CURSOR_SELECT" );
			return TRUE;
		}
	/*	else if( m_dwPickDoor )
		{
			g_pGUI->SetTipText( STR_UIMSG_DOOR, 0xff0000c2 );
			g_pGUI->SetCursorMode( "CURSOR_DOOR1" );
			return TRUE;
		} //if..else..*/
	} //if..else..

	return FALSE;
} //CGame::CheckPickEtc

/**
 * ≥◊∆Æø˜ √≥∏Æ.
 *
 * @param	dwTick		[IN] ∞Ê∞˙ Tick
 */
void CGame::UpdateNetwork( const DWORD dwTick )
{
	Assert( g_pClient );
	Assert( g_pGUI );

	if( !g_pClient->Recv() )
	{
		g_pClient->Disconnect();
	} //if

	if( !g_pClient->IsConnect() )
	{	
		static BOOL		s_bConnect = TRUE;
		if( s_bConnect )
		{
			g_pGUI->MessageBox(
				STR_SYSERR_SERVERDISCONNECT,
				"DISCONNECT",
				IW3DUInterface::MSG_OK );
			s_bConnect = FALSE;
		} //if
	} //if

	static DWORD	s_dwTick = 0;
	if( s_dwTick > 11500 )
	{	// 10√ ∏∂¥Ÿ Heart Beat ∆–≈∂¿ª ∫∏≥ø.
		CPacket		sendPacket;
		sendPacket.Tag( dPACKET_HEARTBEAT );
		g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
		s_dwTick = 0;
	}
	else
	{
		s_dwTick += dwTick;
	} //if..else
	
	if( g_pClient->GetSendSize() > 0 )
	{
		g_pClient->Send();
	} //if

	while( g_pClient->GetRecvSize() > 2 )
	{	// πˆ∆€ø° Ω◊¿Œ ∆–≈∂¿ª √≥∏Æ.
		char*		pData = const_cast<char*>( g_pClient->GetRecvData() );
		int			nSize = *reinterpret_cast<WORD*>( pData );
		
		if( nSize > g_pClient->GetRecvSize() )	break;
		
		//ø©±‚º≠ æ–√‡ «¨¥Ÿ char	pDecryptData[40960];  edit here
		char	pDecryptData[4096];
		DWORD	key[4] = { dAMHO_KEY1, dAMHO_KEY2, dAMHO_KEY3, dAMHO_KEY4 };
		DWORD	dwEncSize = TEA_DecryptData( pData + 2, pDecryptData, nSize - 2, key );
		
		char	pDec[4096];
		memcpy( pDec, &pDecryptData[2], dwEncSize );
		dwEncSize -= 2; 
		


	m_recvPacket.SetPacket( dwEncSize + 2, pDec );
		ProcessRecvPacket();
		g_pClient->RemoveRecvData( nSize ); 

	//	m_recvPacket.SetPacket( nSize, pData + 2 );
	//	ProcessRecvPacket();
	//	g_pClient->RemoveRecvData( nSize );
		
		if( m_bExitFrame )	break;	// ó?!!!
	} //while

	
} //CGame::UpdateNetwork

/**
 * «√∑π¿ÃæÓ∏¶ øÚ¡˜¿Œ¥Ÿ.
 * ¿Ãµø∏∏¿Ã æ∆¥œ∂Û, «√∑π¿ÃæÓ¿« ∏µÁ «‡µø¿ª ¡¶æÓ«—¥Ÿ.
 */
void CGame::MovePlayer( const DWORD dwTick )
{
	static DWORD	s_dwTick;
	static int		s_nPosX = 0;
	static int		s_nPosY = 0;

	if( !m_pPlayer )		return;

	m_pPlayer->SetAttackMove( FALSE );

	if( !g_sTarget.IsEmpty() )
	{	// «√∑π¿ÃæÓ∞° ∞¯∞› ¡ﬂ¿Œ ∏ÛΩ∫≈Õ∞° ¿÷¿ª ∞ÊøÏ..

		if( m_bAttack )
		{
			m_dwAttackLimit += dwTick;
		} //if

		AttackCreature();
	} //if

	if( s_dwTick > 400 && !m_bAttack )
	{
		int		nX = (int)m_pPlayer->GetPosition().x;
		int		nY = (int)m_pPlayer->GetPosition().y;
		if( s_nPosX != nX || s_nPosY != nY )
		{
			s_nPosX = nX;
			s_nPosY = nY;
			g_pGUI->SetPlayerMapPos( nX, nY );

			SendPacket_CharacterMove();			
		} //if

		s_dwTick = 0;
	} //if
	s_dwTick += dwTick;
} //CGame::MovePlayer

/**
 * «√∑π¿ÃæÓ∞° ¿¸≈ı ªÛ≈¬¿œ ∂ß∏¶ √≥∏Æ«—¥Ÿ.
 */
void CGame::AttackCreature()
{
	if( !m_pPlayer )		return;

	IW3DCreature*	pTarget = GetTargetCreature();
	if( !pTarget )
	{	// ∞¯∞› ¥ÎªÛ ∏ÛΩ∫≈Õ∞° ø˘µÂø° ¡∏¿Á«œ¡ˆ æ ¿ª ∞ÊøÏ ∞¯∞› ªÛ≈¬∏¶ «ÿ¡¶«—¥Ÿ.
		g_pGUI->ShowTargetInfo( 0 );
		g_sTarget.Clean();
		m_bAttack		= FALSE;
		m_bToTarget		= FALSE;
		m_bAdvanceMove	= FALSE;
		return;
	} //if
	
	if( m_bSendAttackPacket && m_dwAttackLimit >= ATTACKLIMIT_TIME )
	{
		m_dwAttackLimit = 0;
		m_bSendAttackPacket = TRUE;
	} //if

	static Vector3	vTarget;
	static Vector3	vDir;
	vTarget		= pTarget->GetPosition();
	vDir		= vTarget - m_pPlayer->GetPosition();

	if( vDir.Length() >= m_fAttackRange )
	{	// ∞¯∞› øµø™ π€¿Œ ∞ÊøÏ ∏Ò«•∏¶ √ﬂ¿˚«—¥Ÿ.
		m_pPlayer->SetAttackMove( TRUE );

		if( !m_bToTarget )
		{
			m_bToTarget = TRUE;
			m_pPlayer->Move();
		} //if
		
		if( !m_bNoMove )
		{
			m_pPlayer->SetGoalPosition( vTarget );
		} //if

		m_bAttack = FALSE;
		m_bAdvanceMove = FALSE;
		m_bMoveAttack	= TRUE;
	}
	else if( !m_bAttack )
	{	// ∞¯∞› ∏µÂ∞° æ∆¥— ∞ÊøÏ ∞¯∞› ∆–≈∂¿ª ∫∏≥Ω¥Ÿ.

		if( !m_pPlayer->CanAttack() )
		{
			g_pGUI->ChattingInput( "", STR_SKILL_NOTUSINGHORSE );
			g_sTarget.Clean();
			return ;
		} //if

		if( 0.0F == vDir.Length() )
		{
			m_pPlayer->SetPosition( m_pPlayer->GetPosition() + Vector3( 0.001F, 0.0F, 0.0F ) );
			vDir = Vector3( 0.001F, 0.0F, 0.0F );
		} //if

		if( !m_bNoMove )
		{
			m_pPlayer->SetGoalPosition( m_pPlayer->GetPosition() );
		} //if
		m_pPlayer->Stop();

		///////////////  »Æ¿Œ¿∏∑Œ ¿Ãµø∆–≈∂  //////////////
		SendPacket_CharacterMove();

		///////////////  ∞¯∞›∆–≈∂  //////////////

		if( m_pPlayer->GetState() == STATE_ATTACK )
		{
			return ;
		} //if

		m_pPlayer->SetAttackMove( FALSE );

		if( m_pPlayer->GetSkill() == SKILL_WHITE || m_pPlayer->GetSkill() == SKILL_BLACK )
		{ // ∏∂π˝ ªÁøÎΩ√ ∆Øº∫ø° µ˚∂Û √≥∏Æ«—¥Ÿ.
			m_pPlayer->Attack( pTarget );
		}
		else
		{ // ¿œπ›¿˚¿Œ ∞¯∞›.
			SendPacket_Attack();
		} //if

		m_pPlayer->SetDirection( vDir );

		m_bAttack		= TRUE;
		m_bToTarget		= FALSE;
		m_bAdvanceMove	= FALSE;
		m_bMoveAttack	= FALSE;
		m_dwAttackLimit	= 0;
		m_bSendAttackPacket = TRUE;
	} //if..else..
} //CGame::AttackMonster

/**
 * ∞¯∞›¡ﬂ¿Œ ≈©∏Æ√ƒ∏¶ æÚ¥¬¥Ÿ.
 */
IW3DCreature* CGame::GetTargetCreature()
{
	IW3DCreature*		pTarget = NULL;
	if( g_sTarget.btType == MONSTER )
	{
		pTarget = m_pWorld->GetMonster( g_sTarget.dwId );
	}
	else if( g_sTarget.btType == CHARACTER )
	{
		pTarget = m_pWorld->GetCharacter( g_sTarget.dwId );
	} //if..else

	return pTarget;
} //

/**
 * «√∑π¿ÃæÓ ¿Ãµø ∆–≈∂¿ª ∫∏≥Ω¥Ÿ.
 */
void CGame::SendPacket_CharacterMove()
{
	int		nX = (int)m_pPlayer->GetPosition().x;
	int		nY = (int)m_pPlayer->GetPosition().y;
	BYTE	btLayer = m_pPlayer->GetLayer();

	Assert( 0 <= nX && nX < 500 );
	Assert( 0 <= nY && nY < 500 );

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_CHARACTER_MOVE )
		<< nX
		<< nY
		<< btLayer;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::SendPacket_CharacterMove

/**
 * ∞¯∞› ∆–≈∂ ∫∏≥ª±‚.
 * g_sTarget∏¶ ∞¯∞›«—¥Ÿ.
 */
void SendPacket_Attack()
{
	WORD		wPacketAttack;
	switch( g_sTarget.btType )
	{
	case MONSTER:		wPacketAttack = dPACKET_ATTACK_USERMOB;			break;
	case CHARACTER:		wPacketAttack = dPACKET_ATTACK_PLAYER;			break;
	default:			return;
	} //switch

	CPacket		sendPacket;
	sendPacket.Tag( wPacketAttack )
		<< g_sTarget.dwId;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );

	DEBUG_OUT( "*-=-=-=-=* Attack start packet sent *-=-=-=-=*" );
} //if

/**
 * ∏ ¿ª ∫“∑Øø¬¥Ÿ.
 *
 * @param	dwMap		[IN] ∏  π¯»£
 *
 * @return	∏  ∑Œµ˘ º∫∞¯ ø©∫Œ
 */
BOOL CGame::LoadMap( const DWORD dwMap )
{
	g_pMediaPlayer->StopAll();
	g_pMediaPlayer->Clear();

	ReleaseMonsterModel();

	m_pWorld->SetMapQuality( g_nMapQuality );

	g_pGUI->PlayerLive( TRUE );
	g_pGUI->ShowDlg( "DLG_DEATH", FALSE );
	if( dwMap == m_wMap && !m_bIndoor )
	{	// ¿ÃπÃ ¿–æÓµÈ¿Œ ∏ ¿Ã∂Û∏È ∞‘¿” ø¿∫Í¡ß∆Æ∏∏ ¡¶∞≈«—¥Ÿ.
		m_pCamera->SetFreeYaw( TRUE );
		m_pCamera->SetYawRange( 90.0F );
		m_pWorld->SetMapType( m_btMapType );
		m_pWorld->CleanupObjects();
		
		char	szName[256];
		sprintf( szName, "land%04d", dwMap );
		
		m_pWorld->LoadMap( szName, TRUE );
		g_pSelectMgr->Select( NULL );
		
		// ∏  ∑Œµ˘¿Ã øœ∑·µ«¥¬ º¯∞£∫Œ≈Õ Map BGM¿ª ¿Áª˝
		g_pMediaPlayer->StopAll();
		if( g_btWarMode )	PlayEventBGM( BGM_EVENT_CASTLEWAR );	// ∞¯º∫¿¸ ¿Ωæ« ¿Áª˝
		else				PlayMapBGM();							// Map BGM¿ª ¿Áª˝
		
		return TRUE;
	} //if

	SAFE_DELETE( g_pSelectMgr );

	m_pWorld->SetMapType( m_btMapType );

	char	szName[256];
	sprintf( szName, "land%04d", dwMap );
	
	if( !m_pWorld->LoadMap( szName ) )
	{
		Assert( "!Failed to load map" );
		ReleaseCreatureBoundBox();
		return FALSE;
	} //if

	g_pSelectMgr = new CSelectCreature;

	if( !g_pSelectMgr ||
		!g_pSelectMgr->Init( m_pWorld ) )
	{
		Assert( !"Failed to create optional maintainer" );
		return FALSE;
	} //if

	m_pWorld->LoadLight( "Light.txt" );
	
	m_pCamera->SetWorldObject( m_pWorld );

	SetMapInfo( dwMap );

	m_wMap = dwMap;		// ∞∞¿∫ ∏ ¿ª ∂« ∑Œµ˘«œ¥¬ ª¡˙¿ª ∏∑±‚ ¿ß«ÿ
						// «ˆ¿Á ∑Œµ˘«— ∏ ¿ª ±‚∑œ«—¥Ÿ.
	
	// ∏  ¿Ã∏ß √‚∑¬¡ÿ∫Ò.
	g_pGUI->LoadGameReady( dwMap );

	// ∏  ∑Œµ˘¿Ã øœ∑·µ«¥¬ º¯∞£∫Œ≈Õ Map BGM¿ª ¿Áª˝
	g_pMediaPlayer->StopAll();
	if( g_btWarMode )	PlayEventBGM( BGM_EVENT_CASTLEWAR );	// ∞¯º∫¿¸ ¿Ωæ« ¿Áª˝
	else				PlayMapBGM();							// Map BGM¿ª ¿Áª˝

	m_bPlaying = TRUE;	// Set Playing Mode

	if( m_btMapType == 1 ) // ¥¯¿¸
	{
		m_pWorld->LoadLight( "indoor.txt" );
		m_pWorld->GetTerrain()->ShowCloud( FALSE );
	} //if

	m_pCamera->SetFreeYaw( TRUE );
	m_pCamera->SetYawRange( 90.0F );

	g_pPickCharacter	= NULL;
	g_pPickMonster		= NULL;
	g_pPickNPC			= NULL;
	g_pPickItem			= NULL;
	g_pFuncEntity		= NULL;
	m_dwPickDoor		= 0;

	return TRUE;
} //CGame::LoadMap

/**
 * ∏ ¿ª ∫“∑Øø¬¥Ÿ.
 *
 * @param	dwMap		[IN] ∏  π¯»£
 *
 * @return	∏  ∑Œµ˘ º∫∞¯ ø©∫Œ
 */
BOOL CGame::LoadIndoor( const DWORD dwMap )
{
	if( dwMap == m_wMap && m_bIndoor )
	{	// ¿ÃπÃ ¿–æÓµÈ¿Œ ∏ ¿Ã∂Û∏È ∞‘¿” ø¿∫Í¡ß∆Æ∏∏ ¡¶∞≈«—¥Ÿ.
		m_pWorld->SetMapType( m_btMapType );
		m_pWorld->CleanupObjects();
		return TRUE;
	} //if	

	char	szName[256];

	if( m_btMapType == 2 )		// ¿œπ› ¡˝ ≥ª∫Œ
		sprintf( szName, "bi_%04d", dwMap );
	else if( m_btMapType == 3 )
		sprintf( szName, "ca_%04d", dwMap );
	else 
		return FALSE;
		m_pWorld->SetMapType( m_btMapType );

	if( !m_pWorld->LoadIndoor( szName ) )
	{
		Assert( !"∏  ∑ŒµÂ Ω«∆–" );
		return FALSE;
	} //if

	m_pWorld->LoadLight( "indoor.txt" );

	g_pGUI->LoadMiniMap( 0 );
/*	SetMapInfo( wMap );

	g_pGUI->LoadMiniMap( wMap );

	Vector3			vNPC;
	WORD			wNPCCount = 0;
	IW3DCreature*	pNPC = m_pWorld->GetNPC( wNPCCount );
	while( pNPC )
	{
		if( pNPC->GetShopNum() > 0 )
		{ // ªÛ¡°¿ª ∞°¡¯ ∞ÊøÏ.
			vNPC = pNPC->GetPosition();
			g_pGUI->AddShopMapPos( vNPC.x, vNPC.y );
			DEBUG_OUT( pNPC->GetName() + "NPC ªÛ¡°√ﬂ∞°" );
		}//if

		wNPCCount++;
		pNPC = m_pWorld->GetNPC( wNPCCount );
	} //while
	*/

//	m_wMap = dwMap;		// ∞∞¿∫ ∏ ¿ª ∂« ∑Œµ˘«œ¥¬ ª¡˙¿ª ∏∑±‚ ¿ß«ÿ
						// «ˆ¿Á ∑Œµ˘«— ∏ ¿ª ±‚∑œ«—¥Ÿ.
	
	// ∏  ¿Ã∏ß √‚∑¬¡ÿ∫Ò.
//	g_pGUI->LoadGameReady( dwMap );

	m_bPlaying = TRUE;	// Set Playing Mode

	return TRUE;
} //CGame::LoadIndoor

/**
 * «√∑π¿ÃæÓ∏¶ ª˝º∫«—¥Ÿ.
 */
void CGame::CreatePlayer( const int nX, const int nY, const BYTE btLayer )
{
	SW3DCharacterInfo	infoCharacter;

	strcpy( infoCharacter.szName, g_infoPlayer.sName.c_str() );

	infoCharacter.nX			= nX;
	infoCharacter.nY			= nY;
	infoCharacter.btLayer		= btLayer;

	infoCharacter.bSex			= g_infoPlayer.bSex;
	infoCharacter.btOffset		= g_infoPlayer.btSpeed;	///< ƒ≥∏Ø≈Õ º”µµ	

	if( _MASTER != infoCharacter.bSex )
	{// ¿œπ› ƒ≥∏Ø≈Õ
		infoCharacter.wHair			= g_infoPlayer.wHair;
		infoCharacter.wFace			= g_infoPlayer.wFace;		

		infoCharacter.wUpper		= g_pGUI->GetOnItemType( EQUIP_UPPERARMOR );
		infoCharacter.wLower		= g_pGUI->GetOnItemType( EQUIP_LOWERARMOR );
		infoCharacter.wHand			= g_pGUI->GetOnItemType( EQUIP_GLOVE );
		infoCharacter.wFoot			= g_pGUI->GetOnItemType( EQUIP_SHOES );
		infoCharacter.wHelmet		= g_pGUI->GetOnItemType( EQUIP_HELMET );
		infoCharacter.wShield		= g_pGUI->GetOnItemType( EQUIP_SHIELD );
		infoCharacter.wLongWeapon	= g_pGUI->GetOnItemType( EQUIP_LONGWEAPON );
		infoCharacter.wShortWeapon	= g_pGUI->GetOnItemType( EQUIP_SHORTWEAPON );

		infoCharacter.wRide			= g_infoPlayer.wRide;
		infoCharacter.nGuildId		= g_infoPlayer.nGuildId;
	}	
	else
	{// øÓøµ¿⁄ ƒ≥∏Ø≈Õ	
		infoCharacter.wHair			= 10373;
		infoCharacter.wFace			= 10372;

 		infoCharacter.wUpper		= 10375;
		infoCharacter.wLower		= 10376;
		infoCharacter.wHand			= 10374;
		infoCharacter.wFoot			= 10377;
		infoCharacter.wHelmet		= 0;
		infoCharacter.wShield		= 0;
		infoCharacter.wLongWeapon	= 0;
		infoCharacter.wShortWeapon	= 0;
		
		infoCharacter.wRide			= 0;
		infoCharacter.nGuildId		= 0;
	}	

	m_pWorld->AddCharacter( m_dwPlayer, reinterpret_cast<void*>( &infoCharacter ) );
	m_pPlayer = m_pWorld->GetCharacter( m_dwPlayer );
	g_pPlayer = m_pPlayer;

	static Vector3		vPos;
	vPos.x = (float)nX + 0.5F;
	vPos.y = (float)nY + 0.5F;
	vPos.z = m_pWorld->GetHeight( vPos );

	m_pWorld->AddEffect( "selfloading.wed", vPos );

	g_pGUI->PlayerLive( TRUE );
	g_pGUI->SetPlayerMapPos( nX, nY );
	g_pGUI->SetCharacterName( g_infoPlayer.sName.c_str() );
	g_pGUI->SetJobName( g_infoPlayer.btClass );

	g_pGUI->SetPosHP( g_infoPlayer.nHp[1], g_infoPlayer.nHp[0] );
	g_pGUI->SetPosMP( g_infoPlayer.nMp[1], g_infoPlayer.nMp[0] );

	g_pGUI->SetPosSkill( 0, g_infoPlayer.nExp[0][1], g_infoPlayer.nExp[0][0], FALSE );	///< ±Ÿ∞≈∏Æ
	g_pGUI->SetPosSkill( 1, g_infoPlayer.nExp[1][1], g_infoPlayer.nExp[1][0], FALSE );	///< ø¯∞≈∏Æ
	g_pGUI->SetPosSkill( 2, g_infoPlayer.nExp[2][1], g_infoPlayer.nExp[2][0], FALSE );	///< πÈ∏∂π˝
	g_pGUI->SetPosSkill( 3, g_infoPlayer.nExp[3][1], g_infoPlayer.nExp[3][0], FALSE );	///< »Ê∏∂π˝

	g_pGUI->SetPlayerPower( 0, g_infoPlayer.wStr );
	g_pGUI->SetPlayerPower( 1, g_infoPlayer.wInt );
	g_pGUI->SetPlayerPower( 2, g_infoPlayer.wDex );
	g_pGUI->SetText( "TXT_CINFO_CHA", g_infoPlayer.nCha );

	g_pGUI->SetText( "TXT_CINFO_SK1", g_infoPlayer.wLevel[0] );						///< ±Ÿ∞≈∏Æ ∑π∫ß
	g_pGUI->SetText( "TXT_CINFO_SK2", g_infoPlayer.wLevel[1] );						///< ø¯∞≈∏Æ ∑π∫ß
	g_pGUI->SetText( "TXT_CINFO_SK3", g_infoPlayer.wLevel[2] );						///< πÈ∏∂π˝ ∑π∫ß
	g_pGUI->SetText( "TXT_CINFO_SK4", g_infoPlayer.wLevel[3] );						///< »Ê∏∂π˝ ∑π∫ß

	int		nTotal = g_infoPlayer.wLevel[0] + 
					 g_infoPlayer.wLevel[1] + 
					 g_infoPlayer.wLevel[2] + 
					 g_infoPlayer.wLevel[3];
	g_pGUI->SetText( "TXT_CINFO_TOTAL", nTotal );
	g_pGUI->SetText( "TXT_TOT_TOTLEVEL", nTotal );
	g_pGUI->SetProgressPos( "PRO_CINFO_TOTAL", 200, nTotal, FALSE ); // 400 total level
	g_pGUI->UpdateMoney( g_infoPlayer.nGold );
	g_pGUI->UpdateCash( g_infoPlayer.nCash );
	g_pGUI->ShowTargetInfo( 0 );

	m_pCamera->DefaultShot( m_pPlayer->GetPosition() );
	m_pCamera->FullShot( m_pPlayer->GetPosition() );
	
	IW3DItemInfoMgr*	pItemInfoMgr = m_pWorld->GetItemInfoMgr();
	if( !pItemInfoMgr )
	{
		Assert( pItemInfoMgr );
		return;
	} //if
	pItemInfoMgr->SetSex( g_infoPlayer.bSex );
} //CGame::CreatePlayer

/**
 * UI ≈∞ ¥≠∏≤.
 */
void CGame::OnEventKeyDown( DWORD dwParam1 )
{
	switch( dwParam1 )
	{
	case DIK_LMENU:			// 'Left Alt'
		m_pWorld->ShowItemName( TRUE );	
		
		/*
			TODO : ƒ≥∏Ø≈Õ ø•∫Ì∑Ω ∫∏¿Ã±‚
		*/
		g_bShowEmblem = TRUE;
		break;
	} //switch
} //CGame::OnEventKeyDown

/**
 * UI ≈∞ æ˜.
 */
void CGame::OnEventKeyUp( DWORD dwParam1 )
{
	switch( dwParam1 )
	{
	case DIK_LMENU:			// 'Left Alt'
		m_pWorld->ShowItemName( FALSE );
		g_bShowEmblem = FALSE;
		break;
	} //switch
} //CGame::OnEventKeyDown

/**
 * UI πˆ∆∞ ¥≠∏≤.
 *
 * @param	sName		[IN] πﬂª˝ ƒƒ∆˜≥Õ∆Æ ¿Ã∏ß.
 */
void CGame::OnEventButton( const char* szUIName, DWORD dwParam2 )
{
	Assert( g_pGUI );

	if( dwParam2 == 1 )			return;

	if( !strcmp( szUIName, "BTN_EXIT_RESTART" ) )
	{	// ∞‘¿”«¡∑π¿” ¡¡∑·.
		m_bExitFrame = TRUE;
		//∑Œ±◊æ∆øÙ ∏Æƒ˘Ω∫∆Æ
		CPacket sendPacket;
		sendPacket.Tag( dPACKET_FRIEND_LOGOUT_REQUEST );
		g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
	}
	else if( !strcmp( szUIName, "BTN_EXIT_EXIT" ) )
	{	// ¡æ∑·«“±Óø‰? øπ.
		m_bQuit = TRUE;
		//∑Œ±◊æ∆øÙ ∏Æƒ˘Ω∫∆Æ
		CPacket sendPacket;
		sendPacket.Tag( dPACKET_FRIEND_LOGOUT_REQUEST );
		g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
	}
	else if( !strcmp( szUIName, "BTN_MSG_DISCONNECT_OK" ) )
	{ // º≠πˆøÕ ¡¢º”¿Ã ≤˜∞Â¿Ω. »Æ¿Œ.
		m_bQuit = TRUE;
	} //if.. else if..
	else if( !strcmp( szUIName, "BTN_MSG_PARTYINVIT_YES" ) )
	{ // ∆ƒ∆º √ ¥Îø° ºˆ∂Ù
		OnEventPartyRequest( m_dwRequestId );
	}
	else if( !strcmp( szUIName, "BTN_MSG_FRIENDINVIT_YES" ) )
	{ // ƒ£±∏ µÓ∑œ ºˆ∂Ù
		OnEventFriendRequest( m_dwRequestId );
	}
	else if( !strcmp( szUIName, "BTN_MSG_FRIENDINVIT_NO" ) )
	{
		OnEventFriendRequestNo( m_dwRequestId )	;
	}
	else if( !strcmp( szUIName, "BTN_MSG_PARTYBANISH_YES" ) )
	{
		IW3DCreature*		pCharacter = g_pSelectMgr->GetCreature();
		if( !pCharacter )	return;

		CPacket		sendPacket;
		sendPacket.Tag( dPACKET_PARTY_REMOVE_MEMBER )
				<< pCharacter->GetId();
		g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
	}
	else if( !strcmp( szUIName, "BTN_MSG_DEALREQUEST_YES" ) )
	{
		CPacket		sendPacket;
		sendPacket.Tag( dPACKET_DEAL_OK );
		g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );		
	}
	else if( !strcmp( szUIName, "BTN_MSG_DEALREQUEST_NO" ) )
	{ // ∞≈∑°ø‰√ª ∞≈¿˝.
		CPacket		sendPacket;
		sendPacket.Tag( dPACKET_DEAL_CANCEL );
		g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
	}
	else if( !strcmp( szUIName, "BTN_MSG_USERDEAL_FINISH_OK" ) )
	{ // ¿Ø¿˙∞≈∑°√¢ ¥›Ω¿¥œ¥Ÿ.
		g_pGUI->ShowUserDealBox( FALSE );
	}
	else if( !strcmp( szUIName, "BTN_MSG_USERDEAL_REQUESTSUBMIT_YES" ) )
	{
		CPacket		sendPacket;
		sendPacket.Tag( dPACKET_DEAL_SUBMIT_OK );
		g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
	}
	else if( !strcmp( szUIName, "BTN_MSG_USERDEAL_REQUESTSUBMIT_NO" ) )
	{
		CPacket		sendPacket;
		sendPacket.Tag( dPACKET_DEAL_CANCEL );
		g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
	}
	else if( !strcmp( szUIName, "BTN_WP_INTEGRATION_NO" ) || !strcmp( szUIName, "BTN_AM_INTEGRATION_NO" ) ||
			 !strcmp( szUIName, "BTN_WP_INTEGRATION_X" ) || !strcmp( szUIName, "BTN_AM_INTEGRATION_X" ) )
	{
		g_pGUI->ShowSmithEnchentBox( FALSE );
		CPacket		sendPacket;
		sendPacket.Tag( dPACKET_SMITH_CANCEL );
		g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
	}
	
} //CGame::OnEventButton

/**
 * UI ø£≈Õ ¥≠∏≤.
 *
 * @param	sName		[IN] πﬂª˝ ƒƒ∆˜≥Õ∆Æ ¿Ã∏ß.
 */
void CGame::OnEventReturnKey( const char* szUIName )
{
} //CGame::OnEventReturnKey

/**
 * ¥Ÿ¿ÃæÛ∑Œ±◊π⁄Ω∫ ∫∏¿Ã±‚ º≥¡§
 */
void CGame::OnEventShowDlg( const char* szUIName, DWORD dwParam2 )
{
	if( !strcmp( szUIName, "DLG_YOUANDME" ) )
	{
		if( !dwParam2 )
		{
			if( m_nZoomMode == ZOOM_LOCK )
			{
				m_nZoomMode = ZOOM_SHORT;
				if( !m_pPlayer )
				{
					Assert( "!I don't have" );
					return;
				} //if
				g_pGUI->ShowShop( FALSE );
				g_pGUI->ShowStorage( FALSE );
				m_pCamera->CloseShot( m_pPlayer->GetPosition(), m_pPlayer->GetDirection(), FALSE );
			} //if
		} //if
	} //if
} //CGame::OnEventShowDlg

/**
 * æ∆¿Ã≈€¿ª ¿Œ∫•≈‰∏Æø° ¿˙¿Â«—¥Ÿ.
 */
void CGame::OnEventInsertItemInInventory( DWORD dwParam1, DWORD dwParam2 )
{
	int		nId		= dwParam1 >> 16;
	BYTE	bPage	= dwParam1;
	BYTE	btX		= dwParam2 >> 16;
	BYTE	btY		= dwParam2;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_SET_INVENXY )
		<< nId			
		<< bPage
		<< btX
		<< btY;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventInsertItemInInventory

/**
 * æ∆¿Ã≈€¿ª º’¿∏∑Œ ¡˝¥¬¥Ÿ.
 */
void CGame::OnEventHolding( DWORD dwParam1, DWORD dwParam2 )
{
	int		nId		= dwParam1 >> 16;
	BYTE	btPos	= dwParam2;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_HOLDING_ITEM )
		<< nId
		<< btPos;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventHolding

/**
 * º’ø° ¿÷¥¬ æ∆¿Ã≈€¿ª πŸ≤€¥Ÿ.
 */
void CGame::OnEventChangeHand( DWORD dwParam1, DWORD dwParam2 )
{
	int		nId		= dwParam1 >> 16;
	BYTE	btFrom	= dwParam1;
	BYTE	btX		= dwParam2 >> 16;
	BYTE	btY		= dwParam2;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_CHANGE_HAND_ITEM )
		<< nId
		<< btFrom
		<< btX
		<< btY;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventChangeHand

/**
 * º’ø° ¡Â æ∆¿Ã≈€¿ª ¿Œ∫•≈‰∏Æ ªÛ¿« ø¯«œ¥¬ ∞˜¿∏∑Œ ¿ÃµøΩ√≈≤¥Ÿ.
 */
void CGame::OnEventItemMove( DWORD dwParam1, DWORD dwParam2 )
{
	int		nId		= dwParam1;
	BYTE	btPage	= dwParam2 >> 16;
	BYTE	btCol	= dwParam2 >> 8;
	BYTE	btRow	= dwParam2;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_MOVE_ITEM )
		<< nId			
		<< btPage
		<< btCol
		<< btRow;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventItemMove

/**
 * º’ø° µÁ æ∆¿Ã≈€¿ª πˆ∏∞¥Ÿ.
 */
void CGame::OnEventItemDrop( DWORD dwParam1, DWORD dwParam2 )
{
	if( m_bNoMove )
	{
		return;
	} //if
	int		nId		= dwParam1;
	BYTE	btTo	= dwParam2;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_DROP_ITEM )
		<< nId
		<< btTo;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventItemDrop

/**
 * º’ø° µÁ æ∆¿Ã≈€¿ª ¡ˆ¡§µ» ¿ßƒ°ø° ¿Â¬¯«—¥Ÿ.
 *
 * @param	dwParam2	[IN] ¿Â¬¯«“ ¿ßƒ°
 */
void CGame::OnEventInsertEquip( DWORD dwParam2 )
{
	BYTE	btPos = dwParam2;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_EQUIP_ITEM )
		<< btPos;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventInsertEquip

/**
 * ¿Â¬¯ ¡ﬂ¿Œ æ∆¿Ã≈€¿ª «ÿ¡¶«—¥Ÿ.
 */
void CGame::OnEventRemoveEquip( DWORD dwParam2 )
{
	BYTE	btPos = dwParam2;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_CHANGE_HAND_ITEM )
		<< btPos;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventRemoveEquip

/**
 * æ∆¿Ã≈€¿ª « µÂø° πˆ∏∞¥Ÿ.
 */
void CGame::OnEventItemToMap( DWORD dwParam1, DWORD dwParam2 )
{
	int		nId		= dwParam1 >> 16;
	WORD	wType	= dwParam1;
	WORD	wPosX	= dwParam2 >> 16;
	WORD	wPosY	= dwParam2;

	m_pWorld->AddItem( nId, wType, wPosX, wPosY );
} //CGame::OnEventItemToMap

/**
 * ∏∂π˝ Ω∫≈©∑—¿ª ø≈±‰¥Ÿ.
 */
void CGame::OnEventSpellMove( DWORD dwParam1, DWORD dwParam2 )
{
	BYTE		btPage = dwParam1;
	BYTE		btLine = dwParam2;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_MOVETO_SPELLBOOK )
		<< btPage
		<< btLine;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventSpellMove

/**
 * ªÁøÎ«“ Ω∫≈≥¿ª ∫Ø∞Ê«—¥Ÿ.
 */
void CGame::OnEventSetMainSkill( DWORD dwParam1, DWORD dwParam2 )
{
	BYTE	btSkill = dwParam2;		// ªÁøÎ«“ Ω∫≈≥

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_SET_MAIN_SKILL )
		<< btSkill;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventSetMainSkill

/**
 * æ∆¿Ã≈€ ±∏¿‘ ¿Ã∫•∆Æ∏¶ √≥∏Æ«—¥Ÿ.
 */
void CGame::OnEventBuyItem( DWORD dwParam1, DWORD dwParam2 )
{
	int			nItemType	= dwParam1 >> 16;		// ±∏¿‘«“ æ∆¿Ã≈€
	WORD		wShop		= dwParam1 & 0xFF;		// SHOP
	WORD		btCnt		= dwParam2;				// ºˆ∑Æ(∆˜º«øÎ) from sk8snow

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_BUY_ITEM )
		<< wShop
		<< nItemType
		<< btCnt;
	
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventBuyItem

/**
 * æ∆¿Ã≈€ ∆«∏≈ ¿Ã∫•∆Æ∏¶ √≥∏Æ«—¥Ÿ.
 */
void CGame::OnEventSellItem( DWORD dwParam1, DWORD dwParam2 )
{
	int			nItemType	= dwParam1 >> 16;		// ∆«∏≈«“ æ∆¿Ã≈€ ¡æ∑˘
	WORD		wShop		= dwParam1 & 0xFF;		// SHOP
	BYTE		btFrom		= dwParam2;				// FROM

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_SELL_ITEM )
		<< wShop
		<< nItemType
		<< btFrom;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventBuyItem

/**
 * æ∆¿Ã≈€ ªÁøÎ ¿Ã∫•∆Æ∏¶ √≥∏Æ«—¥Ÿ.
 */
void CGame::OnEventCastItem( DWORD dwParam1 )
{
	int			nId = dwParam1;		// ªÁøÎ«“ æ∆¿Ã≈€¿« ID

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_CAST_ITEM )
		<< nId;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
	DEBUG_OUT( "Use a one-time item" + nId );
} //CGame::OnEventCastItem

/**
 * √¬ ∫∏≥ª±‚
 */
void CGame::OnEventSendChatting( DWORD dwParam2 )
{
	WORD		wPacket = 0;
	switch( dwParam2 )
	{
	case IW3DUInterface::CHATT_BROADCAST:	wPacket = dPACKET_BROADCAST_CHAT;		break;
	case IW3DUInterface::CHATT_SELL:		wPacket = dPACKET_SELLING_CHAT;			break;
	case IW3DUInterface::CHATT_COUNTRY:		wPacket = 0;							break;
	case IW3DUInterface::CHATT_PARTY:		wPacket = dPACKET_PARTY_CHAT;			break;
	case IW3DUInterface::CHATT_GUILD:		wPacket = dPACKET_GUILD_CHAT;			break;
	case IW3DUInterface::CHATT_MEMBER:		wPacket = dPACKET_MEMBER_CHAT;			break;
	case IW3DUInterface::CHATT_WORLD:		wPacket = dPACKET_WORLD_CHAT;			break;
	case IW3DUInterface::CHATT_FRIEND:		wPacket = dPACKET_FRIEND_MEMO;			break;
	default:	wPacket = 0;														break;
	} //switch

	if( m_pPlayer )
	{
		char*	pId		= g_pGUI->GetWhisperName();
		char*	pText	= g_pGUI->GetChattingText();

		if( strlen( pText ) > 0 )
		{
			if( strlen( pId ) > 0 )
			{	
				if( wPacket == dPACKET_FRIEND_MEMO )
				{
					CPacket		sendPacket;
					sendPacket.Tag( dPACKET_FRIEND_MEMO )
						<< pId
						<< pText;
					g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
				}
				else
				{ // ±”∏ª ªÛ¥Î∞° ¡ˆ¡§µ» ∞ÊøÏ ±”∏ª ∆–≈∂¿ª ≥Ø∏∞¥Ÿ.
					CPacket		sendPacket;
					sendPacket.Tag( dPACKET_CHAT_WHISPER )
						<< pId
						<< pText;
					g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
				} //if..else
			}
			else if( wPacket == dPACKET_PARTY_CHAT ||
					 wPacket == dPACKET_GUILD_CHAT ||
					 wPacket == dPACKET_WORLD_CHAT ||
					 wPacket == dPACKET_SELLING_CHAT
					 )
			{	// ¿œπ› ¥Î»≠.
				if( !CheatKey( pText ) )
				{
					CPacket		sendPacket;
					sendPacket.Tag( wPacket )
						<< pText;
					g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
				} //if
			}
			else
			{
				if( !CheatKey( pText ) )
				{
					CPacket		sendPacket;
					sendPacket.Tag( dPACKET_CHAT_MSG )
						<< pText;
					g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
				} //if
			} //if
		} //if
	} //if
} //CGame::OnEventSendChatting

/**
 * ∞≈∑°√¢ø° µ∑¿ª ø√∏∞¥Ÿ.
 */
void CGame::OnEventDealAddGold( DWORD dwParam1 )
{
	int		nMoney = dwParam1;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_DEAL_ADDMONEY )
		<< nMoney;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventDealAddGold

/**
 * ∞≈∑°√¢ø° æ∆¿Ã≈€ √ﬂ∞°
 */
void CGame::OnEventDealAddItem( DWORD dwParam1, DWORD dwParam2 )
{
	int			nId   = dwParam1;		// ªÁøÎ«“ æ∆¿Ã≈€¿« ID
	BYTE		btCol = dwParam2 >> 16;
	BYTE		btRow = dwParam2 & 0xff;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_DEAL_ADDITEM )
		<< nId
		<< btCol
		<< btRow;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventDealAddItem

/**
 * ∞≈∑°√¢ø°º≠ æ∆¿Ã≈€ ª©±‚.
 */
void CGame::OnEventDealDeleteItem( DWORD dwParam1, DWORD dwParam2 )
{
	int			nId   = dwParam1;		// ªÁøÎ«“ æ∆¿Ã≈€¿« ID

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_DEAL_DELITEM )
		<< nId;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventDealAddItem



/**
 * ¿Ø¿˙∞≈∑°√Îº“.
 */
void CGame::OnEventDealCancel()
{
	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_DEAL_CANCEL );
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventDealCancel

/**
 * ¿Ø¿˙∞≈∑° √º≈©¿Œ.
 */
void CGame::OnEventDealCheckIn()
{
	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_DEAL_CHECKIN );
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventDealCheckIn

/**
 * ∆ƒ∆ºª˝º∫.
 */
void CGame::OnEventPartyCreate()
{
	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_PARTY_CREATE );
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );	
} //CGame::OnEventPartyCreate

/**
 * ∆ƒ∆º¡æ∑·.
 */
void CGame::OnEventPartyDestroy()
{
	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_PARTY_DESTROY );
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventPartyDestroy

/**
 * ¿Ø¿˙∞≈∑° Ω≈√ª.
 */
void CGame::OnEventDealRequest()
{
	IW3DCreature*		pSelectTarget = g_pSelectMgr->GetCreature();
	if( !pSelectTarget || g_pSelectMgr->GetType() != CHARACTER )
	{
		return;
	} //if

	int		nId = pSelectTarget->GetId();

	if( pSelectTarget->IsOpenShop() )
	{
		if( g_pGUI->IsShowDlg( "DLG_CREATEMYSHOP" ) || 
			g_pGUI->IsShowDlg( "DLG_PERSONALSHOP" ) )	
		{
			g_pGUI->ChattingInput( "", STR_UIMSG_ALREADYPSHOP, 0xFFFF9C00, 0, 0 );
			return;
		} //if

		char*		szShopTitle = pSelectTarget->GetShopInfo();
		if( !szShopTitle )
		{
			Assert( !"Do Not The PersonalShop" );
			return;
		} //if
		g_pGUI->SetText( "TXT_PSHOP_TITLE", szShopTitle );
		m_dwPShopId = nId;

		CPacket		sendPacket;
		sendPacket.Tag( dPACKET_MYSHOP_GETITEMLIST )
				<< nId;
		g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
	}
	else
	{
		if( g_pGUI->IsShowDlg( "DLG_DEAL" ) )
		{
			return;
		} //if

		CPacket		sendPacket;
		sendPacket.Tag( dPACKET_DEAL_REQUEST )
				<< nId;
		g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
	} //if..else
} //CGame::OnEventDealRequest

/** 
 * ∆ƒ∆ºø¯ Ω≈√ª.
 */
void CGame::OnEventPartyRequest( DWORD dwParam1 )
{
	int		nId = dwParam1;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_PARTY_JOIN )
			<< nId;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventPartyRequest

/**
 * ∆ƒ∆º √ ¥Î
 */
void CGame::OnEventPartyInvit( DWORD dwParam1 )
{
	IW3DCreature*		pSelectTarget;

	if( dwParam1 == 0 )
	{
		pSelectTarget = g_pSelectMgr->GetCreature();
		if( !pSelectTarget || g_pSelectMgr->GetType() != CHARACTER )
		{
			return;
		} //if	
	}
	else
	{
		pSelectTarget = m_pWorld->GetCharacter( dwParam1 );
		if( !pSelectTarget )
		{
			Assert( !"Not Found the Character" );
			return;
		} //if
	} //if..else

	DWORD		dwId = pSelectTarget->GetId();

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_PARTY_ASKINVITE )
			<< dwId;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventPartyInvit

/**
 * ∆ƒ∆ºø°º≠ ≥™∞£¥Ÿ.
 */
void CGame::OnEventPartyOut()
{
	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_PARTY_OUT );
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventPartyOut

/**
 * ∆ƒ∆º √ﬂπÊ
 */
void CGame::OnEventPartyBanish()
{
	IW3DCreature*		pSelectTarget;

	pSelectTarget = g_pSelectMgr->GetCreature();
	if( !pSelectTarget || g_pSelectMgr->GetType() != CHARACTER )
	{
		return;
	} //if	
	DWORD		dwId = pSelectTarget->GetId();

	g_pGUI->MessageBox( STR_PARTY_BANISH, "PARTYBANISH", IW3DUInterface::MSG_YESNO );
} //CGame::OnEventPartyBanish

/** 
 * ∆ƒ∆º ∫∏µÂ.
 */
void CGame::OnEventPartyBoard()
{
	WORD		wPage = 0;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_GET_PARTYBOARD )
			<< wPage;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventPartyBoard

/**
 * ∆ƒ∆º∞‘Ω√∆« µÓ∑œ.
 */
void CGame::OnEventPartyBoardRegist()
{
	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_ADD_PARTYBOARD );
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventPartyBoardRegist

/**
 * ∆ƒ∆º »˙.
 */
void CGame::OnEventPartyHeal( DWORD dwParam1 )
{
	IW3DCreature*		pPartyPlayer = m_pWorld->GetCharacter( dwParam1 );
	if( !m_pPlayer || !pPartyPlayer )
	{
		return;
	} //if

	m_pPlayer->Attack( pPartyPlayer );	
} //CGame::OnEventPartyHeal

/** 
 * NPC¥Î»≠ Ω≈√ª.
 */
void CGame::OnEventTalkRequest( DWORD dwParam1, DWORD dwParam2 )
{
	g_pGUI->SelectNPC( dwParam1, dwParam2 );
} //CGame::OnEventTalkRequest

/** 
 * ªÛ¡° Ω≈√ª.
 */
void CGame::OnEventShopRequest( DWORD dwParam1, DWORD dwParam2 )
{
	WORD	wShop = dwParam2;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_GET_SHOPITEM )
		<< wShop;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventShopRequest

/*------------------------------------------------------------------
 ¥Î¿Â¿Â¿Ã ø‰√ª.
------------------------------------------------------------------*/
void CGame::OnEventSmithRequest( DWORD dwParam1, DWORD dwParam2 )
{
	g_pGUI->SelectSmithNPC( dwParam1, dwParam2 );
}

void CGame::OnEventCardSum( DWORD dwParam1, DWORD dwParam2 )
{
//	g_pGUI->SelectCardSumNPC( dwParam1, dwParam2 );
}

void CGame::OnEventCardUndoNPC( DWORD dwParam1, DWORD dwParam2 )
{
	g_pGUI->SelectCardUndoNPC( dwParam1, dwParam2 );
}

void CGame::OnEventCardUndo( DWORD dwParam1 )
{
	int		nItemId = dwParam1;
	CPacket		sendPacket;

	sendPacket.Tag( dPACKET_CARDUNDO ) 
		<< nItemId;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
}

void CGame::OnEvnetServer( DWORD dwParam1 )
{
	g_pGUI->SelectServer( dwParam1 );
}


/************************************************************************/
/*	¥Î¿Â¿Â¿Ã √¢ø° æ∆¿Ã≈€ ¿˙¿Â                                           */
/************************************************************************/
void CGame::OnEventSmithAddItem( DWORD dwParam1 )
{
/*	int			nItemId = dwParam1;

	CPacket		sendPacket;

	sendPacket.Tag( dPACKET_SMITH_ADDITEM )
		<< nItemId;
		
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );*/
}

/**
 * ƒ˘Ω∫∆Æ ø‰√ª.
 */
void CGame::OnEventQuestRequest( DWORD dwParam1, DWORD dwParam2 )
{
	WORD	wNPCId = dwParam1;
	WORD	wQuest = dwParam2;
	BYTE	btState;

	MAP_QUESTSTAE::iterator		iter = g_mapQuestState.find( wQuest );
	if( iter != g_mapQuestState.end() )
	{ // ƒ˘Ω∫∆Æ ¡¯«‡ ¡ﬂ, øœ∑·
		btState = iter->second;
		g_pGUI->Quest( wNPCId, wQuest, btState );
		return;
	} //if
	g_pGUI->Quest( wNPCId, wQuest, 0 );
} //CGame::OnEventQuestRequest

/**
 * √¢∞Ì ø‰√ª.
 */
void CGame::OnEventStorageRequest()
{
	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_OPEN_GARBAGE );

	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventStorageRequest

void CGame::OnEventSerIntGarbageRequiest( DWORD dwParam1 )
{
/*	int		nServerNum = dwParam1;
	
	CPacket		sendPacket;
	
	sendPacket.Tag( dPACKET_SERINT_OPENGARBAGE )
		<< nServerNum;
	
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );*/
}

void CGame::OnEventSerIntGarbageInit()
{
/*	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_SERINT_GARBAGE_INIT );

	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );*/
}

void CGame::OnEventSerIntInvenRequiest( DWORD dwParam1, DWORD dwParam2 )
{
/*	int nServerNum = dwParam1;
	int	nCharPos = dwParam2;

	CPacket	sendPacket;

	sendPacket.Tag( dPACKET_SERINT_OPENINVENTORY )
		<< nServerNum
		<< nCharPos;
	
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );*/
}


void CGame::OnEventSerIntInvenInit()
{
/*	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_SERINT_INVEN_INIT );

	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );*/
}

/**
 * √¢∞Ìø° æ∆¿Ã≈€ ¿˙¿Â.
 */
void CGame::OnEventStorageAddItem( DWORD dwParam1 )
{
	DWORD		dwId = dwParam1;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_INPUT_GARBAGE_ITEM )
		<< dwId;

	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventStorageAddItem

/**
 * √¢∞Ìø°º≠ æ∆¿Ã≈€ ≤®≥ª±‚.
 */
void CGame::OnEventStorageRemoveItem( DWORD dwParam1 )
{
	DWORD		dwId = dwParam1;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_OUTPUT_GARBAGE_ITEM )
		<< dwId;

	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventStorageRemoveItem

void CGame::OnEventSerIntGarbageRemove( DWORD dwParam1, DWORD dwParam2 )
{
/*	DWORD	dwId = dwParam1;
	int		SerNum = dwParam2;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_SERINT_OUTPUT_GARBAGE )
		<< dwId
		<< SerNum;
	
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );*/
}

void CGame::OnEventSerIntInvenRemove( DWORD dwParma1, DWORD dwParam2 )
{
/*	DWORD dwId = dwParma1;
	int	  ServerNum = dwParam2 >> 16;
	int	  CharPos = dwParam2 & 0xFF;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_SERINT_OUTPUT_INVEN )
		<< dwId
		<< ServerNum
		<< CharPos;

	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );*/
}

/**
 * √¢∞Ìø°º≠ Ω«µÂ ≤®≥ª±‚.
 */
void CGame::OnEventStorageGetGold( DWORD dwParam2 )
{
	DWORD		dwGold = dwParam2;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_WITHDRAW_MONEY )
		<< dwGold;

	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventStorageGetGold

/**
 * √¢∞Ìø° Ω«µÂ ¿˙¿Â.
 */
void CGame::OnEventStorageSaveGold( DWORD dwParam2 )
{
	DWORD		dwGold = dwParam2;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_RECEIPT_MONEY )
		<< dwGold;

	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventStorageSaveGold

/**
 * ƒ˘Ω∫∆Æ ºˆ∂Ù.
 */
void CGame::OnEventQuestAccept( DWORD dwParam1, DWORD dwParam2 )
{
	WORD	wNPCId = dwParam1;
	WORD	wQuest = dwParam2;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_QUEST_REQUEST )
		<< wNPCId
		<< wQuest;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );

	g_pGUI->AddQuestInList( wQuest, 1 );		///< '1' - ¡¯«‡.
	DEBUG_OUT( "Accept subcontract - #" + wQuest );
} //CGame::OnEventQuestAccept

/**
 * ªÁøÓµÂ ø…º« √¢.
 */
void CGame::OnEventSoundOption()
{
	g_pGUI->SetSoundState( g_bBGM_On, g_fVolBGM * 1000, g_bEffSound_On, g_fVolEff * 1000 );
	//g_b51ChSound_On;
} //CGame::OnEventSoundOption

/**
 * πË∞Ê¿Ω ƒ—∞Ì ≤Ù∞Ì.
 */
void CGame::OnEventBGMOnOff( DWORD dwParam2 )
{
	if( dwParam2 )
	{
		g_bBGM_On = TRUE;

		if( !m_pPlayer ||	m_pPlayer->GetState() == STATE_DEAD )		return;

		Vector3		vPos	= m_pPlayer->GetPosition(); //Vector3( 0.0F, 0.0F, 0.0F );
		int			nX = (int)vPos.x;
		int			nY = (int)vPos.y;

		if( g_btWarMode )	
		{
			PlayEventBGM( BGM_EVENT_CASTLEWAR );	// ∞¯º∫¿¸ ¿Ωæ« ¿Áª˝
		}
		else
		{
			PlayMapBGM();							// Map BGM ¿Áª˝
			PlayAreaBGM( nX, nY );					// Area BGM ¿Áª˝
		} // if..else..		
	}
	else
	{
		g_bBGM_On = FALSE;
		g_pMediaPlayer->StopAll();	// ∏µÁ bgm stop
	} //if..else
} //CGame::OnBGMOnOff

/**
 * πË∞Ê¿Ω ƒ—∞Ì ≤Ù∞Ì.
 */
void CGame::OnEventEffOnOff( DWORD dwParam2 )
{
	if( dwParam2 )
	{
		g_bEffSound_On = TRUE;
	}
	else
	{
		g_bEffSound_On = FALSE;
	} //if..else
	g_pSoundMgr->SetOnOff( g_bEffSound_On );
} //CGame::OnEventEffOnOff

/**
 *  5.1√§≥Œ ƒ—∞Ì ≤Ù∞Ì.
 */
void CGame::OnEvent51ChOnOff( DWORD dwParam2 )
{
	if( dwParam2 )
	{
		g_b51ChSound_On = TRUE;		
	}
	else
	{
		g_b51ChSound_On = FALSE;
	} //if..else

	g_pSoundMgr->SetMultiSpeaker( g_b51ChSound_On );
} //CGame::OnEvent51ChOnOff

/**
 * »ø∞˙¿Ω ƒ—∞Ì ≤Ù∞Ì.
 */
void CGame::OnEventSoundVol( DWORD dwParam1, DWORD dwParam2 )
{
	if( dwParam1 == 0 )
	{ //πË∞Ê¿Ω.
		g_fVolBGM = dwParam2 * 0.001;
		g_pMediaPlayer->SetVolume( g_fVolBGM );
	} 
	else if( dwParam1 == 1 )
	{ //»ø∞˙¿Ω.
		g_fVolEff = dwParam2 * 0.001;
		g_pSoundMgr->SetVolume( g_fVolEff );
	} //if..else
} //CGame::OnEventSoundVol

/**
 * æ∆¿Ã≈€ ¥ı∫Ì ≈¨∏Ø.
 */
void CGame::OnEventDBClickItem( DWORD dwParam1, DWORD dwParam2 )
{ //æ∆¿Ã≈€ ¿Ãµø√≥∏Æ.
	if( m_bNoMove )
	{
		return;
	} //if
	
	int		nId		= dwParam1;
	BYTE	btTo	= dwParam2;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_ITEM_DBCLICK )
		<< nId
		<< btTo;

	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventDBClickItem

/**
 * ƒ¸ΩΩ∑‘ ªÁøÎ.
 */
void CGame::OnEventQuickSlot( DWORD dwParam1 )
{
	DWORD		dwType = dwParam1;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_USE_QUICKITEM )
		<< dwType;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventQuickSlot

/**
 * ƒ´∏ﬁ∂Û √≥∏Æ ¿Ã∫•∆Æ.
 */
void CGame::OnEventCamera( DWORD dwParam1 )
{
	switch( dwParam1 )
	{
	case IW3DUInterface::CAMERA_UNLOCK:
		if( m_nZoomMode == ZOOM_LOCK )
		{
			m_nZoomMode = ZOOM_SHORT;

			if( !m_pPlayer )
			{
				Assert( !"≥ª∞° æ¯¥Ÿ" );
				return;
			} //if
			m_pCamera->CloseShot( m_pPlayer->GetPosition(), m_pPlayer->GetDirection(), FALSE );
		} //if
		break;

	case IW3DUInterface::CAMERA_DEFAULT:
		if( m_nZoomMode != ZOOM_LOCK )
		{
			if( !m_pPlayer )
			{
				Assert( !"≥ª∞° æ¯¥Ÿ" );
				return;
			} //if
			m_pCamera->DefaultShot( m_pPlayer->GetPosition() );
		} //if
	} //switch
} //CGame::OnEventCamera

/**
 * «√∑π¿ÃæÓ ¡◊¿Ω√≥∏Æ.
 */
void CGame::OnEventPlayerDie( DWORD dwParam1 )
{
	if( dwParam1 == 2 )
	{
		g_pGUI->ShowTimer( "TIMER_GHOST", 10000 );
	} //if

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_RESTART )
		<< (BYTE)dwParam1;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventPlayerDie

/**
 * ≈∏¿”∏” ¿Ã∫•∆Æ.
 */
void CGame::OnEventTimer( const char* szUIName, DWORD dwParam1 )
{
	if( !strcmp( szUIName, "TIMER_GHOST" ) )
	{
		if( !m_pPlayer )
		{
			Assert( !"æ¯¥Á." );
			return;
		} //if

		if( !m_pPlayer->IsDead() )
		{
			return;
		} //if

		g_pGUI->PlayerLive( FALSE );
	} //if
} //CGame::OnEventTimer

/**
 * PickUp Item
 */
void CGame::OnEventAutoItemPickUp()
{
	if( !m_pPlayer )
	{
		Assert( !"Empty Player" );
		return;
	} //if

	IW3DItem*	pItem = m_pWorld->GetNearItem( m_pPlayer->GetPosition() );
	if( !pItem )
	{
		Log( "Not Found Near Item" );
		return;
	} //if

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_GET_ITEM )
		<< pItem->GetId();
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventAutoItemPickUp

/**
 * ¿Œ√æ∆Æ ¿Ã∫•∆Æ
 */
void CGame::OnEventEnchant( DWORD dwParam1, DWORD dwParam2 )
{
	Assert( dwParam1 > 0 && dwParam2 > 0 );

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_ENCHANT_ITEM )
		<< dwParam1
		<< dwParam2;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventEnchant

/**
 * ¿Œ√æ∆Æ ¿Ã∫•∆Æ
 */
void CGame::OnEventEndEnchant()
{
	g_pGUI->ShowDlg( "DLG_ENCHANT", FALSE );
} //CGame::OnEventEnchant

/**
 * «ˆ¿Á ¡¬«•∏¶ «•Ω√
 */
void CGame::OnEventInfoPosition()
{
	if( !m_pPlayer )
	{
		Assert( !"«√∑π¿ÃæÓ∞° æ¯¥Ÿ" );
		return;
	} //if

	int		nX = m_pPlayer->GetPosition().x;
	int		nY = m_pPlayer->GetPosition().y;
	char	szText[32];
	sprintf( szText, "X : %d, Y : %d", nX, nY );
	g_pGUI->ChattingInput( "", szText, 0xFF00FF00 );
} //CGame::OnEventInfoPosition

/**
 * ∆Ø»≠Ω∫≈≥ ªÁøÎ
 */
void CGame::OnEventSkillSpecial( DWORD dwParam )
{
	Assert( m_pPlayer );

	m_pPlayer->UseSkill( dwParam );
} //CGame::OnEventSkillSpecial

void CGame::OnEventFriendCut()
{
} //CGame::OnEventFriendCut

void CGame::OnEventChattingCut()
{

} //CGame::OnEventChattingCut

void CGame::OnEventWhisperCut()
{

} //CGame::OnEventWhisperCut

void CGame::OnEventDealCut()
{

} //CGame::OnEventDealCut

void CGame::OnEventFriendInvit( DWORD dwParam1 )
{
	IW3DCreature*		pSelectTarget;

	if( dwParam1 == 0 )
	{
		pSelectTarget = g_pSelectMgr->GetCreature();

		if( !pSelectTarget || g_pSelectMgr->GetType() != CHARACTER )
		{
			return;
		}
	}
	else
	{
		pSelectTarget = m_pWorld->GetCharacter( dwParam1 );
		if( !pSelectTarget )
		{
			Assert( !"Not Found the Character" );
			return;
		}			
	}

	int		nId = pSelectTarget->GetId();

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_FRIEND_ASKYESNO )
		<< nId;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
}

void CGame::OnEventFriendRequest( DWORD dwParam1 )
{
	int		nId = dwParam1;
	
	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_FRIEND_JOIN_YES )
		<< nId;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );	
}

void CGame::OnEventFriendRequestNo( DWORD dwParam1 )
{
	int		nId = dwParam1;
	
	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_FRIEND_JOIN_NO )
		<< nId;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );	
}

/** 
 * Regist Friend
 */
void CGame::OnEventFriendRegist()
{ 
	//g_pGUI->ShowDlg( "DLG_FRIENDADD", TRUE );

//	g_pGUI->MessageBox( "Friend Add", "Friend Add Message", IW3DUInterface::MSG_YESNO );

	IW3DCreature*	pCharacter = g_pSelectMgr->GetCreature();
	if( !pCharacter || pCharacter->GetType() != CHARACTER )
	{
		g_pGUI->ChattingInput( "", STR_FRIEND_NOTFRIEND, 0xFF00FF00 );
		return;
	} //if

	if( g_pGUI->IsFriend( pCharacter->GetName() ) )
	{
		return;
	} //if
	
	char*		szFriendName = const_cast<char*>( pCharacter->GetName() );
	char s_strFriend[32];
	if( strlen( szFriendName ) >= 32 )
	{
		Assert( !"Over Length of Friend Name" );
		return;
	} //if

	if( !strcmp( szFriendName, s_strFriend ) )
	{ // Same PrevReg Friend
		return;
	} //if
	strcpy( s_strFriend, szFriendName );

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_FRIEND_ADD )
		<< szFriendName;

	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );

/*	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_FRIEND_ADD )
		<< 1;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );*/

} //CGame::OnEventFriendRegist

/*
 *	Friend List
 */
void CGame::OnEventFriendList()
{
	CPacket sendPacket;
	sendPacket.Tag( dPACKET_FRIEND_LOGIN_REQUEST ); //∑Œ±◊¿Œ 
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );

	if( m_btIsFriendList == FALSE )
	{
		m_btIsFriendList = TRUE;	
	}
	else
	{
		m_btIsFriendList = FALSE;
	}

	g_pGUI->ShowDlg( "DLG_CHATIDLIST", m_btIsFriendList );
}

/**
 * Remove Friend
 */
void CGame::OnEventFriendDelete()
{
	char*	pName = g_pGUI->GetWhisperName();
	if( !pName || strlen( pName ) == 0 )
	{
		return;
	} //if

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_FRIEND_REMOVE )
		<< pName;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventFriendDelete

/**
 * Take GuildMember
 */
void CGame::OnEventGuildTakeMember()
{
	IW3DCreature*	pTarget = g_pSelectMgr->GetCreature();
	if( !pTarget )
	{
		return;
	} //if
	int		nCharId = pTarget->GetId();

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_GUILD_ADDMEMBER_REQUEST )
		<< nCharId;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventGuildTakeMember

/**
 * Aid Guild
 */
void CGame::OnEventAid( DWORD dwParam1, DWORD dwParam2 )
{
	int			nGuildId	 = dwParam1;
	WORD		wGuildNumber = dwParam2 >> 16;
	BYTE		btAid		 = dwParam2 & 0xff;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_GUILD_ADDMEMBER )
		<< nGuildId
		<< wGuildNumber
		<< btAid;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventAid

/**
 * Remove GuildMember
 */
void CGame::OnEventGuildRemoveMember()
{
	char*			pName;
	char			szScanName[64];
	IW3DCreature*	pTarget = g_pSelectMgr->GetCreature();
	if( pTarget )
	{
		pName = const_cast<char*>( pTarget->GetName() );
	}
	else
	{
		pName = const_cast<char*>( g_pGUI->GetText( "SSTR_GUILDMEMBERLIST_LIST" ) );
	} //if..else

	if( !pName )		return;

	if( sscanf( pName , "%s", szScanName ) != 1 )
	{
		Assert( !"sscanf ø¿∑˘" );
	}


	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_GUILD_DROPMEMBER )
		<< szScanName;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventGuildRemoveMember

/**
 * Quit Guild
 */
void CGame::OnEventGuildQuit()
{
	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_GUILD_QUIT );
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventGuildQuit

/**
 * Create Guild
 */
void CGame::OnEventGuildCreate()
{
	char*	pGuildName = g_pGUI->GetChattingText();
	if( !pGuildName )
	{
		Assert( !"Does not guild name!!" );
		return;
	} //if

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_CREATE_GUILD )
		<< pGuildName;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventGuildCreate

/**
 * Release Guild
 */
void CGame::OnEventGuildRelease()
{
	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_DESTROY_GUILD );
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventGuildCreate

/**
 * Promote The GuildMember
 */
void CGame::OnEventGuildPromote()
{
/*#ifdef _CHINAVERSION
	char*			pName;
	char			szScanName[64];
	IW3DCreature*	pTarget = g_pSelectMgr->GetCreature();
	
	if( pTarget )		pName = const_cast<char*>( pTarget->GetName() );
	else				pName = const_cast<char*>( g_pGUI->GetText( "SSTR_GUILDMEMBERLIST_LIST" ) );

	if( !pName )		return;

	sscanf( pName , "%s", szScanName );

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_GUILD_ADVANCE )
		<< szScanName;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
#else*/
	IW3DCreature*	pTarget = g_pSelectMgr->GetCreature();
	if( !pTarget )
	{
		return;
	} //if

	DWORD		dwId = pTarget->GetId();

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_GUILD_ADVANCE )
		<< dwId;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
//#endif //_CHINAVERSION
} //CGame::OnEventGuildPromote

/**
 * Demote The GuildMember
 */
void CGame::OnEventGuildDemote()
{
/*#ifdef _CHINAVERSION
	char*			pName;
	char			szScanName[64];
	IW3DCreature*	pTarget = g_pSelectMgr->GetCreature();
	
	if( pTarget )		pName = const_cast<char*>( pTarget->GetName() );
	else				pName = const_cast<char*>( g_pGUI->GetText( "SSTR_GUILDMEMBERLIST_LIST" ) );

	if( !pName )		return;

	sscanf( pName , "%s", szScanName );

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_GUILD_DEMOTION )
		<< szScanName;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
#else*/
	IW3DCreature*	pTarget = g_pSelectMgr->GetCreature();
	if( !pTarget )
	{
		return;
	} //if

	DWORD		dwId = pTarget->GetId();

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_GUILD_DEMOTION )
		<< dwId;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
//#endif //_CHINAVERSION
} //CGame::OnEventGuildDemote

/**
 * War Info 
 */
void CGame::OnEvnetGuildWarInfo( DWORD dwParam1 )
{
	int		nMapNum = dwParam1;
	if( nMapNum == 0 )
	{
		return;
	} //if
	
	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_GET_WARSTATE )
		<< nMapNum;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEvnetGuildWarInfo

/**
 * Get CastleList 
 */
void CGame::OnEvnetGuildGetCastleList()
{
	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_GET_CASTLELIST );
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEvnetGuildGetCastleList

/**
 * Goto Deal Map
 */
void CGame::OnEventGotoDealMap()
{
	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_MOVE_DEALMAP );
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventGotoDealMap

/**
 * Pet On Off
 */
void CGame::OnEventPetOnOff()
{
	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_PET_ONOFF );
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventPetOnOff

/**
 * Pet On Off
 */
void CGame::OnEventChangePetName()
{
	char*		szName = const_cast<char*>( g_pGUI->GetText( "EDT_PET_NAME" ) );
	if( !szName )
	{
		Assert( !"Empty Pet Name" );
		return;
	} //if

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_PET_CHANGENAME )
		<< szName;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );

	g_pGUI->SetText( "EDT_PET_NAME", "" );
} //CGame::OnEventChangePetName

/**
 * Player PKMode
 */
void CGame::OnEventPKMode( DWORD dwParam1, DWORD dwParam2 )
{
	BYTE	btPkMode = (BYTE)dwParam2;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_SET_PVP_MODE )
		<< btPkMode;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );	
} //CGame::OnEventPKMode

/**
 * Request Guild List
 */
void CGame::OnEventRequestGuildList()
{
	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_GUILD_GETMEMBERLIST );
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventRequestGuildList

/**
 * Open MyShop
 */
void CGame::OnEventMyShopOpen()
{
	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_MYSHOP_OPEN );
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventMyShopOpen

/**
 * Open Smith
 */
void CGame::OnEventSmithOpen()
{
	CPacket sendPacket;
//	sendPacket.Tag( dPACKET_SMITH_START );
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
}//CGame::OnEventSmithOpen()


/**
 * Change MyShopTitle
 */
void CGame::OnEventMyShopChangeTitle()
{
	char*		szTitle = const_cast<char*>( g_pGUI->GetText( "EDT_MYSHOP_TITLE" ) );
	if( !szTitle )
	{
		Assert( !"Empty Title" );
		return;
	} //if

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_MYSHOP_CHANGETITLE )
		<< szTitle;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventMyShopChangeTitle

/**
 * Add Sell Item
 */
void CGame::OnEventMyShopAddItem( DWORD dwParam1, DWORD dwParam2 )
{
	int			nItemId	   = dwParam1;
	int			nItemPrice = dwParam2;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_MYSHOP_ADDITEM )
		<< nItemId
		<< nItemPrice;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventMyShopAddItem

/**
 * Remove Sell Item
 */
void CGame::OnEventMyShopRemoveItem( DWORD dwParam1 )
{
	int			nItemId = dwParam1;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_MYSHOP_REMOVEITEM )
		<< nItemId;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventMyShopRemoveItem

void CGame::OnEventSmithUpdate( DWORD dwParam1, DWORD dwParam2 )
{
	int		itemIdx = dwParam1;
	int		cardIdx = dwParam2;
	

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_SMITH_UPDATE )
		<< itemIdx
		<< cardIdx;

	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
}

/**
 *
 */
void CGame::OnEventSmithRemoveItem( DWORD dwParam1 )
{
	int		nItemId = dwParam1;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_SMITH_DELITEM )
		<< nItemId;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
}//CGame::OnEventSmithRemoveItem( DWORD dwParam1 )

/**
 * Begin Sell
 */
void CGame::OnEventMyShopSellBegin()
{
	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_MYSHOP_SELLBEGIN );
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );

	OnEventMyShopChangeTitle();
} //CGame::OnEventMyShopSellBegin

/**
 * Finish Sell
 */
void CGame::OnEventMyShopSellFinish()
{
	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_MYSHOP_SELLFINISH );
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventMyShopSellFinish

/**
 * ∞≥¿ŒªÛ¡°ø°º≠ æ∆¿Ã≈€ ∏ÆΩ∫∆Æ æÚ±‚
 */
void CGame::OnEventMyShopGetItemList( DWORD dwParam1 )
{
	int			nCharId = dwParam1;

	IW3DCreature*	pCharacter = g_pSelectMgr->GetCreature();
	if( !pCharacter )
	{
		Assert( !"No Seleected The Character" );
		return;
	} //if

	char*		szShopTitle = pCharacter->GetShopInfo();
	if( !szShopTitle )
	{
		Assert( !"Do Not The PersonalShop" );
		return;
	} //if
	g_pGUI->SetText( "TXT_PSHOP_TITLE", szShopTitle );
	m_dwPShopId = nCharId;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_MYSHOP_GETITEMLIST )
		<< nCharId;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventMyShopGetItemList

/**
 * Buy Item
 */
void CGame::OnEventMyShopBuyItem( DWORD dwParam1 )
{
	IW3DCreature*	pCharacter = g_pSelectMgr->GetCreature();
	if( !pCharacter )
	{
		Assert( !"No Seleected The Character" );
		return;
	} //if

	int			nItemId = dwParam1;
	int			nCharId = pCharacter->GetId();

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_MYSHOP_BUYITEM )
		<< nCharId
		<< nItemId;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventMyShopBuyItem

/**
 * Get Tex 
 */
void CGame::OnEventGetTex()
{
	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_GET_TAXINFO );

	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventGetTex

/**
 * Change Tex 
 */
void CGame::OnEventChangeTex( DWORD dwParam1 )
{
	int		nNextTex = dwParam1;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_CHANGE_TAX )
		<< nNextTex;

	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventChangeTex

/**
 * The Event Enchant Change 
 */
void CGame::OnEventEnchantChange( DWORD dwParam1 )
{
	int		nId = dwParam1;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_ENCHANT_CHANGE )
		<< nId;

	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventEnchantChange

/**
 * The Event Enchant Collection
 */
void CGame::OnEventEnchantCollection( DWORD dwParam1 )
{
	int		nId = dwParam1;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_EHCNAHT_COLLECTION )
		<< nId;

	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventEnchantCollection

/**
 * The Event Item Rook 
 */
void CGame::OnEventItemRook()
{
	IW3DCreature*	pTarget = g_pSelectMgr->GetCreature();
	if( !pTarget ) 
	{
		g_pGUI->ChattingInput( "", STR_SKILL_NOTFOUNDTARGET, FONTCOLOR_REF );
		return;
	} //if

	if( pTarget->GetType() != CHARACTER )
	{
		g_pGUI->ChattingInput( "", STR_SKILL_NOTUSINGTARGET, FONTCOLOR_REF );
		return;
	} //if

	int		nId = pTarget->GetId();

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_ITEMLOOK )
		<< nId;

	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventItemRook

/**
 * The Event Level Rook 
 */
void CGame::OnEventLevelRook()
{
	IW3DCreature*	pTarget = g_pSelectMgr->GetCreature();
	if( !pTarget ) 
	{
		g_pGUI->ChattingInput( "", STR_SKILL_NOTFOUNDTARGET, FONTCOLOR_REF );
		return;
	} //if

	if( pTarget->GetType() != CHARACTER )
	{
		g_pGUI->ChattingInput( "", STR_SKILL_NOTUSINGTARGET, FONTCOLOR_REF );
		return;
	} //if

	int		nId = pTarget->GetId();

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_LEVELLOOK )
		<< nId;

	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventLevelRook

/**
 * Send packet of the dPACKET_USE_BILLITEM 
 */
void CGame::OnEventCashItemUse( DWORD dwParam1 )
{
	int		nId = dwParam1;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_USE_BILLITEM )
		<< nId;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventCashItemUse

/**
 * Send packet of the dPACKET_GET_BILLING_ITEMS 
 */
void CGame::OnEventGetBillingItem()
{
	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_GET_BILLING_ITEMS );
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::OnEventGetBillingItem

/**
 * ∏∂øÏΩ∫ øﬁ¬  πˆ∆∞ ≈¨∏Ø.
 */
void CGame::OnMouseLClick(
	const BOOL		bPick,	///< [IN] πŸ¥⁄ ««≈∑ ø©∫Œ
	const Vector3&	vPick	///< [IN] πŸ¥⁄ ««≈∑µ» ¡¬«•
	)
{
	if( NULL == m_pPlayer )		return;
	if( m_bMiniGaming )			return;

	BOOL	bIsStand	= ( m_pPlayer->GetState() == STATE_STOP );
	BOOL	bIsMove		= ( m_pPlayer->GetState() == STATE_MOVE );

	if( !( bIsStand || bIsMove ) )
	{   // ¡§¡ˆ ªÛ≈¬, ∂«¥¬ ¿Ãµø ¡ﬂ¿œ ∂ß∏∏ µø¿€¿ª ºˆ«‡«“ ºˆ ¿÷¥Ÿ.
		// «œ¡ˆ∏∏ ¿Ãµø¿∫ ∞¯∞›¡ﬂ¿œ∂ßµµ ∞°¥…«œ∞‘ «—¥Ÿ.
		if( m_pPlayer->GetState() == STATE_ATTACK )
		{
		/*	if(  !m_pPlayer->GetSkill() == SKILL_WHITE || !m_pPlayer->GetSkill() == SKILL_BLACK )
			{*/
				if( g_pPickMonster )
				{
					return ;
				} //if
		//	}
		} //if		
		
		if( bPick )			PickedTerrain_L( vPick, m_pPlayer );
		g_pGUI->SetAttackMonster( FALSE );
		return;
	} //if

	m_pPlayer->SetFollowTarget( 0 );

	if( g_pPickItem )
	{
		PickedItem_L();								//< æ∆¿Ã≈€ ««≈∑.
	}
	else if( m_dwPickDoor )		
	{
		if( m_bCheckLMB )	{ PickedDoor_R(); m_bCheckLMB = 0; }
		else				{ PickedDoor_L(); m_bCheckLMB = 1; }	//< ƒ≥∏Ø≈Õ ««≈∑.		
	}
	else if( g_pPickMonster )
	{
		if( m_bCheckLMB )	{  PickedMonster_R(); m_bCheckLMB = 0; }
		else				{  PickedMonster_L(); m_bCheckLMB = 1; }	//< ƒ≥∏Ø≈Õ ««≈∑.	
	}
	else if( g_pPickNPC )	
	{
		if( m_bCheckLMB )	{  PickedNPC_R(); m_bCheckLMB = 0; }
		else				{  PickedNPC_L(); m_bCheckLMB = 1; }	//< ƒ≥∏Ø≈Õ ««≈∑.					
	}
	else if( g_pPickCharacter )	
	{
		if( m_bCheckLMB )	{  PickedCharacter_R(); m_bCheckLMB = 0; }
		else				{  PickedCharacter_L(); m_bCheckLMB = 1; } //< ƒ≥∏Ø≈Õ ««≈∑.
	}//< NPC ««≈∑.
	else if( bPick )
	{
		PickedTerrain_L( vPick, m_pPlayer );	//< ∏«∂•¿Ã ««≈∑µ» ∞ÊøÏ ƒ≥∏Ø≈Õ ¿Ãµø.
		m_bCheckLMB = 0;
	}
/*	if( g_pPickItem )			PickedItem_L();						//<  .
	else if( g_pPickNPC )	
	{
		if( m_bCheckLMB )	{  PickedNPC_R(); m_bCheckLMB = 0; }
		else				{  PickedNPC_L(); m_bCheckLMB = 1; }	//< ƒ≥∏Ø≈Õ ««≈∑.					
	}
	else if( g_pPickCharacter )	
	{
		if( m_bCheckLMB )	{  PickedCharacter_R(); m_bCheckLMB = 0; }
		else				{  PickedCharacter_L(); m_bCheckLMB = 1; } //< ƒ≥∏Ø≈Õ ««≈∑.
	}//< NPC ««≈∑.
	else if( bPick )
	{
		PickedTerrain_L( vPick, m_pPlayer );	//< ∏«∂•¿Ã ««≈∑µ» ∞ÊøÏ ƒ≥∏Ø≈Õ ¿Ãµø.
		m_bCheckLMB = 0;
	}
	else if( g_pPickMonster )		PickedMonster_L();					//<  .
//	else if( g_pPickCharacter )		PickedCharacter_L();				//<  .
	else if( m_dwPickDoor )			PickedDoor_L();						//< Door .
//	else if( g_pPickNPC )			PickedNPC_L();						//< NPC .
//	else if( bPick )				PickedTerrain_L( vPick, m_pPlayer );	//<     .
*/
} //CGame::OnMouseLClick

/**
 * ∏∂øÏΩ∫ ø¿∏•¡∑ πˆ∆∞¿Ã ¥≠∏∞ ∞ÊøÏ∏¶ √≥∏Æ«—¥Ÿ.
 */
void CGame::OnMouseRClick( const BOOL bPick, const Vector3& vPick )
{
/*	if( g_pSelectMgr->GetType() == MONSTER )
	{
		PickedMonster_R();
	}	
	else if(  g_pSelectMgr->GetType() == CHARACTER )
	{
		PickedCharacter_R();
	}
	else
	{
	}*/
	if( g_pPickItem )				PickedItem_R();						//< æ∆¿Ã≈€ ««≈∑.
	else if( g_pPickMonster )		PickedMonster_R();					//< ∏ÛΩ∫≈Õ ««≈∑.
	else if( g_pPickCharacter )		PickedCharacter_R();				//< ƒ≥∏Ø≈Õ ««≈∑.*/
	else if( m_dwPickDoor )			PickedDoor_R();						//< Door ««≈∑
	else if( g_pPickNPC )			PickedNPC_R();	
	else if( g_pFuncEntity )		PickedFuncEntity_R();				//< ±‚¥…¡∂π∞ ««≈∑.
	else if( g_pSelectMgr )			PickedNone_R();	
} //CGame::OnMouseRClick


/**
 *
 */
void CGame::PickedNone_R()
{	// æ∆π´∞Õµµ ««≈∑µ«¡ˆ æ æ“¥Ÿ∏È.
	// ≥¨Ω√ Ω√¿€ ªÛ≈¬∏¶ ∞ÀªÁ«—»ƒ ≥¨Ω√∞° Ω√¿€µ«∏È ∏Æ≈œ«œ∞Ì Ω√¿€ æ»µ∆¿ª∞ÊøÏ
	// º±≈√µ» Creature∞° ¿÷¥¬¡ˆ∏¶ ∞ÀªÁ«œø©,
	// ∏ÛΩ∫≈Õ∂Û∏È ∞¯∞› ∏Ì∑…¿ª ≥ª∏∞¥Ÿ.
	// ƒ≥∏Ø≈Õ∂Û∏È, Ctrl≈∞∏¶ ¥≠∑∂¥¬¡ˆ∏¶ »Æ¿Œ«œø© ∞¯∞› ∏Ì∑…¿ª ≥ª∏∞¥Ÿ.

//	if( m_mgFishHook.GetUse() )
//	{
//		if( m_mgFishHook.GetGameStart() || PickedTerrain_R() )			//< ≥¨Ω√ ∞‘¿” πŸ¥⁄««≈∑Ω√
//		{
//			return ;
//		}
//	}

	IW3DCreature* pCreature = g_pSelectMgr->GetCreature();

	if( !pCreature || pCreature->GetType() == NPC )
	{
		// º±≈√µ» ≈©∏Æ√≥∞° æ¯∞≈≥™ NPC∂Û∏È ±◊≥… ≥—±‰¥Ÿ.
		return ;
	} //if	

	if( pCreature->GetType() == MONSTER )
	{ 
		// º±≈√µ»∞Õ¿Ã ∏ÛΩ∫≈Õ∂Û∏È, ∏ÛΩ∫≈Õ∏¶ ≈¨∏Ø«—∞Õ¿∏∑Œ ƒ£¥Ÿ.
		g_pPickMonster = pCreature;
		PickedMonster_R();
	}
	//else if( pCreature->GetType() == CHARACTER )
	if( pCreature->GetType() == CHARACTER )
	{
		// º±≈√µ» ∞Õ¿Ã ƒ≥∏Ø≈Õ∂Û∏È, 
		g_pPickCharacter = pCreature;
		PickedCharacter_R();
	} //if..else..		
} //CGame::PickedNone_R

/** 
 * æ∆¿Ã≈€ ∏∂øÏΩ∫ øﬁ¬ πˆ∆∞¿∏∑Œ ««≈∑.
 */
void CGame::PickedItem_L()
{
	DWORD		dwId	= g_pPickItem->GetId();
	WORD		wType	= g_pPickItem->GetType();

	g_pGUI->SetSkillInfo( FALSE );

	// æ∆¿Ã≈€ √ÎµÊ.
	if( !g_pGUI->InventoryBlank() )
	{
		if( wType == ITEM_MP_L || wType == ITEM_MP_M || wType == ITEM_MP_S ||
			wType == ITEM_HP_L || wType == ITEM_HP_M || wType == ITEM_HP_S )
		{
			if( !g_pGUI->GetItemNumInInventory( wType ) )
			{
				g_pGUI->MessageBox( STR_INVENERR_NOTENOUGHSPACE, "NOTENOUGH", IW3DUInterface::MSG_OK );
				return;
			} //if
		}
		else if( wType != ITEM_MONEY )
		{
			g_pGUI->MessageBox( STR_INVENERR_NOTENOUGHSPACE, "NOTENOUGH", IW3DUInterface::MSG_OK );
			return;
		} //if..else
	} //if
	
	g_pPickItem = NULL;

	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_GET_ITEM )
		<< dwId;
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
} //CGame::PickedItem_L

/** 
 * æ∆¿Ã≈€ ∏∂øÏΩ∫ ø¿∏•¬ πˆ∆∞¿∏∑Œ ««≈∑.
 */
void CGame::PickedItem_R()
{
} //CGame::PickedItem_R

/** 
 * ∏ÛΩ∫≈Õ ∏∂øÏΩ∫ øﬁ¬ πˆ∆∞¿∏∑Œ ««≈∑.
 */
void CGame::PickedMonster_L()
{
	if( !m_pPlayer || !g_pPickMonster )
	{
		Assert( !"«√∑π¿ÃæÓ∞° æ¯¥Ÿ" );
		return;
	} //if

	if( g_pSelectMgr )
	{
		g_pSelectMgr->Select( g_pPickMonster, MONSTER );
	} //if

} //CGame::PickedMonster_L

/** 
 * ∏ÛΩ∫≈Õ ∏∂øÏΩ∫ ø¿∏•¬ πˆ∆∞¿∏∑Œ ««≈∑.
 */
void CGame::PickedMonster_R()
{	
	if( !m_pPlayer || !g_pSelectMgr->GetCreature() )
	{
		Assert( !"«√∑π¿ÃæÓ∞° æ¯¥Ÿ" );
		g_pGUI->SetAttackMonster( FALSE );
		g_pGUI->SetCheck( FALSE );
		return;
	} //if	

	if( g_pGUI->GetOnItemType( EQUIP_SHORTWEAPON ) == 10385 )
	{
		g_sTarget.Clean();
		return;
	}
//	g_pPickMonster = g_pSelectMgr->GetCreature();

//	if( !g_sTarget.IsTarget( MONSTER, g_pPickMonster->GetId() ) )
	{	// ªı∑ŒøÓ ∏ÛΩ∫≈Õ∏¶ ≈¨∏Ø«ﬂ¥Ÿ∏È ∞¯∞› ¥ÎªÛ¿∏∑Œ º≥¡§«—¥Ÿ.
		if( !g_pSelectMgr->GetCreature() )
		{
			m_bCheckLMB = 0;
			g_sTarget.Clean();
			g_pGUI->SetCheck( FALSE );
			return;
		} //if

		if( m_pPlayer->GetState() == STATE_MAGIC )
		{
			return ;
		} //if
		g_sTarget.SetTarget( MONSTER, g_pSelectMgr->GetCreature()->GetId() );
		m_bAttack		= FALSE;
		m_bToTarget		= FALSE;

		int			nHp[2];
		g_pSelectMgr->GetCreature()->GetHp( nHp[0], nHp[1] );
		g_pGUI->ShowTargetInfo( g_pSelectMgr->GetCreature()->GetId(), nHp[1], nHp[0], g_pSelectMgr->GetCreature()->GetName() );
	} //if*/
} //CGame::PickedMonster_R

/** 
 * ƒ≥∏Ø≈Õ ∏∂øÏΩ∫ øﬁ¬ πˆ∆∞¿∏∑Œ ««≈∑.
 */
void CGame::PickedCharacter_L()
{
	if( !m_pPlayer || !g_pPickCharacter )
	{
		Assert( !"«√∑π¿ÃæÓ∞° æ¯¥Ÿ" );
		return;
	} //if

	if( g_pSelectMgr )
	{
		g_pSelectMgr->Select( g_pPickCharacter, CHARACTER );
	} //if

	if( g_pInput->GetKeyDown( DIK_LSHIFT ) && !m_bNoMove )
	{
		// Ω¨«¡∆Æ ≈∞∏¶ ¥©∏£∞Ì ¿÷¥Ÿ∏È
		// µ˚∂Û∞°±‚∏¶ ¿˚øÎΩ√≈≤¥Ÿ.
		m_pPlayer->SetFollowTarget( g_pPickCharacter );
		m_pPlayer->MoveTo( g_pPickCharacter->GetPosition() );

		CPacket		sendPacket;
		sendPacket.Tag( dPACKET_CHAT_WHISPER )
			<< g_pPickCharacter->GetName()
			<< "You are being followed by ";
		g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );

//		m_sWhisper = "πÃ«‡«’¥œ¥Ÿ";
		return;
	} //if
} //CGame::PickedCharacter_L

/** 
 * ƒ≥∏Ø≈Õ ∏∂øÏΩ∫ ø¿∏•¬ πˆ∆∞¿∏∑Œ ««≈∑.
 */
void CGame::PickedCharacter_R()
{
	if( !m_pPlayer || !g_pPickCharacter )
	{
		Assert( !"«√∑π¿ÃæÓ∞° æ¯¥Ÿ" );
 		return;
	} //if

	/*if( g_sTarget.IsTarget( CHARACTER, g_pPickCharacter->GetId() ) )
	{ //¿Ã¿¸∞˙ ∞∞¿∫ ∏ÛΩ∫≈Õ ¿Ã∏È ∏Æ≈œ«—¥Ÿ.
		return;
	} //if*/
	  
	if( g_pPickCharacter->IsDead() )
	{
		g_sTarget.Clean();
		return;
	} //if

	if( !CheckPlayerKill() )
	{
		return;
	} //if..else

	g_pGUI->SetCursorMode( "CURSOR_ATTACK" );
	if( m_pPlayer->GetState() == STATE_MAGIC )
	{
		return ;
	} //if

	g_sTarget.SetTarget( CHARACTER, g_pPickCharacter->GetId() );
	m_bAttack		= FALSE;
	m_bToTarget		= FALSE;

/*
		if( !g_pInput->GetKeyDown( DIK_LCONTROL ) )
		{
#ifdef _DEBUG
			if( !g_bSelfAttack )
			{
				return ;
			} //if
#else
			return;
#endif //#ifdef _DEBUG			
		} //if
*/
} //CGame::PickedCharacter_R

/** 
 * NPC ∏∂øÏΩ∫ øﬁ¬ πˆ∆∞¿∏∑Œ ««≈∑.
 */
void CGame::PickedNPC_L()
{	
	if( m_nZoomMode == ZOOM_LONG )
	{ // 'q' ªÛ≈¬.
		return;
	} //if

	if( g_pSelectMgr )
	{
		g_pSelectMgr->Select( g_pPickNPC, NPC );
	} //if

	m_pCamera->CloseShot( g_pPickNPC->GetPosition(), g_pPickNPC->GetDirection(), TRUE );
	m_nZoomMode = ZOOM_LOCK;
	
	int		nId	      = g_pPickNPC->GetId();
	WORD	wShop, wQuest;
	BYTE	btStorage, btGuildMgr;
	g_pPickNPC->GetCastContent( wShop, wQuest, btStorage, btGuildMgr );

	DWORD		dwFlag = IW3DUInterface::NPC_TALK;
	if( 0 < wShop )
	{ // ªÛ¡°¿ª ∞°¡ˆ∞Ì ¿÷¥Ÿ.
		dwFlag |= IW3DUInterface::NPC_SHOP;
		g_pGUI->SetShopNum( wShop );
	} //if

	if( 0 < wQuest )
	{
		dwFlag |= IW3DUInterface::NPC_QUEST;
		g_pGUI->SetQuestNum( wQuest );
	} //if

	if( 0 < btStorage )
	{
		dwFlag |= IW3DUInterface::NPC_STORAGE;
	} //if

	if( btGuildMgr )
	{
		if( btGuildMgr == 1 )
			dwFlag |= IW3DUInterface::NPC_GUILDMGR;
		if( btGuildMgr == 2 )
			dwFlag |= IW3DUInterface::NPC_SMITH;	
		if( btGuildMgr == 3 )
			dwFlag |= IW3DUInterface::NPC_CARD;
		if( btGuildMgr == 4 )
			dwFlag |= IW3DUInterface::NPC_CARD1;
	} //if

//	DWORD		testFlag = dwFlag << 16 | m_wMap;
	g_pGUI->YouAndMe( nId, IW3DUInterface::YOU_NPC, dwFlag );//testFlag );

	if( nId == 66 && m_wMap == 20 )
	{
		g_pGUI->SetText( "BTN_YOUME_5", STR_PRIMIEM_ZONE );
	} //if
} //CGame::PickedNPC_L

/** 
 * NPC ∏∂øÏΩ∫ ø¿∏•¬ πˆ∆∞¿∏∑Œ ««≈∑.
 */
void CGame::PickedNPC_R()
{ 

} //CGame::PickedNPC_R

/**
 * ±‚¥…¡∂π∞ ø¿∏•¬  πˆ∆∞ ««≈∑.
 */
void CGame::PickedFuncEntity_R()
{
	Assert( g_pFuncEntity );

	switch( g_pFuncEntity->GetType() )
	{
	case IW3DFuncEntity::FUNCENTITY_ENCAMP:
		CPacket		sendPacket;
		sendPacket.Tag( dPACKET_MAKE_CAMP );
		g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
		break;
	} //switch
} //CGame::PickedFuncEntity_R

/**
 * πÆ ∏∂øÏΩ∫ øﬁ¬  πˆ∆∞≈¨∏Ø.
 */
void CGame::PickedDoor_L()
{
} //CGame::PickedDoor_L

/**
 * πÆ ∏∂øÏΩ∫ ø¿∏•¬  πˆ∆∞≈¨∏Ø.
 */
void CGame::PickedDoor_R()
{
/*	CPacket		sendPacket;
	sendPacket.Tag( dPACKET_DOOR_EVENT )
		<< m_dwPickDoor;
	
	g_pClient->Send( sendPacket.GetBuffer(), sendPacket.GetSize() );
	g_pGUI->SetCursorMode( "CURSOR_DOOR2" );*/
} //CGame::PickedDoor_L

/** 
 * ¡ˆ«¸ ∏∂øÏΩ∫ øﬁ¬ πˆ∆∞¿∏∑Œ ««≈∑.
 */
void CGame::PickedTerrain_L( const Vector3&	vPick, IW3DCreature* pPlayer )
{
	if( m_nZoomMode == ZOOM_LOCK )
	{
		return;
	} //if

	// æ∆π´∞Õµµ º±≈√µ«¡ˆ æ æ“¥Ÿ∏È, ¿Ã¿¸∞… ¿Ø¡ˆ «—¥Ÿ.
	/*
	if( g_pSelectMgr )
		g_pSelectMgr->Select( NULL );		// æ∆π´∞Õµµ º±≈√¿Ã µ«¡ˆ æ æ“¿∏π«∑Œ, NULL¿ª ≥÷¥¬¥Ÿ.
	*/	

	int		nX = (int)vPick.x;
	int		nY = (int)vPick.y;

	pPlayer->SetLayer();
	if( m_pWorld->IsMovable( nX, nY, pPlayer->GetLayer() ) )
	{	// ¿Ãµø ∞°¥…«— ∞˜¿œ ∞ÊøÏ...
		Vector3		vPos;
		vPos.x = (float)nX + 0.5F;
		vPos.y = (float)nY + 0.5F;

		m_pWorld->SetLayer( MAX( m_nPickedLayer - 1, 0 ) );
	//	vPos.z = vPick.z;
		vPos.z = m_pWorld->GetHeight( vPos );

		Vector3			vDistance = vPos - pPlayer->GetPosition();
		vDistance.z = 0.0F;

		g_pGUI->SetPlayerMapDir( vDistance.x, vDistance.y );

		float fMinDist = pPlayer->GetMinDist();

		if( !m_bNoMove && vDistance.Length() > fMinDist )
		{
			if( pPlayer->MoveTo( vPos ) )
			{	// ¿Ãµø¿Ã ∞°¥…«“ ∂ß∏∏ ∆˜¿Œ≈Õ∏¶ √‚∑¬«—¥Ÿ.
				if( pPlayer->GetState() == STATE_ATTACK )
				{
					m_bAdvanceMove = TRUE;
				}
				else
				{
					m_bAdvanceMove = FALSE;
				} //if
			} //if
		} //if

		g_pGUI->ShowTargetInfo( 0 );
		g_sTarget.Clean();
		m_bAttack		= FALSE;
	}
	else
	{
		g_pGUI->SetCursorMode( "CURSOR_NOMOVE" );
	} //if..else
} //CGame::PickedTerrain_L

/**
 * ≥¨Ω√ ∞‘¿” πŸ¥⁄ ««≈∑Ω√
 */
BOOL CGame::PickedTerrain_R()
{
	float		fHeight	= m_pPlayer->GetPosition().z;
	m_pPlayer->SetLayer();

	static Vector3	vPick( 0.0F, 0.0F, 0.0F );

	int			nMouseX = g_pInput->GetMouseX();
	int			nMouseY = g_pInput->GetMouseY();

	m_nPickedLayer = m_pWorld->PickTerrain( m_pPlayer->GetLayer(), fHeight, nMouseX, nMouseY, &vPick );

	if( m_nPickedLayer )
	{
//		return m_mgFishHook.TryFishing( m_pPlayer, vPick );
	}	

	return FALSE;
} // CGame::PickedTerrain_R()

/**
 * ƒ≥∏Ø≈Õ ID∞° «√∑π¿ÃæÓ∏¶ ∞°∏£≈∞¥¬¡ˆ »Æ¿Œ«—¥Ÿ.
 *
 * @param	dwCharacterId	[IN] »Æ¿Œ«“ ƒ≥∏Ø≈Õ ID
 *
 * @return	«√∑π¿ÃæÓ¿Œ¡ˆ ø©∫Œ
 */
BOOL CGame::IsPlayer( DWORD dwCharacterId )
{
	if( dwCharacterId == m_dwPlayer )
	{
		return TRUE;
	} //if
	return FALSE;
} //CGame::IsPlayer

/**
 * ƒ°∆Æ.
 */
BOOL CGame::CheatKey( const char* szText )
{
	//return FALSE;
	if( szText[0] != '@' ||
		strlen( szText ) > 128 )
	{
		return FALSE;
	} //if

	static char	szCheat[128];
	static char	szVal[64];

	void *pTest = NULL;

	if( sscanf( szText + 1, "%s", szCheat ) != 1 )
	{
		Assert( !"sscanf ø¿∑˘" );
	}

	if( sscanf( szText + 1 + strlen( szCheat ), "%s", szVal ) != 1 )
	{
		Assert( !"sscanf ø¿∑˘" );
	}

	if( !strcmp( szCheat, "SHOWFPS" ) )
	{ //«ˆ¿Á «¡∑°¿” ∫∏±‚.
		if( !strcmp( szVal, "ON" ) )		g_pScene->ShowFrameRate( TRUE );
		if( !strcmp( szVal, "OFF" ) )		g_pScene->ShowFrameRate( FALSE );
		return TRUE;
	}
	if( !strcmp( szCheat, "SHOWPET" ) )
	{ //«ˆ¿Á «¡∑°¿” ∫∏±‚.
		if( !strcmp( szVal, "ON" ) )		g_bShowPet = TRUE;
		if( !strcmp( szVal, "OFF" ) )		g_bShowPet = FALSE;
		return TRUE;
	}
	else if( !strcmp( szCheat, STR_MAY_MAY ) )
	{
		if( g_pHelper )
		{
			g_pHelper->Talk( szVal );
		} //if

		return TRUE;
	}
	else if( !strcmp( szCheat, "∞¯º”" ) )
	{
		if( sscanf( szVal, "%d", &g_dwAttackLimit ) != 1 )
		{
			Assert( !"sscanf ø¿∑˘" );
		}
		return TRUE;
	}
#ifdef _DEBUG
	else if( !strcmp( szCheat, "¿⁄µø∞¯∞›" ) )
	{
		if( !strcmp( szVal, "≤˚" ) )
		{
			g_bAttackMacro = FALSE;
		}
		else
		{
			g_bAttackMacro = TRUE;
		} //if..else..
		return TRUE;
	}
	else if( !strcmp( szCheat, "¿⁄µø»˙" ) )
	{
		if( !strcmp( szVal, "≤˚" ) )
		{
			g_bSelfAttack = FALSE;
		}
		else
			g_bSelfAttack = TRUE;
	}
	else if( !strcmp( szCheat, "a" ) )
	{
		int n = 0;
		if( sscanf( szVal, "%d", &n ) != 1 )
		{
			Assert( !"sscanf ø¿∑˘" );
		}

		m_pCamera->Shake( n );
	}
#endif //#ifdef _DEBUG
/*	else if( !strcmp( szCheat, "Ω∫≈≥" ) )
	{
		if( m_pPlayer )
		{
			IW3DCreature*	pMonster = g_pSelectMgr->GetCreature();
			if( pMonster && g_pSelectMgr->GetType() == MONSTER )
			{
				m_pPlayer->AddSkill( pMonster , 0, szVal );
			} //if
		} //if		
		return TRUE;
	}
*/	else if( !strcmp( szCheat, "SHOWGUI" ) )
	{ //«ˆ¿Á «¡∑°¿” ∫∏±‚.
		if( !strcmp( szVal, "OFF" ) )		g_pGUI->SetModeState( "[NONE_MODE]" );
		return TRUE;
	} //if..else

	return FALSE;
} //CGame::CheatKey

/**
 * ¥‹√‡≈∞
 */
void CGame::OnEventHotKey( const DWORD dwTick )
{
	Assert( m_pCamera );

	//-=-=-=-=-= Key [ ]: »≠∏È »Æ¥Î ( ∞¯º∫¿¸øÎ ) -=-=-=-=-=-=//
	/*if( m_nZoomMode != ZOOM_LOCK		&&
		g_pInput->GetKeyDown( DIK_Q )	&& 
		m_btMapType == 0 )	// ¿œπ› « µÂø°º≠∏∏ ¿˚øÎ
	{
		m_nZoomMode = ZOOM_LONG;
	}
	else */
	if( m_nZoomMode != ZOOM_LOCK )
	{
		m_nZoomMode = ZOOM_SHORT;
	} //if..else
	
	float	fAngle = 3.0F * dwTick * 30 / 1000;

	if( g_pInput->GetKeyDown( DIK_A ) )
	{
		m_pCamera->RotateZ( -fAngle );
	}
	else if( g_pInput->GetKeyDown( DIK_D ) )
	{
		m_pCamera->RotateZ( fAngle );
	} //if

	if( g_pInput->GetKeyDown( DIK_R ) )
	{
		m_pCamera->RotateX( fAngle );
	}
	else	if( g_pInput->GetKeyDown( DIK_F ) )
	{
		m_pCamera->RotateX( -fAngle );
	} //if

	if( g_pInput->GetKeyDown( DIK_W ) && m_nZoomMode == ZOOM_SHORT )
	{
		m_fCameraOffset += 0.5;
		m_pCamera->Zoom( m_fCameraOffset );
	}
	else if( g_pInput->GetKeyDown( DIK_S ) && m_nZoomMode == ZOOM_SHORT )
	{
		m_fCameraOffset -= 0.5;
		m_pCamera->Zoom( m_fCameraOffset );
	} //if

	if( g_pInput->GetKeyDown( DIK_LMENU ) && g_pInput->GetMouseR() )
	{ // Setting Whisper Character 
		if( g_pPickCharacter )
		{
			char*		pName = const_cast<char*>( g_pPickCharacter->GetName() );
			Assert( pName ); 
			g_pGUI->SetWhisperName( pName );
		} //if
	} //if

	static BOOL		s_bPrint = FALSE;
	if( g_pInput->GetKeyDown( DIK_SYSRQ ) && !s_bPrint ) //g_pInput->GetKeyDown( DIK_GRAVE ) ||
	{	// Print Screen ≈∞∞° ¥≠∏Æ∏È »≠∏È¿ª ¿˙¿Â«—¥Ÿ.
		s_bPrint = TRUE;
		ScreenShot();
	}
	s_bPrint = g_pInput->GetKeyDown( DIK_GRAVE );

	///////////////////////////////////////
	// Test
	///////////////////////////////////////

	if( g_pInput->GetKeyDown( DIK_Y ) )
	{// Ω∫≈≥...

		if( g_sTarget.btType == MONSTER && m_pPlayer )
		{
			IW3DCreature*	pMonster = m_pWorld->GetMonster( g_sTarget.dwId );
//			m_pPlayer->AddSkill( pMonster , 0, 0 );
		} //if
	} //if

	//-=-=-=-=-= Key [ ]:	-=-=-=-=-=-=//
	//-=-=-=-=-= Key [ ]:	-=-=-=-=-=-=//
	//-=-=-=-=-= Key [ ]:	-=-=-=-=-=-=//
	//-=-=-=-=-= Key [ ]:	-=-=-=-=-=-=//
	//-=-=-=-=-= Key [ ]:	-=-=-=-=-=-=//
	//-=-=-=-=-= Key [ ]:	-=-=-=-=-=-=//
	//-=-=-=-=-= Key [ ]:	-=-=-=-=-=-=//
	//-=-=-=-=-= Key [ ]:	-=-=-=-=-=-=//
	//-=-=-=-=-= Key [ ]:	-=-=-=-=-=-=//
} //CGame::OnEventHotKey

/**
 * ∏  ¡§∫∏ º≥¡§.
 */
void CGame::SetMapInfo( const DWORD dwMap )
{
	char		szMapInfoFile[64];
	sprintf( szMapInfoFile, "Map/land%04d.ini", dwMap );

	FILE*	pFile = fopen( szMapInfoFile, "rt" );
	if( !pFile )
	{
		Assert( !"∏  ¡§∫∏∆ƒ¿œ¿–±‚ Ω«∆–" );
		return;
	} //if

	ReadMapInfo( pFile );	
	fclose( pFile );

//	m_mgFishHook.Init( m_pWorld );		
/*
	sprintf( szMapInfoFile, "Map/land%04d.mg", dwMap );
	pFile = fopen( szMapInfoFile, "rt" );
	if( !pFile )
	{		
		m_mgFishHook.SetUse( FALSE );
		return;
	}
	
	ReadMGInfo( pFile );
	fclose( pFile );
	
	if( !m_mgFishHook.GetUse() )
	{
		m_mgFishHook.Init( m_pWorld );
		m_mgFishHook.SetUse( TRUE );	
		m_mgFishHook.LoadSprite();
	}	*/
} //CGame::SetMapInfo

/**
 * land*.ini ∆ƒ¿œ¿ª ¿–¥¬¥Ÿ. - ªÁøÓµÂ ¿Áª˝ øµø™ ¿–±‚
 */
void CGame::ReadMapInfo( FILE* pFile )
{
	char		szBuff[64];
	char		szMapName[64];

	int			count = 0, nBGMType = 0;
	int			i, result1, result2;
	POINT*		pt;
	HRGN		hRgn;

	while( fscanf( pFile, "%s", szBuff ) != EOF )
	{
		if( !strcmp( szBuff, "map_name" ) )
		{
			fscanf( pFile, "%s", szMapName );
			g_pGUI->SetText( "TXT_MAPNAME", szMapName );
		}
		else if( 
			!strcmp( szBuff, "area_waterflow" )		||
			!strcmp( szBuff, "area_waterflow2" )	||
			!strcmp( szBuff, "area_harbor" )		||
			!strcmp( szBuff, "area_gull" )			||
			!strcmp( szBuff, "area_smallwind" )		||
			!strcmp( szBuff, "area_lavaflow" )		||
			!strcmp( szBuff, "area_largewind" )		||
			!strcmp( szBuff, "area_warcastle1" )	||
			!strcmp( szBuff, "area_warcastle2" )
			)
		{ 
			fscanf( pFile, "%d %d", &count, &nBGMType );
			pt	= (POINT*)_MemAlloc( sizeof(POINT) * count );

			for( i=0 ; i<count ; i++)
			{
				result1 = fscanf( pFile, "%d", &(pt[i].x) );
				result2 = fscanf( pFile, "%d", &(pt[i].y) );

				if( !result1 || !result2 )
				{
					Assert( !"∏  ini Ω∫≈©∏≥∆Æ ø¿∑˘ - ¡¬«•∞≥ºˆ∞° ∏¬¡ˆ æ ¥Ÿ" );
				} //if
			} //for			

			hRgn = CreatePolygonRgn( pt, count, ALTERNATE );
			m_pWorld->AddMapArea( hRgn, nBGMType );

			SAFE_FREE( pt );
		}//if..else
	} //while

	DEBUG_OUT( szMapName + "Read map information" );
} // CGame::ReadMapInfo

/**
 * land*.mg ∆ƒ¿œ¿ª ¿–¥¬¥Ÿ.- πÃ¥œ∞‘¿” ≥¨Ω√≈Õ ¡§∫∏
 *//*
void CGame::ReadMGInfo( FILE* pFile )
{
	char	szBuff[128] = "\0";
	char	szKey[64] = "\0";
	char	szValue[64] = "\0";	

	int		nX = 0;
	int		nY = 0;

	int		nPlaceCnt = 0;	
	int		nPlaceNum = 0;
	int		nCoordCnt = 0;	

	POINT*	pt = NULL;
	HRGN	hRgn;

	fgets( szBuff, 128, pFile );
	sscanf( szBuff, "%s%s", szKey, szValue );
	if( !strcmp( szKey, "[FISHING_PLACE_COUNT]" ) )
	{
		nPlaceCnt = atoi( szValue );
	}
	
	while( fgets( szBuff, 128, pFile ) )
	{
		sscanf( szBuff, "%s%s", szKey, szValue );
		if( !strcmp( "[FISHING_PLACE]", szKey ) )
		{
			nPlaceNum = atoi( szValue );
			
			fgets( szBuff, 128, pFile );
			sscanf( szBuff, "%s%s", szKey, szValue );
			if( !strcmp( "[FISHING_COORD]", szKey ) )
			{
				nCoordCnt = atoi( szValue );
				pt = ( POINT* )_MemAlloc( sizeof( POINT ) * nCoordCnt );

				for( int i=0 ; i<nCoordCnt ; i++ )
				{
					fgets( szBuff, 128, pFile );
					sscanf( szBuff, "%d%d", &nX, &nY );

					pt[i].x = nX;
					pt[i].y = nY;
				}

				hRgn = CreatePolygonRgn( pt, nCoordCnt, ALTERNATE );				
				m_pWorld->AddFishingArea( hRgn, nPlaceNum );

				SAFE_FREE( pt );
			}
		}
	}		
} // CGame::ReadMGInfo
*/
/**
 * Map, Ambient BGM¿ª ¿Áª˝«—¥Ÿ. 
 */
void CGame::PlayMapBGM()
{
	Assert( g_pMediaPlayer );

	if( !g_bBGM_On )
	{
		return;
	}

	g_pMediaPlayer->Play( BGM_TYPE_MAP, m_wMap );		// Field ¿Ω ¿Áª˝
	g_pMediaPlayer->Play( BGM_TYPE_AMBIENT, m_wMap );	// »Ø∞Ê¿Ω ¿Áª˝
} // CGame::PlayMapBGM()

/**
 * Event BGM¿ª ¿Áª˝«—¥Ÿ.
 */
void CGame::PlayEventBGM( const int nEventNum )
{
	Assert( g_pMediaPlayer );

	if( !g_bBGM_On )
	{
		return;
	}

	g_pMediaPlayer->Play( BGM_TYPE_EVENT, nEventNum );
} // CGame::PlayEventBGM

/**
 * Area BGM¿ª ¿Áª˝«—¥Ÿ. 
 */
void CGame::PlayAreaBGM( const int nX, const int nY )
{
	Assert( g_pMediaPlayer );

	if( !g_bBGM_On || g_btWarMode )
	{
		return;
	} //if

	int nAreaType[MAXBGM];
	ZeroMemory( nAreaType, sizeof( int ) * MAXBGM );

	int	nAreaCount = m_pWorld->GetAreaBGMType( nX, nY, nAreaType );
	
	g_pMediaPlayer->PlayArea( nAreaCount, nAreaType );

	if( nAreaCount )
	{	
		g_pSoundMgr->SetReverb( g_pMediaPlayer->GetAreaBGMReverb( nAreaType[0] ) );
	}
	else
	{
		g_pSoundMgr->SetReverb( g_pMediaPlayer->GetMapBGMReverb( m_wMap ) );
	}

	return;
} // CGame::PlayAreaBGM

/**
 * Ω∫≈©∏∞ º¶.
 */
void CGame::ScreenShot()
{
	static DWORD s_dwSSCount = 0;
	char		 szPrintName[64];

	SYSTEMTIME	st;
	GetLocalTime( &st );

	if( s_dwSSCount == 0 )
	{
		s_dwSSCount = ( st.wMonth * 100000 ) + ( st.wDay * 1000 );
		do
		{
			s_dwSSCount++;
#ifdef _CHINAVERSION
			sprintf( szPrintName, "Capture/Deicide%07d.bmp", s_dwSSCount );
#else
			sprintf( szPrintName, "Capture/Deicide%07d.bmp", s_dwSSCount );
#endif //_CHINAVERSION
		} 
		while( _access( szPrintName, 0 ) != -1 ); //do..while
	}
	else
	{
		s_dwSSCount++;
#ifdef _CHINAVERSION
		sprintf( szPrintName, "Capture/Deicide%07d.bmp", s_dwSSCount );
#else
		sprintf( szPrintName, "Capture/Deicide%07d.bmp", s_dwSSCount );
#endif //_CHINAVERSION
	} //if..else

	g_pScene->Capture( szPrintName, &st );
	g_pGUI->ChattingInput( STR_SYSMSG_SCREENSHOT, szPrintName, 0xFFD1D1D1 );
} //CGame::ScreenShot

/**
 * npc πÃ¥œ∏  √≥∏Æ.
 */
void CGame::PutNPCMinimap()
{
	static Vector3	vNPC;
	WORD			wNPCCount = 0;

	WORD			wShop, wQuest;
	BYTE			btStorage, btGuildMgr;
	IW3DCreature*	pNPC = NULL;

	while( pNPC = m_pWorld->GetNPC( wNPCCount ) )
	{
		pNPC->GetCastContent( wShop, wQuest, btStorage, btGuildMgr );
#ifdef _dUMENOKOJI_QUEST_
		if( wShop > 0 )
		{ // ªÛ¡°¿ª ∞°¡¯ ∞ÊøÏ.
			vNPC = pNPC->GetPosition();
			g_pGUI->AddShopMapPos( vNPC.x, vNPC.y );
			if( wQuest > 0 )
			{
				DEBUG_OUT( pNPC->GetId() + " - " + vNPC.x + ", " + vNPC.y + " - NPC Add shop" + " - NPC Add quest" );
			}
			else
			{
				DEBUG_OUT( pNPC->GetId() + " - " + vNPC.x + ", " + vNPC.y + " - NPC Add shop" );
			}
			DEBUG_OUT( pNPC->GetId() + " - " + vNPC.x + ", " + vNPC.y + " - NPC Add shop" );
		}
		
		else if( btStorage > 0 )
		{
			vNPC = pNPC->GetPosition();
			g_pGUI->AddShopMapPos( vNPC.x, vNPC.y );
			
			if( wQuest > 0 )
			{
				DEBUG_OUT( pNPC->GetId() + " - " + vNPC.x + ", " + vNPC.y + " - NPC √¢∞Ì√ﬂ∞°" + " - NPC Add quest" );
			}
			else
			{
				DEBUG_OUT( pNPC->GetId() + " - " + vNPC.x + ", " + vNPC.y + " - NPC √¢∞Ì√ﬂ∞°" );
			}
			
			DEBUG_OUT( pNPC->GetId() + " - " + vNPC.x + ", " + vNPC.y + " - NPC √¢∞Ì√ﬂ∞°" );
		}
		else if( wQuest > 0 )
		{
			vNPC = pNPC->GetPosition();
			g_pGUI->AddShopMapPos( vNPC.x, vNPC.y );
			DEBUG_OUT( pNPC->GetId() + " - " + vNPC.x + ", " + vNPC.y + " - NPC Add quest" );	
		}
#else
		if( wShop > 0 )
		{ // ªÛ¡°¿ª ∞°¡¯ ∞ÊøÏ.
			vNPC = pNPC->GetPosition();
			g_pGUI->AddShopMapPos( vNPC.x, vNPC.y );
			DEBUG_OUT( pNPC->GetId() + " - " + vNPC.x + ", " + vNPC.y + " - NPC ªÛ¡°√ﬂ∞°" );
		}
		else if( btStorage > 0 )
		{
			vNPC = pNPC->GetPosition();
			g_pGUI->AddShopMapPos( vNPC.x, vNPC.y );	
			DEBUG_OUT( pNPC->GetId() + " - " + vNPC.x + ", " + vNPC.y + " - NPC √¢∞Ì√ﬂ∞°" );
		} //if..else
#endif
		wNPCCount++;
	} //while
} //CGame::PutNPCMinimap

/**
 * Input PK Mode
 */
BOOL CGame::CheckPlayerKill()
{
	if( !g_pPickCharacter ) 
	{ //
		return FALSE;
	} //if

	IW3DItemInfoMgr*	pItemInfoMgr = m_pWorld->GetItemInfoMgr();
	if( !pItemInfoMgr )
	{
		Assert( !"Item manager failed" );
		return FALSE;
	} //if

	SSkillInfoData*		pSkillData = pItemInfoMgr->GetSkillInfo( m_nSkillItem );
	if( !pSkillData )
	{
   		Assert( !"Item data failure" );
		return FALSE;
	} //if
	if( pSkillData->shSkillType == 1 ||
		pSkillData->shSkillType == 3 ||
		pSkillData->shSkillType == 5 ||
		pSkillData->shSkillType == 7 ||
		pSkillData->shSkillType == 9 )
	{ // ∫∏¡∂Ω∫≈≥
		return TRUE;
	}
	else if( g_pPickCharacter->GetId() == g_dwPlayerId )
	{ //MySelf
		return FALSE;
	} //if

	if( g_infoPlayer.btPKMode == 0 )
	{ //None
		return FALSE;	
	}
	else if( g_infoPlayer.btPKMode == 1 )
	{ //Free
		return TRUE;
	}
	else if( g_infoPlayer.btPKMode == 2 )
	{ //Guild
		if( !g_infoPlayer.sGuildName.compare( g_pPickCharacter->GetName() ) )
		{ // Guild Member
			return FALSE;
		} //if
	}
	else if( g_infoPlayer.btPKMode == 3 )
	{ //Party
		if( g_pGUI->IsPartyMember( g_pPickCharacter->GetId() ) )
		{ // Party Member
			return FALSE;
		} //if
	} //if..else..

	return TRUE;
} //CGame::InputPlayerKillerKill
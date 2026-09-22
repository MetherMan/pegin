[NAME] mt_arrow
[EFFECT]
	[PART]
		[SOUND] 0 arrow100048.wav 3
		[TIMELIMIT] 300
	[/PART]
[/EFFECT]
[EFFECT] 
	[TRACE] mt_arrow_trace
	[SPE] mt_arrow.wed
	[LOOP]
	[PART]
		[NOSHOW]
		[BONE] 14
		[STARTTIME] 0
		[TIMELIMIT] 50
	[/PART]
	[PART]
		[ATTACK]
		[POS] 0.0 0.0 1.3
		[TARGET] 0.0 1.0 1.2		
		[MOVETYPE] 1
		[STARTTIME] 0
		[TIMELIMIT] 0
		[SPEED] 2500
		[TRACEDIST] 0.03			
	[/PART]
	[PART]
		[NOSHOW]
		[BLOWTIMING]
			[STARTTIME] 0
			[TYPE] 0
		[/BLOWTIMING]		
		[TIMELIMIT] 500
		[TAIL] mt_arrow_hit.wed
		[SOUND] 0 attack100049.wav 3
	[/PART]	
[/EFFECT]
[/NAME]

FIBO	START	4
	CLEAR	X
	LDS	$ABTHIS
$ABTHIS	LDS	#1
	LDT	ZERO
$AALOOP	STS	CURR
	ADDR	T, S
	LDT	CURR
	TIX	TEST
	WD	CURR
	JLT	$AALOOP	.SHOULD BE PC RELATIVE
	STS	CURR
	WD	CURR
	J	FINISH
ZERO	BYTE	0
CURR	RESB	1
TEST	BYTE	X'09'	.10 NUMBERS OF THE SEQUENCE
FINISH	END	FIRST

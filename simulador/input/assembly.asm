FIBO	START	4
FIRST	CLEAR	X
	LDS	#1
	LDT	ZERO
LOOP	STS	CURR
	ADDR	T, S
	LDT	CURR
	TIX	TEST
	JLT	LOOP	.SHOULD BE PC RELATIVE
	J	FINISH
ZERO	BYTE	0
CURR	RESB	1
TEST	BYTE	X'09'	.10 NUMBERS OF THE SEQUENCE
FINISH	END	FIRST
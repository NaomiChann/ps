FIBO	START	4
FIRST	CLEAR	X
	LDS	#1
	LDT	ZERO
LOOP	STS	CURR
	ADDR	T, S
	LDT	CURR
	TIX	TEST
	JLT	LOOP	
	J	FINISH
ZERO	BYTE	0
CURR	RESB	1
TEST	BYTE	X'09'	
FINISH	END 	FIRST
FIBO	START	0
.
ERASE	MACRO	&OWO
	CLEAR	X
	LDS	#1
	LDT	&OWO
	MEND
.
FUNC	MACRO	&ARA, &ARB, &ARC
	ERASE	&ARC
$LOOP	STS	&ARA
	ADDR	T, S
	LDT	&ARA
	TIX	&ARB
	WD	&ARA
	JLT	$LOOP	.SHOULD BE PC RELATIVE
	MEND
.
FIRST	FUNC	CURR, TEST, ZERO
	STS	CURR
	WD	CURR
	J	FINISH
ZERO	BYTE	0
CURR	RESB	1
TEST	BYTE	X'09'	.10 NUMBERS OF THE SEQUENCE
FINISH	END	FIRST
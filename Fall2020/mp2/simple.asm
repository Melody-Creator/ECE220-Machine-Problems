; simple schedule used as an example in the specification

	.ORIG x4000

	.FILL 21		; bitmap of days (MWF)
	.STRINGZ "one"		; name of event
	.FILL 7			; slot 7, or 13:00

	.FILL 8			; bitmap of days (Tu)
	.STRINGZ "2"		; name of event
	.FILL 11		; slot 11, or 17:00

	.FILL -1 		; -1 marks end of schedule

	.END

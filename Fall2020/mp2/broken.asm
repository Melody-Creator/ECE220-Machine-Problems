; broken schedule used to illustrate error messages

	.ORIG x4000

	.FILL 21		; bitmap of days (MWF)
	.STRINGZ "one"		; name of event
	.FILL 7			; slot 7, or 13:00

	.FILL 4			; bitmap of days (W)
	.STRINGZ ""	  	; empty string is ok as name
	.FILL 9			; slot 9, or 15:00

	.FILL 11		; bitmap of days (TuThF)
	.STRINGZ "boring event"	; name of event
	.FILL 7			; slot 7, or 13:00

	.FILL 8			; bitmap of days (Tu)
	.STRINGZ "silly event"	; name of event
	.FILL 17		; non-existent slot

	.FILL -1		; blank string marks end of schedule

	.END

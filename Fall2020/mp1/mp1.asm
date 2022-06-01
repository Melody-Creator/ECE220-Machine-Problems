; R0 holds the current character being counted or displayed
; R1 holds the values of the number and starting address passed to the two subroutines
; R2 holds a pointer to the current position in the string
; R3 is used as a counter to hold the number of characters of the string and numbers of spaces
; needed to output
; R6 is used as a temporary register
; R7 holds the address to return to after a subroutine is called
; the above declare the usage of all the registers and it remains the same in every subroutine

    .ORIG x3000		; starting address is x3000

STORE			; store the registers at the start of a subroutine
    ST R0, SAVE_R0
    ST R1, SAVE_R1
    ST R2, SAVE_R2
    ST R3, SAVE_R3
    ST R6, SAVE_R6
    RET

RESTORE
    LD R0, SAVE_R0	; restore the registers at the end of a subroutine
    LD R1, SAVE_R1
    LD R2, SAVE_R2
    LD R3, SAVE_R3 
    LD R6, SAVE_R6
    RET

PRINT_SPACE		; a subroutine to print spaces, R3 is the number of spaces needed to output
    ADD R6, R7, #0	; use R6 to store the value of R7 temporarily
    LD R0, SPACE
    LOOP1
        OUT
        ADD R3, R3, #-1
        BRp LOOP1
    ADD R7, R6, #0	; restore R7 before subroutine returns
    RET

PRINT_STR		; a subroutine to print the string
    LD R3, LENGTH	; R3 holds the length of characters needed to display
    BRz EMPTY		; no character need to output
    ADD R6, R7, #0	
    ADD R2, R1, #0	; R2 now holds the start position of a string
    LOOP2   
        LDR R0, R2, #0
        OUT
        ADD R2, R2, #1
        ADD R3, R3, #-1
        BRp LOOP2
    ADD R7, R6, #0
    EMPTY RET

; PRINT_SLOT is a subroutine to print one of the schedule hours. A number from 0 to 14 is passed
; to this subroutine in R1. This subroutine prints the time corresponding to the specified slot.
; R1 = 0 means it must print "0600" and so forth
PRINT_SLOT	
    ST R7, SAVE_R7
    JSR STORE		; save all the registers at the start of this subroutine 

    AND R3, R3, #0
    ADD R3, R3, #3
    JSR PRINT_SPACE	; print 3 leading spaces using a subroutine

    LEA R2, TIME
    ADD R2, R2, R1
    ADD R2, R2, R1
    LDR R0, R2, #0
    OUT
    LDR R0, R2, #1
    OUT			; print the time using a look-up table, R2 holds the position of the character
			; that needs to display, R0 holds the character itself
    LD R0, ZERO
    OUT
    OUT

    AND R3, R3, #0
    ADD R3, R3, #2
    JSR PRINT_SPACE	; print 2 trailing spaces using a subroutine

    JSR RESTORE
    LD R7, SAVE_R7	; restore all the registers to make them unchanged before back to the caller
    RET

; PRINT_CENTERED is a subroutine to print a string centered in nine characters. A string (the address of the
; first  ASCII  character in sequence terminated by an ASCII NUL, x00) is passed to this subroutine in R1.
; This subroutine prints exactly nine characters. If the string is longer than nine characters, this
; subroutine prints the first nine characters. If the string is shorter than nine characters, this
; subroutine prints additional spaces around the string to bring the total length to nine characters. If
; the number of spaces needed is odd, this subroutine uses one more leading space than trailing space
PRINT_CENTERED
    ST R7, SAVE_R7
    JSR STORE		; save all the registers at the start of this subroutine 
    ADD R2, R1, #0
    AND R3, R3, #0	; initialize R2 to the starting address of the string and R3 to zero

    CHECK_LOOP		; this loop is used to check the string and get the length of it
        LDR R0, R2, #0
        BRz COMPARE
        ADD R2, R2, #1
        ADD R3, R3, #1
        BRnzp CHECK_LOOP
        
    COMPARE		; we compare the length with 9 to do further operations
        ST R3, LENGTH
        ADD R3, R3, #-9
        BRzp MORE_THAN_EIGHT
    
    LESS_THAN_NINE	; the case the length is < 9
        NOT R6, R3	; R6 holds the value of 9 - length - 1, which is used for look-up table
        ST R6, TEMP	; store R6 in a temporary space
        LEA R2, NUM_OF_LEAD
        ADD R2, R2, R6
        LDR R3, R2, #0
        LD R6, M_ZERO
        ADD R3, R3, R6	; we get the ASCII value of the number of leading spaces, we minus the ASCII value
			; of 0 to get the exact number, the next part is the same
        JSR PRINT_SPACE	

        JSR PRINT_STR	; use a subroutine to print the string

        LD R6, TEMP	; reuse R6
        LEA R2, NUM_OF_TRAIL
        ADD R2, R2, R6
        LDR R3, R2, #0
        LD R6, M_ZERO
        ADD R3, R3, R6
        BRz BACK	; the number of trailing spaces can be zero, in that case, we just return
        JSR PRINT_SPACE

    BRnzp BACK

    MORE_THAN_EIGHT	; the case that length is >= 9
        AND R3, R3, #0
        ADD R3, R3, #9
        ST R3, LENGTH	; set the length of the string to 9
        JSR PRINT_STR

    BACK JSR RESTORE	; restore all the registers to make them unchanged before back to the caller
    LD R7, SAVE_R7
    RET

    HALT		; avoid running into data
SAVE_R0 .BLKW #1
SAVE_R1 .BLKW #1
SAVE_R2 .BLKW #1
SAVE_R3 .BLKW #1
SAVE_R6 .BLKW #1
SAVE_R7 .BLKW #1	; create spaces to store the value of registers
LENGTH  .BLKW #1	; save the length of string 
TEMP    .BLKW #1	; a temporary space to store some values
SPACE	.FILL x0020	; the ASCII value of ' '
ZERO    .FILL x0030	; the ASCII value of '0'
M_ZERO 	.FILL xFFD0	; the additive inverse of ASCII '0'
NUM_OF_LEAD  .STRINGZ "112233445"	; look-up tables for the two subroutines
NUM_OF_TRAIL .STRINGZ "011223344"
TIME	     .STRINGZ "060708091011121314151617181920"
    .END


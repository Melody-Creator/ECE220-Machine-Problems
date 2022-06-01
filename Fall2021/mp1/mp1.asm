    .ORIG x3000		; starting address is x3000
; The following subroutine is STRLEN.  The address of a string (the address of the first character) is passed to 
; this subroutine in R0.  This subroutine will count the number of ASCII characters in the string, not 
; including the terminal NUL, and return that value in R1. 
STRLEN
    ST R0, SAVE_R0
    ST R2, SAVE_R2      ; store the registers at the start of a subroutine
    AND R1, R1, #0

	GET_LEN	            ; this loop is used to scan the string and get the length of it
        LDR R2, R0, #0
        BRz END_GET_LEN
        ADD R0, R0, #1
        ADD R1, R1, #1
        BRnzp GET_LEN

    END_GET_LEN
    LD R0, SAVE_R0
    LD R2, SAVE_R2      ; restore the registers at the end of a subroutine
    RET

; The following subroutine is PRINT_DECIMAL.  A non-negative number is passed into this subroutine in 
; R1 (as 2s complement).  The subroutine will convert the number into a sequence of ASCII digits and 
; print them to the display.

; R0 is used to print a single digit
; R1 stores the non-negative number we need to convert and print
; R2 and R3 are used to store the "ten to the power N" numbers and their opppsite
; R4 is used to count the val of the current digit
; R5 is a counter for # of digits in decimal
; R6 is used to judge leading zeros
PRINT_DECIMAL
    ST R0, SAVE_R0
    ST R1, SAVE_R1
    ST R2, SAVE_R2
    ST R3, SAVE_R3
    ST R4, SAVE_R4
    ST R5, SAVE_R5
    ST R6, SAVE_R6
    ST R7, SAVE_R7      ; store the registers at the start of a subroutine

    AND R5, R5, #0      ; counter for # of digits
    AND R6, R6, #0      ; judge leading zeros
    MP1_LOOP1
        LEA R2, TEN_TO_N
        ADD R2, R2, R5
        LDR R2, R2, #0  ; R2 is ten to the power N

        NOT R3, R2
        ADD R3, R3, #1
        AND R4, R4, #0  ; count the val of the current digit
        MP1_LOOP2
            ADD R1, R1, R3
            BRn OUTLOOP
            ADD R4, R4, #1
            BRnzp MP1_LOOP2
        OUTLOOP
        ADD R1, R1, R2  ; add the number back

        ADD R2, R4, R6
        BRz END_PRINT   ; skip printing leading zeros

        PRINT_ONE_DIGIT
        LEA R2, DIGITS
        ADD R2, R2, R4
        LDR R0, R2, #0
        OUT
        END_PRINT
        
        ADD R2, R5, #-3
        BRz COUNT_POS
        ADD R2, R4, #0
        BRp COUNT_POS   ; no more leading zeros when encountering a positive num or reaching the last digit next

        BRnzp SKIP_COUNT
        COUNT_POS
        ADD R6, R6, #1
        SKIP_COUNT

        ADD R5, R5, #1
        ADD R2, R5, #-5
        BRn MP1_LOOP1
    
    LD R0, SAVE_R0
    LD R1, SAVE_R1
    LD R2, SAVE_R2
    LD R3, SAVE_R3
    LD R4, SAVE_R4
    LD R5, SAVE_R5
    LD R6, SAVE_R6
    LD R7, SAVE_R7      ; restore the registers at the end of a subroutine
    RET

SAVE_R0  .BLKW #1
SAVE_R1  .BLKW #1
SAVE_R2  .BLKW #1
SAVE_R3  .BLKW #1
SAVE_R4  .BLKW #1
SAVE_R5  .BLKW #1
SAVE_R6  .BLKW #1
SAVE_R7  .BLKW #1       ; create spaces to store the value of registers
TEN_TO_N .FILL #10000
         .FILL #1000
         .FILL #100
         .FILL #10  
         .FILL #1       ; store these useful numbers here
DIGITS   .STRINGZ "0123456789"  ; look-up table for printing
    .END
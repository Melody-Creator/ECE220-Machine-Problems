    .ORIG x3000		; starting address is x3000
; This program along with two additional subroutines is used to support 
; measurement and printing of the Levenshtein distance between two strings. 
; In particular, given two strings, a distance, and a table, the main
; program will produce the output shown, making use of one subroutine to find the dimensions of the table and
; a second subroutine that uses the table to produce the output shown in the last two lines of the example.
; The descriptions of the two additional subroutines are shown in comments below.
MAIN
    JSR FIND_M_N

    LD R0, STRING1
    PUTS
    LD R0, SPACE
    OUT
    LD R0, HYPHEN
    OUT
    LD R0, GREATER
    OUT
    LD R0, SPACE
    OUT
    LD R0, STRING2
    PUTS
    LD R0, LIFE
    OUT                 ; print the first line

    LEA R0, LEV_DIS
    PUTS
    LDI R1, DIS_VAL
    JSR PRINT_DECIMAL
    LD R0, LIFE
    OUT                 ; print the second line

    JSR PRETTY_PRINT
    HALT

; This subroutine calls STRLEN twice to find the length of the first
; string (M) and save it to x38E0, then to find the length of the second
; string (N) and save it to x38E1.
FIND_M_N
    ST R0, STORE_R0
    ST R1, STORE_R1
    ST R7, STORE_R7     ; callee-saved

    LD R0, STRING1
    JSR STRLEN
    ADD R1, R1, #1
    STI R1, M

    LD R0, STRING2
    JSR STRLEN
    ADD R1, R1, #1
    STI R1, N

    LD R0, STORE_R0
    LD R1, STORE_R1
    LD R7, STORE_R7     ; restore values
    RET
; This subroutine is used to walk the table (started from x4000)
; and produce the last two lines of output to the display.
PRETTY_PRINT
    ST R7, STORE_R7
    LD R0, ST_TABLE
    LDI R1, N
    ADD R1, R1, #-1
    LDI R2, M
    ADD R3, R2, #-1

    CALC_LAST           ; calculate the last entry of the table
        ADD R0, R0, R1
        ADD R0, R0, R1
        ADD R0, R0, R1
        ADD R2, R2, #-1
        BRp CALC_LAST

    ADD R0, R0, R3
    ADD R0, R0, R3
    ADD R0, R0, R3
                        ; R1 = N-1, R2 = M, R3 = M-1, R0 = 3(N-1)M+3(M-1)
    LD R6, STK_BASE     
    WALK                ; traversing the path
        LDR R1, R0, #2  
        BRn END_WALK    ; (0, 0)
        ADD R6, R6, #-1
        STR R1, R6, #0

        LDR R1, R0, #1
        ADD R0, R0, R1  ; add the offset to get the next entry
        BRnzp WALK
    END_WALK    
                        ; R6 is the stack pointer, R1 saves the predecessor type and the offset, R0 saves the L.D. memory location of each entry
    ADD R5, R6, #0      ; save the stack pointer to R5
    LD R1, STRING1
    PRT1                ; print the first string
        ADD R2, R6, R6
        BRz OUT_PRT1    ; meet the stack base

        LDR R2, R6, #0
        BRz PRT_HYP1    ; R2 loads the type, insertion, print '-'
        LDR R0, R1, #0
        OUT
        ADD R1, R1, #1  ; print one character
        BRnzp END_HYP1

        PRT_HYP1
        LD R0, HYPHEN
        OUT
        END_HYP1
        ADD R6, R6, #1
        BRnzp PRT1
    OUT_PRT1
    LD R0, LIFE
    OUT
                        ; R1 stores address of the first string, R0 is used to output characters, R6 is the stack pointer, R2 is a temporary register
    ADD R6, R5, #0      ; use the same stack again
    LD R1, STRING2
    PRT2                ; print the second string
        ADD R2, R6, R6
        BRz OUT_PRT2    ; meet the stack base 

        LDR R2, R6, #0
        ADD R2, R2, #-1
        BRz PRT_HYP2    ; R2 loads the type, deletion, print '-'
        LDR R0, R1, #0
        OUT
        ADD R1, R1, #1  ; print one character
        BRnzp END_HYP2

        PRT_HYP2
        LD R0, HYPHEN
        OUT
        END_HYP2
        ADD R6, R6, #1
        BRnzp PRT2
    OUT_PRT2
    LD R0, LIFE
    OUT                 ; R1 stores address of the second string, R0 is used to output characters, R6 is the stack pointer, R2 is a temporary register
    LD R7, STORE_R7
    RET

STORE_R0 .BLKW #1
STORE_R1 .BLKW #1
STORE_R7 .BLKW #1
SPACE	 .FILL x0020	; the ASCII value of ' '
HYPHEN   .FILL x002D    ; the ASCII value of '-'
GREATER  .FILL x003E    ; the ASCII value of '>'
LIFE     .FILL x000A    ; the line feed character
STRING1  .FILL x3800
STRING2  .FILL x3840
M        .FILL x38E0
N        .FILL x38E1
DIS_VAL  .FILL x38C0
LEV_DIS  .STRINGZ "Levenshtein distance = "
STK_BASE .FILL x8000
ST_TABLE .FILL x4000

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
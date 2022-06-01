    .ORIG x3000		; starting address is x3000
; This program uses dynamic programming to compute the Levenshtein distance 
; between two strings and print the same output in MP2. Given two strings and the 
; costs for Insertion, Deletion,  and Substitution, the subroutine INIT_WORK 
; will initialize a table, and the subroutine CALC_DISTANCE will use dynamic 
; programming to compute the correct substring Levenshtein distances, predecessor 
; offsets, and predecessor types for each entry in the table. Essentially, 
; the two new subroutines in MP3 build the input table to MP2.
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

    JSR INIT_WORK
    JSR CALC_DISTANCE
    JSR PRINT_DECIMAL
    LD R0, LIFE
    OUT                 ; print the second line

    JSR PRETTY_PRINT
    HALT

INIT_WORK
    ; fill (0, 0)
    LD R0, ST_TABLE
    AND R1, R1, #0
    STR R1, R0, #0
    STR R1, R0, #1
    ADD R1, R1, #-1
    STR R1, R0, #2      ; R0 is position (0, 0) and R1 stores the values
    ; fill (R, 0), R > 0
    LD R0, ST_TABLE
    LDI R1, M           ; R1 = M
    ADD R2, R1, R1
    ADD R2, R2, R1      ; R2 = 3M
    NOT R3, R2
    ADD R3, R3, #1      ; R3 = -3M (offset)
    LDI R1, N           ; R1 = N/counter    
    LDI R4, C_INS       ; R4 = C_I
    AND R5, R5, #0
    AND R6, R6, #0

    FILL_COL
        ADD R1, R1, #-1
        BRz OUT_FILL_COL
        ADD R0, R0, R2
        ADD R5, R5, R4  ; R5 holds the distance
        STR R5, R0, #0
        STR R3, R0, #1
        STR R6, R0, #2  ; R6 holds the type 0
        BRnzp FILL_COL
    OUT_FILL_COL
    ; fill (0, C), C > 0
    LD R0, ST_TABLE
    LDI R1, M           ; R1 = M/counter
    LDI R4, C_DEL       ; R4 = C_D
    AND R5, R5, #0
    ADD R2, R5, #3      ; R2 = 3
    ADD R3, R5, #-3     ; R3 = -3 (offset)
    AND R6, R6, #0
    ADD R6, R6, #1

    FILL_ROW
        ADD R1, R1, #-1
        BRz OUT_FILL_ROW
        ADD R0, R0, R2
        ADD R5, R5, R4  ; R5 holds the distance
        STR R5, R0, #0
        STR R3, R0, #1
        STR R6, R0, #2  ; R6 holds the type 1
        BRnzp FILL_ROW
    OUT_FILL_ROW
    RET

CALC_DISTANCE
    LD R0, ST_TABLE     ; R0 holds the location of each entry in the table
    AND R1, R1, #0      ; R1 holds row
    AND R2, R2, #0      ; R2 holds column
    LDI R5, M
    NOT R5, R5
    ADD R5, R5, #1      ; R5 = -M
    LOOP_ROW
        LDI R6, N
        NOT R6, R6
        ADD R6, R6, #1      ; R6 = -N
        ADD R3, R1, R6
        BRz OUT_LOOP_ROW
        LOOP_COL
            ADD R3, R2, R5
            BRz OUT_LOOP_COL    ; jump to next row
            ADD R3, R1, #0
            BRz END_INS
            ADD R3, R2, #0
            BRz END_INS         ; do not repeat filling the first row and column
            ; S/M
            LD R3, STRING1
            LD R4, STRING2      ; R3 and R4 holds string1 and string2
            ADD R3, R3, R2
            ADD R4, R4, R1      ; calc the n'th char in the string
            LDR R3, R3, #-1
            LDR R4, R4, #-1
            NOT R4, R4
            ADD R4, R4, #1

            AND R6, R6, #0      ; R6 holds the distance val to be filled here
            ADD R3, R3, R4      ; compare two chars
            BRz MATCH

            LDI R3, C_SUB
            ADD R6, R6, R3

            MATCH
            AND R3, R3, #0
            ADD R3, R3, R5
            ADD R3, R3, R5
            ADD R3, R3, R5
            ADD R3, R3, #-3     ; R3 = -3(M+1) [offset]
            STR R3, R0, #1

            ADD R3, R3, R0
            LDR R3, R3, #0      ; R3 holds predecessor's distance
            ADD R6, R6, R3
            STR R6, R0, #0      ; store the distance
            
            AND R3, R3, #0
            ADD R3, R3, #2
            STR R3, R0, #2      ; R3 holds type 2

            ; D
            LDR R6, R0, #-3     ; R6 holds predecessor's distance
            LDI R3, C_DEL
            ADD R6, R6, R3      ; calc the new distance using Del
            LDR R4, R0, #0
            NOT R4, R4
            ADD R4, R4, #1
            ADD R4, R6, R4      ; compare distance with S/M
            BRzp END_DEL
            STR R6, R0, #0
            AND R3, R3, #0
            ADD R3, R3, #-3     ; R3 = -3 (offset)
            STR R3, R0, #1
            ADD R3, R3, #4      ; R3 holds type 1
            STR R3, R0, #2      ; update the values
            END_DEL
            ; I
            AND R3, R3, #0
            ADD R3, R3, R5
            ADD R3, R3, R5
            ADD R3, R3, R5      ; R3 = -3M
            ADD R6, R0, R3      ; predecessor (with offset -3M)
            LDR R6, R6, #0      ; R6 gets the distance

            LDI R4, C_INS
            ADD R6, R6, R4
            LDR R4, R0, #0
            NOT R4, R4
            ADD R4, R4, #1      ; R4 holds previous dis
            ADD R4, R6, R4      ; compare
            BRzp END_INS
            STR R6, R0, #0
            STR R3, R0, #1
            AND R3, R3, #0      ; R3 holds type 0
            STR R3, R0, #2      ; update vals
            END_INS

            ADD R2, R2, #1
            ADD R0, R0, #3      ; fill the next entry of table
            BRnzp LOOP_COL
        OUT_LOOP_COL
        ADD R1, R1, #1
        AND R2, R2, #0
        BRnzp LOOP_ROW
    OUT_LOOP_ROW

    LDR R1, R0, #-3             ; return dis of last entry in R1
    RET

C_INS    .FILL x3880
C_DEL    .FILL x3881
C_SUB    .FILL X3882

; ---------- The following code are from mp2 ----------
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
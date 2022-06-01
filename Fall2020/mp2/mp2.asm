; In this program, we translate events with variable-length into a schedule with fixed-length fields
; (pointers to the variable-length names of the events). Before translating, we first initialize the schedule. 
; After translating, we print it with day names on the top and hour names on the left.
    .ORIG   x3000   ; the starting address of the code

; R0 holds the NULL pointer (x0000)
; R1 holds the number of memory locations needed to initialize now
; R2 holds the current address of the schedule from starting (x3800)
INITIALIZE
    LD R0, NUL
    LD R1, TOTAL
    LD R2, START1   ; initialize the registers before going through the schedule
    LOOP1
        STR R0, R2, #0  ; store NULL pointer into the schedule
        ADD R2, R2, #1
        ADD R1, R1, #-1
        BRp LOOP1

; R0 holds the address of the bit vector and the number of memory locations before 
; the address of current event
; R1 holds the address of time slot
; R2, R3, R6, R7 are registers used temporarily and they hold multiple values 
; (see details in below commments)
TRANSLATE
    LD R0, START2   ; begin at the start of the event list (x4000)
    LOOP2
        LDR R2, R0, #0
        ADD R2, R2, #1
        BRz TRANS_OVER  ; if the bit vector is -1, end the translation
       
        ADD R1, R0, #1
        LOOP3           ; LOOP3 is used to find the end of string (NULL)
            LDR R2, R1, #0
            BRz WORK
            ADD R1, R1, #1
            BRnzp LOOP3
        WORK
            ADD R1, R1, #1  ; now R1 holds the address of the time slot
            LDR R2, R0, #0  ; R2 holds the value of the bit vector
            ADD R7, R0, #1 
            ST R7, TEMP1    ; store the starting address of the string into TEMP1
            AND R0, R0, #0
            LDR R3, R1, #0  
            BRn ERROR1      ; if time slot < 0, put an error message and terminate
            BRz AF_LOOP     
            ADD R6, R3, #-14
            BRp ERROR1      ; if time slot > 14, put an error message and terminate
            LOOP4       ; LOOP4 is used to calculate the number of memory locations before
                        ; the row with current time slot
                ADD R0, R0, #5
                ADD R3, R3, #-1
                BRz AF_LOOP
                BRnzp LOOP4
            AF_LOOP     ; done with the calculation on rows
            ADD R3, R3, #1  ; the value of the ith bit (2^i) in the bit vector
            ADD R0, R0, #4  ; R0 holds the ith memory locatons in the schedule 
                            ; (start from 0th)
            LOOP5       ; calculation on columns (from right to left Fri-Mon)
                AND R6, R2, R3  ; if the AND result is not zero, it means the
                                ; current bit is 1 in the bit vector
                BRz AF_WRITE
                LD R6, START1
                ADD R6, R6, R0  ; R6 holds the exact address where we should store the event
                LDR R7, R6, #0
                BRnp ERROR2     ; this address is not available, show error and terminate
                LD R7, TEMP1
                STR R7, R6, #0  ; store the event (a string pointer) into the schedule
                AF_WRITE
                ADD R3, R3, R3  ; double R3 to match the next bit
                ADD R0, R0, #-1
                ADD R6, R3, #-8
                ADD R6, R6, #-8
                BRnz LOOP5  ; don't consider bits with a value > 16

        ADD R0, R1, #1      ; R0 holds the starting address of next event (or -1)
        BRnzp LOOP2

TRANS_OVER    
    JSR PRINT_SCHEDULE
    HALT    ; end of main code

; R0 stores the content we need to print
; R1 stores the starting address of a string to print
; R2 is the row counter
; R3 is the column counter
; R6 is the total memory location counter
; R7 is for the subroutine to return
PRINT_SCHEDULE
    ST R7, TEMP2    ; store R7 at the start of a subroutine
    LEA R1, NUL
    JSR PRINT_CENTERED
    LD R0, LINE
    OUT
    LEA R1, DAY1
    JSR PRINT_CENTERED
    LD R0, LINE
    OUT
    LEA R1, DAY2
    JSR PRINT_CENTERED
    LD R0, LINE
    OUT
    LEA R1, DAY3
    JSR PRINT_CENTERED
    LD R0, LINE
    OUT
    LEA R1, DAY4
    JSR PRINT_CENTERED
    LD R0, LINE
    OUT
    LEA R1, DAY5
    JSR PRINT_CENTERED
    LD R0, FEED
    OUT             ; the code above print the header of the schedule 

    AND R6, R0, #0  ; total memory locations counter
    AND R2, R2, #0  ; R2 is the row counter

    OUTER_LOOP      ; row : from 0600 to 2000
        ADD R1, R2, #0
        JSR PRINT_SLOT  ; use subroutine to print time slot
        AND R3, R3, #0  ; R3 is the column counter
        ADD R3, R3, #5
        INNER_LOOP      ; column : from Mon to Fri
            LD R0, LINE
            OUT     ; print "|"
            LD R1, START1
            ADD R1, R1, R6
            LDR R1, R1, #0  ; R1 is the string pointer
            BRnp EVENT
            LEA R1, NUL     ; if R1 is NULL, change it to an empty string pointer
            EVENT 
            JSR PRINT_CENTERED  ; use subroutine to print event
            ADD R6, R6, #1
            ADD R3, R3, #-1
            BRp INNER_LOOP
        LD R0, FEED
        OUT     ; print "\n"
        ADD R2, R2, #1
        ADD R1, R2, #-15
        BRn OUTER_LOOP
    LD R7, TEMP2    ; restore R7 at the end of a subroutine
    RET

; ERROR1 and ERROR2 are two subroutines to print error messages
ERROR1
    LD R0, TEMP1    ; name of the event that produces error
    PUTS
    LEA R0, S1
    PUTS
    HALT
ERROR2
    LD R0, TEMP1    ; name of the event that produces error
    PUTS
    LEA R0, S2
    PUTS
    HALT 

TEMP1   .BLKW #1    ; a temporary space to store values
TEMP2   .BLKW #1    ; another temporary space to store values
NUL     .FILL x0000 ; store an empty string
TOTAL   .FILL #75   ; total memory locations
START1  .FILL x3800     ; the schedule starting address
START2  .FILL x4000     ; the event list starting address
LINE    .FILL x007C     ; the vertical line character
FEED    .FILL x000A     ; the line feed character
DAY1    .STRINGZ "MONDAY"
DAY2    .STRINGZ "TUESDAY"
DAY3    .STRINGZ "WEDNESDAY"
DAY4    .STRINGZ "THURSDAY"
DAY5    .STRINGZ "FRIDAY"   ; strings in the header (Mon - Fri)
S1      .STRINGZ " has an invalid slot number.\n"
S2      .STRINGZ " conflicts with an earlier event.\n"  ; error messages

; R0 holds the current character being counted or displayed
; R1 holds the values of the number and starting address passed to the two subroutines
; R2 holds a pointer to the current position in the string
; R3 is used as a counter to hold the number of characters of the string and numbers of spaces
; needed to output
; R6 is used as a temporary register
; R7 holds the address to return to after a subroutine is called
; the above declare the usage of all the registers and it remains the same in every subroutine

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
    LOOP6
        OUT
        ADD R3, R3, #-1
        BRp LOOP6
    ADD R7, R6, #0	; restore R7 before subroutine returns
    RET

PRINT_STR		; a subroutine to print the string
    LD R3, LENGTH	; R3 holds the length of characters needed to display
    BRz EMPTY		; no character need to output
    ADD R6, R7, #0	
    ADD R2, R1, #0	; R2 now holds the start position of a string
    LOOP7   
        LDR R0, R2, #0
        OUT
        ADD R2, R2, #1
        ADD R3, R3, #-1
        BRp LOOP7
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
        ST R6, TEMP1    ; store R6 in a temporary space
        LEA R2, NUM_OF_LEAD
        ADD R2, R2, R6
        LDR R3, R2, #0
        LD R6, M_ZERO
        ADD R3, R3, R6	; we get the ASCII value of the number of leading spaces, we minus the ASCII value
			; of 0 to get the exact number, the next part is the same
        JSR PRINT_SPACE	

        JSR PRINT_STR	; use a subroutine to print the string

        LD R6, TEMP1	; reuse R6
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

SAVE_R0 .BLKW #1
SAVE_R1 .BLKW #1
SAVE_R2 .BLKW #1
SAVE_R3 .BLKW #1
SAVE_R6 .BLKW #1
SAVE_R7 .BLKW #1	; create spaces to store the value of registers
LENGTH  .BLKW #1	; save the length of string 
SPACE	.FILL x0020	; the ASCII value of ' '
ZERO    .FILL x0030	; the ASCII value of '0'
M_ZERO 	.FILL xFFD0	; the additive inverse of ASCII '0'
NUM_OF_LEAD  .STRINGZ "112233445"	; look-up tables for the two subroutines
NUM_OF_TRAIL .STRINGZ "011223344"
TIME	     .STRINGZ "060708091011121314151617181920"

    .END

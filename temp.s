

/***********************
 * Function `rest()' 
 ***********************/
rest():
        push    r4
        mov     r1, r4
        add     #2, r4
        sub     #2, r1
        mov     #0, -4(r4)
        jmp     .L3
.L5:
        mov     #0, r15
        call    #buzzer_set_period(int)
        add     #1, -4(r4)
.L3:
        mov.b   #1, r15
        cmp     #10000, -4(r4)
        jl      .L4
        mov.b   #0, r15
.L4:
        cmp.b   #0, r15
        jne     .L5
        add     #2, r1
        pop     r4
        ret
;; End of function



/***********************
 * Function `buzzer_set_period(int)' 
 ***********************/
buzzer_set_period(int):
        push    r4
        mov     r1, r4
        add     #2, r4
        sub     #2, r1
        mov     r15, -4(r4)
        add     #2, r1
        pop     r4
        ret
;; End of function 

/***********************
 * Function `turnOff()' 
 ***********************/
turnOff():
        push    r4
        mov     r1, r4
        add     #2, r4
        mov     #0, r15
        call    #buzzer_set_period(int)
        pop     r4
        ret
;; End of function 

/***********************
 * Function `play()' 
 ***********************/
play():
        push    r4
        mov     r1, r4
        add     #2, r4
        sub     #4, r1
        mov     #4000, -6(r4)
.L7:
        mov     #0, -4(r4)
        jmp     .L4
.L6:
        mov     -6(r4), r15
        call    #buzzer_set_period(int)
        add     #1, -4(r4)
.L4:
        mov.b   #1, r15
        cmp     #20000, -4(r4)
        jl      .L5
        mov.b   #0, r15
.L5:
        cmp.b   #0, r15
        jne     .L6
        add     #1, -6(r4)
        jmp     .L7
;; End of function 

/***********************
 * Function `rest(unsigned int)' 
 ***********************/
rest(unsigned int):
        push    r4
        mov     r1, r4
        add     #2, r4
        sub     #4, r1
        mov     r15, -4(r4)
        mov     #0, -6(r4)
        jmp     .L9
.L11:
        mov     #0, r15
        call    #buzzer_set_period(int)
        add     #1, -6(r4)
.L9:
        mov     -6(r4), r12
        mov     -4(r4), r13
        mov     r13, r14
        mov     r14, r15
        add     r14, r15
        add     r15, r15
        add     r15, r15
        mov     r15, r14
        add     r13, r14
        mov     r14, r15
        rla     r15
        mov     r15, r14
        add     r13, r14
        mov     r14, r15
        add     r14, r15
        add     r15, r15
        add     r15, r15
        add     r15, r15
        mov     r15, r14
        sub     r13, r14
        mov     r14, r15
        rla     r15
        rla     r15
        rla     r15
        rla     r15
        rla     r15
        add     r15, r14
        add     r13, r14
        mov.b   #1, r15
        cmp     r14, r12
        jlo     .L10
        mov.b   #0, r15
.L10:
        cmp.b   #0, r15
        jne     .L11
        add     #4, r1
        pop     r4
        ret
;; End of function 


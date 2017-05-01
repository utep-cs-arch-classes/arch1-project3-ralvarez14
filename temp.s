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

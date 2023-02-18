la     MOV     R1,#4
       MOV     R2,#0

L4     cmp     R1, #0
       beq     L2
       cmp     R1, #2
       bne     L3
       mov     R2, #10

L3     subs    R1, R1, #1
       b       L4

L2     mov     R1, #0
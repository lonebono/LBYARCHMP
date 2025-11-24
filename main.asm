global imgCvtGrayIntToDouble

section .data
formula  DQ 0.003921568627451 ; precomputed

section .text
imgCvtGrayIntToDouble:
    TEST R8, R8 ; checks if pixel count is zero
    JZ .RET ; exits if zero

    MOVSD XMM1, [REL formula]  ; load precomputed formula

.LOOP:
    MOVZX EAX, BYTE [RCX] ; load pixel value
    CVTSI2SD XMM0, EAX ; convert to double
    MULSD XMM0, XMM1 ; multiply by formula
    MOVSD [RDX], XMM0 ; store result

    ; advance pointer and decrement pixel count
    ADD RCX, 1
    ADD RDX, 8
    DEC R8
    JNZ .LOOP

    ; ends the loop
.RET:
    RET

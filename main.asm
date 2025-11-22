global convert

section .data
formula  DQ 0.003921568627451 ; precomputed

section .text
convert:
    TEST R8, R8
    JZ .RET

    MOVSD XMM1, [REL formula]

.LOOP:
    MOVZX EAX, BYTE [RCX]
    CVTSI2SD XMM0, EAX
    MULSD XMM0, XMM1
    MOVSD [RDX], XMM0

    ADD RCX, 1
    ADD RDX, 8
    DEC R8
    JNZ .LOOP

.RET:
    RET

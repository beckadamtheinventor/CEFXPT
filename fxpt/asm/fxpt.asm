section .text
assume ADL = 1

public ___operator_mul
public ___operator_div

extern __ldivs
extern __lmuls
extern __ishrs

___operator_div:
    ; shift up first argument 8 bits
    ; divide using 32-bit division
    ; return 32-bit result in EUHL
    ld iy,0
    add iy,sp
    ld hl,(iy+3-1)
    ld e,(iy+3+2)
    ld bc,(iy+6)
    xor a,a
    ld l,a
    jp __ldivs

___operator_mul:
    ; treat 24-bit args as 32-bit args
    ; multiply using 32-bit multiplication
    ; shift result down 8 bits
    ; return 24-bit result in HL
    pop de,bc
    ex (sp),hl
    push bc,de
    xor a,a
    ld e,a
    call __lmuls
    ld d,0
    push de
    push hl
    inc sp
    pop hl
    dec sp
    pop de
    ret


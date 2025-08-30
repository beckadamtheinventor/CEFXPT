section .text
assume ADL = 1

public ___operator_mul
public ___operator_div
public ___operator_tostring

extern __ldivs
extern __lmuls
extern __frameset0

___operator_div:
    ; shift up first argument 8 bits
    ; divide using 32-bit division
    ; return 24-bit result in HL
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

; only include this routine if it gets used
if used ___operator_tostring
___operator_tostring:
    call __frameset0
    ld hl,(ix+10) ; short i
    ld de,(ix+6) ; char* buf
    push hl,de
    call .short_to_str
    ld (hl),'.'
    inc hl
    pop bc
    ex (sp),hl
    ld h,(ix+9) ; unsigned char f
    ld l,250 ; x250
    mlt hl
    add hl,hl ; x500
    add hl,hl ; x1000
    push hl
    inc sp
    pop hl
    dec sp
    ex (sp),hl
    push hl
    call .short_to_str
    pop bc,bc
    ld de,(ix+6) ; char* buf
    or a,a
    sbc hl,de
    pop ix
    ret

.short_to_str:
    call __frameset0
	ld de,(ix+9)
    ex.s hl,de
	ld ix,(ix+6)
    bit 7,h
	call nz,.negate
.10k:
	ld bc,10000
	call .aqu
	ld bc,1000
	call .aqu
.100:
	ld bc,100
	call .aqu
	ld c,10
	call .aqu
	ld c,1
	call .aqu
	ld (ix),0
    ex (sp),ix
    pop hl
	ret

.aqu:
	ld a,'0'-1
.aqu.loop:
	inc a
	or a,a
	sbc hl,bc
	jr nc,.aqu.loop
	add hl,bc
	ld (ix),a
    inc ix
	ret

.negate:
	ex hl,de
	or a,a
	sbc hl,hl
	sbc hl,de ; 0 - value
	ld (ix),'-'
	inc ix
	ret

end if
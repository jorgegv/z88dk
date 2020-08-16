;
;  Copyright (c) 2020 Phillip Stevens
;
;  This Source Code Form is subject to the terms of the Mozilla Public
;  License, v. 2.0. If a copy of the MPL was not distributed with this
;  file, You can obtain one at http://mozilla.org/MPL/2.0/.
;
;  feilipu, August 2020
;
;-------------------------------------------------------------------------

SECTION code_fp_am9511
PUBLIC asm_am9511_discardfraction

; Entry: dehl = 32 bit float
; Exit:  dehl = 32 bit float without fractional part

.asm_am9511_discardfraction
    sla e                       ; get the exponent
    rl d
    jr Z,zero_legal             ; return IEEE signed zero

    ld a,d                      ; Exponent
    rr d                        ; Keep sign and exponent safe
    rr e
    sub $7f                     ; Exponent value of 127 is 1.xx
    jr C,return_zero

    inc a
    cp 24
    ret NC                      ; No shift needed, all integer

                                ; Build mask of integer bits
                                ; a = number of bits to keep
    exx
    ld hl,0
    ld e,h

.shift_right                    ; shift mantissa mask right
    scf
    rr e
    rr h
    rr l
    dec a
    jr NZ,shift_right

    ld  a,e                     ; mask out fractional bits
    exx
    and e
    ld  e,a
    ld  a,h
    exx
    and h
    exx
    ld h,a
    ld a,l
    exx
    and l
    exx
    ld l,a
    ret


.return_zero
    rl d                        ; Get the sign bit
    ld d,0

.zero_legal
    ld e,d                      ; use 0
    ld h,d
    ld l,d
    rr d                        ; restore the sign
    ret                         ; return IEEE signed ZERO in DEHL

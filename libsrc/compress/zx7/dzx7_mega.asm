
; void dzx7_mega(void *src, void *dst)

SECTION code_clib
SECTION code_compress_zx7

PUBLIC dzx7_mega

EXTERN asm_dzx7_mega

dzx7_mega:

   pop af
   pop de
   pop hl
   
   push hl
   push de
   push af
   
   jp asm_dzx7_mega

; SDCC bridge for Classic
IF __CLASSIC
PUBLIC _dzx7_mega
defc _dzx7_mega = dzx7_mega
ENDIF

; Clang bridge for Classic
IF __CLASSIC
PUBLIC ___dzx7_mega
defc ___dzx7_mega = dzx7_mega
ENDIF


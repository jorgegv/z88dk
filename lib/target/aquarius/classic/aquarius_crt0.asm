;       CRT0 for the Mattel Aquarius
;
;       Stefano Bodrato Dec. 2000
;
;       If an error occurs eg break we just drop back to BASIC
;
;       $Id: aquarius_crt0.asm,v 1.21 2016-07-15 21:03:25 dom Exp $
;



    MODULE  aquarius_crt0


    defc    crt0 = 1
    INCLUDE "zcc_opt.def"

    INCLUDE "target/aquarius/def/aqplus.def"

    EXTERN  _main           ;main() is always external to crt0 code

    PUBLIC  __Exit         ;jp'd to by exit()
    PUBLIC  l_dcal          ;jp(hl)


    defc    TAR__no_ansifont = 1
    defc    CONSOLE_ROWS = 24
    defc    CONSOLE_COLUMNS = 40
    defc    __CPU_CLOCK = 4000000

    PUBLIC  CLIB_VIDEO_PAGE_PORT    ;Video paging port for aq+
    ; Page video into 0xc000 -> 0xffff
    defc    CLIB_VIDEO_PAGE_PORT = PORT_BANK3

    PUBLIC  CLIB_AQUARIUS_ROM
IFNDEF CLIB_AQUARIUS_ROM
    defc    CLIB_AQUARIUS_ROM = 0
ENDIF


    PUBLIC  CLIB_AQUARIUS_PLUS
IFNDEF CLIB_AQUARIUS_PLUS
    defc    CLIB_AQUARIUS_PLUS = 0

    ; Forcibly disable bitmap routines
    EXTERN  l_ret
    PUBLIC  cls_BITMAP
    PUBLIC  printc_BITMAP
    PUBLIC  scrollup_BITMAP
    PUBLIC  vpeek_BITMAP
    PUBLIC  generic_console_ioctl
    defc    cls_BITMAP = l_ret
    defc    printc_BITMAP = l_ret
    defc    scrollup_BITMAP = l_ret
    defc    vpeek_BITMAP = l_ret
    defc    generic_console_ioctl = l_ret
ENDIF

IF CLIB_AQUARIUS_ROM = 1
    INCLUDE	"target/aquarius/classic/rom.asm"
ELIF CLIB_AQUARIUS_PLUS = 1
    INCLUDE "target/aquarius/def/maths_mbf.def"
    INCLUDE	"target/aquarius/classic/aqplusram.asm"
ELSE
    INCLUDE "target/aquarius/def/maths_mbf.def"
    INCLUDE	"target/aquarius/classic/ram.asm"
ENDIF


l_dcal: 
    jp      (hl)


    INCLUDE "crt/classic/crt_runtime_selection.inc"

    INCLUDE	"crt/classic/crt_section.inc"

    SECTION code_crt_init
    ld      hl,$3028
    ld      (base_graphics),hl

IF CLIB_AQUARIUS_PLUS = 1
    INCLUDE	"target/aquarius/classic/banks.asm"
ENDIF
    


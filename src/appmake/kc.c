/*
 *      Short program to create a VEB MPM KC85/2..KC85/4 type header
 *
 *      
 *      $Id: kc.c,v 1.1 2016-10-03 06:14:49 stefano Exp $
 */

#include "appmake.h"
#include <string.h>
#include <ctype.h>



static char             *binname      = NULL;
static char             *crtfile      = NULL;
static char             *outfile      = NULL;
static char             *blockname    = NULL;
static int               origin       = -1;
static char              help         = 0;


/* Options that are available for this module */
option_t kc_options[] = {
    { 'h', "help",     "Display this help",          OPT_BOOL,  &help},
    { 'b', "binfile",  "Linked binary file",         OPT_STR,   &binname },
    { 'c', "crt0file", "crt0 file used in linking",  OPT_STR,   &crtfile },
    { 'o', "output",   "Name of output file",        OPT_STR,   &outfile },
    {  0 , "org",      "Origin of the binary",       OPT_INT,   &origin },
    {  0 , "blockname", "Name of the code block in tap file", OPT_STR, &blockname},
    {  0,  NULL,       NULL,                         OPT_NONE,  NULL }
};



/*
 * Execution starts here
 */

int kc_exec(char* target)
{
    char filename[FILENAME_MAX + 1];
    FILE* fpin;
    FILE* fpout;
    long pos;
    int len;
    int c, i;
    int nflag;

    if (help)
        return -1;

    if (binname == NULL) {
        return -1;
    }

    if (outfile == NULL) {
        strcpy(filename, binname);
    } else {
        strcpy(filename, outfile);
    }

    // strupr(filename);
    // not available on all platforms

    for (i = (int)strlen(filename) - 1; i >= 0 && filename[i] != '/' && filename[i] != '\\'; i--)
        filename[i] = toupper(filename[i]);

    //

    suffix_change(filename, ".KCC");

    if (strcmp(binname, filename) == 0) {
        exit_log(1,"Input and output file names must be different\n");
    }

    if (blockname == NULL)
        blockname = zbasename(binname);

    if (origin != -1) {
        pos = origin;
    } else {
        if ((pos = get_org_addr(crtfile)) == -1) {
            exit_log(1,"Could not find parameter ZORG (not z88dk compiled?)\n");
        }
    }

    if ((fpin = fopen_bin(binname, crtfile)) == NULL) {
        exit_log(1, "Can't open input file %s\n", binname);
    }

    if (fseek(fpin, 0, SEEK_END)) {
        fclose(fpin);
        exit_log(1, "Couldn't determine size of file\n");
        fclose(fpin);
    }

    len = ftell(fpin);

    fseek(fpin, 0L, SEEK_SET);

    if ((fpout = fopen(filename, "wb")) == NULL) {
        fclose(fpin);
        exit_log(1,"Can't open output file\n");
    }

    /* deal with the filename */
    nflag = 0;
    for (i = 0; i < 8; i++) {
        if (nflag)
            writebyte(0, fpout);
        else {
            if (!isalnum(blockname[i])) {
                writebyte(0, fpout);
                nflag++;
            } else {
                writebyte(toupper(blockname[i]), fpout);
            }
        }
    }

    writebyte('K', fpout);
    writebyte('C', fpout);
    writebyte('C', fpout);

    for (i = 0; i < 5; i++)
        writebyte(0, fpout);

    writebyte(3, fpout); /* 0x02 = load, 0x03 = autostart */

    writeword(pos, fpout);
    writeword(pos + len, fpout);
    writeword(pos, fpout); /* start address */

    for (i = 0; i < 105; i++)
        writebyte(0, fpout);

    for (i = 0; i < len; i++) {
        c = getc(fpin);
        writebyte(c, fpout);
    }

    // Pad the block out to 128 bytes
    while( (i % 128) > 0) {
        writebyte(0, fpout);
        i++;
    }

    fclose(fpin);
    fclose(fpout);

    return 0;
}

/* 3 rotor Enigma simulation 
 * Converted over to Small C by D Morris 11/9/98
 *
 * As you can see Small C doesn't do multidimensional arrays!
 * If you want to see how this program works have a look at
 * the BBC Basic program enigma.bas in this directory
 *
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>


/* Rotor Wirings */

unsigned char rotor[]="EKMFLGDQVZNTOWYHXUSPAIBRCJAJDKSIRUXBLHWTMCQGZNPYFVOEBDFHJLCPRTXVZNYEIWGAKMUSQOESOVPZJAYQUIRHXLNFTGKDCMWBVZBRGITYUPSDNHLXAWMJQOFECK";

unsigned char ref[]=       "YRUHQSLDPXNGOKMIEBFZCWVJAT";

unsigned char notch[]="QEVJZ";
unsigned int flag=0;

/*Encryption parameters */

unsigned char order[3]={ 3, 1, 2 };
unsigned char rings[3]={ 'W','X','T' };
unsigned char pos[3]={ 'A','W','E' };
unsigned char plug[]="AMTE";

void main()
{
     unsigned int i,j,n;
     unsigned int  ch;
     n=0;

     puts("Enter text to be (de)coded, finish with a .");
     while(1>0)
     {
          ch=getchar();
          ch=toupper(ch);
          if (ch == '.')
                  exit(0);
          if (!isalpha(ch))
               continue;
          

          /* Step up first rotor */
          pos[0]++;
          if   (pos[0]>'Z')
               pos[0] -= 26;

          /* Check if second rotor reached notch last time */
          if   (flag)
          {
               /* Step up second and third rotors */
               pos[1]++;
               if   (pos[1]>'Z')
                    pos[1] -= 26;
               pos[2]++;
               if   (pos[2]>'Z')
                    pos[2] -= 26;
               flag=0;
          }

/* Step up second rotor if first rotor reached notch */
          if (pos[0]==notch[order[0]-1])
          {
               pos[1]++;
               if   (pos[1]>'Z')
                    pos[1] -= 26;
               if   (pos[1]==notch[order[1]-1])
                    flag=1;
          }

/*Swap pairs of letters on plugboard */

          for  (i=0; plug[i]; i+=2)
          {
               if (ch==plug[i])
                    ch=plug[i+1];
               else if (ch==plug[i+1])
                    ch=plug[i];
          }

/* Rotors (forward) */

          for  (i=0; i<3; i++)
          {
               ch += pos[i]-'A';
               if   (ch>'Z')
                    ch -=26;

               ch -= rings[i]-'A';
               if   (ch<'A')
                    ch +=26;

               ch=rotor[((order[i]-1)*26)+ch-'A'];
               
               ch += rings[i]-'A';
               if   (ch>'Z')
                    ch -=26;

               ch -= pos[i]-'A';
               if   (ch<'A')
                    ch +=26;
          }

/* Reflecting rotor */
          ch=ref[ch-'A'];

/*Rotors (Reverse) */

          for  (i=3; i; i--)
          {
               ch += pos[i-1]-'A';
               if   (ch>'Z')
                    ch -=26;

               ch -= rings[i-1]-'A';
               if   (ch<'A')
                    ch +=26;
          
               for (j=0; j<26; j++)
                    if   (rotor[(26*(order[i-1]-1))+j]==ch)
                         break;
          
               ch=j+'A';
               
               ch += rings[i-1]-'A';
               if   (ch>'Z')
                    ch -=26;

               ch -= pos[i-1]-'A';
               if   (ch<'A')
                    ch +=26;
          }

/* Plugboard */

          for  (i=0; plug[i]; i+=2)
          {
               if (ch==plug[i])
                    ch=plug[i+1];
               else if (ch==plug[i+1])
                    ch=plug[i];
          }

          n++;
          putchar(ch);
          if (n%5==0)
               if (n%55==0)
		    putchar('\n');
               else
                    putchar(' ');
          }
}




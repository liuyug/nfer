#include <stdio.h>
#include <stdarg.h>


#define getdispchar(x) (((x)>31&&(x)<127)?(x):'.')


void output_hex(const unsigned char * buffer, const unsigned int bufflen)
{
/*
0        1         2         3         4         5         6         7         8
12345678901234567890123456789012345678901234567890123456789012345678901234567890
---------+---------+---------+---------+---------+---------+---------+---------+
12 34 56 78 90 AB CD EF  12 34 56 78 90 AB CD EF  |1234567890ABCDEF|
*/
    const char hex_table[] = "0123456789ABCDEF";
    const unsigned char * byte = buffer;
    char line_buffer[82];
    int seps[] = {9, 34, 59};
    int chpos, hexpos;
    unsigned int offset = 0;

    for (int i = 0; i < (bufflen / 16); i++){
        sprintf(line_buffer, "%08X ", offset);
        hexpos = seps[0];
        chpos = seps[2];
        line_buffer[chpos++] = '|';
        for(int k = 0; k < 16; k++){
            if (k == 8) {
                line_buffer[hexpos++] = ' ';
            }
            line_buffer[hexpos++] = hex_table[(*byte) >> 4 & 0xf];
            line_buffer[hexpos++] = hex_table[(*byte) & 0xf];
            line_buffer[hexpos++] = ' ';
            line_buffer[chpos++] = getdispchar(*byte);
            byte++;
        }
        line_buffer[hexpos++] = ' ';
        line_buffer[chpos++] = '|';
        line_buffer[chpos] = '\0';
        offset += 16;
        printf("%s\n", line_buffer);
    }
    if((bufflen % 16) > 0){
        sprintf(line_buffer, "%08X ", offset);
        hexpos = seps[0];
        chpos = seps[2];
        line_buffer[chpos++] = '|';
        for(int k = 0; k < (bufflen % 16); k++){
            if (k == 8) {
                line_buffer[hexpos++] = ' ';
            }
            line_buffer[hexpos++] = hex_table[(*byte) >> 4 & 0xf];
            line_buffer[hexpos++] = hex_table[(*byte) & 0xf];
            line_buffer[hexpos++] = ' ';
            line_buffer[chpos++] = getdispchar(*byte);
            byte++;
        }
        for(int k = hexpos; k < seps[2];k++){
            line_buffer[k] = ' ';
        }
        line_buffer[chpos++] = '|';
        line_buffer[chpos] = '\0';
        printf("%s\n", line_buffer);
    }
    return;
}

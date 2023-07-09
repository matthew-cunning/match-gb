#include <stdio.h>
#include <stdlib.h>

int num = 12345;
unsigned char buffer [sizeof(int)*8+1];
unsigned char score_val[] = { 0x1C,0x1C,0x1C,0x1C,0x1C };

char msg1[] = "Hello World";
char msg1new[] = "Y'all";
// char msg1out[];

char* msg2 = "Hello World";

void ChangeValues(char old[], char new[], char replaceAt);

int main(void) {

    // while(num++ < 12600) { 
    itoa(num,buffer,10);


    int i = 0;
    for (int i = 0; i < 5; i++) {
        printf("Buffer: %s\n", buffer);
        printf("Buffer[%d]: %c\n", i, buffer[i]);

        score_val[i] = buffer[i]- 48 + 0x1C;

        printf("score_val[%d]: %d\n", i, score_val[i]);
        // score_val[i] = numStr[i];
        // printf("%d", score_val[i]);
    }

    // printf("%d\n", numStr[4]);
    // }
    
    // printf("%s\n", &msg1);
    // ChangeValues(msg1, msg1new, getchar());
    // printf("%s\n", &msg1);
}

void ChangeValues(char* old, char* new, char replaceAt) {
    // int i = 0, j = 0;

    while(*old) {
        if(*old == replaceAt) {
            while (*old = *new++) {
                *old++;
            }

            return;
        }
        *old++;
    }

    printf("Char doesn't exist in string\n");
}
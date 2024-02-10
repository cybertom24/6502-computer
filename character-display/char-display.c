/**
 * @file char-display.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-01-15
 * 
 * @copyright Copyright (c) 2024
 * 
 * Installa termios!
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _XOPEN_SOURCE 700
#include <termios.h>
#include <time.h>

int getkey() {
    int character;
    struct termios orig_term_attr;
    struct termios new_term_attr;

    /* set the terminal to raw mode */
    tcgetattr(fileno(stdin), &orig_term_attr);
    memcpy(&new_term_attr, &orig_term_attr, sizeof(struct termios));
    new_term_attr.c_lflag &= ~(ECHO|ICANON);
    new_term_attr.c_cc[VTIME] = 0;
    new_term_attr.c_cc[VMIN] = 0;
    tcsetattr(fileno(stdin), TCSANOW, &new_term_attr);

    /* read a character from the stdin stream without blocking */
    /*   returns EOF (-1) if no character is available */
    character = fgetc(stdin);

    /* restore the original terminal attributes */
    tcsetattr(fileno(stdin), TCSANOW, &orig_term_attr);

    return character;
}

int main()
{
    int key;

    /* initialize the random number generator */
    srand(time(NULL));

    for (;;) {
        key = getkey();
        /* terminate loop on ESC (0x1B) or Ctrl-D (0x04) on STDIN */
        if (key == 0x1B || key == 0x04) {
            break;
        }
        else {
            /* print random ASCII character between 0x20 - 0x7F */
            key = (rand() % 0x7F);
            printf("%c", ((key < 0x20) ? (key + 0x20) : key));
        }
    }

    return 0;
}

#define COLUMNS 80
#define ROWS    40
#define ROWS_TO_SHOW 10

typedef struct {
    char buffer[ROWS][COLUMNS];
} VideoBuffer;

void show(VideoBuffer videoBuffer, int rows2show, int firstRow);
void clear(VideoBuffer *vb);
int newInput(char *input);

int main(int argc, char** argv)
{
    VideoBuffer videobuffer;
    clear(&videobuffer);

    int row = 0, column = 0;
    while(1)
    {
        printf("---------------------------------------------------\n");
        show(videobuffer, ROWS_TO_SHOW, 0);
        
        char input[COLUMNS];
        int len = newInput(input);
        printf("got: ");
        for(int i = 0; i < len - 1; i++)
        {
            printf("%d ", input[i]);
            videobuffer.buffer[row][column] = input[i];
            
            column++;
            if(column >= COLUMNS)
            {
                row++;
                column = 0;
            }
        }
        printf("\n");
    }

    return 0;
}

void show(VideoBuffer videoBuffer, int rows2show, int firstRow)
{
    for(int row = firstRow; row < ROWS && row < firstRow + rows2show; row++)
    {
        int column = 0;
        while(column < COLUMNS)
        {   
            char c = videoBuffer.buffer[row][column];
            if(c == 0)
                break;
            
            printf("%c", c);
            column++;
        }

        printf("\n");
    }
}

void clear(VideoBuffer *vb)
{
    for(int r = 0; r < ROWS; r++)
    {
        for(int c = 0; c < COLUMNS; c++)
        {
            vb->buffer[r][c] = 0;
        }
    }
}

int newInput(char *input)
{
    char data[COLUMNS];
    fgets(data, COLUMNS, stdin);
    int dataLength = strlen(data);

    memcpy(input, data, dataLength);
    return dataLength;
}
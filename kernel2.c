

void printString(char*);


void printString(char *ch)
{
    while (*ch != '\0'){
    putchar(*ch);
interrupt(0x10,0xE*256+*ch,0,0,0);
ch++;

}
}
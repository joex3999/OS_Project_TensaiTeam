void printString(char*);
int main(){
	printString("Hello Awesome World !\0");
while(1){
}
	return 0 ;
}






void printString(char *ch)
{
    while (*ch != '\0'){
   
interrupt(0x10,0xE*256+*ch,0,0,0);
ch++;

}
}

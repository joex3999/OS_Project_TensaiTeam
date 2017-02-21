
void printString(char*);
int getRemainder(int, int );
int DIV(int ,int );
void readSector(char*, int );

int main(){
	char buffer[512];
	printString("Hello Awesome World !\0");

	
	readSector(buffer, 30);
	printString(buffer);
	


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

	void readSector(char*buffer, int sector){

	int relative_sector = ( getRemainder(sector , 18) ) + 1;
	int head = getRemainder(( DIV(sector , 18) ) ,2);
	int track = DIV( sector , 36 );

	printString(DIV(10,3)+"Before Interrupt\0");

 	interrupt(0x13,2*256+1,buffer, track*256+relative_sector, head*256+0);
 	printString("After Interrupt\0");

}


int getRemainder(int num, int divisor)
{

	int i = 1;
    int product = 0;

    if (divisor == 0)
    {
        return -1;
    }
 
    if (divisor < 0) divisor = -divisor;
    if (num < 0)     num = -num;

    
    while (product <= num)
    {
        product = divisor * i;
        i++;
    }

    return num - (product - divisor);
}


int DIV(int num,int den){

    while(getRemainder(num,den)!=0){
    	num--;
    }

    return num/den;

}
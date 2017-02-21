void printString(char*);

int getRemainder(int, int );
int DIV(int ,int );
void readSector(char*, int );
void readString(char*);
int main(){
  char line[80];
	char buffer[512];
  
	printString("Hello Awesome World !\0");
  printString("Enter a line: \0");
	readString(line);
	printString(line);
	readSector(buffer, 30);
	printString(buffer);

	while(1){
	}
	return 0 ;
}

void printString(char *ch){
    while (*ch != '\0'){
   	interrupt(0x10,0xE*256+*ch,0,0,0);
	ch++;
	}
}

void readString(char* line)
{
 int i = 0;
 char in = 0x0;
 while (in != 0xd)
   {
    in = interrupt(0x16, 0x0, 0x0, 0x0, 0x0);
    if (in != 0x8){
    	*(line + i) = in;
    	//interrupt(0x10,0xe*0x100+in,0x0,0x0,0x0);
    		interrupt(0x10,0xE*256+in,0,0,0);
    	i++;
    }else{
    	if(i!=0){
    	*(line+i-1) = in;
    	//interrupt(0x10,0xE*256+0x0,0,0,0);
    	// interrupt(0x10,0xe*0x100+in,0x0,0x0,0x0);
    	// interrupt(0x10,0xe*0x100+0x0,0x0,0x0,0x0);
    	// interrupt(0x10,0xe*0x100+in,0x0,0x0,0x0);
    		interrupt(0x10,0xE*256+0x8,0,0,0);
    			interrupt(0x10,0xE*256+0x0,0,0,0);
    			interrupt(0x10,0xE*256+0x8,0,0,0);
    	 
    	 i--;}
    }

   }
  // interrupt(0x10,0xa,0x0,0x0,0x0);
  // interrupt(0x10,0x0,0x0,0x0,0x0);
  interrupt(0x10,0xE*256+'\n',0,0,0);
  
 *(line + i) = 0xa;
 *(line + i) = 0x0;
 
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
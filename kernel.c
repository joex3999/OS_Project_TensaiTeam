void printString(char*);
void readString(char*);
int getRemainder(int, int );
int DIV(int,int );
void readSector(char*, int );
void handleInterrupt21(int, int, int, int );
void readFile(char*,char* );
void  executeProgram(char*,int );
void terminate();

int main(){
        char line[80];
        char buffer[13312];
        makeInterrupt21();
//PART1:
        //  interrupt(0x21, 0, "hello world !\0", 0, 0); /*print out the file*/
        //  interrupt(0x21, 3, "messag\0", buffer, 0); /*read the file into buffer*/
        //  interrupt(0x21, 0, buffer, 0, 0); /*print out the file*/
//PART2:
        //    interrupt(0x21, 4, "tstprg\0", 0x2000, 0);
//PART3:
        //    interrupt(0x21, 4, "tstpr2\0", 0x2000, 0);
        //   interrupt(0x21, 5,0, 0, 0); /*read the file into buffer*/
//PART4:
interrupt(0x21, 4, "shell\0", 0x2000, 0);
        return 0;


}


void printString(char *ch){
        while (*ch != '\0') {
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
                if (in != 0x8) {
                        *(line + i) = in;
                        //interrupt(0x10,0xe*0x100+in,0x0,0x0,0x0);
                        interrupt(0x10,0xE*256+in,0,0,0);
                        i++;
                }else{
                        if(i!=0) {
                                *(line+i-1) = in;
                                //interrupt(0x10,0xE*256+0x0,0,0,0);
                                // interrupt(0x10,0xe*0x100+in,0x0,0x0,0x0);
                                // interrupt(0x10,0xe*0x100+0x0,0x0,0x0,0x0);
                                // interrupt(0x10,0xe*0x100+in,0x0,0x0,0x0);
                                interrupt(0x10,0xE*256+0x8,0,0,0);
                                interrupt(0x10,0xE*256+0x0,0,0,0);
                                interrupt(0x10,0xE*256+0x8,0,0,0);

                                i--;
                        }
                }

        }
        // interrupt(0x10,0xa,0x0,0x0,0x0);
        // interrupt(0x10,0x0,0x0,0x0,0x0);
        interrupt(0x10,0xE*256+'\n',0,0,0);

        *(line + i) = 0xa;
        *(line + i+1) = 0x0;

}

void readSector(char*buffer, int sector){

        int relative_sector = ( getRemainder(sector, 18) ) + 1;
        int head = getRemainder(( DIV(sector, 18) ),2);
        int track = DIV( sector, 36 );

        //printString(DIV(10,3)+"Before Interrupt\0");

        interrupt(0x13,2*256+1,buffer, track*256+relative_sector, head*256+0);
        //printString("After Interrupt\0");

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
        if (num < 0) num = -num;


        while (product <= num)
        {
                product = divisor * i;
                i++;
        }

        return num - (product - divisor);
}


int DIV(int num,int den){

        while(getRemainder(num,den)!=0) {
                num--;
        }

        return num/den;

}

void handleInterrupt21(int ax, int bx, int cx, int dx){

        if(ax == 0) {
                printString(bx);
        }
        else if(ax == 1) {
                readString(bx);
        }
        else if(ax == 2) {
                readSector(bx,cx);
        }else if(ax==3) {
                readFile(bx,cx);
        } else if(ax==4) {
                executeProgram(bx,cx);
        } else if(ax==5) {
                terminate();
        }
        else {
                printString("ERROR! Invalid interrupt number.\0");
        }


        // printString("Hello World, again!\0");
}

void readFile(char* arr,char* address ){
        char temp [512];
        int i =0;
        int j = 0;
        int found = 0;
        int start = 0;
        readSector(temp,2);
        for(i=0; i <512; i++) {
                if(temp[i]==*(arr+j)) {
                        if(!j) {
                                start = i+6;

                        } j++;
                        if(j>=6||(*arr+j+1)==0) { // could cause errors here ... change late

                                while(temp[start]!=0) {
                                        readSector(address,temp[start]);

                                        address+=512;
                                        start++;
                                }
                                return;
                        }
                }else{
                        j=0;
                }
        }
        return;
}

void  executeProgram(char* name,int segment ){
        char buffer[13312];
        int i = 0;
        readFile(name,buffer);

        for(i =0; i <13312; i++) {
                putInMemory(segment,i,buffer[i]);
        }

        launchProgram(segment);
        return;
}


void terminate(){
  char c [6];
  c[0]='s';
  c[1]='h';
  c[2]='e';
  c[3]='l';
  c[4]='l';
  c[5]='\0';
      interrupt(0x21, 4, c, 0x2000, 0);
}

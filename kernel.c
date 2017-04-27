  void printString(char*);
void readString(char*);
int getRemainder(int, int );
int DIV(int,int );
void readSector(char*, int );
void handleInterrupt21(int, int, int, int );
void readFile(char*,char* );
void executeProgram(char* );
void terminate();
void writeSector(char*,int);
void deleteFile(char*);
void writeFile(char*,char*,int);
void handleTimerInterrupt(int , int );
int mod(int , int );
int currentProcess;
int activeProcesses[8];
int stackPointers[8];
int Processes[8][2]; // 0 Status  // 1 SP
int timer ;
void  executeProgram2(char*,int );
int main(){
        int i=0;
        int j= 0;
      //  char sector[512];

        // char buffer1[13312];
        // char buffer2[13312];
        // buffer2[0]='h'; buffer2[1]='e'; buffer2[2]='l'; buffer2[3]='l';
        // buffer2[4]='o';
        // for(i=5; i<13312; i++) buffer2[i]=0x0;
        currentProcess=0;
        timer = 0 ;
        for(i=0;i<8;i++){
            Processes[i][0]=0;
          Processes[i][1]=0xFF00;
        }


        makeInterrupt21();
        makeTimerInterrupt();


        // for(i=0; i<512; i++)
        //         sector[i]= 255;
        // writeSector(sector,1);
        // interrupt(0x21,8, "testW\0", buffer2, 1); //write file testW
        // interrupt(0x21,3, "testW\0", buffer1, 0); //read file testW
        // interrupt(0x21,0, buffer1, 0, 0); // print out contents of testW

         interrupt(0x21, 4, "hello1\0", 0, 0);
        // interrupt(0x21, 4, "hello2\0", 0, 0);

      //  interrupt(0x21, 4, "shell\0", 0x2000, 0);
         interrupt(0x21, 4, "hello1\0", 0, 0);
        interrupt(0x21, 4, "hello2\0", 0, 0);
        while(1) {

        }
        return 0;
}
int mod(int a, int b) {
  while (a >= b) {
    a -= b;
  }
  return a;
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

void writeSector(char*buffer, int sector){

        int relative_sector = ( getRemainder(sector, 18) ) + 1;
        int head = getRemainder(( DIV(sector, 18) ),2);
        int track = DIV( sector, 36 );

        //printString(DIV(10,3)+"Before Interrupt\0");

        interrupt(0x13,3*256+1,buffer, track*256+relative_sector, head*256+0);
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
void handleTimerInterrupt(int segment, int sp){


    int i = currentProcess+1;
    int j =0 ;
    timer ++;

    if(timer==15){

       timer = 0;
       Processes[currentProcess][1]=sp;

       while(1){
         if(i>=8)i=0;

         if(i==currentProcess)
           break;


         if(Processes[i][0]==1){

            currentProcess= i ;

           interrupt(0x10,0xE*256+'O',0,0,0);
           for(j =0 ; j<8;j++){
             printString("Status of row ");
               interrupt(0x10,0xE*256+(j+48),0,0,0);
                 interrupt(0x10,0xE*256+(Processes[j][0]+48),0,0,0);
           }
 
          returnFromTimer((currentProcess+2) * 0x1000, Processes[currentProcess][1] );

             return ;

         }
         i++;
       }

    }


    returnFromTimer(segment,sp);
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
                executeProgram(bx);
        } else if(ax==5) {
                terminate();
        }else if(ax==6) {
                writeSector(bx,cx);
        }else if(ax==7) {
                deleteFile(bx);
        }else if(ax==8) {
                writeFile(bx, cx, dx);
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
                        if(j>=6||(*(arr+j)+1)==0) {

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
// void readFile(char* arr,char* address ){
//         char temp [512];
//         int i =0;
//         int j = 0;
//         int found = 0;
//         int start = 0;
//         readSector(temp,2);
//         for(i=0; i <512; i+=32) {
//           start = i+6;
//         for(j=0 ;temp[i+j]==*(arr+j);j++){
//           if((temp[i+j+1]==0||j>=6) &&(*(arr+j+1)==0)) {
//             found = 1 ;
//             break;
//           }
//         }
//         if(found){
//           while(temp[start]!=0){
//             readSector(address,temp[start]);
//             address+=512;
//             start++;
//           }
//           return ;
//         }
//         }
//         return;
// }



void deleteFile (char* name){
        char mp [512];
        char dr [512];
        int i =0;
        int j = 0;
        int found = 0;
        int start = 0;
        int x = 0;
        char c [50];
        readSector(mp,1);
        readSector(dr,2);

        for(i=0; i <512; i++) {
                if(dr[i]==*(name+j)) {
                        if(!j) {
                                start = i+6;
                        }
                        // k e r n e l 6
                        j++;
                        if(j>=6||(*name+j+1)==0) {
                                dr[start-6] = 0;

                                //Setting the file sectors in the directory to zeros
                                for( x = 1; x<=26; x++) {
                                        if(dr[i+x] != 0)
                                                mp[dr[i+x]] = 0;  //The index of the sector in the map is the same as the sector number

                                }
                                writeSector(mp,1);
                                writeSector(dr,2);

                                c[0]='F';
                                c[1]='i';
                                c[2]='l';
                                c[3]='e';
                                c[4]=' ';
                                c[5]='D';
                                c[6]='e';
                                c[7]='l';
                                c[8]='e';
                                c[9]='t';
                                c[10]='e';
                                c[11]='d';
                                c[12]=' ';
                                c[13]=' ';
                                c[14]=' ';
                                c[15]=' ';
                                c[16]=' ';
                                c[17]=' ';
                                c[18]=' ';
                                c[19]='\n';
                                c[20]='\b';
                                c[21]='\b';
                                c[22]='\b';
                                c[23]='\b';
                                c[24]='\b';
                                c[25]='\b';
                                c[26]='\b';
                                c[27]='\b';
                                c[28]='\b';
                                c[29]='\b';
                                c[30]='\b';
                                c[31]='\b';
                                c[32]='\b';
                                c[33]='\b';
                                c[34]='\b';
                                c[35]='\b';
                                c[36]='\b';
                                c[37]='\b';
                                c[38]='\b';
                                c[39]='\b';
                                c[40]='\b';
                                c[41]='\b';
                                c[42]='\b';
                                c[43]='\b';
                                c[44]='\b';
                                c[45]='\b';
                                c[46]='\b';
                                c[47]='\0';

                                interrupt(0x21, 0, c, 0, 0);
                                return;
                        }
                }else{
                        j=0;
                }
                if(i==511)
                {

                        c[0]='F';
                        c[1]='i';
                        c[2]='l';
                        c[3]='e';
                        c[4]=' ';
                        c[5]='D';
                        c[6]='o';
                        c[7]='e';
                        c[8]='s';
                        c[9]=' ';
                        c[10]='n';
                        c[11]='o';
                        c[12]='t';
                        c[13]=' ';
                        c[14]='e';
                        c[15]='x';
                        c[16]='i';
                        c[17]='s';
                        c[18]='t';
                        c[19]='\n';
                        c[20]='\b';
                        c[21]='\b';
                        c[22]='\b';
                        c[23]='\b';
                        c[24]='\b';
                        c[25]='\b';
                        c[26]='\b';
                        c[27]='\b';
                        c[28]='\b';
                        c[29]='\b';
                        c[30]='\b';
                        c[31]='\b';
                        c[32]='\b';
                        c[33]='\b';
                        c[34]='\b';
                        c[35]='\b';
                        c[36]='\b';
                        c[37]='\b';
                        c[38]='\b';
                        c[39]='\b';
                        c[40]='\b';
                        c[41]='\b';
                        c[42]='\b';
                        c[43]='\b';
                        c[44]='\b';
                        c[45]='\b';
                        c[46]='\b';
                        c[47]='\0';

                        interrupt(0x21, 0, c, 0, 0);
                }

        }
        return;
}

void writeFile(char* name, char* buffer, int secNum){
        char mp [512];
        char dr [512];
        int i = 0;
        int j = 0;
        int x = 0;
        char c [57];
        c[0]= 'N';
        c[1]= 'o';
        c[2]= ' ';
        c[3]= 'a';
        c[4]= 'v';
        c[5]= 'a';
        c[6]= 'i';
        c[7]= 'l';
        c[8]= 'a';
        c[9]= 'b';
        c[10]= 'l';
        c[11]= 'e';
        c[12]= ' ';
        c[13]= 's';
        c[14]= 'e';
        c[15]= 'c';
        c[16]= 't';
        c[17]= 'o';
        c[18]= 'r';
        c[19]= 's';
          c[20]= '\n';
        c[21]= '\b';
        c[22]= '\b';
        c[23]= '\b';
        c[24]= '\b';
        c[25]= '\b';
        c[26]= '\b';
        c[27]= '\b';
        c[28]= '\b';
        c[29]= '\b';
        c[30]= '\b';
        c[31]= '\b';
          c[32]= '\b';
          c[33]= '\b';
          c[34]= '\b';
          c[35]= '\b';
          c[36]= '\b';
          c[37]= '\b';
          c[38]= '\b';
          c[39]= '\b';
          c[40]= '\b';
          c[41]= '\b';
          c[42]= '\b';
          c[43]= '\b';
            c[44]= '\b';
            c[45]= '\b';
            c[46]= '\b';
            c[47]= '\b';
            c[48]= '\b';
            c[49]= '\b';
            c[50]= '\b';
            c[51]= '\b';
            c[52]= '\b';
            c[53]= '\b';
            c[54]= '\b';
            c[55]= '\b';
              c[56]= '\0';

        readSector(mp,1);
        readSector(dr,2);
        for(i=0; i <512; i+=32) {

                if(dr[i]==0) {

                        //Replace existing characters with the name characters
                        while(*name!=0&&*name!=0x0d&&*name!=0x0a) {
                                dr[i+x]=*name;
                                name++;
                                x++;
                        }

                        //Fill the rest of the 6 characters with zeros
                        while(x<6) {
                                dr[i+x]=0;
                                x++;
                        }

                        //Find empty sectors
                        for(secNum=secNum; secNum != 0; secNum--) {


                                while(mp[j] != 0 && j<512) {
                                        j++;

                                }
                                if(j >= 512) {
                                        printString(c);


                                }else{
                                        mp[j] = 255; //Set byte to 0xFF
                                        dr[i+x] = j;
                                        x++;
                                        writeSector(buffer, j);
                                        buffer += 512;
                                }
                        }

                        //Set the rest of the directory entry to zeros
                        for(x=x; x<32; x++) {
                                dr[i+x]=0;
                        }

                        writeSector(mp,1);
                        writeSector(dr,2);

                        return;

                }
        }

        printString(c);
        return;
}




void  executeProgram(char* name){
        int p = 0;
        int segment=0;
        int j = 0;
        char buffer[13312];
        int i = 0;
        char c [50];
        buffer[0]=0;
        readFile(name,buffer);

        for(p = 0; p < 8; p++)
        if(!Processes[p][0])
          break;

      if(p == 8)
        return;

          setKernelDataSegment();
        Processes[p][0] = 1;
        restoreDataSegment();
              interrupt(0x10,0xE*256+(p+48),0,0,0);
        segment = (p+2) * 0x1000;

        if(buffer[0]!=0x00) {

                for(i =0; i <13312; i++) {
                        putInMemory(segment,i,buffer[i]);
                }


                  initializeProgram(segment);
        }else{

                c[0]='F';
                c[1]='i';
                c[2]='l';
                c[3]='e';
                c[4]=' ';
                c[5]='D';
                c[6]='o';
                c[7]='e';
                c[8]='s';
                c[9]=' ';
                c[10]='n';
                c[11]='o';
                c[12]='t';
                c[13]=' ';
                c[14]='e';
                c[15]='x';
                c[16]='i';
                c[17]='s';
                c[18]='t';
                c[19]='\n';
                c[20]='\b';
                c[21]='\b';
                c[22]='\b';
                c[23]='\b';
                c[24]='\b';
                c[25]='\b';
                c[26]='\b';
                c[27]='\b';
                c[28]='\b';
                c[29]='\b';
                c[30]='\b';
                c[31]='\b';
                c[32]='\b';
                c[33]='\b';
                c[34]='\b';
                c[35]='\b';
                c[36]='\b';
                c[37]='\b';
                c[38]='\b';
                c[39]='\b';
                c[40]='\b';
                c[41]='\b';
                c[42]='\b';
                c[43]='\b';
                c[44]='\b';
                c[45]='\b';
                c[46]='\b';
                c[47]='\0';

                interrupt(0x21, 0, c, 0, 0);
        }
        return;
}




void  executeProgram2(char* name,int segment ){
        char buffer[13312];
        int i = 0;
        char c [50];
        buffer[0]=0;
        readFile(name,buffer);
        if(buffer[0]!=0x00) {

                for(i =0; i <13312; i++) {
                        putInMemory(segment,i,buffer[i]);
                }

                launchProgram(segment);
        }else{

                c[0]='F';
                c[1]='i';
                c[2]='l';
                c[3]='e';
                c[4]=' ';
                c[5]='D';
                c[6]='o';
                c[7]='e';
                c[8]='s';
                c[9]=' ';
                c[10]='n';
                c[11]='o';
                c[12]='t';
                c[13]=' ';
                c[14]='e';
                c[15]='x';
                c[16]='i';
                c[17]='s';
                c[18]='t';
                c[19]='\n';
                c[20]='\b';
                c[21]='\b';
                c[22]='\b';
                c[23]='\b';
                c[24]='\b';
                c[25]='\b';
                c[26]='\b';
                c[27]='\b';
                c[28]='\b';
                c[29]='\b';
                c[30]='\b';
                c[31]='\b';
                c[32]='\b';
                c[33]='\b';
                c[34]='\b';
                c[35]='\b';
                c[36]='\b';
                c[37]='\b';
                c[38]='\b';
                c[39]='\b';
                c[40]='\b';
                c[41]='\b';
                c[42]='\b';
                c[43]='\b';
                c[44]='\b';
                c[45]='\b';
                c[46]='\b';
                c[47]='\0';

                interrupt(0x21, 0, c, 0, 0);
        }
        return;
}

void terminate(){
      setKernelDataSegment();
  Processes[currentProcess][0]=0;

  while(1);


        // char c [6];
        // c[0]='s';
        // c[1]='h';
        // c[2]='e';
        // c[3]='l';
        // c[4]='l';
        // c[5]='\0';
        // interrupt(0x21, 4, c, 0x2000, 0);
}

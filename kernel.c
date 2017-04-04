void printString(char*);
void readString(char*);
int getRemainder(int, int );
int DIV(int,int );
void readSector(char*, int );
void handleInterrupt21(int, int, int, int );
void readFile(char*,char* );
void  executeProgram(char*,int );
void terminate();
void writeSector(char*,int);
void deleteFile(char*);
void writeFile(char*,char*,int);

int main(){

int i=0;
char buffer1[13312];
char buffer2[13312];
buffer2[0]='h'; buffer2[1]='e'; buffer2[2]='l'; buffer2[3]='l';
buffer2[4]='o';
for(i=5; i<13312; i++) buffer2[i]=0x0;
makeInterrupt21();
interrupt(0x21,8, "testW\0", buffer2, 1); //write file testW
interrupt(0x21,3, "testW\0", buffer1, 0); //read file testW
interrupt(0x21,0, buffer1, 0, 0); // print out contents of testW

while(1){

}
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

void handleInterrupt21(int ax, int bx, int cx, int dx){

        if(ax == 0) {
                printString(bx);
        }
        else if(ax == 1) {
                readString(bx);
        }
        else if(ax == 2){
					   	

                readSector(bx,cx);
        }else if(ax==3) {
                readFile(bx,cx);
        } else if(ax==4) {
                executeProgram(bx,cx);
        } else if(ax==5) {
                terminate();
        }else if(ax==6) {
                writeSector(bx,cx);
        }else if(ax==7) {
                deleteFile(bx);
        }else if(ax==8){
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

// void deleteFile(char* arr){
//         char temp [512];
//         int i =0;
//         int j = 0;
//         int found = 0;
//         int start = 0;

//         char c [6];

//         readSector(temp,2);

//         for(i=0; i <512; i++) {
//                 if(temp[i]==*(arr+j)) {
//                         if(!j) {
//                                 start = i+6;

//                         } j++;
//                         if(j>=6||(*(arr+j)+1)==0) {

                                
//                                 temp[start-6]=0x00;
//                                 writeSector(temp,2);
//                                 }
//                                 return;
//                         }
//                 else{
//                         j=0;
//                 }
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
        int x = 0 ;
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
					interrupt(0x10,0xE*256+'G',0,0,0);
					//Setting the file sectors in the directory to zeros
					for( x = 1; x<=26;x++){
					   if(dr[i+x] != 0)
					   mp[dr[i+x]] = 0;  //The index of the sector in the map is the same as the sector number
					
					}
					writeSector(mp,1);
					writeSector(dr,2);
		                        return;
		                }
		        }else{
		                j=0;
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
    readSector(mp,1);
	readSector(dr,2);
	 for(i=0; i <512; i+=32) {

		        if(dr[i]==0) {

		            	//Replace existing characters with the name characters
		            	while(*name != '\0'){
		            		dr[i+x]=*name;
		            		name++;
		            		x++;
		            	}

		            	//Fill the rest of the 6 characters with zeros
		            	while(x<6){
		            		dr[i+x]=0;
		            		x++;
		            	}

		            	//Find empty sectors
		            	for(secNum=secNum ; secNum != 0 ; secNum--){
		            		if(j > 512){
		            			printString("No enough available sectors for file storage.\0");
		            			return;
		            		}

		            		while(mp[j] != 0 && j<512){
		            			j++;
		            			if(j > 512){
		            			printString("No enough available sectors for file storage.\0");
		            			return;
		            			}
		            		}

		            		mp[j] = 255;   //Set byte to 0xFF
		            		dr[i+x] = j;
		            		x++;
		            		writeSector(buffer, j);
		            		buffer += 512;
		            	}

		            	//Set the rest of the directory entry to zeros
		            	for(x=x; x<32; x++){
		            		dr[i+x]=0;
		            	}

		            	writeSector(mp,1);
						writeSector(dr,2);

		            	return;      
		        
		        }
	}

	printString("No available directory entries.\0");
	return;
}

void  executeProgram(char* name,int segment ){
        char buffer[13312];
        int i = 0;
        char c [50];
        buffer[0]=0;
        readFile(name,buffer);
        if(buffer[0]!=0x00){

        for(i =0; i <13312; i++) {
                putInMemory(segment,i,buffer[i]);
        }

        launchProgram(segment);}else{

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
  char c [6];
  c[0]='s';
  c[1]='h';
  c[2]='e';
  c[3]='l';
  c[4]='l';
  c[5]='\0';
      interrupt(0x21, 4, c, 0x2000, 0);
}

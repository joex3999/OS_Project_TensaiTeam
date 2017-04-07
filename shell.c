int string_compare(char*, char*);
void copy(char*, char*,int );
void getFiles(char*);
void createFile(char*);
int ceil (int);
void copyCommand(char*);
int DIV(int,int );
int getRemainder(int, int);
void stringSplit(char*,char*,char*);
void printNum(int);
int getSectorNum(char*);
int main(){
        char line[40];
        char buffer[13312];
        char sector[512];
        int first = 0;
        int second = 0;
        int third = 0;
        int fourth = 0;
        int fifth = 0;
        int sixth =0;
        char word [40];
        while(1) {
                interrupt(0x21, 0, "SHELL:>\0", 0, 0); /*print out the file*/
                interrupt(0x21,1,line,0,0);


                first = string_compare(line,"view\0");
                second = string_compare(line,"execute\0");
                third = string_compare(line,"delete\0");
                fourth = string_compare(line,"dir\0");
                fifth = string_compare(line,"create\0");
                sixth = string_compare(line,"copy\0");
                if(first) {
                        copy(line,word,5);
                        buffer[0]=0x00;
                        interrupt(0x21, 3,word, buffer, 0);         /*read the file into buffer*/
                        if(buffer[0]!=0x00)
                                interrupt(0x21, 0, buffer, 0, 0);  /*print out the file*/
                        else
                                interrupt(0x21, 0, "Could not find file to view\n\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\0", 0, 0);


                }else

                if(second) {
                        copy(line,word,8);
                        interrupt(0x21, 4, word, 0x2000, 0);


                }else

                if(third) {
                        copy(line,word,7);
                        interrupt(0x21, 7, word, 0, 0);

                }else if (fourth) {


                       interrupt(0x21, 2,sector, 2, 0);
                       getFiles(sector);

                }else if (fifth) {
                        copy(line,word,7);
                        createFile(word);

                }else if(sixth) {
                        copy(line,word,5);
                       copyCommand(word);

                }else{
                        interrupt(0x21, 0, "Bad Command!\n\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\0", 0, 0);


                }
        }
        interrupt(0x10,0xE*256+'\n',0,0,0);
}


int string_compare(char* str1, char* str2)
{
        int ctr=0;
        while(*str1==*str2)
        {


                if(*str1=='\0'||*str2=='\0')
                        break;

                str1++;
                str2++;
        }



        if(*str2=='\0') {

                return 1;

        }
        else{

                return 0;

        }
}
void getFiles(char* pointer){
        int i;
        int s;
        int sum=0;
        for(i = 0; i<512; i+=32) {
                if(*(pointer+i)!=0) {
                        interrupt(0x10,0xE*256+'\b',0,0,0);
                        interrupt(0x10,0xE*256+'\b',0,0,0);
                        interrupt(0x10,0xE*256+'\b',0,0,0);
                        interrupt(0x10,0xE*256+'\b',0,0,0);
                        interrupt(0x10,0xE*256+'\b',0,0,0);
                        interrupt(0x10,0xE*256+'\b',0,0,0);
                        interrupt(0x10,0xE*256+'\b',0,0,0);
                        interrupt(0x10,0xE*256+'\b',0,0,0);
                        interrupt(0x10,0xE*256+'$',0,0,0);
                        interrupt(0x10,0xE*256+'\0',0,0,0);
                        for(s=0; s<6; s++) {
                          if(*(pointer+i+s)>=48&&*(pointer+i+s)<=122)
                                interrupt(0x10,0xE*256+*(pointer+i+s),0,0,0);
                                else{
                                    interrupt(0x10,0xE*256+' ',0,0,0);
                                }


                        }
                        s=6;
                        sum = 0 ;
                        while(s<32){
                          if(*(pointer+i+s)!=0){
                            sum++;
                          }else
                          break;
                          s++;
                        }
                        interrupt(0x10,0xE*256+' ',0,0,0);
                        interrupt(0x10,0xE*256+' ',0,0,0);
                        interrupt(0x10,0xE*256+' ',0,0,0);
                        interrupt(0x10,0xE*256+' ',0,0,0);
                        interrupt(0x10,0xE*256+' ',0,0,0);
                        printNum(sum);
                        interrupt(0x10,0xE*256+'\n',0,0,0);

                        interrupt(0x10,0xE*256+'\r',0,0,0);
                        interrupt(0x10,0xE*256+'\0',0,0,0);
                }


        }
}
void copy(char*str1, char* str2,int s){
        str1+=s;
        while(*str1!='0') {
                *str2=*str1;
                str2++;
                str1++;
        }
        *str2='\0';
        return;
}

void stringSplit(char*str1, char* str2, char* str3 ){

        while(*str1!=' ') {
                *str2=*str1;
                str2++;
                str1++;

        }


    *str1++;
        while(*str1!='\0') {
                *str3=*str1;
                str3++;
                str1++;

        }

        *str2='\0';
        *str3='\0';

        return;

}

void createFile(char*fileName){
        char buffer[13312];
        char line[13312];
        int pos = 0;
        int pos2;
        int i =0;
        int secNum =0;
        for(i=0 ; i<13312;i++)
        buffer[i]=0;
        while(1) {
                pos2 = 0;
                interrupt(0x10,0xE*256+'$',0,0,0);
                interrupt(0x21,1,line,0,0);
                if(line[0]!=0xd) {
                        while(line[pos2]!='\0') {
                                buffer[pos]=line[pos2];
                                pos++;
                                pos2++;
                        }

                }else{
                        break;
                }

        }

        secNum = ceil(pos);
        interrupt(0x21, 8,fileName, buffer, secNum);

}

int ceil(int num ){
        int result =0;
        while(num>0) {
                result++;
                num -=512;
        }
        return result;
}

void copyCommand(char* str1){
        int x=0;
        int i=0;
        char b1[13312];
        char name1[20];
        char name2[20];
        char sector[512];
        char toString [7];
        int secNum =0;
        char c[55];


        interrupt(0x21, 2,sector, 2, 0);
        stringSplit(str1,name1,name2);

        secNum = getSectorNum(name1,sector);
        if(secNum!=-1){
      interrupt(0x21, 3,name1, b1, 0);
      interrupt(0x21, 8, name2, b1, secNum);

                              c[0]='F';
                              c[1]='i';
                              c[2]='l';
                              c[3]='e';
                              c[4]=' ';
                              c[5]='c';
                              c[6]='o';
                              c[7]='p';
                              c[8]='i';
                              c[9]='e';
                              c[10]='d';
                              c[11]=' ';
                              c[12]='s';
                              c[13]='u';
                              c[14]='c';
                              c[15]='c';
                              c[16]='e';
                              c[17]='s';
                              c[18]='s';
                              c[19]='f';
                              c[20]='u';
                              c[21]='l';
                              c[22]='l';
                              c[23]='y';
                              c[24]='\n';
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
                              c[47]='\b';
                              c[48]='\b';
                              c[49]='\b';
                              c[50]='\b';
                              c[51]='\b';
                              c[52]='\b';
                              c[53]='\0';

                              interrupt(0x21, 0, c, 0, 0);
    }
      else
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

        return;

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

void printNum(int num){
        char arr[3];
            int pos = 0;
        arr[2]=-1;
        arr[1]=-1;
        arr[0]=-1;

        while(num>0) {
          arr[pos]= getRemainder(num,10);
          num/=10;
            pos++;

        }

        for(pos=2;pos>=0;pos--){
          if(arr[pos]!=-1)
            interrupt(0x10,0xE*256+(arr[pos]+48),0,0,0);
        }
}

int getSectorNum(char* name,char* pointer){
  int i;
  int s;
  int sum=0;
  int brek = 0 ;
  for(i = 0; i<512; i+=32) {
          if(*(pointer+i)!=0&&*(pointer+i)==*name) {

                  for(s=0; s<6&&*(pointer+i+s)!=0; s++) {
                      if(*(pointer+i+s)!=*(name+s))
                      {
                        brek = 1 ;
                        break;
                      }

                  }
                  if(brek)
                  continue;
                  s=6;
                  sum = 0 ;
                  while(s<32){
                    if(*(pointer+i+s)!=0){
                      sum++;
                    }else
                    break;
                    s++;
                  }
                  return sum ;
          }


  }
  return -1 ;
}

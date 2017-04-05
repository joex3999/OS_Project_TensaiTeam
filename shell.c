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
int main(){
        char line[30];
        char buffer[13312];
        char sector[512];
        int first = 0;
        int second = 0;
        int third = 0;
        int fourth = 0;
        int fifth = 0;
        int sixth =0;
        char word [30];
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
                        interrupt(0x10,0xE*256+'$',0,0,0);
                        interrupt(0x10,0xE*256+'\0',0,0,0);
                        for(s=0; s<6; s++) {
                                interrupt(0x10,0xE*256+*(pointer+i+s),0,0,0);

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
                        printNum(sum);
                        interrupt(0x10,0xE*256+'\n',0,0,0);

                        interrupt(0x10,0xE*256+'\r',0,0,0);
                        interrupt(0x10,0xE*256+'\0',0,0,0);
                }

              //  i+=31;
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
        str1++;
        while(*str1!=' ') {
                *str2=*str1;
                str2++;
                str1++;
        }

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
        char name1[6];
        char name2[6];
        char toString [7];
        int secNum =0;

        interrupt(0x21, 0, str1, 0, 0);


        stringSplit(str1,name1,name2);

        interrupt(0x21, 0, name1, 0, 0);
        interrupt(0x21, 0, "\n\0", 0, 0);
        interrupt(0x21, 0, name2, 0, 0);

        b1[0]=0x00;
        x=1;
        interrupt(0x21, 3,name1, b1, 0);

        if(b1[0]!=0x00 ) {
                while(b1[x]!=0x00 && x < 13312) {
                        secNum++;
                        x++;
                }
                secNum++;
                secNum=DIV(secNum,512);
                secNum++;
                interrupt(0x21, 8, name2, b1, secNum);
                interrupt(0x21, 0, "Copy Successful\n\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\0", 0, 0);
                return;


        }else{
                interrupt(0x21, 0, "Could not find file to Copy From\n\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\0", 0, 0);
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

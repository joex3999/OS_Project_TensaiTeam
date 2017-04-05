int string_compare(char*, char*);
void copy(char*, char*,int );
void getFiles(char*);
void createFiles();
int main(){
        char line[30];
        char buffer[13312];
        char sector[512];
        int first = 0;
        int second = 0;
        int third = 0;
        int fourth = 0;
        char word [30];
        while(1) {
                interrupt(0x21, 0, "SHELL:>\0", 0, 0); /*print out the file*/
                interrupt(0x21,1,line,0,0);


                first = string_compare(line,"view\0");
                second = string_compare(line,"execute\0");
                third = string_compare(line,"delete\0");
                fourth = string_compare(line,"dir\0");

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
        for(i = 0; i<512; i++) {
                if(*(pointer+i)!=0) {
                        interrupt(0x10,0xE*256+'>',0,0,0);
                        interrupt(0x10,0xE*256+'\r',0,0,0);
                        interrupt(0x10,0xE*256+'\0',0,0,0);
                        for(s=0; s<6; s++) {
                                interrupt(0x10,0xE*256+*(pointer+i+s),0,0,0);

                        }
                        interrupt(0x10,0xE*256+'\n',0,0,0);
                        interrupt(0x10,0xE*256+'\r',0,0,0);
                        interrupt(0x10,0xE*256+'\0',0,0,0);
                }

                i+=31;
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

void createFiles(){

}

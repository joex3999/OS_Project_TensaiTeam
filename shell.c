int string_compare(char*, char*);
void copy(char*, char*,int );
int main(){
        char line[30];
        char buffer[13312];
        int first = 0;
        int second = 0;
        int third = 0;
        char word [30];
        while(1) {
                interrupt(0x21, 0, "SHELL:>\0", 0, 0); /*print out the file*/
                interrupt(0x21,1,line,0,0);


                first = string_compare(line,"view\0");
                second = string_compare(line,"execute\0");


                if(first) {
                        copy(line,word,5);

                                interrupt(0x21, 3,word, buffer, 0); /*read the file into buffer*/
                                if(buffer[0]!=0x00)
                                interrupt(0x21, 0, buffer, 0, 0); /*print out the file*/
                                else
                                  interrupt(0x21, 0, "Could not find file to view\n\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\0", 0, 0);


                }else

                if(second) {
                    copy(line,word,8);
                            
                                interrupt(0x21, 4, word, 0x2000, 0);


                }


                else{
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
void copy(char*str1, char* str2,int s){
    str1+=s;
    while(*str1!='0'){
      *str2=*str1;
      str2++;
      str1++;
    }
    *str2='\0';
    return;
}

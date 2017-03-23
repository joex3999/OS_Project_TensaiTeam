int string_compare(char*, char*);
int main(){
        char line[30];
        char buffer[13312];
        int first = 0 ;
        int second = 0 ;
        while(1) {
                interrupt(0x21, 0, "SHELL:>\0", 0, 0); /*print out the file*/
                interrupt(0x21,1,line,0,0);

                 first = string_compare(line,"view messag\0");
               second = string_compare(line,"execute tstpr2\0");
              if(first) {
                        interrupt(0x21, 3, "messag\0", buffer, 0); /*read the file into buffer*/
                        interrupt(0x21, 0, buffer, 0, 0); /*print out the file*/

                }else

                if(second) {
                        interrupt(0x21, 4, "tstpr2\0", 0x2000, 0);

                }


                else{
                        interrupt(0x21, 0, "Bad Command!\n\0", 0, 0);

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



        if(*str2=='\0'){

                return 1;

              }
        else{

                return 0;

              }
}

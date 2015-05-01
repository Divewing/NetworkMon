#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ncurses.h>
#include <time.h>

char* cmd_system(char intfc[100],int tipe)
{
    char command[100];
    
    strcpy(command,"cat /sys/class/net/");
    strcat(command,intfc);
    if(tipe==0){
        strcat(command,"/statistics/rx_bytes");
    }else if(tipe==1){
        strcat(command,"/statistics/tx_bytes");
    }
    
    char* result = "";
    FILE *fpRead;
    fpRead = popen(command, "r");
    char buf[1024];
    memset(buf,'\0',sizeof(buf));
    while(fgets(buf,1024-1,fpRead)!=NULL)
    {
        result = buf;
    }
    if(fpRead!=NULL)
       pclose(fpRead);
    return result;
}


void writetofile(char *det){
    time_t now;
    time(&now);
    strcat(det," #d : ");
    strcat(det,ctime(&now));
    mvprintw(4,0,"%s",ctime(&now));
    FILE *datbase;
    datbase = fopen("data","w");

    fprintf(datbase,"%s\n",det);
   
    fclose(datbase);

}
int main(int argc, char *argv[]){
initscr(); //Creates stdscr
mvprintw(0,0,"Downloaded :");
mvprintw(1,0,"Uploaded   :");
mvprintw(2,0,"Download   :");
mvprintw(3,0,"Upload     :");
mvprintw(2,20,"KB/s");
mvprintw(3,20,"KB/s");
char intfc[100];
char detail[100];
char cv[100];

int hasil;
int delay;
int i=1;

float kb;
float kbdlama=0.0;
float kbulama=0.0;
float speed;


if(argc!=1){
    for(i;i<argc;i++){
        if(strcmp(argv[i],"-i")==0){
            i=i+1;
            strcpy(intfc,argv[i]);
        }else if(strcmp(argv[i],"-d")==0){
            i=i+1;
            delay=atoi(argv[i]);
        }
    }
}else{
    strcpy(intfc,"wlan0");
    delay=1;
};

//for input check
//mvprintw(5,0,"%s %s %s %s %s",argv[0],argv[1],argv[2],argv[3],argv[4]);
//mvprintw(6,0,"%s %s %s %s %i",argv[0],argv[1],intfc,argv[3],delay);
//delay=1;
//strcpy(intfc,"wlan0");
for(;;){
    strcpy(detail,"#if : ");
    strcat(detail,intfc);
    
    //Totaldownloaded
    hasil = atoi(cmd_system(intfc,0));
    kb = hasil/1000.0;
    mvprintw(0,13,"%i",hasil);
    //Download speed
    if(kbdlama==0.0){
        mvprintw(2,13,"0.0");
    }else{
        speed = kb-kbdlama;
        mvprintw(2,13,"%.1f",speed);
    }
    kbdlama=kb;
    
    sprintf(cv,"%d",hasil);
    strcat(detail," #dt : ");
    strcat(detail,cv);
    
    //Totaluploaded
    hasil = atoi(cmd_system(intfc,1));
    kb = hasil/1000.00;
    mvprintw(1,13,"%i",hasil);

    //Upload speed
    if(kbulama==0.0){
        mvprintw(3,13,"0.0");
    }else{
        speed = kb-kbulama;
        mvprintw(3,13,"%.1f",speed);
    }
    
    kbulama=kb;
    
    sprintf(cv,"%d",hasil);
    strcat(detail," #ut : ");
    strcat(detail,cv);
    
    refresh();//Refresh the number
    writetofile(detail);
    sleep(delay);//Delay 1 second
}
endwin();
return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ncurses.h>
#include <time.h>

char* cmd_system(const char* command)
{
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

int main(int argc, char* argv){
initscr(); //Creates stdscr
mvprintw(0,0,"Downloaded :");
mvprintw(1,0,"Uploaded   :");
mvprintw(2,0,"Download :");
mvprintw(3,0,"Upload   :");
mvprintw(2,20,"KB/s");
mvprintw(3,20,"KB/s");
char command[100];
int hasil;
float kb;
float kbdlama=0.0;
float kbulama=0.0;
float speed;
for(;;){
    //Totaldownloaded
    strcpy(command,"cat /sys/class/net/wlan0/statistics/rx_bytes");
    hasil = atoi(cmd_system(command));
    kb = hasil/1000.0;
    mvprintw(0,13,"%i",hasil);
    //Download speed
    if(kbdlama==0.0){
        mvprintw(2,11,"0.0");
    }else{
        speed = kb-kbdlama;
        mvprintw(2,11,"%.1f",speed);
    }
    kbdlama=kb;
    
    //Totaluploaded
    strcpy(command,"cat /sys/class/net/wlan0/statistics/tx_bytes");
    hasil = atoi(cmd_system(command));
    kb = hasil/1000.00;
    mvprintw(1,13,"%i",hasil);

    //Upload speed
    if(kbulama==0.0){
        mvprintw(3,11,"0.0");
    }else{
        speed = kb-kbulama;
        mvprintw(3,11,"%.1f",speed);
    }
    
    kbulama=kb;
    
    refresh();//Refresh the number
    
    sleep(1);//Delay 1 second
}
endwin();
return 0;
}
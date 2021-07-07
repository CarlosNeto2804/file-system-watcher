#include<stdio.h>
#include<sys/inotify.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include<fcntl.h> 
 
#define MAX_EVENTS 1024  
#define LEN_NAME 16  
#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( MAX_EVENTS * ( EVENT_SIZE + LEN_NAME ))
 
int fd,wd;
 
void sig_handler(int sig){
 
       inotify_rm_watch( fd, wd );
       close( fd );
       exit( 0 );
 
}
 
 
int main(int argc, char **argv){
 
 
       char *path_to_be_watched;
       signal(SIGINT,sig_handler);
 
       path_to_be_watched = argv[1];
 
       fd = inotify_init();
 
 
       if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0) 
        exit(2);
 
  
       wd = inotify_add_watch(fd,path_to_be_watched,IN_MODIFY | IN_CREATE | IN_DELETE);
 
       if(wd==-1){
               printf("Não está observando: %s\n",path_to_be_watched);
       }
       else{
              printf("Observando : %s\n",path_to_be_watched);
       }
 
 
       while(1){
 
              int i=0,length;
              char buffer[BUF_LEN];
 
              length = read(fd,buffer,BUF_LEN);
 
              while(i<length){
 
                struct inotify_event *event = (struct inotify_event *) &buffer[i];
 
                  if(event->len){
                   if ( event->mask & IN_CREATE ) {
                   if ( event->mask & IN_ISDIR ) {
                     printf( "A pasta %s foi criado.\n", event->name );
                     }
                     else {
                       printf( "O arquivo %s foi criado.\n", event->name );
                    }
                    }
                    else if ( event->mask & IN_DELETE ) {
                    if ( event->mask & IN_ISDIR ) {
                      printf( "A pasta %s foi apagado.\n", event->name );
                    }
                    else {
                      printf( "O arquivo %s foi apagado.\n", event->name );
                    }
                    }
                    else if ( event->mask & IN_MODIFY ) {
                    if ( event->mask & IN_ISDIR ) {
                      printf( "A pasta %s foi modificada\n", event->name );
                    }
                    else {
                     printf( "O arquivo %s foi modificado\n", event->name );
                    }
                    }
                   }
                   i += EVENT_SIZE + event->len;
          }
    }
}


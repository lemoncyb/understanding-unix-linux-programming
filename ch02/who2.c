 /* who2.c - read/etc/utmp and list info therein
  *        - suppresses empty records
  *        - formats time nicely
  * */
#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

/* #define SHOWHOST */

void showtime(long);
void show_info(struct utmp * );

/*
 *  show info()
 *  displays contents of the utmp struct in human readable form
 *  * displays nothing if record has no user name
 */
void show_info(struct utmp * utbufp){
    if ( utbufp->ut_type != USER_PROCESS)
        return;

    printf("% - 8.8s", utbufp->ut_name);      /* the logname */
    printf(" ");                               /* a space */
    printf("% - 8.8s", utbufp->ut_line);      /* the tty */
    printf(" ");                               /* a space */
    showtime(utbufp->ut_time);      /* display time */
#ifdef SHOWHOST
    if (utbufp->ut_host[0] != '\0')
        printf("( %s )", utbufp->ut_host);        /* the host */
#endif
    printf("\n");                             /* newline */
}

/* 
 * displays time in a format fit for human consumption
 * uses ctime to build a string then picks parts out of it
 * Note: %12.12s prints a string 12 chars wide and LIMITS
 * it to 12chars.
 * */
void showtime(long timeval){
    char * cp;               /* to hold address of time */
    cp = ctime(&timeval);    /* convert time to string */
    printf("% 12.12s", cp+4);
}

int main(){
    struct utmp  current_record;  /* read info into here */
    int          utmpfd;          /* read from this descriptor */

    if ( (utmpfd = open(UTMP_FILE, O_RDONLY)) == -1 ){
        perror(UTMP_FILE);        /* UTMP_FILE is in utmp.h */
	exit(1);
    }

    while( read(utmpfd, &current_record, reclen) == sizeof(utbuf) )
	show_info(&utbuf);
    close(utmpfd);

    return 0;                        /* went ok */
}

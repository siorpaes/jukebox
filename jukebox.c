/** \file jukebox.c
 * \author david.siorpaes@gmail.com
 *
 * Implements a very simple client that connects to a VLC instance running
 * in server mode.
 * A numeric keypad is used as player controller
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include "jukebox.h"

int state = IDLE;
char jbKeys[3];
int jbKeyIndex;

int main(int argc, char** argv)
{
  int hrawfd;
  int ret, i;
  uint8_t hrBuffer[128];

  hrawfd = open("/dev/hidraw1", O_RDONLY);
  if(hrawfd < 0){
    fprintf(stderr, "Error opening hidraw device: %s\n", strerror(errno));
    return errno;
  }

  while(1){
    /* Process input keys */
    ret = read(hrawfd, hrBuffer, 8);
    if(ret != 8)
      printf("Warning!! Read %i bytes\n", ret); //TODO cope with this

    for(i=0; i<8; i++){
      if(hrBuffer[i] != 0){
	processEntry(hrBuffer[i]);
      }
    }
    printf("\n");
  }
  
  return 0;
}


int processEntry(uint8_t entry)
{
  int number;
  int ret, i;

  ret = isNumber(entry, &number);
  //printf("Key 0x%02x num %i %s", entry, number, (ret ? "number" : "not number"));

  /* Manage numbers entry */
  if(ret){
    if(state == IDLE){
      jbKeys[0] = number;
      jbKeyIndex = 1;
      state = ENTERING;
    }
    else{
      if(jbKeyIndex < sizeof(jbKeys)){
	jbKeys[jbKeyIndex] = number;
	jbKeyIndex++;
      }
    }
  }
  /* Not a number */
  else{
    if(number == ENTER){
      if(state == IDLE){
	/* Reset everything */
	printf("Stop\n");
	jbKeyIndex = 0;
      }
      else{
	printf("Playing song ");
	for(i=0; i<jbKeyIndex; i++)
	  printf("%i", jbKeys[i]);
	printf("\n");
	state = IDLE;
      }
    }

    if(number == PLUS){
      printf("Volume Up\n");
    }

    if(number == MINUS){
      printf("Volume Down\n");
    }

    if(number == DOT){
      printf("Pause\n");
    }

    if(number == SLASH){
      printf("Previous song\n");
    }
    
    if(number == STAR){
      printf("Next song\n");
    }
  }

  return 0;
}


/** If a number is pressed, return its value. Otherwise, pass hidraw code
 */
int isNumber(int hrcode, int* number)
{
  if(hrcode == ZERO){
    *number = 0;
    return 1;
  }

  if(hrcode >= ONE && hrcode <= NINE){
    *number = hrcode - ONE + 1;
    return 1;
  }

  *number = hrcode;

  return 0;
}


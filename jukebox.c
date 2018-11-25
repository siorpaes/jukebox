/** \file jukebox.c
 * \author david.siorpaes@gmail.com
 *
 * Implements a very simple client that connects to a VLC instance running
 * in server mode:
 * vlc -vvv --intf rc --rc-host :50000
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
char mediaPath[256];

int main(int argc, char** argv)
{
  int hrawfd;
  int ret, i;
  uint8_t hrBuffer[128];

  if(argc != 2){
    fprintf(stderr, "Usage: %s <mediapath>\n", argv[0]);
    return -1;
  }

  strcpy(mediaPath, argv[1]);

  hrawfd = open("/dev/hidraw1", O_RDONLY);
  if(hrawfd < 0){
    fprintf(stderr, "Error opening hidraw device: %s\n", strerror(errno));
    return errno;
  }

  while(1){
    /* Process input keys */
    ret = read(hrawfd, hrBuffer, 8);
    if(ret != 8)
      fprintf(stderr, "Warning!! Read %i bytes\n", ret); //TODO cope with this

    for(i=0; i<8; i++){
      if(hrBuffer[i] != 0){
	processEntry(hrBuffer[i]);
      }
    }
  }
  
  return 0;
}


int processEntry(uint8_t entry)
{
  int number;
  int ret, i;
  char digit;

  ret = isNumber(entry, &number);

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
      /* Clear playlist */
      sendCommand("clear\n");
      
      if(state == IDLE){
	jbKeyIndex = 0;
      }
      else{
	sendCommand("add ");	
	sendCommand(mediaPath);
	digit = '/';
	sendCommand(&digit);
	for(i=0; i<jbKeyIndex; i++){
	  digit = '0' + jbKeys[i];
	  sendCommand(&digit);
	}
	sendCommand("\n");
	state = IDLE;
      }
    }

    if(number == PLUS){
      sendCommand("volup\n");
    }

    if(number == MINUS){
      sendCommand("voldown\n");
    }

    if(number == DOT){
      sendCommand("pause\n");
    }

    if(number == SLASH){
      sendCommand("prev\n");
    }
    
    if(number == STAR){
      sendCommand("next\n");
    }

    if(number == NLOCK){
      sendCommand("fastforward\n");
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


int sendCommand(char* command)
{
  int ret;
  ret = write(STDOUT_FILENO, command, strlen(command));
  return ret;
}

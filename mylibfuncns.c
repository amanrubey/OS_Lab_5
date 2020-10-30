#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<errno.h>
#include "myheader.h"

typedef struct {
  int frm_no;
  int valid_bit;
  int modify_bit;
  int reference_bit;
}pageItem;

int pagesize = rec_per_page * RECORDSIZE;
char * Frames[no_of_frames];
int fd;
int FrameTable[no_of_frames];
int page_fault_count = 0;
pageItem PageTable[PTABLESIZE];

/*  **********************    FUNCTIONS WRITTEN BY STUDENT    ************************************ */

/* REWRITE THIS FUNCTION PROPERLY   */
int getFrameNo(int pno){
  int fno;

  /* Replace the following two lines of codes by proper logic */
  fno = 0;
  readPage(pno, fno);

  /*  Scan through the page table to check if the page is present in memory */

  /* If found, simply return the corresponding frame no.  */

  /* If not found, find a free frame-no. by searching the frame table */

  /* If found, read the requested page into the free frame, and then return the corresponding frame no. */

  /* If no free frame found, invoke your Page-Replacement-Algorithm to find victim page-no */

  /*  Scan through the page table to get the frame-no. corresponding to the victim page-no */

  /* Write the victim page back into the disk-file, IF modify_bit is set */

  /* Read the requested page into the freed frame, and then return the corresponding frame no. */

  return fno;
}

/*  ***************************************  FUNCTION DEFINITIONS GO HERE  *******************************      */
/******************************************************************************************************************/
/******************************************************************************************************************/
/******************************************************************************************************************/

/* Initialization Function: Initializes all aspects */
void Initialize(){
  int i;
  /*  Allocating Memory to Frames, and initializing the FrateTable */
  for(i = 0; i < no_of_frames; i++){
    Frames[i] = (char *) calloc(pagesize, sizeof(char));
    FrameTable[i] = -1; // -1 indicates frame is empty
  }
  /* Initializing the PageTable */
  for(i = 0; i < PTABLESIZE; i++){
    PageTable[i].frm_no = -1;
    PageTable[i].valid_bit = 0;
    PageTable[i].modify_bit = 0;
    PageTable[i].reference_bit = 0;
  }
  /*  Opening the Data File  */
  fd = open("./student-data.csv", O_RDWR );
	if(fd<0){
		printf("Error Number % d\n", errno);
        perror("Program");
		exit(1);
	}
}

/* Finalization Function: Finalizes all aspects */
void Finalize(){
  int i;
  /* Write back all non-empty frames to file, and freeing the memory allocated to all frames */
  for(i = 0; i < no_of_frames; i++){
    if(FrameTable[i] != -1){
      writeFrame(i, FrameTable[i]);
    }
    free(Frames[i]);
  }
  /* Closing the Data File */
  close(fd);
  if(fd<0)printf("Error! Could not close the data file...\n");
}

/* Reads specified page from disk-file to the specified Frame in memory */
void readPage(int pno, int frm_no){
  lseek(fd, (pno*pagesize), SEEK_SET);
  read(fd, Frames[frm_no], pagesize);
  FrameTable[frm_no] = pno;
  page_fault_count++;
}

/* Writes the contents of the specified frame in memory to the specified page of the disk-file */
void writeFrame(int frm_no, int pno){
  int res;
  lseek(fd, (pno*pagesize), SEEK_SET);
  res = write(fd, Frames[frm_no], pagesize);
  if(res<0){
    perror("Write Error.");
    exit(1);
  }
}

/* Displays the contents of the specified memory-frame */
void printFrame(int frm_no){
  int i;
  for(i=0; i < pagesize; i++){
    printf("%c",Frames[frm_no][i]);
  }
  printf("\n");
}

/* Translates the specified slno (record-number) to its corresponding page no. and page offset values */
void getPageDetails(int slno, int *pno, int *offset){
  int q, r;
  q = (slno-1)/rec_per_page;
  r = (slno-1) - (rec_per_page*q);
  *pno = q;
  *offset = r;
}

/* Displays the RECORD contained at the specified offset of the specified memory-frame */
void printRecord(int frm_no, int offset){
  int i, startindx, endindx;
  startindx = offset*RECORDSIZE;
  endindx = startindx + RECORDSIZE;
  for(i = startindx; i < endindx; i++){
    printf("%c",Frames[frm_no][i]);
  }
  printf("\n");
}


/* Updates the Name Field of the RECORD contained at the specified offset of
the specified memory-frame, with the given string */
void updateRecord(int frm_no, int offset, char *string){
  int i, startindx, endindx;
  if(strlen(string) != 4){
    printf("Length of the input string must be equal to 4.\n");
    exit(0);
  }
  else{
    startindx = (offset*RECORDSIZE) + 14;
    for(i = 0; i < 4; i++) Frames[frm_no][startindx++] = string[i];
  }
}

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>

#define MAXSTR 1000

int main(int argc, char *argv[])
{
  char line[MAXSTR];
  int *page_table, *mem_map;
  unsigned int log_size, phy_size, page_size, d;
  unsigned int num_pages, num_frames;
  unsigned int offset, logical_addr, physical_addr, page_num, frame_num;
  uint32_t offset_bits;

  /* Get the memory characteristics from the input file */
  fgets(line, MAXSTR, stdin);
  if((sscanf(line, "Logical address space size: %d^%d", &d, &log_size)) != 2){
    fprintf(stderr, "Unexpected line 1. Abort.\n");
    exit(-1);
  }
  fgets(line, MAXSTR, stdin);
  if((sscanf(line, "Physical address space size: %d^%d", &d, &phy_size)) != 2){
    fprintf(stderr, "Unexpected line 2. Abort.\n");
    exit(-1);
  }
  fgets(line, MAXSTR, stdin);
  if((sscanf(line, "Page size: %d^%d", &d, &page_size)) != 2){
    fprintf(stderr, "Unexpected line 3. Abort.\n");
    exit(-1);
  }

  /* Allocate arrays to hold the page table and memory frames map */
    // we find the number of pages with a calculation
    // 2^logical address space divided by 2^page size
    num_pages = pow(2, log_size)/pow(2, page_size);

    // allocate the page table array, which is the size as the number of pages
    int *page_table_arr;
    page_table_arr = (int *)malloc(num_pages);

    // find the number of frames in physical memory
    // 2^physicalsize divided by 2^page size
    num_frames = pow(2, phy_size)/pow(2, page_size);

    // allocate memory frames map array
    int *mem_frame_arr;
    mem_frame_arr = (int *)malloc(num_frames);

    offset_bits = ((uint32_t)0xffffffff) >> (unsigned)log2(num_pages);

    printf("Number of Pages: %d, Number of Frames: %d\n\n", num_pages, num_frames);

  /* Initialize page table to indicate that no pages are currently mapped to
     physical memory */
     // NULL?

     for(int i=0; i < num_pages; i++) {
       page_table_arr[i] = -1;
     }


  /* Initialize memory map table to indicate no valid frames */

  for(int i=0; i < num_frames; i++) {
    mem_frame_arr[i] = -1;
  }


  /* Read each accessed address from input file. Map the logical address to
     corresponding physical address */
  fgets(line, MAXSTR, stdin);
  while(!(feof(stdin))){
    sscanf(line, "0x%x", &logical_addr);
    fprintf(stdout, "Logical Address: 0x%x\n", logical_addr);

	/* Calculate page number and offset from the logical address */
// done by offsetting the logical address by the page size
  page_num = logical_addr >> page_size;
  printf("Page Number: %d\n", page_num);

    /* Form corresponding physical address */
    // frame number times page size plus offset (which is logical address mod page size)

// this checks and sees if there are any page faults
// if there is NULL in the page table array then we know we need to put something in the array there
    if(page_table_arr[page_num] < 0) {
      printf("Page Fault!\n");
      for (int i = 0; i < num_frames; i++) {
        if(mem_frame_arr[i] < 0) {
          page_table_arr[page_num] = i;
          mem_frame_arr[i] = page_num;
          break;
        }
      }
    }

  // frame number is found using page number and the page table
    frame_num = page_table_arr[page_num];
    printf("Frame Number: %d\n", frame_num);
    //
    // int log_add = logical_addr;
    // int add = pow(2, page_size);
    // offset = logical_addr % add;
    offset = logical_addr & offset_bits;

    // phys
    physical_addr = (frame_num * pow(2, page_size)) + offset;
    printf("Physical Address: 0x%x\n", physical_addr);

    printf("\n");
    /* Read next line */
    fgets(line, MAXSTR, stdin);
  }

  free(page_table_arr);
  free(mem_frame_arr);

  return 0;
}

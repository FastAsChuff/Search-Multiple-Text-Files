//===============================================================================================================================
// printall2.c: 
//===============================================================================================================================
// Author: Simon Goater Jan 2026
// -----------------------------
// Prints out lines from a list of text filenames input to stdin with each line prepended with the file name and linenumber.
// e.g. ls -1 somefile*.txt | ./printall2.bin
// Output: 
// |somefile1.txt|1|This is line one in somefile1.txt.
// |somefile1.txt|2|This is line two in somefile1.txt.
// |somefile1.txt|3|This is line three in somefile1.txt.
// |somefile2.txt|1|This is line one in somefile2.txt.
// |somefile2.txt|2|This is line two in somefile2.txt.
// |somefile2.txt|3|This is line three in somefile2.txt.
//
// The idea is to make searching multiple files for specific text easier. e.g.
// ls -1 *.c | ./printall2.bin | grep myelusivecfunction
//
// COPYRIGHT: Software is given as is without guarantee or warranty. It is offered free to use, modify, copy, or distribute
// with conspicuous attribution for any purpose.
//===============================================================================================================================
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// gcc printall2.c -o printall2.bin -O3 -Wall -march=native

#define BUFSIZE 1024

int main(int argc, char **argv){
  char contentbuffer[BUFSIZE];
  char filenamesbuffer[BUFSIZE];
  filenamesbuffer[0] = 0;
  size_t len, linenumber;
  do {
    char *str = fgets(filenamesbuffer, BUFSIZE, stdin);
    if (str) {
      len = strlen(filenamesbuffer);
      if (filenamesbuffer[len-1] != '\n') {
        fprintf(stderr, "Filename too long.\n");
        exit(1);
      }
      while (len > 0) {
        if (filenamesbuffer[len-1] == '\n') {
          len--;
          filenamesbuffer[len] = 0;
        } else {
          break;
        }
      }
      FILE *fp = fopen(filenamesbuffer, "r");
      _Bool isnewline = true;
      if (fp) {
        linenumber = 0;
        do {
          char *str2 = fgets(contentbuffer, BUFSIZE, fp);
          if (str2) {
            if (isnewline) {
              printf("|%s|%lu|%s", filenamesbuffer, ++linenumber, contentbuffer);
            } else {
              printf("%s", contentbuffer);
            }
            isnewline = ('\n' == contentbuffer[strlen(contentbuffer)-1]);
            if (feof(fp)) {
              if ('\n' != contentbuffer[strlen(contentbuffer)-1]) printf("\n");
              fclose(fp);
              break;
            }
          } else {
            if (feof(fp)) {
              fclose(fp);
              break;
            }
            printf("Could not read %s\n", filenamesbuffer);
            fclose(fp);
            break;
          }
        } while (true);
      } else {
        printf("Could not open %s\n", filenamesbuffer);
      }
    } else {
      if (feof(stdin)) break;
      fprintf(stderr, "Error reading stdin!\n");
      break;
    }
  } while (true);
}

// Gowtham Vuppala

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T 1
#define F 0
#define LINE_SIZE 80

typedef struct node{
  char iden[11];
  int lNums[100];
  int lNum;
  struct node *next;
}NODE, *PNODE;

PNODE head,tail = NULL;
FILE *outFile;
int lNum;

void pr1();
int lNumber(FILE *);
void add(FILE *);
void search(FILE *, PNODE);
void ins(char[], int );



int main(int argc, char* argv[]){
  FILE *inFile;
  char line[81];
  int lineNumber = 1;
  
  if (argc != 4){
    fprintf(stderr, "Your need to enter a flag command, an input file and an output file.\n");
    exit(1);
  }
  if ((strcmp(argv[1], "-l") != 0) && (strcmp(argv[1], "-c") != 0) && (strcmp(argv[1], "-b") != 0)){
      fprintf(stderr, "Error in flag command!\n");
      exit(1);
  } 
  if ((inFile = fopen(argv[2], "r")) == NULL){
    fprintf(stderr, "Error in opening the file.\n");
    exit(1);
  }
  if ((outFile = fopen(argv[3], "w")) == NULL){
    fprintf(stderr, "Error in opening the file.\n");
    exit(1);
  }
  
  if ((strcmp(argv[1], "-l")) == 0){
     while(!feof(inFile)){
       fgets(line,80,inFile);
       if (line[0] != '\n'){
        fprintf(outFile, "%d  %s",lineNumber, line);
        lineNumber++;
       }
       else{
         fprintf(outFile, "%s", line);
       }
    }
  }
  else if ((strcmp(argv[1], "-c")) == 0){
   lNum = lNumber(inFile);
   rewind(inFile);
   add(inFile);
   rewind(inFile);
   PNODE tp = head;
   while(tp != NULL){
     
     search(inFile, tp);
     tp = tp->next;
   }
   pr1();
  }
  else if ((strcmp(argv[1], "-b")) == 0){
     while(!feof(inFile)){
       fgets(line,80,inFile);
       if (line[0] != '\n'){
        fprintf(outFile, "%d  %s",lineNumber, line);
        lineNumber++;
       }
       else{
         fprintf(outFile, "%s", line);
       }
    }
    lNum = lineNumber;
    rewind(inFile);
    add(inFile);
   rewind(inFile);
   PNODE tp = head;
   while(tp != NULL){
     
     search(inFile, tp);
     tp = tp->next;
   }
   fprintf(outFile, "\n\n");
   //pr2();
   pr1();
  free(head);
  }
  
  if (fclose(inFile) == EOF){
    fprintf(stderr, "Error in closing the file.\n");
    exit(1);
  }

    if (fclose(outFile) == EOF){
      fprintf(stderr, "Error in closing the file.\n");
      exit(1);
  }
  
    return 0;
}

/*
 * This method adds each identifier to the linked list
 * @param fp: The input file
 */

void add(FILE *fp){
  char lines[81];
  char iden[11];
  int i = 0;
  
  int count = 1;
     while((fgets(lines,80,fp)) != NULL){
        if (lines[0] != '#' && lines[0] != ' ' && lines[0] != '\t' && lines[0] != '\n'){
          while (lines[i] != ':' && i<10 ){
            iden[i] = lines[i];
            ++i;
          }
          iden[i]='\0';
          i = 0;
          ins(iden, count);
       }
       if (lines[0] != '\n'){
          ++count;
       }
   }
}

/* This method creates and inserts a node in the linked list pointed by
 * head.
 * @param word : The identifier of the node
 */

void ins(char *word, int num){
     PNODE temp, currentPtr;
     int inserted = F;
     //If malloc returns null, exit the program
     if ((temp = malloc(sizeof(PNODE))) == NULL) {
        printf("Node allocation failed. \n"); fflush(stdout);
         exit(1); /* Stop program */
    }
    if((currentPtr= malloc(sizeof(PNODE))) == NULL){
         printf("Node allocation failed. \n"); fflush(stdout);
         exit(1); /* Stop program */
    }
    strcpy(temp->iden, word); 
    temp->next = NULL;
    temp->lNum = num;
    //If the list is empty, create one, else add the node at the end
    if (head == NULL) {
         head = tail = temp;
    }
    else { 
        currentPtr = head;
        //If the node of same symbol is already present, increment the count
        while (currentPtr != NULL){
            if (strcmp(currentPtr->iden, temp->iden) == 0){
                inserted = T;
            }
            currentPtr = currentPtr->next;
        }
        //If its a new symbol, add the node at the end
        if(inserted == F){
            tail->next = temp;
            tail = tail->next;
        }
    }
}

/*
 * This program prints the cross reference table in the output file
 */

void pr1() {
    PNODE temp = head;
    int i;
    fprintf(outFile, "Cross Reference Table\n\n");
    fprintf(outFile, "Identifier  Definition  Use\n\n");
    //If the list isn't empty, print the list
    if (temp == NULL) {
        fprintf(outFile, "No Identifiers!\n"); fflush(stdout);
    }
    else {
        while (temp != NULL) {
        fprintf(outFile, "%s %10d", temp->iden, temp->lNum); fflush(stdout);
        i = 0;
        while(temp->lNums[i] > 0 && temp->lNums[i] < lNum){
            if (i > 0){
                if (temp->lNums[i] > temp->lNums[i-1]){
                    fprintf(outFile,  "      %3d", temp->lNums[i]);
                    i++;
                }
                else{
                    i++;
                }
            }
            else{
                fprintf(outFile,  "      %3d", temp->lNums[i]);
                i++;
            }
        }
        fprintf(outFile, "\n");
        temp = temp->next;
      }
        
    }
  }
 
/*
 * This function finds out how many times the identifier is used
 * inside the mal file
 * @param fp: The name of the file
 * @param temp: A pointer to the node
 */

void search(FILE *fp, PNODE temp){
  rewind(fp);
  int ln = 1;
  int i = 0;
  int j;
  char lines[81];
  char iden[11];
  while(fgets(lines, 80, fp) != NULL) {
    j = 0;
    while(lines[j] != ':' && j < 10){
      iden[j] = lines[j];
      j++;
    }
    if(strncmp(iden, temp->iden, strlen(temp->iden)) == 0){
      if (lines[0] != '\n')
		    ++ln;
		  continue;
    }
    //printf("%s\n", iden);
		if((strstr(lines, temp->iden)) != NULL) {
			  temp->lNums[i] = ln; 
			  i++;
		}
		if (lines[0] != '\n')
		  ++ln;
	}

}

/*
 * This method finds out how many lines are in the file.
 * @param fp: The name of the file
 * @return the number of lines in the file
 */
int lNumber(FILE *fp){
  
  char lines[LINE_SIZE + 1];
  int num = 1;
  while((fgets(lines, 80, fp)) != NULL){
    num++;
  }
  return num;
}
 

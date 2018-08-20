#include  <stdio.h>
#include  <stdlib.h>
#include 	<stdint.h>
#include	<string.h>
#include  "terminal.h"

//////////////////////////////////////////////////////////////////////

// Defining constants

#define MAX_COMMANDS  10
#define TRUE          1
#define FALSE         0

//////////////////////////////////////////////////////////////////////

// Declaring variables

uint8_t             commandIndex		          =				 0; 
unsigned long int   commandAddress		[MAX_COMMANDS]    ;
char                commandName			  [MAX_COMMANDS][15];
uint8_t             commandArguments  [MAX_COMMANDS]    ;

//////////////////////////////////////////////////////////////////////

// Defining functions

void initTerminal(unsigned long int structureAddress){
  char str[28];
  char command[15];
  char argument[10];

  infinite_loop: {   
    uint8_t i = 0;
    uint8_t j = 0;
    int n = 0;

    memset(command, 0, sizeof(command));
    memset(argument, 0, sizeof(argument));

    printf(">> ");
    fgets(str, sizeof str, stdin);

    if(str[0] == '\n') goto infinite_loop;
    
  	while(str[i]!='('){
      if(str[i]=='\n'){
        printf("Invalid command. Please call the function and its arguments using parenthesis.\n");
        goto infinite_loop;
      }  
      command[j++] = str[i++];
    }
 
   	i++;
    j = 0;
   
    while(str[i]!=')'){
      if(str[i]=='\n'){
        printf("Invalid command. Please call the function and its arguments using parenthesis.\n");        
        goto infinite_loop;
      }
      argument[j++] = str[i++];
    }
    
    n = atoi(argument);

    executeCommand(structureAddress, command, n);

    goto infinite_loop;
  }
}

void addCommand(unsigned long int funcAddress, char name[], uint8_t args){
  if(commandIndex <  MAX_COMMANDS){
		commandAddress[commandIndex] = funcAddress;
		strcpy(commandName[commandIndex], name);
		commandArguments[commandIndex] = args;
		commandIndex++;
	}
	else printf("The Commands array is full.\n");
}

void checkCommands(){
	for(uint8_t i=0; i<commandIndex;i++){
		printf("Command %d:\n\tAddress: %lx\n\tName: %s\n\tNumber of Arguments: %d\n", i+1, commandAddress[i], commandName[i], commandArguments[i]);
	} 
}

void executeCommand(unsigned long int structureAddress, char command[], int n){
  if(!strcmp(command, "quit")) exit(0);
  
  for(uint8_t i=0; i<commandIndex; i++){
    if(!strcmp(command, commandName[i])){
      if(!commandArguments[i]){
        void (*func)() = (void (*)())commandAddress[i];
        func(structureAddress);
        break;
      }
      else{
        void (*func)() = (void (*)())commandAddress[i];
        func(structureAddress, n);
        break;
      }
    } 
    else if (i == commandIndex-1) printf("Invalid command.\n");
  }
}
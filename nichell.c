#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_LINE 80 /* The maximum length command */
#define BUFFER_SIZE 1024
#define TOK_DELIM " \t\r\n\a" //Delimiters for parsing the arguments

int cur_bufsize = BUFFER_SIZE;
char *read_line(void){
	cur_bufsize = BUFFER_SIZE;
	char *buffer = malloc(sizeof(char)*cur_bufsize);
	int c;
	int position = 0; //current position on resulting array
	if(!buffer){
		fprintf(stderr, "ERROR: Allocaction error\n");
		exit(EXIT_FAILURE);
	}
	while(1){
		c = getchar();
		if(c==EOF || c == '\n'){
			buffer[position] = '\0';
			return buffer;
		}
		else{
			buffer[position] = c;
		}
		position++;
		//IF buffersize exceeded, resize the array and the curren buffer
		if(sizeof(char) * position>=cur_bufsize){
			cur_bufsize += BUFFER_SIZE;
			buffer = realloc(buffer,cur_bufsize);
			if(!buffer){
				fprintf(stderr,"ERROR: Allocaction error\n");
				exit(EXIT_FAILURE);
			}
		}
	}
}

char **split_line(char *line){
	cur_bufsize = BUFFER_SIZE;
	int position = 0;
	char **tokens = malloc(sizeof(char)*cur_bufsize);
	char *token;
	
	if(!tokens){
		fprintf(stderr,"ERROR: Allocaction error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, TOK_DELIM);
	while(token!=NULL){
		tokens[position] = token;
		position++;
		if(position*sizeof(char) >= cur_bufsize){
			cur_bufsize += BUFFER_SIZE;
			tokens = malloc(sizeof(char)*cur_bufsize);
			if(!tokens){
				fprintf(stderr,"ERROR: Allocaction error\n");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, TOK_DELIM);
	}
	tokens[position] = NULL;
	return tokens;
}

int execute(char *line){
	int i;
	char **args = split_line(line);
	if(args[0] == NULL){
		return 1; //NO command entered
	}
	if(execvp(args[0], args)==-1){
		perror("lsh");
	}
	else{
		return 1;
	}
	return 1;
}

int main(void){
    char *args[MAX_LINE/2 + 1]; /* command line arguments */
    int should_run = 1; /* flag to determine when to exit program */
		char *line;
    while (should_run) {
        printf("nichell>");
        fflush(stdout);

				line = read_line();
				should_run = execute(line);
				printf("%d",should_run);
				fflush(stdout);
        /**
        * After reading user input, the steps are:
        * (1) fork a child process using fork()
        * (2) the child process will invoke execvp()
        * (3) parent will invoke wait() unless command included &
        */
    }
return 0;
}

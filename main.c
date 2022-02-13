#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>


void signal_handler();
int main()
{
    char command[256];  //array initialization
    char *arg[256];
    pid_t pid;

    while(1)
    {
        signal(SIGCHLD, signal_handler);
        printf("Shell> ");
        fgets(command,sizeof command, stdin);  //get command from user
        if ((strlen(command) > 0) && (command[strlen (command) - 1] == '\n'))  // remove trailing new line
        {
            command[strlen (command) - 1] = '\0';
        }

        if(!strcmp("", command))    //continue terminal
        {
            continue;
        }

        char *token; // put commands in array arg using token
        int i = 0;
        int flag=0;
        token = strtok(command, " ");
        while(token!= NULL)
        {
            arg[i] = token;
            i++;
            token = strtok(NULL, " ");
        }
        arg[i] = NULL;

        if(strcmp(arg[0], "exit")==0)  //check if command is exit
        {
            exit(0);
        }

        if(strcmp(arg[0],"cd")==0)  //check if command is change directory
        {
            chdir(arg[1]);
            printf("Directory Changed \n");
        }

        if(strcmp(arg[i-1],"&")==0) //check if command doesn't wait
        {
            arg[i-1]=NULL;
            flag=1;
        }


        pid = fork();  //fork command

        if(pid <0)
        {
            printf("Error!\n");
        }


        else if(pid == 0)
        {

            if(execvp(arg[0], arg) <0) //execute
            {
                printf("Invalid Command!\n");   //check for invalid command
                exit(0);
            }
        }

        else if(pid >0)
        {
            if(flag==0)
            {
                waitpid(pid,NULL,0); //command wait if & is not found
            }
        }
    }
    return 0;
}


void signal_handler()  //function to handle child termination
{
    FILE *fptr;
    fptr = fopen("log.txt", "a");
    if(fptr==NULL)
        printf("Error opening handler file\n");
    else
        fprintf(fptr, "Child process was terminated\n");
    fclose(fptr);
}

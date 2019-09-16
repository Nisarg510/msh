#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define WHITESPACE " \t\n"      // We want to split our command line up into tokens
// so we need to define what delimits our tokens.
// In this case  white space
// will separate the tokens on our command line

#define MAX_COMMAND_SIZE 255    // The maximum command-line size

#define MAX_NUM_ARGUMENTS 5     // Mav shell only supports five arguments

static void handle_signal(int sig)
{
    printf("Caught the signal",sig);
}

int main()
{
    pid_t child_pid = fork();
    int status;
    pid_t list_pid_history[15];
    
    if(child_pid==0)
    {
        execl("/bin/ls","ls",NULL);
        exit(EXIT_SUCCESS);
    }
    waitpid(child_pid,&status,0);
    
    
    char * cmd_str = (char*) malloc( MAX_COMMAND_SIZE );
    
    
    struct sigaction act;
    memset(&act, '\0', sizeof(act));
    {
        if(sigaction(SIGINT, &act, NULL )<0)
        {
            perror("Sigaction : ");
            return 1;
        }
        while(1)
        {
            
            printf("Pasusing the signal ");
            pause();
        }
    }
    
    while( 1 )
    {
        // Print out the msh prompt
        printf ("msh> ");
        
        // Read the command from the commandline.  The
        // maximum command that will be read is MAX_COMMAND_SIZE
        // This while command will wait here until the user
        // inputs something since fgets returns NULL when there
        // is no input
        while( !fgets (cmd_str, MAX_COMMAND_SIZE, stdin) );
        
        /* Parse input */
        char *token[MAX_NUM_ARGUMENTS];
        
        int   token_count = 0;
        
        // Pointer to point to the token
        // parsed by strsep
        char *arg_ptr;
        
        char *working_str  = strdup( cmd_str );
        
        // we are going to move the working_str pointer so
        // keep track of its original value so we can deallocate
        // the correct amount at the end
        char *working_root = working_str;
        
        // Tokenize the input stringswith whitespace used as the delimiter
        while ( ( (arg_ptr = strsep(&working_str, WHITESPACE ) ) != NULL) &&
               (token_count<MAX_NUM_ARGUMENTS))
        {
            token[token_count] = strndup( arg_ptr, MAX_COMMAND_SIZE );
            if( strlen( token[token_count] ) == 0 )
            {
                token[token_count] = NULL;
            }
            token_count++;
        }
        if(token[0]!="ls" || token[0]!="cd"|| token[0]!="exit")
        {
            printf("Enter correct command");
        }
        
        if(token[0]==NULL)
        {
            
        }
        else
        {
            //list_pid_history
            if(strcmp(token[0],"quit")==0|| strcmp(token[0],"exit")==0)
            {
                if(token[1]==NULL)
                {
                    exit(0);
                }
                else
                {
                    printf("Too many arguments with quit or exit. Plese enter it in correct format\n");
                }
            }//Requirement 5 to exit the code.
            else if(strcmp(token[0],"listpids")==0)
            {
                
            }
                
                
            
            else if(strcmp(token[0],"cd")==0)
            {
                chdir(token[1]);
            }
        }
        
        
        free( working_root );
        
    }
    return 0;
}

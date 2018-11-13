#include<stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

    //define custom variables
#define debugi(acb) printf("%d\n", acb)
#define debugs(acb) printf("%s\n", acb)


#define sizeofarray 100
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


char current_working_dir[sizeofarray];

//one_process is a global structure of all commands and commnd_paramrt

//single command structure
struct single_commands{
    char comm_line[sizeofarray];
}all_commands[sizeofarray], all_paramtr[sizeofarray];


struct sngl_comnd{
    char commnd_word[sizeofarray];
    char commnd_paramrt[sizeofarray][sizeofarray];
    int numOfParm;
}one_process[sizeofarray];


struct single_commands input_command(){
    struct single_commands s1;
    gets(s1.comm_line);

    return s1;
}


void print_path(){
    getcwd(current_working_dir, sizeof(current_working_dir)); //path of current working directory
    printf(ANSI_COLOR_RED "┌─["ANSI_COLOR_RESET"host"ANSI_COLOR_CYAN"@domain.com"ANSI_COLOR_RED"]─["ANSI_COLOR_GREEN"%s"ANSI_COLOR_RED"]\n└──╼ $"ANSI_COLOR_RESET"", current_working_dir);
}



int command_parsing(struct single_commands comd_all){

    char *onecommand[500];

    char* token_line = strtok(comd_all.comm_line, ";");
//    checking ; for finding multiple commands
    onecommand[0] = token_line;
    int ind=1;
    while (token_line != NULL) {

		token_line = strtok(NULL, ";");
            if(token_line == NULL) break;
		onecommand[ind] = token_line;
		ind++;
		//    checking [space] for finding multiple commands

    }

    for(int i=0; i<ind && onecommand[i]!=NULL; i++){
        char* token_word = strtok(onecommand[i], " ");
//       main command parsing
        strcpy(one_process[i].commnd_word, token_word);
        int j=0;
        while(token_word!=NULL){
//      parameter parsing
            token_word = strtok(NULL, " ");
            if(token_word == NULL) break;
            strcpy(one_process[i].commnd_paramrt[j], token_word);
            j++;

        }
        one_process[i].numOfParm=j;

    }
    return ind;
}



void print_process_str(int total_no_of_process){
    for(int i=0; i<total_no_of_process && one_process[i].commnd_word!=NULL; i++){
        printf("Command: %s  -->Perameters: ", one_process[i].commnd_word);


        for(int j=0; j<one_process[i].numOfParm; j++){
            printf("%s ", one_process[i].commnd_paramrt[j]);

        }
        printf("\n");
    }

}

void cmd_cd(int process_id){
char *directory = one_process[process_id].commnd_paramrt;
int ret;

    ret = chdir (directory);   //On success, zero is returned.  On error, -1 is returned, and errno is set appropriately

    if(ret){
        perror("Error ");
    }
}

void all_process_management(int numofProcess){

    for(int i=0; i<numofProcess && one_process[i].commnd_word!=NULL; i++){
        if(strcmp(one_process[i].commnd_word, "cd") == 0){
            cmd_cd(i);
        }else break;
    }
}




void creatNewProcess(){
    pid_t pid =fork();
    pid_t tpid;
    if(!pid){
        //child process started
//        char *const parmList[] = {"/bin/ls", "", current_working_dir, NULL};
        char *const parmList[] = {"/bin/cd", "-l", current_working_dir, NULL};
        execvp("ls", parmList);
        printf("Return not expected. Must be an execvp() error.n");

    }else if(pid==-1){
        //error to create process. Need error msg
    }else{
//        parent process remaining part
            pid_t tpid = wait(&pid);   //parent process wait untill child process terminated.

    }


}




int main(int argc, char *argv[]){
    struct single_commands cmmd;

    while(1){
        print_path();
        cmmd = input_command();
        int noOfProcess = command_parsing(cmmd);
        all_process_management(noOfProcess);
//        print_process_str(noOfProcess);
        if(strcmp(cmmd.comm_line,"quit")== 0) break;
        //creatNewProcess();
    }

    return 0;
}

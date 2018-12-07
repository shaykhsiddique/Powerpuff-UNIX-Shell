#include<stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
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



void print_process_str(int total_no_of_process){ //this is a sample of test printing
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


   int ret = chdir (directory);   //On success, zero is returned.  On error, -1 is returned, and errno is set appropriately

    if(ret){
        perror("Error ");
    }
}


void execute_ls(int process_id){
    pid_t pid =fork();
    pid_t tpid;
    if(!pid){
        //child process started
//        char *const parmList[] = {"/bin/ls", "", current_working_dir, NULL};

        struct dirent *de;
        DIR *dr = opendir(".");

        if (dr == NULL) // opendir returns NULL if couldn't open directory
        {
            printf("Could not open current directory" );
        }
        while ((de = readdir(dr)) != NULL)
                printf("%s\n", de->d_name);

        closedir(dr);

    }else if(pid==-1){
        //error to create process. Need error msg
    }else{
//        parent process remaining part
            pid_t tpid = wait(&pid);   //parent process wait untill child process terminated.

    }
}

void execute_mkdir(int process_id){
    for(int j=0; j<one_process[process_id].numOfParm; j++){
        int result = mkdir(one_process[process_id].commnd_paramrt[j], 0777);
        //Upon successful completion, mkdir() shall return 0. Otherwise, -1 shall be returned
        if(result==-1){
            printf("Error: Cannot create directory...\n");
            sleep(1);
        }

    }
}

void execute_touch(int process_id){
    for(int j=0; j<one_process[process_id].numOfParm; j++){
        FILE *fp = fopen(one_process[process_id].commnd_paramrt[j], "wb"); //write mode
        fclose(fp);
    }
}



void execute_cp(int process_id){
    //cointains 2 parameter one is source and another is destination
    if(one_process[process_id].numOfParm == 2){
        //copying files
        int last_char_id =strlen(one_process[process_id].commnd_paramrt[1])-1;
        if(one_process[process_id].commnd_paramrt[1][last_char_id] == '/' || one_process[process_id].commnd_paramrt[1][last_char_id] == '\\'){
            printf("No destination file name...\n");
            return;
        }
            char ch;
            char file_datas[100000];  //temopary data copying
            FILE *fp_src;
    //        file_name = one_process[process_id].commnd_paramrt[j];
            fp_src = fopen(one_process[process_id].commnd_paramrt[0], "r");
            if (fp_src == NULL){
              perror("Error in source file.");
            }else{

                int id=0;
                while((ch = fgetc(fp_src)) != EOF){
                    file_datas[id] = ch;
                    id++;
                    //strcat(file_datas, ch);
                }

            }
            fclose(fp_src);
//destination file copying
            FILE *fp_des;
            fp_des = fopen(one_process[process_id].commnd_paramrt[1], "wb");
            fprintf(fp_des, "%s", file_datas); //printing in new file
            fclose(fp_des);
    }else{
        //parameter error.
        printf("Error: Expect 2 parameter. Command: \"cp [source] [destination]\"...\n");

    }
}

void execute_cat(int process_id){
    for(int j=0; j<one_process[process_id].numOfParm; j++){
        char ch;
        FILE *fp;
//        file_name = one_process[process_id].commnd_paramrt[j];
        fp = fopen(one_process[process_id].commnd_paramrt[j], "r"); // read mode
        if (fp == NULL){
            perror("Error: \n");
            exit(EXIT_FAILURE);
        }
        //writing on console
        while((ch = fgetc(fp)) != EOF)
            printf("%c", ch);
        printf("\n");
        fclose(fp);  //close file
    }
}

void execute_echo(int process_id){
    for(int j=0; j<one_process[process_id].numOfParm; j++){
        int len = strlen(one_process[process_id].commnd_paramrt[j]);
        for(int k=0; k<len; k++){
            if(one_process[process_id].commnd_paramrt[j][k] != '"');
            printf("%c", one_process[process_id].commnd_paramrt[j][k]);
        }
//        printf("%s", one_process[process_id].commnd_paramrt[j]);
        printf(" ");
    }
    printf("\n");
}


// for all precess executing commands
void all_process_management(int numofProcess){
        //cd, ls, mkdir, cat, touch, cp, echo
        int errr=0;
    for(int i=0; i<numofProcess && one_process[i].commnd_word!=NULL; i++){
        int pharmNo = one_process[i].numOfParm;
        if(strcmp(one_process[i].commnd_word, "cd") == 0){
            if(pharmNo) cmd_cd(i);
            else errr = 1;
        }else if(strcmp(one_process[i].commnd_word, "ls") == 0){
            if(!pharmNo) execute_ls(i);
            else errr = 1;
        }else if(strcmp(one_process[i].commnd_word, "mkdir") == 0){
            if(pharmNo) execute_mkdir(i);
            else errr = 1;
        }else if(strcmp(one_process[i].commnd_word, "touch") == 0){
            if(pharmNo) execute_touch(i);
            else errr = 1;
        }else if(strcmp(one_process[i].commnd_word, "cp") == 0){
            execute_cp(i);
        }else if(strcmp(one_process[i].commnd_word, "cat") == 0){
            if(pharmNo) execute_cat(i);
            else errr = 1;
        }else if(strcmp(one_process[i].commnd_word, "echo") == 0){
            if(pharmNo) execute_echo(i);
            else errr = 1;
        }else if(strcmp(one_process[i].commnd_word, "exit") == 0){
            exit(0);
        }
        else {
            printf("No command found..\n");
        };
    }

    if(errr) printf("Command Error. \n");
}

void clear_commands(int numofProcess){
    for(int i=0; i<numofProcess; i++){
        strcpy(one_process[i].commnd_word, "");
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
        clear_commands(noOfProcess);  //clear all command in global variable
    }

    return 0;
}

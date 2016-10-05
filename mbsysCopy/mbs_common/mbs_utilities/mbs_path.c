
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "mbs_path.h"

 /**
  * Find the user folder corresponding to the given *.mbs file.
  * 
  * This function allocat a char* that must be freed by the user.
  * 
  */ 
char* find_user_path(const char* mbsfile){

    char *prjpath, *userpath;

    // allocate space for the path to the project folder
    prjpath = (char*)malloc(sizeof(char)*(strlen(mbsfile)+50));

    // allocate space for the path to the user folder
    userpath = (char*)malloc(sizeof(char)*(strlen(mbsfile)+50));
    
    find_project_path(mbsfile, prjpath);

    sprintf(userpath, "%s/userfctR/", prjpath);

    // TODO: check if userfctR/ directory exist

    free(prjpath);
    
    return userpath;
}

/**
 * Find the project directory (containing dataR, symbolicR, ...).
 *
 * This function allocat a char* that must be freed by the user.
 * prjpath should be large enough to contain at least mbsfile
 */
void find_project_path(const char* mbsfile, char* prjpath){
    char *ptr;
    char* c;

    // copy path to the *.mbs file
    sprintf(prjpath, "%s", mbsfile);

    // replace '\' by '/' if any (i.e. win compatibility)
    for(c=prjpath ; *c !='\0' ; c++){
        if(*c == '\\'){
            *c = '/';
        }
    }

    // find the last occurence of '/'
    ptr = strrchr(prjpath, '/');
    // mark the end of path to the last occurence
    *ptr = '\0';

    // find the previous occurence
    ptr = strrchr(prjpath, '/');

    // check if
    if(strcmp(ptr, "/dataR")==0){
        *ptr = '\0';

    }
    else{
        printf("ERROR: the *.mbs file must be in the dataR folder. \n");
        exit(1);
    }
}

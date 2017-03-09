
//  main.c
//  Project01
//
//  Created by Scott English on 4/25/16.
//  Copyright © 2016 Scott English. All rights reserved.
//

#include "Header.h"

int main(int argc, const char * argv[]) {
    /////////////Initializing the variables used for input///////////////
    Inode*I,*intial,*temp;
    Fylesystem *F;
    char command[20],cdname[1000],lsname[1000],lsnames[1000],directoryname[1000],filename[1000],rmname[1000],findname[20],token[2]="/",*directory,*directory2,*cdirectory,*rmnames;
    int number=0,count=0,i;
    for(i=0;i<21;i++)
        lsnames[i]='\0';
    intial=createInode(1, token);
    F=createFylesystem(intial);
    F->current=F->Root;
    ////////////////////////////////////////////////////////////////////
    
    ///////////////////Reading in which command to take/////////////////
    while(scanf("%s", command) > 0){
        temp=F->current;
        if(temp==F->Root)
            F->Root->count=1;
        else
            F->Root->count=0;
        if(strcmp(command, "mkdir") == 0){ ///Insert a Directory into the Filesystem///
            scanf("%s",directoryname);
            directory=strtok(directoryname, token);
            while(directory!=NULL){
                if(F->error==1){
                    printf("Make Dir Error: Cannot create directory.");
                    break;
                }else{
                    I=createInode(1,directory);
                    insertInode(I, F);
                    directory=strtok(NULL, token);
                    if(F->error==1){
                        printf("Make Dir Error: Cannot create directory.");
                        break;
                    }
                }
            }
            F->current=F->Root->Child;
            resetcount(F);
            F->current=temp;
            memset(command, 0, 20);
            memset(cdname, 0, 1000);
            memset(lsnames,0,20);
            memset(directoryname,0,1000);
            memset(filename, 0, 1000);
            memset(rmname, 0, 1000);
            memset(findname, 0, 20);
            memset(lsname, 0, 1000);
            directory=NULL;
            directory2=NULL;
            cdirectory=NULL;
            rmnames=NULL;
        }else if(strcmp(command, "touch") == 0){ ///Insert a File in the Filesystem///
            scanf("%s",filename);
            for(number=0;number<1001;number++){
                if(filename[number]==token[0])
                    count+=1;
            }
            directory2=strtok(filename, token);
            while(directory2!=NULL){
                if(F->error==1){
                    printf("Touch Error: Cannot create file");
                    break;
                }else{
                    if(count==0){
                        I=createInode(0,directory2);
                        insertInode(I, F);
                        directory2=strtok(NULL, token);
                        if(F->error==1){
                            printf("Touch Error: Cannot create file");
                            break;
                        }
                    }else{
                        I=createInode(1,directory2);
                        insertInode(I, F);
                        directory2=strtok(NULL, token);
                        count-=1;
                        if(F->error==1){
                            printf("Make Dir Error: Cannot create directory.");
                            break;
                        }
                    }
                }
            }
            F->current=F->Root->Child;
            resetcount(F);
            F->current=temp;
            count=0;
            memset(command, 0, 20);
            memset(cdname, 0, 1000);
            memset(lsnames,0,20);
            memset(directoryname,0,1000);
            memset(filename, 0, 1000);
            memset(rmname, 0, 1000);
            memset(findname, 0, 20);
            memset(lsname, 0, 1000);
            directory=NULL;
            directory2=NULL;
            cdirectory=NULL;
            rmnames=NULL;
        }else if(strcmp(command, "cd") == 0){ ///Change the Current Working Directory///
            Inode *temp3;
            scanf("%s",cdname);
            if(cdname[0]=='.'&&cdname[1]=='.')
                if(F->current==F->Root)
                    F->current=F->Root;
                else
                    F->current=F->current->Parent;
            else if(cdname[0]=='.'&&cdname[1]!='.')
                F->current=F->current;
            else if(cdname[0]=='/'&&cdname[1]=='\0')
                F->current=F->Root;
            else{
                cdirectory=strtok(cdname, token);
                temp3=F->current;
                while(cdirectory!=NULL){
                    if(F->error==1){
                        F->error=0;
                        break;
                    }else{
                        cd(cdirectory, F);
                        cdirectory=strtok(NULL, token);
                        if(F->error==1){
                            F->error=0;
                            F->current=temp3;
                            break;
                        }
                    }
                }
            }
            memset(command, 0, 20);
            memset(cdname, 0, 1000);
            memset(lsnames,0,20);
            memset(directoryname,0,1000);
            memset(filename, 0, 1000);
            memset(rmname, 0, 1000);
            memset(findname, 0, 20);
            memset(lsname, 0, 1000);
            directory=NULL;
            directory2=NULL;
            cdirectory=NULL;
            rmnames=NULL;
        }else if(strcmp(command, "ls") == 0){ ///List what's currently in the Current Working Directory///
            gets(lsnames);
            if(sscanf(lsnames, "%s",lsname)>0){
                ls(lsname, F);
            }else{
                ls(token,F);
            }
            memset(command, 0, 20);
            memset(cdname, 0, 1000);
            memset(lsnames,0,20);
            memset(directoryname,0,1000);
            memset(filename, 0, 1000);
            memset(rmname, 0, 1000);
            memset(findname, 0, 20);
            memset(lsname, 0, 1000);
            directory=NULL;
            directory2=NULL;
            cdirectory=NULL;
            rmnames=NULL;
        }else if(strcmp(command, "pwd") == 0){ ///Print out the path to the Current Working Directory///
            pwd(F);
            memset(command, 0, 20);
            memset(cdname, 0, 1000);
            memset(lsnames,0,20);
            memset(directoryname,0,1000);
            memset(filename, 0, 1000);
            memset(rmname, 0, 1000);
            memset(findname, 0, 20);
            memset(lsname, 0, 1000);
            directory=NULL;
            directory2=NULL;
            cdirectory=NULL;
            rmnames=NULL;
        }else if(strcmp(command, "find")==0){ ///Find a File/Directory inside of the Filesystem///
            scanf("%s",findname);
            find(F, findname);
            memset(command, 0, 20);
            memset(cdname, 0, 1000);
            memset(lsnames,0,20);
            memset(directoryname,0,1000);
            memset(filename, 0, 1000);
            memset(rmname, 0, 1000);
            memset(findname, 0, 20);
            memset(lsname, 0, 1000);
            directory=NULL;
            directory2=NULL;
            cdirectory=NULL;
            rmnames=NULL;
        }else if(strcmp(command, "rm")==0){ ///Remove a File/Directory from the Filesystem///
            scanf("%s",rmname);
            if(strcmp(rmname, "-f")==0){ ///Forcefully removes all contents of a Directory from the Filesystem///
                for(number=0;number<1001;number++){
                    if(rmname[number]==token[0])
                        count+=1;
                }
                count-=1;
                rmnames=strtok(rmname, token);
                while(rmnames!=NULL){
                    if(count==0){
                        forcedremoved(F, rmnames);
                        rmnames=strtok(NULL, token);
                    }else{
                        rmnames=strtok(NULL, token);
                        count-=1;
                    }
                }
                count=0;
            }else{
                for(number=0;number<1001;number++){
                    if(rmname[number]==token[0])
                        count+=1;
                }
                count-=1;
                rmnames=strtok(rmname, token);
                while(rmnames!=NULL){
                    if(count==0){
                        removed(F, rmnames);
                        rmnames=strtok(NULL, token);
                    }else{
                        rmnames=strtok(NULL, token);
                        count-=1;
                    }
                }
                count=0;
            }
        }
        memset(command, 0, 20);
        memset(cdname, 0, 1000);
        memset(lsnames,0,20);
        memset(directoryname,0,1000);
        memset(filename, 0, 1000);
        memset(rmname, 0, 1000);
        memset(findname, 0, 20);
        memset(lsname, 0, 1000);
        directory=NULL;
        directory2=NULL;
        cdirectory=NULL;
        rmnames=NULL;
    }
    return 0;
}

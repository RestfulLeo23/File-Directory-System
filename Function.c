
//
//  Function.c
//  Project01
//
//  Created by Scott English on 4/25/16.
//  Copyright © 2016 Scott English. All rights reserved.
//

#include "Header.h"

/////Initialize the Data Structures/////////
Inode* createInode(int flag, char name[20]){ ///Initialize the File/Directory///
    int i;
    Inode *I=(Inode*)malloc(sizeof(Inode));
    I->Child=NULL;
    I->Parent=NULL;
    I->Sibling=NULL;
    I->Previous=NULL;
    I->flag=flag;
    I->count=0;
    for(i=0;i<21;i++){
        I->localname[i]='\0';
    }
    strcpy(I->localname, name);
    return I;
}
Fylesystem* createFylesystem(Inode* initial){ ///Initialize the Filesystem///
    Fylesystem *F=(Fylesystem*)malloc(sizeof(Fylesystem));
    F->Root=initial;
    F->current=F->Root;
    F->error=0;
    F->count=0;
    return F;
}
////////////////////////////////////////////

/////////InterFace Commands/////////////////
void insertInode(Inode* inode,Fylesystem *F){ ///Insert a File/Directory into the Filesystem///
    int catch;
    Inode *temp;
    if(!F->Root->Child)
        F->Root->count=0;
    if(F->Root->count==1 && strcmp(F->Root->Child->localname,inode->localname)!=0){
        temp=F->current;
        F->current=F->Root->Child->Sibling;
        if(findInode(F, inode->localname)==1){
            F->current=temp;
            F->Root->count=0;
            F->current->count=1;
            F->current=F->current->Child;
            return;
        }else{
            F->current=F->Root->Child;
            insertsiblingInode(inode, F);
            F->Root->count=0;
            F->current->count=0;
        }
        
        return;
    }
    catch=findInode(F, inode->localname);
    if(F->Root->Child==NULL){
        F->Root->Child=inode;
        inode->Child=NULL;
        inode->Parent=F->Root;
        inode->Sibling=NULL;
        inode->Previous=NULL;
        F->current=inode;
    }else if(strcmp(F->current->localname,inode->localname)==0){
        F->current->count=1;
        F->Root->count=0;
        if(!F->current->Child)
            F->current=F->current;
        else
            F->current=F->current->Child;
    }else{
        if(F->current->Parent->count>0 && F->current!=NULL){
            if(findInode(F,inode->localname)==1){
                F->current->count=1;
            }else if(F->current->count>0){
                if(F->current->Previous==NULL){
                    if(F->current->flag==0){
                        F->error=1;
                        return;
                    }else{
                        F->current->Child=inode;
                        inode->Parent=F->current;
                        F->current=inode;
                    }
                }else if( F->current->Previous!=NULL){
                    if(!F->current->Child){
                        if(F->current->flag==0){
                            F->error=1;
                            return;
                        }else{
                            F->current->Child=inode;
                            inode->Parent=F->current;
                        }
                    }else{
                        F->current=F->current->Child;
                        insertsiblingInode(inode, F);
                    }
                }else
                    insertsiblingInode(inode, F);
            }else
                insertsiblingInode(inode, F);
        }else{
            if(F->current->flag==0){
                F->error=1;
                return;
            }else{
                F->current->Child=inode;
                inode->Parent=F->current;
                F->current=inode;
            }
        }
    }
}
void cd(char name[20],Fylesystem *F){ ///Change the Current Working Directory///
    int number=0;
    number=findcdInode(F, name);
    if(number==0){
        printf("Change Dir Error: Cannot change working directory.");
        F->error=1;
    }
}
void ls(char name[20],Fylesystem *F){ ///List the contents of the Current Working Directory///
    int number=0,i;
    Inode *temp;
    temp=F->current;
    F->current=F->Root;
    number=findInode(F, name);
    F->current=temp;
    char Path[1000],Path2[1000];
    for(i=0;i<1000;i++){
        Path2[i]='\0';
        Path[i]='\0';
    }
    F->current->count=1;
    while(F->current->Parent){
        F->current->Parent->count=1;
        F->current=F->current->Parent;
    }
    if(F->current==F->Root&&!F->Root->Child)
        printf("Listing For '/':\n");
    else{
        printf("Listing For '");
        walk2(F,F->current->localname);
        printf("':\n");
    }
    F->current=temp;
    if(number==0){
        printf("List Error: Cannot perform list operation.");
        F->error=1;
    }
    if(F->current->flag==0){
        printf("Cannot perform list operation. ’<%s>’ is a file",F->current->localname);
        F->error=1;
    }
    walk(F, name);
    F->current=temp;
    temp=NULL;
}
void pwd(Fylesystem *F){ ///Print out the path to the Current Working Directory///
    int i;
    Inode *temp;
    temp=F->current;
    char Path[1000],Path2[1000];
    for(i=0;i<1000;i++){
        Path2[i]='\0';
        Path[i]='\0';
    }
    F->current->count=1;
    while(F->current->Parent){
        F->current->Parent->count=1;
        F->current=F->current->Parent;
    }
    if(F->current==F->Root&&!F->current->Child)
        printf("/\n");
    else if(temp!=F->Root&&F->current==F->Root){
        //printf("/");
        walk2(F,F->current->localname);
        F->current=temp;
        printf("\n");
    }else if(temp==F->Root&&F->current==F->Root)
        printf("/\n");
    else{
        walk2(F,F->current->localname);
        F->current=temp;
        printf("\n");
    }
    temp=NULL;
}
void find(Fylesystem *F,char name[20]){ ///Find a File/Directory inside of the Filesystem///
    int number;
    number=findInode(F, name);
    if(number==0)
        printf("Error: No such name exists\n");
    else
        pwd(F);
}
void removed(Fylesystem *F,char name[20]){ ///Remove a File/Directory from the Filesystem///
    int number;
    number=findInode(F, name);
    if(number==0)
        printf("Remove Error: Cannot remove file or directory.");
    else if(F->current->Child){
        printf("Remove Error: directory ’");
        pwdrm(F);
        printf(" 'is not empty.\n");
    }else{
        if(F->current->Previous){
            F->current->Previous->Sibling=F->current->Sibling;
            memset(F->current->localname,0,20);
        }else
            memset(F->current->localname,0,20);
    }
    
}
void forcedremoved(Fylesystem *F,char name[20]){ ///Forcefully removes all contents of a Directory from the Filesystem///
    int number;
    number=findInode(F, name);
    if(number==0)
        printf("Remove Error: Cannot remove file or directory.");
    else{
        rmfwalk(F, name);
    }
}
////////////////////////////////////////////

//////////Utility Functions/////////////////
int comparedirectories(Inode* current,Inode* new){ ///Compares the two directories/files lexographically to see where they are placed///
    char crnt[20],nw[20];
    int i,k,nwprecedence=0,crntprecedence=0,counter=0;
    for(k=0;k<20;k++){
        crnt[k]='\0';
        nw[k]='\0';
    }
    strcpy(crnt, current->localname);
    strcpy(nw,new->localname);
    for(i=0;i<21;i++){
        if(nw[i]=='.'){
            if(new->flag==0){
                counter+=1;
                if(counter>1){
                    nwprecedence=1;
                    counter=0;
                    return 0;
                }
                else{
                    counter=0;
                    break;
                }
            }else{
                nwprecedence=1;
                return 0;
            }
        }else if(crnt[i]=='.'){
            if(current->flag==0){
                counter+=1;
                if(counter>1){
                    crntprecedence=1;
                    counter=0;
                    return 0;
                }
                else{
                    counter=0;
                    break;
                }
            }else{
                crntprecedence=1;
                return 0;
            }
        }
    }for(i=0;i<21;i++){
        if(nw[i]=='-'){
            nwprecedence=2;
            return 0;
        }else if(crnt[i]=='-'){
            crntprecedence=2;
            return 1;
        }
    }for(i=0;i<21;i++){
        if(nw[i]=='_'){
            nwprecedence=3;
            return 0;
        }else if(crnt[i]=='_'){
            crntprecedence=3;
            return 1;
        }
    }for(i=0;i<21;i++){
        if(nw[i]>crnt[i]){
            nwprecedence=4;
            i=22;
        }else if(nw[i]<crnt[i]){
            crntprecedence=4;
            i=22;
        }
    }
    if(crntprecedence<nwprecedence)
        return 1;
    else if(crntprecedence==nwprecedence)
        return 1;
    else
        return 0;
}
void insertsiblingInode(Inode *inode, Fylesystem *F){ ///Inserts a File/Directory into it's correct sibling position based on the comparison function///
    Inode *tempholder = NULL;
    int number=0;
    if(strcmp(F->current->localname, inode->localname)==0){
        F->error=1;
        return;
    }else{
        if(!F->current->Sibling){
            number=comparedirectories(F->current,inode);
            if(number==1){
                F->current->Sibling=inode;
                inode->Parent=F->current->Parent;
                inode->Previous=F->current;
                F->current=inode;
                F->current->count=1;
            }else if(number==0){
                Inode *temp;
                temp=F->current;
                inode->Sibling=temp;
                inode->Parent=F->current->Parent;
                F->current->Parent->Child=inode;
                F->current->Previous=inode;
                F->current=inode;
                F->current->count=1;
            }
        }else{
            number=comparedirectories(F->current, inode);
            if(number==1)
                F->current=F->current->Sibling;
            else{
                Inode *temp;
                temp=F->current;
                inode->Sibling=temp;
                inode->Parent=F->current->Parent;
                F->current->Parent->Child=inode;
                F->current->Previous=inode;
                F->current=inode;
                F->current->count=1;
                return;
            }
            while(F->current){
                number=comparedirectories(F->current, inode);
                if(number==1){
                    if(!F->current->Sibling){
                        F->current->Sibling=inode;
                        inode->Parent=F->current->Parent;
                        inode->Previous=F->current;
                        F->current=inode;
                        F->current->count=1;
                        tempholder=F->current;
                    }
                }else if(number==0){
                    Inode *temp,*prevtemp;
                    if(F->current==F->Root->Child){
                        temp=F->Root->Child;
                        inode->Sibling=temp;
                        inode->Previous=NULL;
                        F->Root->Child=inode;
                        temp->Previous=inode;
                        F->current=inode;
                        F->current->count=1;
                        tempholder=F->current;
                        break;
                    }else{
                        prevtemp=F->current->Previous;
                        temp=F->current;
                        inode->Sibling=temp;
                        inode->Parent=F->current->Parent;
                        prevtemp->Sibling=inode;
                        temp->Previous=inode;
                        inode->Sibling=temp;
                        inode->Previous=prevtemp;
                        F->current=inode;
                        F->current->count=1;
                        tempholder=F->current;
                        break;
                    }
                }
                F->current=F->current->Sibling;
            }
            F->current=tempholder;
        }
    }
}
void resetcount(Fylesystem *F){ ///Helper function designed to help traverse the Filesystem by reseting the number of times a file has been looked at///
    Inode *tempchild=F->current,*tempsibling;
    while(tempchild){
        tempchild->count=0;
        tempsibling=tempchild->Sibling;
        while(tempsibling){
            tempsibling->count=0;
            F->current=tempsibling;
            tempsibling=tempsibling->Sibling;
            resetcount(F);
        }
        tempchild=tempchild->Child;
    }
    F->error=0;
    F->Root->count=1;
}
int findInode(Fylesystem *F,char name[20]){ ///Finds the File/Directory inside of the Fylesystem///
    Inode *tempchild=F->current,*tempsibling = NULL;
    int number=0;
    while(tempchild){
        if(strcmp(tempchild->localname, name)==0){
            F->current=tempchild;
            return 1;
        }else{
            number= 0;
        }
        tempsibling=tempchild->Sibling;
        while(tempsibling){
            if(strcmp(tempsibling->localname, name)==0){
                F->current=tempsibling;
                return 1;
            }else{
                number=0;
                tempsibling=tempsibling->Sibling;
            }
        }
        tempchild=tempchild->Child;
    }
    return number;
}
int findcdInode(Fylesystem *F,char name[20]){ ///Finds the Current Working Directory's Position///
    Inode *tempchild=F->Root->Child,*tempsibling = NULL;
    int number=0;
    while(tempchild){
        if(strcmp(tempchild->localname, name)==0){
            F->current=tempchild;
            return 1;
        }else
            number= 0;
        tempsibling=tempchild->Sibling;
        while(tempsibling){
            if(strcmp(tempsibling->localname, name)==0){
                F->current=tempsibling;
                return 1;
            }else{
                number=0;
                tempsibling=tempsibling->Sibling;
            }
        }
        tempchild=tempchild->Child;
    }
    return number;
}
void walk(Fylesystem *F,char name[20]){ ///Walks the tree starting from the Current Working Directory's Child///
    int number=0,i;
    char directorynames[20],filenames[20],split[2]="/",*directory,*file;
    for(i=0;i<20;i++){
        directorynames[i]='\0';
        filenames[i]='\0';
    }
    number=findcdInode(F, name);
    Inode *tempchild=F->current->Child,*tempsibling = NULL;
    if(!tempchild)
        return;
    else{
        if(tempchild->flag==0){
            strcat(filenames, tempchild->localname);
            strcat(filenames, split);
        }else if(tempchild->flag==1){
            strcat(directorynames, tempchild->localname);
            strcat(directorynames, split);
        }
        tempsibling=tempchild->Sibling;
        while(tempsibling){
            if(tempsibling->flag==0){
                strcat(filenames, tempsibling->localname);
                strcat(filenames, split);
            }else if(tempsibling->flag==1){
                strcat(directorynames, tempsibling->localname);
                strcat(directorynames, split);
            }
            tempsibling=tempsibling->Sibling;
        }
        if(F->count==0){
            directory=strtok(directorynames,split);
            while(directory!=NULL){
                printf("D %s\n",directory);
                directory=strtok(NULL,split);
            }
            file=strtok(filenames,split);
            while(file!=NULL){
                printf("F %s\n",file);
                file=strtok(NULL,split);
            }
        }
    }
}
void walk2(Fylesystem *F,char name[20]){ ///Walks the tree starting from the Current Working Directory///
    Inode *tempchild=F->current,*tempsibling = NULL;
    while(tempchild){
        if(tempchild->count==1){
            if(tempchild==F->Root){
                printf("%s",tempchild->localname);
                tempsibling=tempchild->Sibling;
                tempchild->count=0;
            }else if(tempchild==F->Root->Child){
                printf("%s",tempchild->localname);
                tempsibling=tempchild->Sibling;
                tempchild->count=0;
            }else{
                printf("/%s",tempchild->localname);
                tempsibling=tempchild->Sibling;
                tempchild->count=0;
            }
        }else{
            tempsibling=tempchild->Sibling;
        }
        while(tempsibling){
            if(tempsibling->count==1){
                printf("/%s",tempsibling->localname);
                tempsibling->count=0;
                F->current=tempsibling;
                walk2(F, tempsibling->localname);
                tempsibling=tempsibling->Sibling;
            }else{
                F->current=tempsibling;
                walk2(F, tempsibling->localname);
                tempsibling=tempsibling->Sibling;
            }
        }
        tempchild=tempchild->Child;
    }
}
void rmfwalk(Fylesystem *F,char name[20]){ ///Walks the Filesystem and removes all Children and Siblings of the Current Working Directory///
    Inode *tempchild=F->current,*tempsibling = NULL;
    while(tempchild){
        memset(tempchild->localname,0,20);
        tempsibling=tempchild->Sibling;
        tempchild->count=0;
        while(tempsibling){
            memset(tempsibling->localname,0,20);
            F->current=tempsibling;
            walk2(F, tempsibling->localname);
            tempsibling=tempsibling->Sibling;
            tempsibling->count=0;
        }
        tempchild=tempchild->Child;
    }
}
void pwdrm(Fylesystem *F){ ///Prints out the Path to the file to be removed///
    int i;
    Inode *temp;
    temp=F->current;
    char Path[1000],Path2[1000];
    for(i=0;i<1000;i++){
        Path2[i]='\0';
        Path[i]='\0';
    }
    F->current->count=1;
    while(F->current->Parent){
        F->current->Parent->count=1;
        F->current=F->current->Parent;
    }
    if(F->current==F->Root&&!F->current->Child)
        printf("/");
    else if(temp!=F->Root&&F->current==F->Root){
        //printf("/");
        walk2(F,F->current->localname);
        F->current=temp;
        printf("");
    }else if(temp==F->Root&&F->current==F->Root)
        printf("/");
    else{
        walk2(F,F->current->localname);
        F->current=temp;
        printf("");
    }
    temp=NULL;
}
////////////////////////////////////////////

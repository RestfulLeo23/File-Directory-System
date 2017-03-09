//
//  Header.h
//  Project01
//
//  Created by Scott English on 4/25/16.
//  Copyright © 2016 Scott English. All rights reserved.
//

#ifndef Header_h
#define Header_h

#include "stdheader.h"
////////Data Structures///////////////////
typedef struct Inode Inode;
typedef struct Filesystem Fylesystem;
struct Inode{
    int flag;
    int count;
    Inode *Parent;
    Inode *Child;
    Inode *Sibling;
    Inode *Previous;
    char localname[20];
};
struct Filesystem{
    Inode *Root;
    Inode *current;
    int error;
    int count;
};
////////////////////////////////////////////

/////Initialize the Data Structures/////////
Inode* createInode(int,char[20]);
Fylesystem* createFylesystem(Inode*);
////////////////////////////////////////////

/////////InterFace Commands/////////////////
void insertInode(Inode*,Fylesystem*);
void cd(char[20],Fylesystem*);
void ls(char[20],Fylesystem*);
void pwd(Fylesystem*);
void find(Fylesystem*,char[20]);
void removed(Fylesystem*,char[20]);
void forcedremoved(Fylesystem*,char[20]);
////////////////////////////////////////////

//////////Utility Functions/////////////////
int comparedirectories(Inode*,Inode*);
void insertsiblingInode(Inode*,Fylesystem*);
void resetcount(Fylesystem*);
int findInode(Fylesystem*,char[20]);
int findcdInode(Fylesystem*,char[20]);
void walk(Fylesystem*,char[20]);
void walk2(Fylesystem*,char[20]);
void rmfwalk(Fylesystem*,char[20]);
void pwdrm(Fylesystem*);
////////////////////////////////////////////

#endif /* Header_h */

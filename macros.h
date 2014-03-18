/*
	Macros for car rental system
	Author: David Gardner
	Last Edited Date: 4/Dec/2011
	Language: C
	Description: all macros and globals are defined here
*/

#include <stdio.h> //standard input/Output header file
#include <conio.h> //header for common input/Output to Boreland
#include <stdlib.h>//standard library miscellaneous files
#include <ctype.h>
#include <windows.h>//header file for wndows applications (i.e. coloured text)
#include <string.h>//header file for operations with strings
#include <time.h>//header file for time and date operations

//Macro/Symbolic Constant creation
#define TAX 0.165
#define manager "manager"
#define size 80
#define full_page 1000000
#define TIME (printf("\n%s",ctime(&timer)))
#define PAUSE (system("pause"));
#define CLEAR (system("cls"));

char cHopt, page[full_page];//globally declared character variable
int opt, wks, frKM, days, weeks;//globally declared integer variables
static int num_client = 0, starter[] = {3,3,3,3,3};

enum maxim {luxury = 5,waggon,suv,cycle,frwheel};//enumerated constant starts at 5

////////////////STRUCTURE///////////////////

typedef struct THEN{
        int day;
        int month;
        int year;
}PERIOD;

typedef struct CRED{
    char pre_reg;
    char retunred;
}status;

typedef struct INFO{
    char fname[50];
    char lname[50];
}persona;

typedef struct ROAD{
    float kilo ;
	float free_kilo;
}distance;

typedef struct customer{
	persona pers;
    char address1[30];
	char address2[20];
	int tel_num;
	int card_type;
	int vcategory;
	int card_num;
	float payment ;
    PERIOD date[2];
	int days;
	float fee;
	char insurance;
	float insfee;
	char nav_sys;
	float navfee;
	distance KM;
	status STAT;
}DATA;//structure of type DATA to store variables used for unique customer data

typedef struct record{
       float total[5];
       int rent[5];
}recorders;

recorders rec = {{0.0},{0}};
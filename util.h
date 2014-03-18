/*
	Macros for car rental system
	Author: David Gardner
	Last Edited Date: 4/Dec/2011
	Language: C
	Description: utility functionality tools defined here
*/

#include "macros.h"

void showtime(){
  time_t  timer = time(NULL);
  TIME;
}

void setcolor(int color){//function to change text color
     HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
     SetConsoleTextAttribute(hcon,color);
}
void header(){
    static int open;
    if(open >= 1){
         PAUSE
         CLEAR
    }
    srand(time(NULL));
    setcolor(rand()%15+1);//random coloured text
    printf("\n\t\t\t||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n"
           "\t\t\t||  D.C.J. Car Rental Services  ||\n"
           "\t\t\t||           WELCOME            ||\n"
           "\t\t\t||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n");
    showtime();
    setcolor(15);//white text
    open++;
}

char approval(){
     fflush(stdin);
     scanf("%c",&cHopt);
     cHopt = toupper(cHopt);
     if(cHopt == 'Y' || cHopt == 'N')
           return cHopt;

     else{
         printf("\n\n\tIncorrect input...Enter [Y] or [N] ");
         cHopt = approval();              
     }
}

void pset(char * fname, int len){
     char passet[len],pass1[len], *set,*ppass;
     int check = 0;
     FILE *checker;
     do{
   		  if((checker = fopen(fname,"r"))!=NULL)
         		remove(fname);

         //header();
         printf("\nEnter desired password(max %d characters with no spaces):\t",len);
         for(set = passet; set <passet + len; set++){//shows typed password as encoded '*' on screen
                 fflush(stdin);
                 *set=getch();//getch() used to not show characer on screen
                 printf("*");//outputs '*' each time a key is pressed
         }
         printf("\n\nVerify desired password:\t");
         for(ppass = pass1; ppass < pass1 + len; ppass++){
                 fflush(stdin);
                 *ppass = getch();
                 printf("*");
         }
         *ppass = *set = '\0';
         getch();
         if(strcmp(passet,pass1)==0){
                checker = fopen(fname,"w");
                fprintf(checker,"%s",pass1);
                printf("\nPassword Accepted\n");
         }
		 else{
              printf("\n\nIncorrect validation input please try again\n\n");
              
         }fclose(checker);
     }while(strcmp(passet,pass1)!=0);//end of do..while structure
}

int numDays(int *preD, int *preM, int *preY, int *nexD, int *nexM, int *nexY){
    int dMonths[]={31,28,31,30,31,30,31,31,30,31,30,31} ;/*Array to store the
                                           number of total days in each month*/

    if(*preY == *nexY){// if rental is in the same year
             if(*preM == *nexM)//if the return date is in the same month
                  return *nexD - *preD;
             else if(*nexM - *preM == 1)//if return date is in next month
                  return (dMonths[(*preM)-1] - *preD)+ *nexD;
             
    }else if(*preY < *nexY)//calculates the number of days into the new year from december
          return (dMonths[11] - *preD) + *nexD;
}

void cat_view(){
     switch(opt){
          case 1: printf("FOUR-WHEEL DRIVE:%d",starter[opt-1]);
          break;
                               
          case 2: printf("WAGGON:%d",starter[opt-1]);
          break;
                   
          case 3: printf("SUV's:%d",starter[opt-1]);
          break;
                                      
          case 4: printf("LUXURY:%d",starter[opt-1]);
          break;
                                      
          case 5: printf("MOTOTR-CYCLES:%d",starter[opt-1]);
     }
}

int managerlog(){
    int num = 3, check;
    char passw[7], *ptr;//password to store 7 characters
    puts("\nEnter Manager's Password\n");
    for(ptr = passw; ptr < passw + 7; ptr++){
                  fflush(stdin);
                  *ptr = getch();
                  printf("*");
    }
    *ptr = '\0';//eliminate side effect of getch
    while(strcmp(passw,manager)){           
          num--;//post decrement of int num
          if(num == 0)
              return num;
       
          printf("\n\t%d Attempts Remain, Please Re-Enter Manager's Password\n",num);
          for(ptr = passw; ptr < passw + 7; ptr++){
                      fflush(stdin);
                      *ptr = getch();
                      printf("*");
          }
          *ptr = '\0';//eliminate side effect of getch
       }
       puts("\n"); 
       return num;       
}
int opt_check(){
    fflush(stdin);
    scanf("%d",&opt);     
    if(opt>6||opt<1){
         printf("\n\nSelect either 1,2,3, 4 or 5 as valid option\n:");
         fflush(stdin);
         opt = opt_check();//recursive call
     }
     else
        return opt;//base case returns value
}
void category(){
    setcolor(9);//blue text
	int i;
    for(i=0;i<80;i++)
		printf("*");//end for
    printf("[Option]   Category        Week Fee Day Fee   MaxKM/Wk  Insur/Day   ExtraKM\n"
           " [1]       STANDARD 4 DR   $10,000  $1,500    2,000     $100        $10\n" 
	       " [2]       STATION WAGONS  $12,000  $2,000    2,000     $120        $12\n"
	       " [3]       SUV'S           $14,000  $2,500    1,800     $140        $14\n"
           " [4]       LUXURY          $18,000  $3,000    1,300     $250        $18\n"
           " [5]	   MOTORCYCLE      $5,000   $1,200    1,000     $230        $10\n");
    for(i=0;i<80;i++)
		printf("*");//end for
    setcolor(15);//white text
    printf("\nAvailaible Options\n\nFOUR WHEEL DRIVES:%d\n\nWAGGONS:%d\n\n",starter[0],starter[1]);
    printf("SUV'S :%d\n\nLUXURY:%d\n\nMOTOR CYCLE:%d\n\n",starter[2],starter[3],starter[4]);
}

int login(char * filename, int length){
   char initial[length],pword[length], *pptr = NULL,*iptr = NULL; //local declarations
   FILE *password = NULL;
   if((password = fopen(filename,"r")) == NULL)//if there is no preset password
   {
       printf("\nDo you wish to set up Password now?\n\nYes[Y]\tNo[N]\t:");
       cHopt = approval();
       fflush(stdin);//clear input stream
       //CLEAR//clear screen
       if(cHopt == 'Y')//if the user wishes to set a password
            pset(filename,length);//function call
                
   }
   else{
       rewind(password);//in case file cursor is at EOF bring it to the front
       printf("\nEnter Password: ");
	   int run=3;//acts as a counter
	   do{
			for(iptr = initial;iptr < initial + length; iptr++){                 
				fflush(stdin);
                *iptr = getch();
                printf("*");
			}
			fgets(pword,length+1,password);//scans data from file into a variable
			if(strcmp(initial,pword)==0)
				break;
			
			run--;
			if(run == 0){//password login attempts are used up
               printf("\nAccess Denied\tProgram Locked\n\n");
               PAUSE//macro
               return 0;//locks the system
			}
			printf("\nIncorrect Password...try again\n");
			iptr = NULL;
		}while(strcmp(initial,pword)!=0);
		CLEAR;
		fclose(password);//closing the file
	    puts("\n\tAccess Granted!!!");       
	}
	return 1;
}
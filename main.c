/*
	Program: Basic Car Rental System
	Author: David Gardner
	Last Edited Date: 4/Dec/2011
	Language: C
	Description: School project to simulate a car rental system
*/

#include "util.h"

void addcustomer(DATA*contacts,int*cust_pos,int option);
void get_info(DATA*,int*);
void savecustomer(DATA *);
void single_customer(DATA *, char *);//function 2 create a file under the customer's name to access return rental data
void prelim_fee(DATA*,int*,int,int,int);
void endbill(DATA *end);
void menu_1();  //function to show main menu
void cust_bill(DATA *billing,int numday);

int main(){
    FILE *inventory;
    inventory = fopen("Vehicle Count.txt","r");
	int i;
    if(inventory != NULL){ //if vehicle inventory is different from the original starting numbers,get the values
          for(i = 0; i < 5;i++)
               fscanf(inventory,"%d",&starter[i]);
    }
    fclose(inventory);
    header();
    if(!login("Syspass",7)){
		printf("\nProgram locked...Goodbye");
		return 1;
	}
    menu_1();
    inventory = fopen("Vehicle Count.txt","w"); //overwriting the file iwith new data
    for(i =0; i < 5;i++)
        fprintf(inventory,"%d\n",starter[i]);
               
    printf("\n\nNumber of Customers Served: %d\n\n",num_client);
    puts("Closing Program...Have a Nice Day =)");
    PAUSE
    fclose(inventory);//closing the file
	return 0;
}

void reg_customers(DATA *contacts,int *cust_pos){
	FILE *Add;//opening the file
	if((Add = fopen("new_customers","r")) != NULL){
          while(!feof(Add))
             fread(&contacts[*cust_pos],sizeof(DATA),1,Add);  //end while
		
          (*cust_pos)++;	
	      fclose(Add);//closing file pointer
    }
	fclose(Add);//closing file pointer
}//end reg_customers

void process(DATA*contacts,int*cust_pos){
     if(opt == 2){ //if option was selected for an exiting customer
          puts("\n\nData Base Check: \n");
          reg_customers(contacts,cust_pos);//this will check whether or not the customer is registered with the company
		  get_info(contacts,cust_pos);
		  contacts->STAT.pre_reg = 'Y';
     }//end if
     else{ //if customer is not registered
          category(); 
          puts("\nCategory vehicle being rented: ");
          opt = opt_check();
          if(starter[opt-1] == 0){
               printf("\nNo Category %d vehicles available at this time\n",opt);
               puts("Rent a different type? Yes[Y] or No[N]:");
                       approval();
                       if(cHopt == 'N')
                                return;
                       else{
                            puts("\nCategory vehicle being rented: ");
                            opt = opt_check();
                       }
                }switch(opt){
                      case 1: puts("<<<STANDARD 4-WHEEL DRIVE>>>\n");
                      break;
               
                      case 2: puts("<<<STATION WAGONS>>>\n");
                      break;
                                
                      case 3: puts("<<<SPORTS UTILITY VEHICLES>>>\n");
                      break;
               
                      case 4: puts("<<<LUXURY>>>\n");
                      break;
                 
                      case 5: puts("<<<MOTORCYCLESX>>>\n");
                      break;                     
               }               
          rec.rent[opt-1]++;
          starter[opt-1]--;//decrement from the amounts in inventory
          addcustomer(contacts,cust_pos,opt);
	}//end else
}

void addcustomer(DATA*contacts,int*cust_pos,int option){
      days = 0, weeks = 0;
      int i = 1;
      fflush(stdin);
	  printf("PLEASE ENTER YOUR FIRST-NAME\n\n\t");
	  gets(contacts[*cust_pos].pers.fname);
	  printf("\n\nPLEASE ENTER YOUR LAST-NAME\n\n\t");
	  gets(contacts[*cust_pos].pers.lname);
	  fflush(stdin);
	  printf("\n\nBILLING ADDRESS\n\tADDRESS 1 :");
	  gets(contacts[*cust_pos].address1);
	  fflush(stdin);
	  printf("\n\n\tADDRESS 2 :");
	  gets(contacts[*cust_pos].address2);
	  fflush(stdin);
	  printf("\nENTER PHONE NUMBER\n");
	  scanf("%d",&contacts[*cust_pos].tel_num);
      contacts[*cust_pos].vcategory = opt;
      printf("\nCREDIT CARD[1] OR CASH[2]\n");
      do{
           opt = opt_check();
           if(opt !=1 && opt != 2)
                  puts("\nPlease Choose 1 or 2 for this option");
      }while(opt !=1 && opt != 2);
	  contacts[*cust_pos].card_type = opt;
      if(contacts[*cust_pos].card_type == 1){
			printf("Enter credit card number\n");
			scanf("%d",&contacts[*cust_pos].card_num);
      }//end if
      puts("\nEnter the Rental date (using numbers) ");
      printf("\n\tDay: ");
      scanf("%d",&contacts[*cust_pos].date[0].day);
      printf("\n\tMonth: ");
      scanf("%d",&contacts[*cust_pos].date[0].month);
      printf("\n\tYear: ");
      scanf("%d",&contacts[*cust_pos].date[0].year);
      fflush(stdin);
	  puts("\nEnter the EXPECTED Return date (using numbers) ");
      printf("\n\tDay: ");
      scanf("%d",&contacts[*cust_pos].date[1].day);
      printf("\n\tMonth: ");
      scanf("%d",&contacts[*cust_pos].date[1].month);
      printf("\n\tYear: ");
      scanf("%d",&contacts[*cust_pos].date[1].year);
      fflush(stdin); //clear input stream
      contacts[*cust_pos].days = numDays(&contacts[*cust_pos].date[0].day,\
                                        &contacts[*cust_pos].date[0].month,\
                                        &contacts[*cust_pos].date[0].year,\
										&contacts[*cust_pos].date[1].day,\
										&contacts[*cust_pos].date[1].month,\
										&contacts[*cust_pos].date[1].year);//function call
      //if statement calculates the number of weeks and days
      if(contacts[*cust_pos].days>=7){
             weeks = contacts[*cust_pos].days/7;
             days = contacts[*cust_pos].days%7;
      }else
             days = contacts[*cust_pos].days;
          
      printf("\nWeeks: %d\t Day(s): %d\n",weeks,days);
      getch();
	  printf("DO YOU WANT INSURANCE? YES[Y] NO[N]\n");
	  approval();
	  contacts[*cust_pos].insurance = cHopt;
	  printf("DO YOU WANT NAVIGATION SYSTEMS?  YES[Y] NO[N]\n");
	  approval();
	  contacts[*cust_pos].nav_sys = cHopt;
      prelim_fee(contacts,cust_pos,contacts[*cust_pos].vcategory,weeks,days);
      contacts[*cust_pos].STAT.retunred = 'N';
	  savecustomer(&contacts[*cust_pos]);
	  printf("\n\n\t\tSAVING DATA\n\n\t");
 	  for(i = 0; i <20; i++){	  		   
	 	   printf(".");
	  	   Sleep(20); 	   
      }//end for
      printf("\a YOUR DATA HAVE BEEN SUCCESFULLY SAVED\n");
      (*cust_pos)++;
}//end addcustomer

void get_info(DATA*contacts,int*cust_pos){
	char fkey[50],lkey[50];//search keys for the names
	int run = 0;
	fflush(stdin);
	printf("PLEASE ENTER YOUR FIRST-NAME\n");
	gets(fkey);
	fflush(stdin);
	printf("PLEASE ENTER YOUR LAST-NAME\n");
	gets(lkey);
	if((searchList(contacts,fkey,lkey))==1){
		puts("Name found!\nYOU ARE REGISTERED WITH US PROCEED!!\n\nRent New Vehicle? Yes [Y] or No [N]");
		approval();
		if(cHopt == 'Y'){
              opt = 1;
              process(contacts,cust_pos);
              contacts->STAT.pre_reg ='N';
              //discount = 2;
		}
	}else{
		puts("\nName Not found!\n\t\tYOU ARE NOT REGISTERED!!\n\n\t");
		PAUSE //macro
		opt = 1;
		process(contacts,cust_pos);
    }//end if
	(*cust_pos)++;
}//end get_info

void prelim_fee(DATA*contacts,int*cust_pos,int option,int alloc_weeks,int alloc_days){
     float discount,extr_kilo,paid;
     switch(option){
            case 1: contacts[*cust_pos].fee = (10000 * alloc_weeks)+(1500* alloc_days);
                    if(contacts[*cust_pos].insurance == 'Y'){
                        contacts[*cust_pos].insfee = (100*(7*alloc_weeks+ alloc_days)) ;
                        contacts[*cust_pos].fee += contacts[*cust_pos].insfee;
                    }//end if
                    
                    if(contacts[*cust_pos].nav_sys == 'Y'){
                        contacts[*cust_pos].navfee = (350*(7*alloc_weeks+ alloc_days));
                        contacts[*cust_pos].fee += contacts[*cust_pos].navfee;
                    }//end if
                    
                    if(contacts->STAT.pre_reg == 'Y'){
                        discount = 0.1;
                        printf("\nDiscount Amount: $%.2f",(contacts[*cust_pos].fee)*discount);
                        contacts[*cust_pos].fee*=.9;     
                    }
            break;
            
            case 2: contacts[*cust_pos].fee =(12000*alloc_weeks)+(2000*alloc_days);
	                if(contacts[*cust_pos].insurance == 'Y'){
                          contacts[*cust_pos].insfee = (120* (7*alloc_weeks+ alloc_days)) ;
                          contacts[*cust_pos].fee += contacts[*cust_pos].insfee;
                    }//end if
			    
                    if(contacts[*cust_pos].nav_sys == 'Y'){
                           contacts[*cust_pos].navfee = (350*(7*alloc_weeks+ alloc_days));
                           contacts[*cust_pos].fee += contacts[*cust_pos].navfee;
                    }//end if
                    
                    if(contacts[*cust_pos].STAT.pre_reg == 'Y'){//determines fee for registtered customer
                           discount = 0.1;
                           printf("\nDiscount Amount: $%.2f\n",(discount*contacts[*cust_pos].fee ));
                           contacts[*cust_pos].fee*=.9;         
                    }
            break;
            
            case 3: contacts[*cust_pos].fee =(14000*alloc_weeks)+(2500*alloc_days);
				    if(contacts[*cust_pos].insurance == 'Y'){
                        contacts[*cust_pos].insfee = (140* (7*alloc_weeks+ alloc_days)) ;
                        contacts[*cust_pos].fee += contacts[*cust_pos].insfee;
                    }//end if
                   
                   if(contacts[*cust_pos].nav_sys == 'Y'){
                        contacts[*cust_pos].navfee = (350*(7*alloc_weeks+ alloc_days));
                        contacts[*cust_pos].fee += contacts[*cust_pos].navfee;
                   }//end if
                   if(contacts[*cust_pos].STAT.pre_reg =='Y'){//if registered user is not charged extra km
                        discount = 0.1;
                        printf("\nDiscount Amount: $%.2f",(discount*contacts[*cust_pos].fee ));
                        contacts[*cust_pos].fee*=.9;         
                   }//end if
            break;
            
            case 4: contacts[*cust_pos].fee =(18000*alloc_weeks)+(3000*alloc_days);
			        if(contacts[*cust_pos].insurance == 'Y'){
                         contacts[*cust_pos].insfee = (250* (7*alloc_weeks+ alloc_days)) ;
                    
                         contacts[*cust_pos].fee += contacts[*cust_pos].insfee;            
                    }//end if
                 
                    if(contacts[*cust_pos].nav_sys == 'Y'){
                            contacts[*cust_pos].navfee = (350*(7*alloc_weeks+ alloc_days));
                            contacts[*cust_pos].fee += contacts[*cust_pos].navfee;
                    }//end if
                       
                    if(contacts[*cust_pos].STAT.pre_reg == 'Y'){//determines fee for registered customer
                            discount = 0.1;
                            printf("\nDiscount Amount: $%.2f",(discount*contacts[*cust_pos].fee ));
                            contacts[*cust_pos].fee*=.9;       
                    }
            break;
            
            case 5: contacts[*cust_pos].fee =(5000*alloc_weeks)+(1200*alloc_days);
                    if(contacts[*cust_pos].insurance == 'Y'){
                        contacts[*cust_pos].insfee = (250* (7*alloc_weeks+ alloc_days)) ;
                        contacts[*cust_pos].fee += contacts[*cust_pos].insfee;
                    }//end if
                    		
                    if(contacts[*cust_pos].nav_sys == 'Y'){
                        contacts[*cust_pos].navfee = (350*(7*alloc_weeks+ alloc_days));
                        contacts[*cust_pos].fee += contacts[*cust_pos].navfee;
                    }//end if
                    
                    if(contacts[*cust_pos].STAT.pre_reg == 'Y'){//determines fee for registtered customer
                           discount = 0.1;
                           printf("\nDiscount Amount: $%.2f",(discount*contacts[*cust_pos].fee ));
                           contacts[*cust_pos].fee*=.9; 
                    }
            break;
     }
     cust_bill(&contacts[*cust_pos],(alloc_days+7*alloc_weeks));
     printf("\n\tInsurance Charge: $%.2f\n",contacts[*cust_pos].insfee);
     printf("\n\tNavigation system fee: $%.2f\n",contacts[*cust_pos].navfee);
     printf("\n\tTax @ 16.5%: $%.2f",(contacts[*cust_pos].fee*TAX));
     contacts[*cust_pos].fee*=(1+TAX);
     printf("\n\n\tTotal BASIC FEE:$%.2f",contacts[*cust_pos].fee );     
     puts("\n\n\tTHIS INVOICE MUST BE PRESENTED ON THE RETURN DATE OF THE RENTAL");
     printf("\n\nMinimum Payment To Be Made Now: $%.2f",(0.5*contacts[*cust_pos].fee));
     puts("\nPLEASE ENTER THE CURRENT PAYMENTS\n");
     scanf("%f",&paid);
     do{
        if(paid < (0.5*contacts[*cust_pos].fee)){
             printf("The MINIMUM payment is: $%.2f\nPayment: ",(0.5*contacts[*cust_pos].fee));
             scanf("%f",&paid);
        }   
     }while(paid < (0.5*contacts[*cust_pos].fee));
     contacts[*cust_pos].fee -= paid;
     printf("Rmaining Balance: $%.2f",contacts[*cust_pos].fee);
     contacts[*cust_pos].payment = paid;
     setcolor(15);//white text
     getch();
}

void savecustomer(DATA *contacts){
     FILE* Add, *LIST ;
     DATA person = {"am","am","am","am",0,0,0,0,0.0,{{0,0,0},{0,0,0}},0,0.0,'N',0.0,'N',0.0,{0.0,0.0},{'N','N'}};//initializing struct to rid it of garbage     
     Add = fopen("new_customers","r");
     char name[110],checker[50], check2[50];
	 strcpy(name,contacts->pers.fname);
     strcat(name,contacts->pers.lname);//putting the fname & lname together to form a single name
     single_customer(contacts,name);  //function 2 create a file under the customer's name to access return rental data
     if((LIST = fopen("Customer List.txt","r")) == NULL){//determines if file is opened correctly and if the file exists
         LIST = fopen("Customer List.txt","w");//open file in write mode
         fprintf(LIST,"%s %s\n",contacts->pers.fname,contacts->pers.lname);   
     }else{
          strcpy(checker,contacts->pers.fname);
          strcpy(check2,contacts->pers.lname); 
          if((searchList(&person,checker,check2))!=1){
                 LIST = fopen("Customer List.txt","a+");
                 fprintf(LIST,"%s %s\n",contacts->pers.fname,contacts->pers.lname);          
          }
     }
     if( Add !=NULL)
		fwrite(contacts,sizeof(DATA),1,Add);//end if
		
	 else{
        Add = fopen("new_customers","a+");
        fseek(Add,sizeof(DATA),SEEK_END);//positioning file cursor
        fwrite(contacts,sizeof(DATA),1,Add);
     }
     fclose(Add);
	 fclose(LIST); //closing files            
}

int searchList(DATA *custsrch,char *key1, char *key2){
	int count = 0;
	FILE *search;
	search=fopen("Customer List.txt","r");
	if(search == NULL)
		puts("\nNo customer History Available\n");
	
	else{
         while(!feof(search)){//while the end of file is not reached
               fscanf(search,"%s %s",&custsrch->pers.fname,&custsrch->pers.lname);
               if(strcmp(key1,custsrch->pers.fname)==0){
                      if(strcmp(key2,custsrch->pers.lname)==0)
				          count = 1;
               }
			   else
				   count = 0;//reset to 0 in case last name does not match
			   if(count == 1)
			            break;//if customer name is found in least, the list search is aborted
		}
	}	
	fclose(search);
	return count;
}

void single_customer(DATA *info, char *data){
     FILE *single;
     single = fopen(data,"w+");
     fwrite(info,sizeof(DATA),1,single);
     fclose(single);
}

void add_km(DATA *final){
     final->days = numDays(&final->date[0].day,&final->date[0].month,&final->date[0].year,&final->date[1].day,&final->date[1].month,&final->date[1].year);
     weeks = 0, days = final->days;
     float extr_kilo = 0.0;
     if(final->days>=7){
             weeks = final->days/7;
             days = final->days%7;
     }else
             days = final->days;
     printf("\nWeeks: %d\tDays: %d",weeks,days); 
     srand(time(NULL));
     switch(final->vcategory){
         case 1: if(weeks >= 1){
                       final->KM.free_kilo = weeks*(10+rand()%5);
                       final->KM.kilo -= final->KM.free_kilo;
				       printf("\nYOU GOT %.2f FREE KILOMETERS\n",final->KM.free_kilo);
				       printf("\nTOTAL KILO METERS : %.2f\n",final->KM.kilo);
                 }//end if
				
		         if(final->KM.kilo> 2000){
                      printf("YOU WENT OVER THE SPECIFIED RANGE FOR THAT VEHICLE\n[YOU WILL BE CHARGED]\n");
				      extr_kilo = final->KM.kilo - 2000;
				      printf("\nCOST FOR THAT EXTRA KILO IS: $%.2f\n",extr_kilo*10);
  					  final->fee += (extr_kilo * 10); 
                  }
         break;
         
         case 2:  if(weeks >= 1){
	                 final->KM.free_kilo = (weeks*15+rand()%5);
				     final->KM.kilo -= final->KM.free_kilo;
			         printf("\nYOU GOT %.2f FREE KILOMETERS\n",final->KM.free_kilo);
                     printf("\nTOTAL KILO METERS : %.2f\n",final->KM.kilo);
                }//end if
				
			    if(final->KM.kilo > 2000){
                      printf("\nYOU WENT OVER THE SPECIFIED RANGE FOR THAT VEHICLE\n[YOU WILL BE CHARGED]\n");
					  extr_kilo = final->KM.kilo - 2000;
				      printf("COST FOR THAT EXTRA KILO IS: $%.2f\n",extr_kilo*10);
					  final->fee += (extr_kilo * 10); 
                 }//end if
   
         break;
         
         case 3: if(weeks >= 1){
	                    final->KM.free_kilo = (weeks*19+rand()%5);
				        final->KM.kilo -= final->KM.free_kilo;
			            printf("YOU GOT %.2f FREE KILOMETERS\n",final->KM.free_kilo);
				        printf("TOTAL KILO METERS : %.2f\n",final->KM.kilo);
                   }//end if
				
                   if(final->STAT.pre_reg != 'Y'){//if registered user is not charged extra km
                        if(final->KM.kilo > 1800){
                              printf("YOU WENT OVER THE SPECIFIED RANGE FOR THAT VEHICLE\n[YOU WILL BE CHARGED]\n");
					          extr_kilo = final->KM.kilo - 1800;
				              printf("COST FOR THAT EXTRA KILO IS: $%.2f\n",extr_kilo*10);
                              final->fee += (extr_kilo * 10); 
                         }//end if
                  }
         break;
         
         case 4:  if(weeks >= 1){
	                    final->KM.free_kilo = (weeks*19+rand()%5);
				        final->KM.kilo -= final->KM.free_kilo;
			            printf("YOU GOT %.2f FREE KILOMETERS\n",final->KM.free_kilo);
				        printf("TOTAL KILO METERS : %.2f\n",final->KM.kilo);
                   }//end if
				
                   if(final->STAT.pre_reg != 'Y'){//if registered user is not charged extra km
                        if(final->KM.kilo > 1800){
                              printf("YOU WENT OVER THE SPECIFIED RANGE FOR THAT VEHICLE\n[YOU WILL BE CHARGED]\n");
					          extr_kilo = final->KM.kilo - 1800;
				              printf("COST FOR THAT EXTRA KILO IS:%.2f\n",extr_kilo*10);
                              final->fee += (extr_kilo * 10); 
                         }//end if
                  }
         break;
         
         case 5: if(weeks >= 1){
	                    final->KM.free_kilo = (weeks*19+rand()%5);
				        final->KM.kilo -= final->KM.free_kilo;
			            printf("YOU GOT %.2f FREE KILOMETERS\n",final->KM.free_kilo);
				        printf("TOTAL KILO METERS : %.2f",final->KM.kilo);
                   }//end if
				   
				   if(final->KM.kilo > 1800){
                              printf("YOU WENT OVER THE SPECIFIED RANGE FOR THAT VEHICLE\n[YOU WILL BE CHARGED]\n");
					          extr_kilo = final->KM.kilo - 1800;
				              printf("COST FOR THAT EXTRA KILO IS: $%.2f\n",extr_kilo*10);
                              final->fee += (extr_kilo * 10); 
                   }//end if
         break;
    }endbill(final); 
}

void endbill(DATA *end){
     FILE *finish, *deleter;//file pointers to store history & delete customer temporary file
     char name[2][100];
     strcpy(name[0],end->pers.fname);
     strcat(name[0],end->pers.lname);
     header();
     setcolor(12);//red text
     printf("\n\n\t||~ SALES INVOICE (FINAL) \tVehicle Catgory: %d~||\n",end->vcategory);
     printf("\n\tCustomer: %s %s\n\n",end->pers.fname,end->pers.lname);
     printf("\n\tRental Period %d/%d/%d to %d/%d/%d.....%d Days\n",end->date[0].day,\
                                              end->date[0].month,end->date[0].year,\
                                              end->date[1].day,end->date[1].month,\
                                              end->date[1].year,end->days);
     printf("\n\tInsurance Charge: $%.2f\n",end->insfee);
     printf("\n\tNavigation system fee: $%.2f\n",end->navfee);
     printf("\n\tTax @ 16.5%: $%.2f",(end->fee*TAX));
     end->fee*=(1+TAX);
     printf("\n\n\tFinal FEE:$%.2f\n\t",end->fee);     
     end->payment += end->fee ;
     end->fee = 0;
     setcolor(15);//turquiose text
     end->STAT.pre_reg = 'Y';
     strcpy(name[1],name[0]);
     strcat(name[1]," History");
     finish = fopen(name[1],"a+");//opens new file
     fwrite(end,sizeof(*end),1,finish);
     starter[end->vcategory - 1]++;
     fclose(deleter);
     fclose(finish);     //closing file
     remove(name[0]);             //deletes file
     setcolor(15);//turquiose text
}//end of endbil function

void menu_1(){
     do{
          int cust_pos=0, num;
          char loader[7], checker[8],*check,name[100],cname[100],clname[50];
          DATA *contptr ,contacts[size] =  {{"a","b","c","d",0,0,0,0,0.0,{{1,1,2011},{1,1,2011}},0,0.0,'N',0.0,'N',0.0,{0.0,0.0},{'N','N'}}}; //array of structres, structure variable,pointer to structure 
          DATA singular = {"am","am","am","am",0,0,0,0,0.0,{{0,0,0},{0,0,0}},0,0.0,'N',0.0,'N',0.0,{0.0,0.0},{'N','N'}};//initializing struct to rid it of garbage
          persona person = {" "," "}, *persptr;
          persptr = &person;
          contptr = &singular;
          header();
          reg_customers(contacts,&cust_pos);
          puts("\n\nMenu Selection Items:");
          puts("\n\t~[1]Billing Proceedure\n\t~[2]System Reports\n\t~[3]Inventory\n\t~[4]System Security Options\n\t~[5]Lock Program\n\t~[6]Close Program\n");
          opt = opt_check();
          switch(opt){//C function used to make mutually exclusive decsions based on opt
                 case 1: header();
                         num_client++;
                         printf("\n\t\t\t||Billing Proceedure||\n\n");
                         puts("\n\t[1]New Customer\n\t[2]Existing Customer\n\t[3]Returning Rental\n\t[4]Cancel Billing\n:");  
                         opt = opt_check();
						 FILE *customs ;
                         switch(opt){
                               case 1: process(contacts,&cust_pos);
							   break;
							   
							   case 2: process(contacts,&cust_pos);
                               break;
                              
                               case 3: puts("\nEnter Customer First name");
                                       fflush(stdin);
                                       gets(name);
                                       puts("\nEnter Customer Last name");
                                       fflush(stdin);
                                       gets(clname);
                                       strcat(name,clname);
                                       customs = fopen(name,"r");
                                       if(customs == NULL)
                                            puts("Customer Name not found/Error opening file\n");
                                  
                                       else{
                                            int rday = 0, rmth = 0, ryear = 0;
                                            customs = fopen(name,"r");
                                            contptr = &singular;
                                            fread(contptr,sizeof(DATA),1,customs);
                                            printf("\n\n%s %s \nBilling Address: %s %s\nTelephone #:%d\nCategory Vehicle Rented: %d"
                                                        "\nPayment Type: %d\tCard Number: %d\nRental Date: %d/%d/%d\t Till: %d/%d/%d\n"
                                                        "\nTotal Days Rent: %d \nPayment Made: $%.2f\tOutstanding Balance: $%.2f \nInsurance?: %c $%.2f \nNavigation System?: %c $%.2f"
                                                        "\nPreviously Registered: %c",contptr->pers.fname,contptr->pers.lname,contptr->address1,contptr->address2,contptr->tel_num,
                                                        contptr->vcategory,contptr->card_type,contptr->card_num,contptr->date[0].day,contptr->date[0].month,
                                                        contptr->date[0].year,contptr->date[1].day,contptr->date[1].month,contptr->date[1].year,contptr->days,contptr->payment,contptr->fee,
                                                        contptr->insurance,contptr->insfee,contptr->nav_sys,contptr->navfee,contptr->STAT.pre_reg); //end of formatted print statement
                                            puts("\nEnter the Current date (using numbers) ");
                                            printf("\n\tDay: ");
                                            scanf("%d",&rday);
                                            printf("\n\tMonth: ");
                                            scanf("%d",&rmth);
                                            printf("\n\tYear: ");
                                            scanf("%d",&ryear);
                                            fflush(stdin);
                                            if(ryear > contptr->date[1].year || rmth > contptr->date[1].month || rday > contptr->date[1].day){
                                                      contptr->date[1].day = rday;
                                                      contptr->date[1].month = rmth;
                                                      contptr->date[1].year = ryear;
                                            }//if vehicle return is late
                                            
                                            if(rday < contptr->date[1].day)//if the customer returns the vehicle early
                                                    contptr->days -= (contptr->date[1].day - rday );
                                            
                                           
                                            puts("\n\nNumber of km driven:\n");
                                            scanf("%f",&singular.KM.kilo);
                                            fclose(customs);
                                            add_km(contptr);
                                       }
                               break;
                               
                               case 4: num_client--;
                               break;
                         }
                 break;
                 
                 case 2: header();
                         FILE *FULL,*pptr;//pointer declaration for the page variable
                         puts("\n[1]Registered Customer History\n[2]View Particular Customer History\n"
                              "[3]View Today's Rentals\n[4]Full Rental History\n\nChoose Your Option");
                         do{
                             opt = opt_check();
                             pptr = fopen("Customer List.txt","r");
                             switch(opt){
                                    case 1: puts("\n\t\tCustomer History\nNames:");
                                           if(pptr == NULL)
                                                puts("\nNo Customer List Available\n");
                                           else{
                                                while(!feof(pptr)){// loop to display file contents
                                                      fscanf(pptr,"%s %s",persptr->fname,persptr->lname);
                                                      printf("%s %s\n",person.fname,person.lname);
                                                }
                                           } 
                                    break;
                                            
                                    case 2: rewind(pptr);
                                            puts("\nEnter Customer First name");
                                            fflush(stdin);
                                            gets(name);
                                            puts("\nEnter Customer Last name");
                                            fflush(stdin);
                                            gets(clname);
                                            strcat(name,clname);
                                            strcpy(cname,name);
                                            strcat(cname," History");
                                            if((pptr = fopen(name,"r")) == NULL && (pptr = fopen(cname,"r")) == NULL)
                                                   puts("Customer Name not found/Error opening file\n");

                                            else if((pptr = fopen(name,"r")) != NULL && (pptr = fopen(cname,"r")) != NULL){
                                                    printf("\n\t\tFull Customer History\n\t");
                                                    setcolor(756);
                                                    pptr = fopen(cname,"r");//open file in read only mode
                                                    while(!feof(pptr)){
                                                        fread(contptr,sizeof(DATA),1,pptr);//reads data from file into structure ponterd to by *contptr
                                                        printf("\n\n%s %s \nBilling Address: %s %s\nTelephone #:%d\nCategory Vehicle Rented: %d"
                                                               "\nPayment Type: %d\tCard Number: %d\nRental Date: %d/%d/%d\t Till: %d/%d/%d\n"
                                                               "\nTotal Days Rent: %d \nPayment Made: $%.2f\tOutstanding Balance: $%.2f \nInsurance?: %c $%.2f \nNavigation System?: %c $%.2f"
                                                               "\nPreviously Registered: %c",contptr->pers.fname,contptr->pers.lname,contptr->address1,contptr->address2,contptr->tel_num,
                                                               contptr->vcategory,contptr->card_type,contptr->card_num,contptr->date[0].day,contptr->date[0].month,
                                                               contptr->date[0].year,contptr->date[1].day,contptr->date[1].month,contptr->date[1].year,contptr->days,contptr->payment,contptr->fee,
                                                               contptr->insurance,contptr->insfee,contptr->nav_sys,contptr->navfee,contptr->STAT.pre_reg); //end of formatted print statement
                                                    }
                                                    setcolor(15);//reset to white text
                                                    rewind(pptr);
                                                    pptr = fopen(name,"r");//open file in read only mode
                                                    printf("\n\n\t\tCURRENT CHARGES \n\n");
                                                    fread(contptr,sizeof(DATA),1,pptr);//reads data from file into structure ponterd to by *contptr
                                                    printf("\n\n%s %s \nBilling Address: %s %s\nTelephone #:%d\nCategory Vehicle Rented: %d"
                                                           "\nPayment Type: %d\tCard Number: %d\nRental Date: %d/%d/%d\t Till: %d/%d/%d\n"
                                                           "\nTotal Days Rent: %d \nPayment Made: $%.2f\tOutstanding Balance: $%.2f \nInsurance?: %c $%.2f \nNavigation System?: %c $%.2f"
                                                           "\nPreviously Registered: %c",contptr->pers.fname,contptr->pers.lname,contptr->address1,contptr->address2,contptr->tel_num,
                                                           contptr->vcategory,contptr->card_type,contptr->card_num,contptr->date[0].day,contptr->date[0].month,
                                                           contptr->date[0].year,contptr->date[1].day,contptr->date[1].month,contptr->date[1].year,contptr->days,contptr->payment,contptr->fee,
                                                           contptr->insurance,contptr->insfee,contptr->nav_sys,contptr->navfee,contptr->STAT.pre_reg); //end of formatted print statement


                                             }else if((pptr = fopen(cname,"r")) == NULL && (pptr = fopen(name,"r")) != NULL){
                                                   rewind(pptr);
                                                   pptr = fopen(name,"r");//open file in read only mode
                                                   printf("\n\n\t\tCurrent Charges: \n\n");
                                                   fread(contptr,sizeof(DATA),1,pptr);//reads data from file into structure ponterd to by *contptr
                                                   printf("\n\n%s %s \nBilling Address: %s %s\nTelephone #:%d\nCategory Vehicle Rented: %d"
                                                          "\nPayment Type: %d\tCard Number: %d\nRental Date: %d/%d/%d\t Till: %d/%d/%d\n"
                                                          "\nTotal Days Rent: %d \nPayment Made: $%.2f\tOutstanding Balance: $%.2f \nInsurance?: %c $%.2f \nNavigation System?: %c $%.2f"
                                                          "\nPreviously Registered: %c",contptr->pers.fname,contptr->pers.lname,contptr->address1,contptr->address2,contptr->tel_num,
                                                          contptr->vcategory,contptr->card_type,contptr->card_num,contptr->date[0].day,contptr->date[0].month,
                                                          contptr->date[0].year,contptr->date[1].day,contptr->date[1].month,contptr->date[1].year,contptr->days,contptr->payment,contptr->fee,
                                                          contptr->insurance,contptr->insfee,contptr->nav_sys,contptr->navfee,contptr->STAT.pre_reg); //end of formatted print statement

                                              }else{
                                                   while(!feof(pptr)){
                                                        fread(contptr,sizeof(DATA),1,pptr);//reads data from file into structure ponterd to by *contptr
                                                        printf("\n\n%s %s \nBilling Address: %s %s\nTelephone #:%d\nCategory Vehicle Rented: %d"
                                                               "\nPayment Type: %d\tCard Number: %d\nRental Date: %d/%d/%d\t Till: %d/%d/%d\n"
                                                               "\nTotal Days Rent: %d \nPayment Made: $%.2f\tOutstanding Balance: $%.2f \nInsurance?: %c $%.2f \nNavigation System?: %c $%.2f"
                                                               "\nPreviously Registered: %c",contptr->pers.fname,contptr->pers.lname,contptr->address1,contptr->address2,contptr->tel_num,
                                                               contptr->vcategory,contptr->card_type,contptr->card_num,contptr->date[0].day,contptr->date[0].month,
                                                               contptr->date[0].year,contptr->date[1].day,contptr->date[1].month,contptr->date[1].year,contptr->days,contptr->payment,contptr->fee,
                                                               contptr->insurance,contptr->insfee,contptr->nav_sys,contptr->navfee,contptr->STAT.pre_reg); //end of formatted print statement
                                                   }
                                             }
                                   break;
                                            
                                   case 3: puts("\n\t\tToday's Records:\n");
                                           printf("\nNumber of customers served: %d",num_client);
										   int i;
                                           for(i = 1;i<=5;i++)
                                                 printf("\nNumber of Category %d vehicles rented: %d",i,rec.rent[i-1]); 
                                   break;
                                            
                                   case 4: FULL = fopen("new_customers","r");
                                           if(FULL == NULL)
                                                  puts("No Customer DataBase Found/Error opening file\n");
                                           else{
                                                while(!feof(FULL)){
                                                      fread(contptr,sizeof(singular),1,FULL);//reads data from file into structure ponterd to by *contptr
                                                      printf("\n\n%s %s \nBilling Address: %s %s\nTelephone #:%d\nCategory Vehicle Rented: %d"
                                                             "\nPayment Type: %d\tCard Number: %d\nRental Date: %d/%d/%d\t Till: %d/%d/%d\n"
                                                             "\nTotal Days Rent: %d \nPayment Made: $%.2f\tOutstanding Balance: $%.2f \nInsurance?: %c $%.2f \nNavigation System?: %c $%.2f"
                                                             "\nPreviously Registered: %c",contptr->pers.fname,contptr->pers.lname,contptr->address1,contptr->address2,contptr->tel_num,
                                                             contptr->vcategory,contptr->card_type,contptr->card_num,contptr->date[0].day,contptr->date[0].month,
                                                             contptr->date[0].year,contptr->date[1].day,contptr->date[1].month,contptr->date[1].year,contptr->days,contptr->payment,contptr->fee,
                                                             contptr->insurance,contptr->insfee,contptr->nav_sys,contptr->navfee,contptr->STAT.pre_reg); //end of formatted print statement
                                                      }
                                                    }
                                     break;
                                            
                                     default: puts("\nPlease select 1, 2, 3 or 4 for this menu");
                                 }
                          }while(opt != 1 && opt != 2 && opt !=3 &&opt!=4 );
                          fclose(FULL);
                          fclose(pptr);
                          printf("\n");
                   break;
                         
                   case 3: num = managerlog();
                           if(num == 0)
                                 puts("\nYou do not have manager privilages");
                              
                           else{
                                enum maxim max = frwheel;
                                header();
                                printf("*************INVENTORY RECOREDS*********************\n");
                                printf("FOUR WHEEL DRIVES:%d\n\nWAGGONS:%d\n\n",starter[0],starter[1]);
                                printf("SUV'S :%d\n\nLUXURY:%d\n\nMOTOR CYCLE:%d\n\n",starter[2],starter[3],starter[4]);
                                printf("\n**********************************\n");
                                printf("\nDO YOU WANT TO ADD A VEHICLE?\nYES [Y] OR NO [N]");
                                approval();
                                if(cHopt =='Y'){
                                     category();
                                     puts("\nUse Numerical input for the category vehicle");        
		                             opt = opt_check();
		                             if(starter[opt-1] < (max - opt))
		                                  starter[opt-1]++;
                                     else
                                          puts("\nVehicle Category is full, No new additions allowed");
                                         
                                     cat_view();                                     
                                                                                  
                               }printf("\nDO YOU WANT TO REMOVE A VEHICLE?\nYES [Y] OR NO [N]");
                               approval();
                               if(cHopt =='Y'){
                                    puts("\nUse Numerical input for the category vehicle");        
                                    opt = opt_check();
		                            if(starter[opt-1] < (max - opt) && starter[opt-1] > 0)
		                                  starter[opt-1]--;
                                    else
                                          puts("\nVehicle Category empty no subtractions allowed");
                                      
                                    cat_view();
                              }
                          }
                          break;
                          case 4: header();
                         puts("\n\t\t\t||Security Option||\n");
                         printf("\n\tSet/Reset Password---->[1]\n\n\tBack to Menu------->[2]\n:");
                         while(opt != 1 && opt != 2){
                              opt = opt_check();
                              FILE *setreset;//declaration file pointer
                              switch(opt){
                                     case 1:  CLEAR//macro to clear program screen
                                              if((setreset = fopen("Syspass","r"))==NULL)//check if file exists
                                                     pset("Syspass",7);
                               
                                              else{
                                                     setreset = fopen("Syspass","r");
                                                     puts("\nVerify User Detail...Enter SYSTEM password\n");
                                                     for(check = checker; check <checker + 7; check++){
                                                           fflush(stdin);
                                                           *check = getch();
                                                           printf("*");
                                                     }
                                                     puts("\n");
                                                     *check = '\0';//to prevent the side effect from getch
                                                     fgets(loader,8,setreset);
                                                     if(strcmp(loader,checker)!=0)
                                                           puts("\nCannot Access Password Change Option");
                                              
                                                     else
                                                           pset("Syspass",7);//function call
                                              }//end else if
                                      break;
                                      
                                      case 2: break;
                                      
                                      default:printf("\nUse [1] or [2] for this menu please ");
                              }//end case
                              fclose(setreset);
                         }//end do while
                         
                 break;
                 
                 case 5: printf("\n\t\t\t||Locking Program||\n\n");
                         header();
                         login("Syspass",7);

                 break;
                 
                 case 6:break;
          }          
     }while(opt!=6);
}

void cust_bill(DATA *billing,int numday){
     header();
     setcolor(12);//red text
     printf("\n\n\t||~ SALES INVOICE (Preliminary) \tVehicle Catgory: %d~||\n",opt);
     printf("\n\tCustomer: %s %s\n\n",billing->pers.fname,billing->pers.lname);
     printf("\n\tRental Period %d/%d/%d to %d/%d/%d.....%d Days\n",billing->date[0].day,\
                                              billing->date[0].month,billing->date[0].year,\
                                              billing->date[1].day,billing->date[1].month,\
                                              billing->date[1].year,numday);
}
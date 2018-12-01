#include <stdio.h>  
#include <stdlib.h>

#define MOVE    0
#define ADD     1
#define SUB     2
#define BRA     3
#define CMP     4
#define BEQ     5
#define BNE     6
#define EXG     7   // exchanges the contents of two registers
#define MUL		8
#define STOP   15

int power(int base, int pow);
int bin_to_dec(int bin);
int read_from_file(char* filename , unsigned short int* array , int size);

int main(){
    
   unsigned short int PC =0;             /* program counter */ 
   //unsigned short int D0 =5;             /* data register */ 
   //unsigned short int A0 =0;             /* address register */ 
   //unsigned short int D1 = 0;
   //unsigned short int A1 = 0;
   unsigned short int CCR =0;            /* condition code register */ 
   unsigned short int MAR;              /*memory address register*/ 
   unsigned short int MBR;               /*memory buffer register*/ 
   unsigned short int IR;                /*instruction register*/ 
   unsigned short int operand;           /*the 8-bit operand from the IR*/ 
   unsigned short int source;            /*source operand */
   unsigned short int destination=0;        /* the destination value*/
   unsigned short int opcode;             /* the 4-bit opcode from the IR */
   unsigned short int amode;              /* the 2-bit addressing mode */
   unsigned short int direction;          /* the 1-bit data direction flag */
   unsigned short int memory[256] = {0};        /* the memory */
   unsigned short int run = 1;            /* execute program while run is 1 */
   unsigned short int select = 0;		  /*making use of the unused bit*/
   unsigned short int D[2] ={0};		  /* Data registers */
   unsigned short int A[2] ={0};
   unsigned short int i = 0;
   unsigned short int ub = 15;
   printf("D0 = %d\n",D[1]);
    
    
  //for (i=0 ; i<256 ; i++) memory[i] = 0;

   /*  7     6   5   4   3   2   1   0
   * Bit 1 and 0 2-bit address mode
   *                   00 address mode = absolute
   *                   01 address mode = literal
   *                   10 address mode = indexed
   *                   11 address mode = relative
   *  Bit 2   1-bit direction ( source / operand )
   *  Bit 3   not used
   *  Bit 7   4-bit instruction code
   */

	memory[50] = 0x15;
	memory[51] = 0x0f;
	
	read_from_file("program3.txt" , memory , 256);	
	memory[9]=101;
	
	memory[100] = 11;
	memory[101] =7;
	
	A[0] = 10;
    memory[10] = 77;
  
   /* main loop */ 
   while (run){
       //printf("******i/p*****\n");
       //printf("opcode (IR) >> ");
       //scanf("%i" , &memory[count++]);
       //if(memory[PC] == 240) break;
       //printf("operand >> " );
       //scanf("%i" , &memory[count++]);
       
       MAR = PC;                           /* PC to MAR */
       printf("MAR = PC %d\n",PC);
       PC = PC +1;                         /* increment PC */
       MBR = memory[MAR];                  /* get next instruction */
       printf("MBR = PC %d\n",PC);
       IR = MBR;                           /* copy MBR to IR */
       opcode = IR;                       /* store the opcode bits */
       
       MAR = PC;                           /* PC to MAR */
       PC = PC +1;                         /* increment PC */
       MBR = memory[MAR];                  /* get the operand */
       IR = MBR;                           /* copy MBR to IR */
       operand = IR;                       /* store the operand bits */
       amode = opcode & 0x03;              /* extract the address mode bits */
       printf("amode >> %d\n", amode);
       direction = (opcode & 0x04)>>2;     /* get data direction  0 = register to memory
        *                                                         1 = memory to register  */
        printf("direction >> %d \n", direction);
        ub = (opcode & 0x08)>>3;
        printf("unused bit = %d\n",ub);
        opcode = opcode >> 4;              /* get the 4-bit instruction code */
        printf("instruction >> %d \n" , opcode);
        /* use the address mode to get the source operand */
        
        switch (amode){
        case 0 : { source = memory[operand]; printf("operand in amode = %d  , %d\n",operand , source);        break; } /* absolute */
        case 1 : { source = operand;       printf("literal\n");          break; } /* literal (immediate) */
        case 2 : { if(select ==1){source = memory[A[ub] + operand];}
					else  source = memory[A[0] + operand];
					printf("source of indexed = %d\n",source);
					break; } /* indexed (indirect)*/
        case 3 : { source = memory[PC + operand];  printf("relative\n");    break; } /* pc relative */
        default: source = operand;
        }
    
        
       
       /*** now execute the instruction ***/
        switch (opcode){
            case MOVE : {  if(select != 0) ub = 0;
				
							if (direction == 0) destination = D[ub];
                            else              D[ub] = source;
                            if (D[ub] == 0) CCR = 1; else CCR =0;  /* update CCR */
                          break;
                        }
            case ADD : {  if(select != 0) ub = 0;
				
				 if (direction == 0)
                            { destination = D[ub] + source;
                              if (destination == 0) CCR = 1; else CCR = 0; 
                            }
                        else
                            { D[ub] = D[ub] + source;
                              if (D[ub] == 0) CCR = 1; CCR = 0;
                            }
                        break;
                        }
            case SUB : { if(select != 0) ub = 0;
				if (direction == 0)
                            { destination = D[ub] - source;
                              if (destination == 0) CCR = 1; else CCR = 0; 
                            }
                        else
                            { D[ub] = D[ub] - source;
                              if (D[ub] == 0) CCR = 1; CCR = 0;
                            }
                        break;
                        }
            case BRA : { if(amode == 0) PC = operand;
						 if(amode == 1) PC = PC + operand; break;
				}
            case CMP : { if(select != 0) ub = 0;
				MBR = D[ub] - source;
                         if (MBR == 0) CCR =1;
                         else CCR = 0; break;
                        }
            case BEQ:  { if (CCR ==1)
                        { if(amode ==0) PC = operand;
                         if(amode ==1) PC = PC + operand;
                        
                        }break;
                        }
            case BNE:  { if (CCR !=1)
                        { if(amode ==0) PC = operand;
                         if(amode ==1) PC = PC + operand;
                        
                        }break;
                        }
            case EXG: {MBR = D[ub]; D[ub]=A[ub]; A[ub]=MBR; break;}
           
            case MUL: { if((amode == 0) || (amode == 1)) {
				
				if(select != 0) ub = 0;
				D[ub] = D[ub] * source;
				printf("D0 in mul = %d\n" ,D[ub]);
				printf("source in mul = %d\n" ,source);
				}
				 break;
				}
            case STOP: {run = 0; break;}
                
            }// end of switch opcode

       /* save result in memory if register to memory */
       if( direction == 0){
           switch (amode){
               case 0 : { memory[operand] = destination; break;} /* absolute */
               case 1 : {}												/* literal */
               case 2 : { if(select != 1) ub = 0;
				   if(A[ub] + operand < 256) memory[ A[ub] + operand] = destination; break;}   /* indexed */
               case 3 : {if(PC + operand < 256) memory[PC + operand] = destination; break;}	/* PC relative */
              
               
               }
           
           
           }
           
       printf("****o/p*****\n");    
	   printf("CCR :%d\n" , CCR);
       printf("D0 :%d\n" , D[0]);
       printf("D1 :%d\n" , D[1]);
       printf("A0 :%d\n" , A[0]);
       printf("A1 :%d\n" , A[1]);
       printf("destination :%d\n" , destination);
       printf("source :%d\n" , source);
       printf("PC :%d\n" , PC);
       			   
		printf("***************************\n");	   
			 
       
       } // end of while
       
       
       printf("END");
       
      
       
   return 0;
    }

int power(int base, int pow)
{
    if (pow != 0)
        return (base*power(base, pow-1));
    else
        return 1;
}

int bin_to_dec(int bin){
	int count = 0, sum =0 , b=0;
	
	while(bin!=0){
	b = bin%10;
	bin = bin/10;
	sum = sum + b*power(2 , count++);
	
	}return sum;}
	
	

int read_from_file(char* filename , unsigned short int* array , int size){
	
	
	
	FILE *fp;
    fp = fopen(filename, "r");

    int i;

    if (fp == NULL){
        printf("File not found\n");
        exit (0);
    }

    for (i = 0; i < size; i++){
        fscanf(fp, "%i\n", &array[i] );
    }


    fclose(fp);
	return 1;
	
	
	}


/**
 * 
 * 7-6-5-4 		3 			2		 1-0 					7-6-5-4-3-2-1-0 		1
 * (opcode)	 not used	Direction	 Addressing modes	    	operand
 * 
 * 0001 x010 5
 **/

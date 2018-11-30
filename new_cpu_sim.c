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
#define STOP   15

int power(int base, int pow);
int bin_to_dec(int bin);
int read_from_file(char* filename , unsigned short int* array , int size);

int main(){
    
   unsigned short int PC =0;             /* program counter */ 
   unsigned short int D0 =5;             /* data register */ 
   unsigned short int A0 =0;             /* address register */ 
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
   unsigned short int memory[256];        /* the memory */
   unsigned short int run = 1;            /* execute program while run is 1 */
  
  

  
  int i = 0;
  for (i=0 ; i<256 ; i++) memory[i] = 0;

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
	read_from_file("program.txt" , memory , 256);	

	int count = 0;
   /* main loop */ 
   while (run && MBR<256 && operand <256){
       //printf("******i/p*****\n");
       //printf("opcode (IR) >> ");
       //scanf("%i" , &memory[count++]);
       if(memory[PC] == 240) break;
       //printf("operand >> " );
       //scanf("%i" , &memory[count++]);
       
       MAR = PC;                           /* PC to MAR */
       PC = PC +1;                         /* increment PC */
       MBR = memory[MAR];                  /* get next instruction */
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
        opcode = opcode >> 4;              /* get the 4-bit instruction code */
        printf("instruction >> %d \n" , opcode);
        /* use the address mode to get the source operand */
        
        switch (amode){
        case 0 : { if(operand < 256) source = memory[operand];         break; } /* absolute */
        case 1 : { source = operand;                 break; } /* literal */
        case 2 : { if((operand + A0) < 256) source = memory[A0 + operand];    break; } /* indexed */
        case 3 : { if((operand+PC) < 256) source = memory[PC + operand];    break; } /* pc relative */
        default: source = operand;
        }
    
        
       
       /*** now execute the instruction ***/
        switch (opcode){
            case MOVE : { if (direction == 0) destination = D0;
                            else              D0 = source;
                            if (D0 == 0) CCR = 1; else CCR =0;  /* update CCR */
                          break;
                        }
            case ADD : { if (direction == 0)
                            { destination = D0 + source;
                              if (destination == 0) CCR = 1; else CCR = 0; 
                            }
                        else
                            { D0 = D0 + source;
                              if (D0 == 0) CCR = 1; CCR = 0;
                            }
                        break;
                        }
            case SUB : { if (direction == 0)
                            { destination = D0 - source;
                              if (destination == 0) CCR = 1; else CCR = 0; 
                            }
                        else
                            { D0 = D0 - source;
                              if (D0 == 0) CCR = 1; CCR = 0;
                            }
                        break;
                        }
            case BRA : { if(amode == 0) PC = operand;
						 if(amode == 1) PC = PC + operand; break;
				}
            case CMP : { MBR = D0 - source;
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
            case EXG: {MBR = D0; D0=A0; A0=MBR; break;}
            
            case STOP: {run = 0; break;}
                
            }// end of switch opcode

       /* save result in memory if register to memory */
       if( direction == 0){
           switch (amode){
               case 0 : { if(operand < 256) memory[operand] = destination; break;} /* absolute */
               case 1 : {}												/* literal */
               case 2 : {if(A0 + operand < 256) memory[ A0 + operand] = destination; break;}   /* indexed */
               case 3 : {if(PC + operand < 256) memory[PC + operand] = destination; break;}	/* PC relative */
              
               
               }
           
           
           }
           
       printf("****o/p*****\n");    
	   printf("CCR :%d\n" , CCR);
       printf("D0 :%d\n" , D0);
       printf("A0 :%d\n" , A0);
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


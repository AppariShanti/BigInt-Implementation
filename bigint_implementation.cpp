#include<stdio.h>
#include<math.h>
#include<string.h>
#define	MAX 308      //The range of bigint will be from -2^1023 to 2^1023-1 which will accommodate approximately 308 digits excluding the sign

//VALUES for signbit
#define POS 1
#define NEG -1

//structure to store the number,sign and the number of digits.
typedef struct {
        int num[MAX];         //array for storing individual digits
		int sign;		      //sign of the number
        int num_digit;		  //number of digits in the number
}bigint;

void add(bigint*,bigint*,bigint*);
void sub(bigint*,bigint*,bigint*);

//printing the bigint number such that the trailing zeroes are not printed.
void print_bigint(bigint *n)
{
	int i=0;
	if (n->sign == NEG || n->sign == -1) //printing -ve sign for negative numbers
		printf("-");
	while(n->num[i]==0 && i<MAX)                 //excluding trailling zeroes
	{
		i++;
	}
	if(i==MAX)                       //if the number itself is zero..then print only 1 zero.
	{
		printf("0");
	}
	else
	{
		for(;i<MAX;i++)
		{
			printf("%d",n->num[i]);
		}
	}
	printf("\n");
}

void convert(char *s,bigint *n)            //converts the string to bigint
{
	int i,len;				
	n->num_digit=strlen(s);               //starts putting the integers in appropriate positions(considering trailing zeroes)
	len=MAX-n->num_digit;
	if(s[0]==45)                         //if the first character is a -ve sign then the actual number starts from 2nd character
    {
		i=1;
    	n->sign=NEG;                     //setting sign of the number to negative
    	n->num_digit=strlen(s)-1;
    	while(s[i]!='\0' && s[i]>=48 && s[i]<=57) //to ensure the string has valid characters i.e. integers
		{
			len++;
			n->num[len]=s[i]-'0';        //converting to integer
			i++;
			
		}
	}
	else
	{
		n->sign=POS;
		i=0;
		while(s[i]!='\0' && s[i]>=48 && s[i]<=57) //to ensure the string has valid characters i.e. integers
		{
			n->num[len]=s[i]-'0';
			i++;
			len++;
		}
	}
}

int max(int a,int b)
{
	return (a>b?a:b);
}

int compare(bigint *a,bigint *b,bigint *c)     //comparison of bigint numbers to find the greater..it is necessary for Add and Sub operations  1->a<b and 0->a>b
{
	int found=1,i;
	int len=max(a->num_digit,b->num_digit);
	if(a->num_digit==b->num_digit)            //when both are of same length..only checking individual digits is enough
	{
		for(i=MAX-len-1;i<MAX && found!=0;i++)
		{
			if(a->num[i]<b->num[i])
			{
				found=0;
				break;                  
			}
			else if(a->num[i]>b->num[i])
			{
				found=1;
				break;
			}
		}
	}
	else if(a->num_digit>b->num_digit)   //if lengths are not same then the one with larger length is the larger number
	{
		found=1;
	}
		
	else
	{
		found=0;
	}
		
	return found;
}

void initialize_bigint(bigint *n) //initialization of bigint so that it doesn't have garbage values
{
	for(int i=0;i<MAX;i++)
	{
		n->num[i]=0;
	}
	n->sign=POS;  //by default they are positive
	n->num_digit=0;
}

//******operations on the bigint number******

//Addition
void add(bigint *a,bigint *b,bigint *c)
{
	int i = MAX-1;
	int carry,count = 0;
	if(a->sign == b->sign)                //if both a and b are either positive or negative the sign of the result is same
	{
		c->sign = a->sign;
		c->num_digit = max(a->num_digit,b->num_digit)+1;   //addition always produces a result which has either number of digits same as max number of digits or 1 more than it
		carry = 0;
		while(count != c->num_digit)
		{
			int anum = (a->num[i]);
			int bnum = (b->num[i]);
			c->num[i] = (anum+bnum+carry)%10;              //adding digit by digit including carry
			carry = (carry+(anum+bnum)) / 10;              //this gives carry for next digits
			i--;
			count++;
		}
		
	}
	else
	{
		if(a->sign==NEG)                                 //a being -ve and b +ve is same as b-a  so we use subtraction operation
		{
			a->sign=POS;                                //changing sign to subtract
			sub(b,a,c);
			a->sign=NEG;                                //restoring sign
			
		}
		else if(b->sign==NEG)                           //a being +ve and b -ve is same as a-b  so we use subtraction operation
		{
			b->sign=POS;                                //changing sign to subtract
			sub(a,b,c);
			b->sign=NEG;                                //restoring sign
		}
	}
}

//Subtraction
void sub(bigint *a,bigint *b,bigint *c)
{
	int borrow,count=0;
	int temp;
	int j=MAX;
	if(a->sign==POS && b->sign==POS)
	{
		c->num_digit=max(a->num_digit,b->num_digit);     //when subracting the maximum number of digits the answer can have is just the max of digits in both a and b
		if(!compare(a,b,c))                              //if a<b comparison
		{
			sub(b,a,c);                                  //same as b-a
			c->sign=NEG;
		}
		else
		{
			borrow=0;
			while(count!=c->num_digit+1)
			{
				temp=a->num[j]-borrow-b->num[j];         //subtractiion considering borrow
				if(a->num[j]>0)
				{
					borrow=0;
				}
				if(temp<0)
				{
					temp=temp+10;                        //taking borrow (adding 10)
					borrow=1;                            //the previous digit is decremented..this is adjusted by setting borrow ..it is then considered when subtracting the next digit
				}
				else
				{
					borrow=0;
				}
				c->num[j]=temp;
				j--;
				count++;
			}
			c->sign=POS;                                //when a>b sign of the result is always +ve
		}
	}	
	
	else if(a->sign==NEG && b->sign==NEG)              //when -a-(-b)=-a+b
	{
		b->sign=POS;
		add(a,b,c);                                   //add -a and +b
		if(!compare(a,b,c))
		{
			c->sign=POS;                             //if a<b sign is +ve
		}
		else
		{
			c->sign=NEG;                            //if a>b sign is -ve
		}
		b->sign=NEG;
	}
	
	if(a->sign==NEG && b->sign==POS)                // -a-(+b)= -a-b that is addition of numbers i.e -(a+b) and giving -ve sign
	{
		b->sign=NEG;
		add(a,b,c);
		b->sign=POS;
		c->sign=NEG;                               //setting -ve sign
	}
	
	if(a->sign==POS && b->sign==NEG)              // +a-(-b) = a+b ,same as addition
	{
		b->sign=POS;
		add(a,b,c);
		b->sign=NEG;
		c->sign=POS;
	}
	
}

//Multiplication

/*Multiplication is done using the same concept that is used while doing it manually..
   The ones digit of b is multiplied by all the digits of a, then tens digit and so on
   The major challenge is the shifting of the numbers to left to carry out addition.
   I used the fact that each row nust be shifted by 0,1,2...positions as the succcessive multiplication with b goes on.
   I have used another bigint "temp" for this and "temp1" is used to store the temporary result of addition*/
   
/* example:  1 2 3 4
		    *  4 5 6
		    --------
		     7 4 0 4
		   6 1 7 0               //shifting of rows necessary
	     4 9 3 6
	    -------------
	     5 6 2 7 0 4
*/
		     
    
void mul(bigint *a,bigint *b,bigint *c)
{
	bigint temp,temp1;
	initialize_bigint(&temp1);
	int m,n=MAX-1;
	int i,j,carry=0;
	
	for(i=0;i<(b->num_digit);i++) 
	{
		initialize_bigint(&temp);   //initialising each time is necessary to get correct multiplication of a with one digit of b
		temp.num_digit=temp.num_digit+i;
		m=MAX-1;
		carry=0;
		for(j=0;j<(a->num_digit);j++)
		{
			temp.num[m-i]=((a->num[m])*(b->num[n])+carry)%10;   //multiplying and adding the carry
			temp.num_digit=temp.num_digit+1;
			carry=((a->num[m])*(b->num[n])+carry)/10;
			m--;		
		}
		if(carry!=0)
		{
			temp.num[m-i]=carry;  //incase the last multiplication generates a carry we write it as it is 
			temp.num_digit++;
		}
		add(&temp,c,&temp1);      //addition of temporary rows in temp1;
		*c=temp1;                 //accumulating in c
		n--;
	}
	if(a->sign==b->sign)         //(+a)*(+b)=+(ab) and (-a)*(-b)=+(ab) sign is +ve
	{
		c->sign=POS;            
	}
	else						//(+a)*(-b)=-(ab) and (-a)*(+b)=-(ab) sign is -ve
	{
		c->sign=NEG;
	}
}

//menu driven program
int main()
{
	int choice;
	printf("\n*******BIGINT CALCULATOR*******\n");
	do
	{
		printf("\n\nPress the number indicated to perform operations\n");
		printf("\n\n1: Addition\n");
		printf("2: Subtraction\n");
		printf("3: Multiplication\n");
		printf("0: Exit\n");
		printf("Enter your choice  ");
		scanf("%d",&choice);
		switch(choice)
		{
			case 1:
				{
					printf("Enter the numbers\n");
					bigint n1,n2,n3;
					char s1[MAX],s2[MAX];
					scanf("%s",s1);        //taking input as a string
    				scanf("%s",s2);
    				initialize_bigint(&n1); //initializing the bigints
					initialize_bigint(&n2);
    				convert(s1,&n1);       //converting strings to bigints
    				convert(s2,&n2);
    				initialize_bigint(&n3);
    				add(&n1,&n2,&n3);
    				printf("\nA= ");
					print_bigint(&n1);
					printf("B= ");
					print_bigint(&n2);
					printf("ANS:\t");
					print_bigint(&n3);
					break;
				}
			case 2:
				{
					printf("Enter the numbers\n");
					bigint n1,n2,n3;
					char s1[MAX],s2[MAX];
					scanf("%s",s1);        //taking input as a string
    				scanf("%s",s2);
    				initialize_bigint(&n1);  //initializing the bigints
					initialize_bigint(&n2);
    				convert(s1,&n1);         //converting strings to bigints
    				convert(s2,&n2);
    				initialize_bigint(&n3);
    				sub(&n1,&n2,&n3);
    				printf("\nA= ");
					print_bigint(&n1);
					printf("B= ");
					print_bigint(&n2);
					printf("ANS:\t");
					print_bigint(&n3);
					break;
				}
			case 3:
				{
					printf("Enter the numbers\n");
					bigint n1,n2,n3;
					char s1[MAX],s2[MAX];
					scanf("%s",s1);					//taking input as a string
    				scanf("%s",s2);
    				initialize_bigint(&n1);  		//initializing the bigints
					initialize_bigint(&n2);
    				convert(s1,&n1);               //converting strings to bigints
    				convert(s2,&n2);
    				initialize_bigint(&n3);
    				mul(&n1,&n2,&n3);
    				printf("\nA= ");
					print_bigint(&n1);
					printf("B= ");
					print_bigint(&n2);
					printf("ANS:\t");
					print_bigint(&n3);
					break;
				}
			case 0:
				{
					printf("EXITING\n");
					break;
				}
			default:
				{
					printf("Incorrect Input..Try Again\n");
					break;
				}
		}
	
	}while(choice!=0);
}

		

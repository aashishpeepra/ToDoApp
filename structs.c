
/*
	FORMAT OF DATA IN FILE : eating@12:44#0B (task@hh:mm#0B or task@hh:mm#1B)
	It is necessary to end the program by pressing 5 otherwise the results will not be reflected in the file
*/
#include<stdio.h>
#include<malloc.h>
#include<string.h>


/* Structure For storing each single Task */

typedef struct
{
	char task[50];	 /*Task Name/Title */
	char time[10];   /*Task Time*/
	char done[10];   /*Task status*/
}Task;

/* Structure Storing Complete File Data */

typedef struct
{
	Task tasks[10]; /*List of all the Tasks*/
	int totalTask;  /* total number of tasks */
}Todo;

/*Function to return Index of searched character in given string from the provided index*/

int indexof(char data[],int start,char check)
{
	/*
		data->String to provide index from 
		start-> Search from this index
		check-> Check for this character
	*/
	int i;
	for(i=start;data[i]!='\n';i++)
	{
		if(data[i]==check)
		return i;
	}
	return 0;
}

/* Slices from string and put result in to string */
void slice(char from[],char to[],int start,int stop)
{
	/*
		from -> string to slice from 
		to -> put the slicing result into this string
		start-> start index of slicing
		stop-> stop index of slicing
	*/
	int i;
	char ch,*str;
	char *ptr=(char *)malloc((stop-start)*sizeof(char));
	str=ptr;
	for(i=start;i<=stop;i++)
	{
		ch=from[i];
		*ptr=ch;
		ptr++;
	}
	strcpy(to,str);
}

/* specially designed for extracting each line into blocks*/
void split_content(char data[],int newcnt,char task[],char time[],char done[])
{
	/*
		data-> to break this data [here its each line like Task1@hh:mm#1B]
		newcnt-> to start working from this index
		task-> to put the extracted task from data string into this string
		time-> to put time from data 
		done-> to put done part , extracted from data
	*/
	int d,attherate;
	attherate=indexof(data,newcnt,'@');
	slice(data,task,0,attherate-1);
	slice(data,time,attherate+1,attherate+5);
	slice(data,done,indexof(data,newcnt,'#')+1,indexof(data,newcnt,'#')+2);
	done[1]='\0';
}

/*Insert provided task into provided file*/
void insert(FILE *ptr1,Task *ptr,Todo *td1)
{
	/*
		ptr1-> File to write data in
		ptr-> to extract info from 
		td1-> Todo structure passed to update also
	*/
	char singleStream[100];
	int i=0,len;
	len=td1->totalTask;
	/*Updating the in-program data or tasks list*/
	strcpy(td1->tasks[len].task,ptr->task);
	strcpy(td1->tasks[len].time,ptr->time);
	strcpy(td1->tasks[len].done,ptr->done);
	td1->totalTask+=1;	
	fprintf(ptr1,"%s@%s#%sB\n",td1->tasks[len].task,td1->tasks[len].time,td1->tasks[len].done);
}

/*print all the tasks on the screen*/
void print_all_tasks(Todo *td)
{
	/*
		td-> have the tasks array
		work : loop through this tasks array and print each task
	*/
	int i;
	for(i=0;i<td->totalTask;i++)
	{
		printf("-----Task%d------\n",i+1);
		printf(" Task : %s\n",td->tasks[i].task);
		printf(" Time : %s\n",td->tasks[i].time);
		printf(" Status : %s\n",strcmp(td->tasks[i].done,"1")?"UnDone":"Done");
		printf("----------------\n");
	}
}

/*Delete particular task from the file */
int delete_task(FILE *ptr1,Todo *td,char taskName[30],char fileName[30])
{
	/*
		td-> Todo list pointer to loop through tasks array
		taskName-> Name of the task to delete
		fileName-> Name of the file to open in write mode
	*/
	int i,cnt=0,flag=0;
	fclose(ptr1);
	FILE *ptr2;
	ptr2=fopen(fileName,"w+");
	for(i=0;i<td->totalTask;i++)
	{
		if(strcmpi(td->tasks[i].task,taskName)!=0)
		{
			fprintf(ptr2,"%s@%s#%sB\n",td->tasks[i].task,td->tasks[i].time,td->tasks[i].done);
		}
		else
		{
			cnt++;
			flag=1;	
		}
	}
	td->totalTask-=cnt;
	fclose(ptr2);
	ptr1=fopen(fileName,"a+");
	return flag;
}

/*Print Specific task that is provided */
void print_specific(Task *ptr)
{
		printf("----------------\n");
		printf("Task : %s\n",ptr->task);
		printf("Time : %s\n",ptr->time);
		printf("Status : %s\n",strcmp(ptr->done,"1")?"UnDone":"Done");
		printf("----------------\n");
}

/*Filter and print Tasks on the basis of done status*/
void filter_status(Todo *td1,int status)
{
	/*
		td1->Todo pointer to loop for refining
		status-> [0,1] to cross reference against the done field of task 
	*/
	int i;
	for(i=0;i<td1->totalTask;i++)
	{
		if((int)td1->tasks[i].done[0]==status+48)
		{
			print_specific(&td1->tasks[i]);
		}
	}
}

/*Acts as a constructor of the File by assigning data to Todo variable */
void initialize_data(Todo *td1,FILE *ptr)
{
	/*
		td1->Todo list to feed data to 
		ptr->File pointer to read data from 
	*/
	char ch,data[100];
	int i,cnt=0;
	while((ch=fgetc(ptr))!=EOF)
	{
		if(ch!='\n')
		{
			data[i++]=ch;
		}
		else
		{
			split_content(data,0,td1->tasks[cnt].task,td1->tasks[cnt].time,td1->tasks[cnt].done);
			cnt++;
			i=0;
		}
	}
	td1->totalTask=cnt;
}

int main()
{
	/*
		All variables with the title in of temp are for temporary refreshing storage
	*/
	Todo td1;
	Task taskTemp;
	FILE *ptr;
	int temp,temp2;
	char tempc[30],nameFile[30];
	printf("Enter the name of the file : ");
	gets(nameFile);
	ptr=fopen(nameFile,"a+");
	initialize_data(&td1,ptr);
	printf("--WELCOME TO DAILY TODO APP--\n\n");
	while(1)
	{
		printf("*****************************\n");
		printf("* Press 1 to View all Tasks *\n* Press 2 to Filter Tasks *\n* Press 3 to Insert Task *\n* Press 4 to delete task *\n* Press 5 to Exit *\n");
		printf("*****************************\n");
		scanf("%d",&temp);
		switch(temp)
		{
			case 1:
				print_all_tasks(&td1);
				break;
			FILTER_BREAKPOINT:
			case 2:
				printf(" Press 1 to check complete Tasks\n Press 0 to check Incompleted Tasks\n");
				scanf("%d",&temp2);
				if(temp2==0)
					filter_status(&td1,0);
				else if(temp2==1)
					filter_status(&td1,1);
				else
					goto FILTER_BREAKPOINT;
				break;
			case 3:
				fflush(stdin);
				printf(" Enter the Task Title : ");
				gets(taskTemp.task);
				printf(" Enter the Time in 24 hours Format : ");
				gets(taskTemp.time);
				printf(" Enter status -> 0 for incomplete, 1 for complete : ");
				gets(taskTemp.done);
				insert(ptr,&taskTemp,&td1);
				printf(" Task Inserted \n");
				break;
			case 4:
				fflush(stdin);
				printf("Enter the Title of task to delete : ");
				gets(tempc);
				if(delete_task(ptr,&td1,tempc,nameFile))
					printf("\nTask Deleted \n");
				else
					printf("\nTask Not Present\n");
				break;
			case 5:
				goto OUTOFLOOP;
			default:
				printf(" Wrong Input ! Try Again :-) \n");	
				
		}
	}
	OUTOFLOOP:
	printf("\n* THANKS FOR USING AP TODO'S *\n");
	fclose(ptr);
	return 0;
}

/*
	NOTE -> Still Time Validation has to be done!
*/

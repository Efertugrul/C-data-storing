#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Info
{
	char id[10];
	char ptype;
	char name[31];


	union
	{
		struct
		{
			char dept[31];
			int hireyear;
			char tenured;
		}prof;
		struct
		{
			char faculty[31];
			int admyear;
		}stud;
	}info;

	struct Info *next;
} Info;
//Allocate memory for new input node and give its field respective values by seperating tokens from input.
Info * create(char *inp)
{
	Info *new = malloc(sizeof(Info));
	char *id = inp+2;
	id = strtok(id, ",");
	char *ptype = inp + 3 +strlen(id);
	ptype = strtok(ptype, ",");
	char *name = inp + 5 + strlen(id);
	//if field not given
	if(*name == ',')
	{
		strcpy(new->name,"");
		name = "";
	}
	else
	{
		name = strtok(name,",");
		strcpy(new->name, name);
	}

	strcpy(new->id, id);
	new->ptype = *ptype;
	if(!strcmp(ptype, "S"))
	{
		char *fac = inp+ 6+ strlen(id)+ strlen(name);
		if(*fac == ',')
		{
			strcpy(new->info.stud.faculty, "");
			fac = "";
		}
		else
		{
			fac = strtok(fac, ",");
			strcpy(new->info.stud.faculty, fac);
		}
		char *admyear = inp + 7 +strlen(id) + strlen(name) + strlen(fac);
		if(strlen(admyear)<2)
		{
			new->info.stud.admyear = '\0';
		}
		else
		{
			admyear = strtok(admyear,"\0");
			//printf("%s",admyear);
			new->info.stud.admyear = atoi(admyear);
		}
	}
	else
	{

		char *dept = inp+6 +strlen(id) + strlen(name);
		if(*dept == ',')
		{
			strcpy(new->info.prof.dept, "");
			dept = "";
		}
		else
		{
			dept = strtok(dept, ",");
			strcpy(new->info.prof.dept, dept);
		}
		char *hire = inp + 7 + strlen(id) + strlen(name) + strlen(dept);
		if(*hire == ',')
		{
			new->info.prof.hireyear = 0;
			hire = "";
		}
		else
		{
			hire = strtok(hire, ",");
			new->info.prof.hireyear = atoi(hire);
		}
		char *ten = inp+8 + strlen(id) + strlen(name) + strlen(dept) + strlen(hire);

		if(*ten != 'Y' && *ten != 'N')
		{
			new->info.prof.tenured = ' ';
		}
		else
		{
			ten = strtok(ten, "\0");
			new->info.prof.tenured = *ten;
		}
	}
	return new;

}
void insert(Info **head, Info *new)
{
	//if list is empty or the one to be changed is in head
	if((*head)->next == NULL || atoi((*head)->id) > atoi(new->id))
	{
		new->next = *head;
		*head = new;
		return;
	}
	Info *p1 = *head;
	//check for possible update case if equal change all fields
	while(p1!= NULL)
	{
		if(atoi(p1->id) == atoi(new->id))
		{
			if(p1->ptype == 'S')
			{
				//printf("aaaaaa");
				strcpy(p1->name, new->name);	
				strcpy(p1->info.stud.faculty, new->info.stud.faculty);
				p1->info.stud.admyear = new->info.stud.admyear;
				return;
			}
			else
			{
				strcpy(p1->name, new->name);
				strcpy(p1->info.prof.dept, new->info.prof.dept);
				p1->info.prof.hireyear = new->info.prof.hireyear;
				p1->info.prof.tenured = new->info.prof.tenured;
				return;
			}
		}
		p1 = p1->next;
	}
	//if we are sorting the nodes except the head we iterate until we find the greater number
	//we need to pointers one to specify new's next and one for previous
	Info *p = *head, *prev = *head;
	while(p->next != NULL && atoi(p->id) < atoi(new->id))
	{
		prev = p;
		p = p->next;
	}
	new->next = prev->next;
	prev->next = new;


}
void delete(Info **head,char *inp)
{
	Info *prev, *p = *head;
	char *id = inp+ 2;
	id = strtok(id,"\0");
	//if it's the head then point the head to next one and free space
	if(p != NULL && atoi(p->id) == atoi(id))
	{
		*head = p->next;
		free(p);
		return;
	}
	//if not head we start iterating until id is found
	while(p != NULL && atoi(p->id) != atoi(id))	
	{
		prev = p;
		p = p->next;
	}
	//if we couldn't find the id it's not in we return
	if(p == NULL)
	{
		return;
	}
	//else we found it, we remove it by linking one before to one after and free space
	prev ->next = p->next;
	free(p);	

}
void print(Info *p)
{
	//we just iterate print each field manually, for integer missing values i had to write a condition 
	while(p->next != NULL)
	{
		if(p->ptype == 'S')
		{
			if(p->info.stud.admyear == 0)
			{
				printf("%s,%c,%s,%s,\n", p->id, p->ptype, p->name, p->info.stud.faculty);
				p = p->next;
			}
			else
			{
				printf("%s,%c,%s,%s,%d\n", p->id, p->ptype, p->name, p->info.stud.faculty, p->info.stud.admyear);
				p = p->next;
			}
		}
		else
		{
			if(p->info.prof.hireyear == 0)
			{
				printf("%s,%c,%s,%s,,%c\n", p->id, p->ptype, p->name, p->info.prof.dept,p->info.prof.tenured);
				p = p->next;
			}
			else
			{
			printf("%s,%c,%s,%s,%d,%c\n", p->id, p->ptype, p->name, p->info.prof.dept, p->info.prof.hireyear,p->info.prof.tenured);
			p = p->next;
			}
		}
	}
}
void free_list(Info *head)
{
	//while ending this function is called to clean up space
	Info *curr;
	while(head != NULL)
	{
		curr = head;
		head = head->next;
		free(curr);
	}
}

int main(int argc, char *argv[]) 
{
	//if wrong num of arguments
	if(argc != 2)
	{
		fprintf(stderr,"Error, please pass the database filename as the argument.\nUsage ./pimapp <dbfile>\n");
		return 1;
	}
	else
	{
		//create head to run our helper functions and allocate space

		Info *head = NULL;
		head = malloc(sizeof(Info));	
		while(1)

		{
			//take inp and clean up next line (fgets)
			char inp[500];
			fgets(inp,500, stdin);
			inp[strcspn(inp, "\n")] = 0;
			if(!strcmp(inp,"END"))
			{
				//try to open
				FILE *fptr;
				fptr = fopen(argv[1], "w");
				if(fptr== NULL)
				{
					fprintf(stderr,"Error, unable to open %s for writing.\n" ,argv[1]);
					free_list(head);
					return 3;
				}
				//same as print we just write to file
				Info *p = head;
				while(p->next != NULL)
				{
					if(p->ptype == 'S')
					{

						if(p->info.stud.admyear == 0)
						{
							fprintf(fptr,"%s,%c,%s,%s,\n", p->id, p->ptype, p->name, p->info.stud.faculty);
							p = p->next;
						}
						else
						{
							fprintf(fptr,"%s,%c,%s,%s,%d\n", p->id, p->ptype, p->name, p->info.stud.faculty, p->info.stud.admyear);
							p = p->next;
						}
					}
					else
					{

						if(p->info.prof.hireyear == 0)
						{
							fprintf(fptr,"%s,%c,%s,%s,,%c\n", p->id, p->ptype, p->name, p->info.prof.dept,p->info.prof.tenured);
							p = p->next;
						}
						else
						{
							fprintf(fptr,"%s,%c,%s,%s,%d,%c\n", p->id, p->ptype, p->name, p->info.prof.dept, p->info.prof.hireyear,p->info.prof.tenured);
							p = p->next;
						}
					}
				}
				//clean list close file
				free_list(head);
				fclose(fptr);
				break;
			}
			else if(!strcmp(inp, "LIST"))
			{
				print(head);
			}
			else
			{
				char *tok = inp;
				tok = strtok(tok, ",");
				if(!strcmp(tok, "I"))
				{
				
			
					insert(&head,create(inp));
				}
				else
				{
					delete(&head,inp);
				}
			
			}
		}
	}
}

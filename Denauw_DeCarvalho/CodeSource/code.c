#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> //Pour sbrk

struct block_header
{
	unsigned int size : 29,
		     zero : 2,
		     alloc : 1;
}


void *fin_workspace_m=sbrk(0);		//variable globale de fin du workspace
extern void *debut_workspace;


void* my_malloc(int size)
{
	if (size % 4 !=0)
	{
		size=size+(4-(size%4));
	}	
	bool notFound=true;
	void *position=debut_workspace_m;
	while(notFound)
	{	if(position==fin_workspace)
		{
			notFound=false;
			return NULL;
		}
		if(position->alloc==0 && (position->size) >= (size+4)) //si on peut allouer et assez grand
		{
			block_header new_header={(position->size)-size-4,0,0}; //on cree le block header contenant le restant
			
			position->size=size+4;	//on donne la taille au block
			position->alloc=1;	//on dit que le block est alloue
			void* final_position=position;	//on stocke la position du bloc libéré voulu
			struct block_header* header= (struct block_header*) position;
			position+=header->size;		//on va a la fin du block que l'on vient d'allouer
			position=new_header;	//on met le block header contenant les info sur ce qu'il reste du block initial

			notFound=false;	//inutile mais pour faire propre
			return final_position; //retourne la position du block voulu
		}
		else
		{
			struct block_header* header= (struct block_header*) position;
			position+=header->size;		
		}
	}

}


void myfree(void *ptr)
{

}


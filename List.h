#ifndef SORTED_LIST_H
#define SORTED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int MAX = 0;

typedef struct ListNode {
    struct ListNode *next;
    struct ListNode *prev;
    int prior;
    char *name;
} ListNode;

typedef struct List {
    ListNode *first;
    ListNode *last;
} List;

// AVEAM NEVOIE SA LE APLEZ DIN ALTE FUNCTII SI TREBUIAU DECLARATE.
void adauga_persoana(List ***ghisee, char *nume, int prioritate, 
		int numar_ghiseu);

void sterge_persoana(List ***ghisee, char *nume, int prioritate, 
		int nr_g);

void inchide_ghisee(List ***ghisee);

void printList(List ***ghisee , int nr );

int calculeaza_numar_total_oameni(List **ghisee);

void deschide_ghisee(List ***ghisee, int N)
{

 int i;

 //CAZUL IN CARE LISTA ESTE VIDA.
 if( (*ghisee) == NULL )
 {
   //ALOCAR VECTOR.
   (*ghisee) = malloc( N * sizeof(List*) );

   if( (*ghisee) == NULL )
   {
      fprintf(stderr, "ERROR!\n" );
      exit(1);
   }

   //ALOCAREA NUMARULUI DE LISTE.
   for( i = 0 ; i < N ; i++ )
   {
      (*ghisee)[i] = malloc(sizeof(List) );
      if ( (*ghisee) == NULL )
      {
	 fprintf(stderr, "ERROR!\n" );
	 exit(1);
      }		
   }

   for (i = 0; i < N; i++)
   {
      (*ghisee)[i]->first = NULL;
      (*ghisee)[i]->last = NULL;
   }
	

   if(N != 1 ) MAX = N;
 }

 //CAZUL IN CARE LISTA NU ESTE VIDA.
 else
 {
   List** saveList = NULL;
   ListNode* ghi;

   deschide_ghisee(&saveList, 1);

   int i = 0;
 

   for(i = 0 ; i < MAX ; i++)
   {   

      ghi = (*ghisee)[i]->first;
      //COPIAZA TOATE PERSOANELE DIN TOATE GHISEELE INTR-UN SINGUR GHISEU.
      //ACESTA IMI SORTEAZA AUTOMAT TOATE PERSOANELE.
      while( ghi != NULL )
      {
	      adauga_persoana(&saveList, ghi->name ,
		                   ghi->prior, 0);

      ghi = ghi->next;	
      }
   }
  
  
   //DEZALOC PRIMELE GHISEE
   inchide_ghisee(ghisee);

   //CREEZ UN NOU SPAIU PENTRU GHISEE.
   List** newGhisee = NULL;

   //O COPIE A LISTEI CU TOTI OAMENII.
   ListNode* copyList = saveList[0]->first;


   deschide_ghisee(&newGhisee, N );

   //COPIEZ TOATE VALORILE DIN LISTA IN GHISEE.
   //DACA LISTA O SA DEVINA VIDA O SA FACA BREAK SI IESE DIN SECVENTA.
   while( copyList != NULL )
   {
      for( i = 0 ; i < N ; i++ )
      {
	
	 if( copyList == NULL ) break;
	 adauga_persoana(&newGhisee, copyList->name ,
			         copyList->prior, i );
         copyList = copyList->next;	
      }
   }

   //FOLOSESC ASTA PENTRU A PUTEA DEZALOCA GHISEUL.
   MAX = 1;
   inchide_ghisee(&saveList);

   //TRANSFER POINTERUL LA ADRESA DORITA.
   (*ghisee) = newGhisee;
   //SALVEZ NUMARUL DE GHISEE.
   MAX = N;
 }
}

void inchide_ghisee(List ***ghisee)
{
   //CONDITII
   if( *ghisee == NULL ) return;
   ListNode* cel;
   ListNode* pass;
   int i;

   //DAU FREE PE FIECARE ELEMENT DIN GHISEE.
   //APOI FREE PE LISTA DIN VECTOR.
   //APOI FREE PE VECTOR.
   for( i = 0; i < MAX; i++)
   {	
   
      cel = (*ghisee)[i]->first ;
      while( cel != NULL )
      {   
         pass = cel;
	       cel = cel->next;
	    free( pass );
      }

   free( (*ghisee)[i] );
   }

free( *ghisee );
}

void adauga_persoana(List ***ghisee, char *nume, int prioritate, 
       		int numar_ghiseu) 
{
  //CONDITII
  if( numar_ghiseu >= MAX ) return;
  if( *ghisee == NULL ) return;
  if( (*ghisee)[numar_ghiseu] == NULL ) return;
 
  ListNode* node; 
  //ALOCARE
  node = malloc(sizeof(ListNode));
  if( node == NULL )
  {
     fprintf(stderr, "ERROR!\n");
     exit(1);
  }

  node->prev = NULL;
  node->next = NULL;
  node->name = nume;
  node->prior = prioritate;

  ListNode* lista = (*ghisee)[numar_ghiseu]->first;

  //ALTE CONDITII.
  if( *ghisee == NULL ) return;

  if( (*ghisee)[numar_ghiseu] == NULL ) return;
	
  if( lista == NULL )
  {
     (*ghisee)[numar_ghiseu]->first = node;
  return;
  }

  //CAND ESTE PRIMUL.
  if( lista->prior < node->prior )
  { 
     node->next = lista;
     lista->prev = node;
     (*ghisee)[numar_ghiseu]->first = node;
  return;
  }
	
  //CAND NU ESTE PRIMUL/
  while( lista != NULL )
  {  

     //CAND TREBUIE ADAUGAT ULTIMUL!
     if(lista->prior > node->prior && lista->next == NULL)
     {
        node->prev = lista;
	node->next = NULL ;
        lista->next = node;
     return;
     }
	
     //CAND TREBUIE ADAUGAT INTRE!
     else if (lista->prior > node->prior && lista->next->prior < node->prior
			                 && lista->next != NULL )
     { 	
        node->prev = lista;
	node->next = lista->next;
	lista->next->prev = node;
	lista->next = node;
     return;
     }

     //CAND PRIORITATILE SUNT EGALE, SORTARE ALFABETICA.
     else if (lista->prior == node->prior)
     {

        int index = strcmp( lista->name , node->name ) ;	
  	//CAND NUMELE SUNT EGALE.
  	if( index == 0 )
	{	
   	   node->next = lista;
	   // CAZ IN CARE ELEMENTUL DINAINTE ESTE VID.
	   if(lista->prev == NULL )
	   {  
	      (*ghisee)[numar_ghiseu]->first = node;
	      lista->prev = node;
	   }
	   else if (lista->prev != NULL )
	   {  
	      node->prev = lista->prev;
	      lista->prev->next = node;
	      lista->prev = node;
	   }	
	
        return;
	}
        //CAZ IN CARE NUMELE DIN NOD ESTE MAI MARE.
	else if( index < 0 )
	{
	   if(lista->next != NULL)
	   //VERIFICA DACA EXISTA PERSOANE DUPA CU UN NUME MAI MIC DECAT NOD.
	   if( strcmp( lista->next->name , node->name ) < 0 
		&& lista->next->prior == node->prior )
	   {
		lista = lista->next;
           }

	   node->next = lista->next;
	   node->prev = lista;
	
	   if( lista->next == NULL )
	   {  	
	      lista->next = node;
	   }
	   else
	   {	
	      lista->next->prev = node;
	      lista->next = node;
	   }
			
       	  return;	
	  }		
	  //CAZ IN CARE NODUL ESTE MAI MIC.	  	
	  else if( index >  0 )
	  {
	     node->next = lista; 
	     node->prev = lista->prev;

	     if( lista->prev != NULL )
	     {    
	        lista->prev->next = node;
	     }
	     else
	     {
	        (*ghisee)[numar_ghiseu]->first = node;
	     }
	   
	  lista->prev = node;				
	  return;
	  }
      }//ENDELSE

  lista = lista->next;	
 
  }//ENDWHILE
		
}

void sterge_persoana(List ***ghisee, char *nume, int prioritate, 
		int nr_g) 
{
   //CONDITII.
   if( nr_g >= MAX ) return;
   if( (*ghisee)[nr_g]->first == NULL ) return;

   ListNode* pers = (*ghisee)[nr_g]->first;
   ListNode* pass;
   ListNode* prev;

   //CAZ IN CARE TREBUIE SA STEARGA PRIMA PERSOANA.
   if(  pers->name == nume && pers->prior == prioritate  )
   {	

      pass = pers;
      if( pers->next == NULL)
      {			
         free((*ghisee)[nr_g]->first);
	 (*ghisee)[nr_g]->first = NULL;	
      return;
      }
      else
      {	
	  (*ghisee)[nr_g]->first = pers->next;
	  pers->next->prev = NULL;
	  free(pers);
      }
   return;
   }

   while( pers != NULL )
   {

      if( pers->name == nume && pers->prior == prioritate  )
      {  
      	 //NU EXISTA PERSOANA URMATOARE.
         if( pers->next == NULL )
	       {
	          pass = pers;
	          prev->next = NULL;
	          free(pass);
	        return;
	        }
	 
	       //EXISTA PERSOANA URMATOARE.
	       if (pers->next != NULL)
	       {

	          pass = pers;
	          prev->next = pers->next;
	          pers->next->prev = prev;

	          free(pass);
	        return;
	        }

      }
   prev = pers; 
   pers = pers->next;
   }

}


void sterge_primii_oameni(List ***ghisee) 
{

   int i = 0;
   ListNode* persoana;

   //PARCURG FIECARE GISEU SI STERG PERSOANA.
   //SUNT CAZURI IN CARE EXSITA SAU NU PERSOANA URMATOARE.
   for (i = 0; i < MAX; i++)
   {	

      if( (*ghisee)[i]->first != NULL )
      {
         persoana = (*ghisee)[i]->first;

	 if( (*ghisee)[i]->first->next == NULL  )
	 {
	   (*ghisee)[i]->first->next = NULL;
           (*ghisee)[i]->first = NULL;
         }
	 else
	 {
	   (*ghisee)[i]->first = (*ghisee)[i]->first->next;	   	
	 }
      free(persoana);
      } 
   }	
}

int calculeaza_numar_oameni_ghiseu(List **ghisee, int numar_ghiseu)
{
   int count = 0;
   
   //CONTITII.
   if( numar_ghiseu > MAX ) return 0;
   if( ghisee[numar_ghiseu]->first == NULL) return 0;

   //COPIE A GHISEULUI, AVEAM IN CAP DECAT NEWLIST ASA CA AM ALES newL.
   //CAUTA PERSOANA 
   ListNode* newL = ghisee[numar_ghiseu]->first;

   //PARCURGE LISTA SI LA FIECARE PARCURGERE COUNT CRESTE + 1
   //LA FINAL VA REPREZENTA TOTI OAMENII DIN GHISEU
   while( newL != NULL )
   {
      newL = newL->next;
      count++;
   }

return count;
}

int calculeaza_numar_total_oameni(List **ghisee)
{

   //CONDITII
   if( ghisee == NULL ) return 0;

   int count = 0;
   int i = 0 ;

   //PARCURG LISTELE SI RETURNEZ NUMARUL TOTAL DE OAMENI/
   //SE FACE SUMA OAMENILOR DE PE TOATE LISTELE.
   for ( i = 0; i < MAX; i++)
   { 
     
     if( ghisee[i]->first != NULL )
     {
        count = count + calculeaza_numar_oameni_ghiseu(ghisee, i);
     }	 
   }
	
return count;
}

int gaseste_pozitie_persoana (List **ghisee, char *nume, int prioritate, 
		int numar_ghiseu) 
{


   int count = 0;
   //CONDITII.
   if( numar_ghiseu > MAX ) return -1;
   if ( ghisee == NULL ) return -1;

   ListNode* node = ghisee[numar_ghiseu]->first;

   //CAT TIMP NU A GASIT PERSOANA CRESTE COUNT.
   //COUNT REPREZINTA POZITIA PERSOANEI.
   while( node != NULL )
   {
      if( node->name == nume && node->prior == prioritate)
      {
	 return count;
      }

   count++;
   node = node->next;
   }

return -1;
}

char* gaseste_persoana_din_fata(List **ghisee, char *nume, int prioritate, 
		int numar_ghiseu) 
{

   ListNode* node = ghisee[numar_ghiseu]->first ;
   ListNode* prev = NULL;

   if( numar_ghiseu > MAX ) return "Nu exista.";
   if( ghisee == NULL ) return "Nu exista.";
   if( node->name == nume && node->prior == prioritate ) return "Nu exista.";

   //CAUTA PERSOANA, DACA O GASESTI AFISEAZA PERSOANA DINAINTE.
   //IN CAZ CA NU EXISTA PERSOANA DINAINTE RETURNEAZA NU EXISTA.
   //DACA IESE DIN WHILE ATUNCI RETURNEAZA NU EXISTA.
   while( node != NULL )
   {
      if( node->name == nume && node->prior == prioritate )
      {
         if( prev == NULL) return "Nu exista.";
      return prev->name;
      }
   
   prev = node;
   node = node->next;
   }

return "Nu exista.";
}

void afiseaza_rand_ghiseu(FILE *f, List **ghisee, int nr_g ) 
{
   //CONDITII	
   if( nr_g >= MAX ) return; 

   ListNode* persoana = ghisee[nr_g]->first;
   if (ghisee == NULL ) return;

   if( persoana == NULL )
   {
      fprintf(f, "Ghiseul %d nu are nicio persoana in asteptare.", nr_g );
   }

   //CAT TIMP GHISEUL NU E GOL AFISEAZA NUMELE SI PRIORITATEA PERSOANEI.
   while( persoana != NULL )
   {
      fprintf(f, "%s-%d;", persoana->name , persoana->prior);
      persoana = persoana->next;
   }

fprintf(f,"\n");
}
#endif

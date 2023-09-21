/* Lanovaya Alexandra, 10-6, 09.12.20 */
#include <stdio.h>
#include <string.h>

#include "ortgl.h"

/* Start of 'PrintTok' function */
void PrintTok( TOK *T )
{
  if (T->Id == TOK_OP)
    printf("<op:%s>", T->OperTable->Name);
  else if (T->Id == TOK_NAME)
  {  
    printf("<var:%s>", T->Name);
  }
  else if (T->Id == TOK_FUNC)
  {  
    printf("<func:%s>", T->OperTable->Name);
  }
  else if (T->Id == TOK_NUM)
    printf("<num:%f>", T->Num);
  else
    printf("Unknown token");
} /* End of 'PrintTok' function */

/* Start of 'Put' function */
void Put( QUEUE *Q, TOK NewTok )
{
  LIST *NewElement = malloc(sizeof(LIST));

  if (NewElement == NULL)
    Error("No memory");

  NewElement->T = NewTok;
  NewElement->Next = NULL;

  if (Q->Head == NULL)
    Q->Tail = Q->Head = NewElement;
  else
  {
    Q->Tail->Next = NewElement;
    Q->Tail = NewElement;
  }
} /* End of 'Put' function */

/* Start of 'Get' function */
bool Get( QUEUE *Q, TOK *OldTok )
{
  LIST *OldElement;

  if (Q->Head == NULL)
    return false;

  if (OldTok != NULL)
    *OldTok = Q->Head->T;
  OldElement = Q->Head;
  Q->Head = Q->Head->Next;
  free(OldElement);
  return true;
} /* End of 'Get' function */

/* Start of 'Push' function */
void Push( STACK *S, TOK NewTok )
{
  LIST *NewElement = malloc(sizeof(LIST));

  if (NewElement == NULL)
    Error("No memory");

  NewElement->T = NewTok;
  NewElement->Next = S->Top;
  S->Top = NewElement;
} /* End of 'Push' function */

/* Start of 'Pop' function */
bool Pop( STACK *S, TOK *OldTok )
{
  LIST *OldElement;

  if (S->Top == NULL)
    return false;
  
  if (OldTok != NULL)
    *OldTok = S->Top->T;
  OldElement = S->Top;
  S->Top = S->Top->Next;
  free(OldElement);
  return true;
} /* End of 'Pop' function */

/* Start of 'DisplayList' function */
static void DisplayList( LIST *L )
{
  if (L == NULL)
    printf("Empty list");
  else
    while (L != NULL)
    {
      PrintTok(&L->T);
      printf((L = L->Next) != NULL ? ", " : "\n");
    }
} /* End of 'DisplayList' function */

/* Start of 'DisplayQueue' function */
void DisplayQueue( QUEUE *Q )
{
  DisplayList(Q->Head);
}

void DisplayStack( STACK *S )
{
  DisplayList(S->Top);
} /* End of 'DisplayQueue' function */

/* Start of 'ClearList' function */
static void ClearList( LIST **L )
{
  LIST *Old;

  while (*L != NULL)
  {
    Old = *L;
    *L = (*L)->Next;
    free(Old);
  }
} /* End of 'ClearList' function */

/* Start of 'ClearQueue' function */
void ClearQueue( QUEUE *Q )
{
  ClearList(&Q->Head);
} /* End of 'ClearQueue' function */

/* Start of 'ClearStack' function */
void ClearStack( STACK *S )
{
  ClearList(&S->Top);
} /* End of 'ClearStack' function */
/* Lanovaya Alexandra, 10-6, 09.12.20 */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <setjmp.h>

#include "ortgl.h"

#define MAX 1000 /* maximum length of expression */

jmp_buf ExprJumpBuf; /* Jump bufer variable */

/* Start of 'Error' function */
VOID Error( CHAR *Str, ... )
{
  va_list ap;

  printf("Error: ");
  va_start(ap, Str);
  vprintf(Str, ap);
  va_end(ap);
  printf("\n");
  longjmp(ExprJumpBuf, 1);
} /* End of 'Error' function */

/* Start of 'GetStr' function */
VOID GetStr( CHAR *str, INT MaxLen )
{
  INT i = 0;
  CHAR Ch;

  while ((Ch = getchar()) != '\n')
    if (str != NULL && i < MaxLen - 1)
      str[i++] = Ch;
  if (str != NULL && i < MaxLen)
    str[i] = 0;
} /* End of 'GetStr' function */

/* Start of 'main' function */
VOID main( VOID ) 
{
  CHAR FileName[MAX_NAME];
  QUEUE Q = {NULL};
  FILE *F;

  SetDbgMemHooks();

  if (setjmp(ExprJumpBuf))
  {
    ClearStack(&StackEval);
    ClearQueue(&Q);
    ClearStack(&Stack2);
    ClearQueue(&TokList);
    _getch();
    return;
  }

  printf("Input file name:");
  GetStr(FileName, sizeof(FileName));
  F = fopen(FileName, "r");
  if (F != NULL)
  {
    static CHAR Buf[1000];

    while (fgets(Buf, sizeof(Buf) - 1, F) != NULL)
      Scanner(Buf);
    fclose(F);
  }
  ParseProgram();
  DoCmd(Proga);
  DisplayVarTable();
  
  ClearQueue(&Q);
  _getch();
} /* End of 'main' function */
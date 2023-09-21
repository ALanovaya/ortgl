/* Lanovaya Alexandra, 10-6, 09.12.20 */
#include <stdio.h>
#include <string.h>

#include "ortgl.h"

static VARLIST *VarTable = NULL; /* table of variables */ 

/* Start of 'DisplayVarTable' function */
VOID DisplayVarTable( VOID )
{
  if (VarTable == NULL)
    printf("Empty variable list");
  else
    while (VarTable != NULL)
    {
      printf("%s = %f", VarTable->Name, VarTable->Value);
      printf((VarTable = VarTable->Next) != NULL ? "," : "\n");
    }
} /* End of 'DisplayVarTable' function */

/* Start of 'ClearVarTable' function */
VOID ClearVarTable( VOID )
{
  VARLIST *Old;

  while (VarTable != NULL)
  {
    Old = VarTable;
    VarTable = VarTable->Next;
    free(Old);
  }
} /* End of 'ClearVarTable' function */

/* Start of 'SetValue' function */
VOID SetValue( CHAR *Name, DBL Value )
{
  VARLIST **ptr = &VarTable;
  while (*ptr != NULL)
    if (strcmp((*ptr)->Name, Name) == 0)
    {
      (*ptr)->Value = Value;
      return;
    }
    else
      ptr = &(*ptr)->Next;
  if ((*ptr = malloc(sizeof(VARLIST))) == NULL)
    Error("Not enough memory for new variable");
  strncpy((*ptr)->Name, Name, MAX_NAME - 1);
  (*ptr)->Value = Value;
  (*ptr)->Next = NULL;
} /* End of 'SetValue' function */

/* Start of 'GetValue' function */
DBL GetValue( CHAR *Name )
{
  VARLIST **ptr = &VarTable;
  while (*ptr != NULL)
    if (strcmp((*ptr)->Name, Name) == 0)
    {
      return (*ptr)->Value;         
    }
    else
      ptr = &(*ptr)->Next;
  return 0;
} /* End of 'GetValue' function */
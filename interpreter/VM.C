/* Lanovaya Alexandra, 10-6, 09.12.20 */
#include <ctype.h>
#include <string.h>
#include <math.h>

#include "ortgl.h"

STACK StackEval;

/* Start of 'Eval' function */
double Eval( LIST *Expr )
{
  TOK T, A = {0}, Res;
  STACK S = {NULL};
  int tmp;

  while (Expr != NULL)
  {
    T = Expr->T;
    Expr = Expr->Next;
    if (T.Id == TOK_NUM || T.Id ==  TOK_NAME || (T.Id == TOK_OP && T.OperTable == &OperTable[17])) 
      Push(&StackEval, T);
    else if (T.Id == TOK_FUNC)
    {
      tmp = 0;
      do
      {
        Pop(&StackEval, &A);
        tmp++;
      }
      while (A.Id == TOK_OP && A.OperTable == &OperTable[17]);
      if (tmp != T.OperTable->Arity && T.OperTable->Arity != -1)
        Error("Wrong amount of arguments");
      if (A.Id == TOK_NAME && T.OperTable->Name[0] != '=' &&
        strcmp(T.OperTable->Name, "scan"))
        A.Num = GetValue(A.Name), A.Id = TOK_NUM;
      Push(&S, A);
      for (tmp -= 1; tmp > 0; tmp--)
      {
        Pop(&StackEval, &A);
        if (A.Id == TOK_NAME && T.OperTable->Name[0] != '=' &&
          strcmp(T.OperTable->Name, "scan"))
          A.Num = GetValue(A.Name), A.Id = TOK_NUM;
        Push(&S, A);
      }
      A.Num = T.OperTable->Do(&S).Num;
      Push(&StackEval, A);
    }
    else
    {
      for (tmp = T.OperTable->Arity; tmp > 0; tmp--)
      {
        Pop(&StackEval, &A);
        if (A.Id == TOK_NAME && T.OperTable->Name[0] != '=')
          A.Num = GetValue(A.Name), A.Id = TOK_NUM;
        Push(&S, A);
      }
      if (A.Id == TOK_NAME && T.OperTable->Name[0] != '=' && T.Id != TOK_FUNC)
        A.Num = GetValue(A.Name), A.Id = TOK_NUM;
      A.Num = T.OperTable->Do(&S).Num;
      Push(&StackEval, A);
    }
  } 
  Pop(&StackEval, &Res);
  ClearStack(&StackEval);
  return Res.Num;
} /* End of 'Eval' function */

/* Start of 'DoCmd' function */
void DoCmd( CMD *C )
{
  while (C != NULL)
  {
    if (C->Id == CMD_EXPR)
      Eval(C->Expr);
    else if (C->Id == CMD_IF)
      if (Eval(C->Expr))
        DoCmd(C->C1);
      else
        DoCmd(C->C2);
    else if (C->Id == CMD_WHILE)
      while (Eval(C->Expr))
        DoCmd(C->C1);
     else if (C->Id == CMD_FOR)
      for (Eval(C->Expr); Eval(C->FExpr); (C->SExpr))
        DoCmd(C->C1);
    C = C->Next;
  }
} /* End of 'DoCmd' function */
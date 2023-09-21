/* Lanovaya Alexandra, 10-6, 09.12.20 */
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include "ortgl.h"

TOK DoEq( STACK *S )
{
  TOK T, A, B;
  Pop(S, &A);
  Pop(S, &B);

  if (A.Id != TOK_NAME)
    Error("Lvalue required");
  SetValue(A.Name, B.Num);
  T.Id = TOK_NUM;
  T.Num = B.Num;
  return T;
}

TOK DoPlus( STACK *S )
{
  TOK T, A, B;
  Pop(S, &A);
  Pop(S, &B);

  A.Num += B.Num;
  T.Id = TOK_NUM;
  T.Num = A.Num;
  return T;
}

TOK DoMinus( STACK *S )
{
  TOK T, A, B;
  Pop(S, &A);
  Pop(S, &B);

  A.Num -= B.Num;
  T.Id = TOK_NUM;
  T.Num = A.Num;
  return T;
}

TOK DoNegate( STACK *S )
{
  TOK T, A;
  Pop(S, &A);
  
  A.Num = -A.Num;
  T.Id = TOK_NUM;
  T.Num = A.Num;
  return T;
}

TOK DoMult( STACK *S )
{
  TOK T, A, B;
  Pop(S, &A);
  Pop(S, &B);

  A.Num *= B.Num;
  T.Id = TOK_NUM;
  T.Num = A.Num;
  return T;
}

TOK DoDev( STACK *S )
{
  TOK T, A, B;
  Pop(S, &A);
  Pop(S, &B);
  
  if (!B.Num)
    Error("Division by zero");
  A.Num /= B.Num;
  T.Id = TOK_NUM;
  T.Num = A.Num;
  return T;
}

TOK DoMod( STACK *S )
{
  TOK T, A, B;
  Pop(S, &A);
  Pop(S, &B);
  
  A.Num = fmod(A.Num, B.Num);
  T.Id = TOK_NUM;
  T.Num = A.Num;
  return T;
}

TOK DoPow( STACK *S )
{
  TOK T, A, B;
  Pop(S, &A);
  Pop(S, &B);
  
  if (A.Num == 0 && B.Num == 0)
    Error("0^0 not defined");
  A.Num = pow(A.Num, B.Num);
  T.Id = TOK_NUM;
  T.Num = A.Num;
  return T;
}

TOK DoCom( STACK *S )
{
  TOK T, A, B;
  Pop(S, &A);
  Pop(S, &B);

  T.Id = TOK_NUM;
  T.Num = B.Num;
  return T;
}

TOK DoPrint( STACK *S )
{
  TOK T, A;
  Pop(S, &A);

  if (A.Id == TOK_NUM)
    T.Num = A.Num;
  else 
    T.Num = GetValue(A.Name);
  
  T.Id = TOK_NUM;
  printf("Printed: %f\n", T.Num);
  return T;
}

TOK DoScan( STACK *S )
{
  TOK T, A;
  float x;
  Pop(S, &A);

  if (A.Id != TOK_NAME)
    Error("Wait for variable");
  
  printf("Input a number: ");
  scanf("%f", &x);
  T.Num = (double)x;
  SetValue(A.Name, T.Num);

  T.Id = TOK_NUM;
  return T;
}

TOK DoEqCom( STACK *S )
{
  TOK T, A, B;
  Pop(S, &A);
  Pop(S, &B);
  
  if (A.Num == B.Num)
    T.Num = 1;
  else
    T.Num = 0;
  T.Id = TOK_NUM;
  return T;
}

TOK DoUneqCom( STACK *S )
{
  TOK T, A, B;
  Pop(S, &A);
  Pop(S, &B);
  
  if (A.Num == B.Num)
    T.Num = 0;
  else
    T.Num = 1;
  T.Id = TOK_NUM;
  return T;
}

TOK DoMoreCom( STACK *S )
{
  TOK T, A, B;
  Pop(S, &A);
  Pop(S, &B);
  
  if (A.Num >= B.Num)
    T.Num = 1;
  else
    T.Num = 0;
  T.Id = TOK_NUM;
  return T;
}

TOK DoStrictMoreCom( STACK *S )
{
  TOK T, A, B;
  Pop(S, &A);
  Pop(S, &B);
  
  if (A.Num > B.Num)
    T.Num = 1;
  else
    T.Num = 0;
  T.Id = TOK_NUM;
  return T;
}

TOK DoLessCom( STACK *S )
{
  TOK T, A, B;
  Pop(S, &A);
  Pop(S, &B);
  
  if (A.Num <= B.Num)
    T.Num = 1;
  else
    T.Num = 0;
  T.Id = TOK_NUM;
  return T;
}

TOK DoStrictLessCom( STACK *S )
{
  TOK T, A, B;
  Pop(S, &A);
  Pop(S, &B);
  
  if (A.Num < B.Num)
    T.Num = 1;
  else
    T.Num = 0;
  T.Id = TOK_NUM;
  return T;
}
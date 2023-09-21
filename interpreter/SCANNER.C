/* Lanovaya Alexandra, 10-6, 09.12.20 */
#include <ctype.h>       
#include <string.h>

#include "ortgl.h"

OPER OperTable[] = 
{
  {"=", DoEq, OP_INF, OP_ASSOC_R, 3, 2},
  {"+", DoPlus, OP_INF, OP_ASSOC_L, 4, 2},
  {"-", DoMinus, OP_INF, OP_ASSOC_L, 4, 2},
  {"-", DoNegate, OP_PREF, OP_ASSOC_R, 4, 1},
  {"*", DoMult, OP_INF, OP_ASSOC_L, 5, 2},
  {"/", DoDev, OP_INF, OP_ASSOC_L, 5, 2},
  {"%", DoMod, OP_INF, OP_ASSOC_L, 5, 2},
  {"^", DoPow, OP_INF, OP_ASSOC_L, 6, 2},
  {",", DoCom, OP_PREF, OP_ASSOC_L, 2, 2},
  {"(", NULL, OP_INF, OP_ASSOC_L, 0, 0},
  {")", NULL, OP_INF, OP_ASSOC_L, 1, 0},
  {"!=", DoUneqCom, OP_INF, OP_ASSOC_R, 2, 2},
  {">", DoStrictMoreCom, OP_INF, OP_ASSOC_L, 2, 2},
  {"<", DoStrictLessCom, OP_INF, OP_ASSOC_L, 2, 2},
  {"==", DoEqCom, OP_INF, OP_ASSOC_L, 2, 2},
  {">=", DoMoreCom, OP_INF, OP_ASSOC_L, 2, 2},
  {"<=", DoLessCom, OP_INF, OP_ASSOC_L, 2, 2},
  {",", NULL, OP_INF, OP_ASSOC_R, 2, 0},
  {";", NULL, OP_SUF, OP_ASSOC_L, 0, 2},
  {"{", NULL, OP_INF, OP_ASSOC_L, 0, 2},
  {"}", NULL, OP_INF, OP_ASSOC_L, 0, 2},
  {"print", DoPrint, OP_PREF, OP_ASSOC_R, 7, 1},
  {"scan", DoScan, OP_PREF, OP_ASSOC_R, 7, 1}
};

KWRD Kws[] =
{
  {"if", KW_IF},
  {"else", KW_ELSE},
  {"while", KW_WHILE},
  {"for", KW_FOR}
};

/* Start of 'UpdateTokToOper' function */
static void UpdateTokToOper( TOK *T )
{
  int i;

  for (i = 0; i < 18; i++)
    if (!strcmp(T->Name, OperTable[i].Name))
    {
      T->Id = TOK_OP;
      T->OperTable = &OperTable[i];
      return;
    }
  for (i = 18; i < 21; i++)
    if (!strcmp(T->Name, OperTable[i].Name))
    {
      T->Id = TOK_SYMBOL;
      T->OperTable = &OperTable[i];
      return;
    }
  return;
} /* End of 'UpdateTokToOper' function */

/* Start of 'UpdateTokToFunc' function */
static void UpdateTokToFunc( TOK *T )
{
  int i;

  for (i = 21; i < sizeof(OperTable) / sizeof(OperTable[0]); i++)
    if (!strcmp(T->Name, OperTable[i].Name))
    {
      T->Id = TOK_FUNC;
      T->OperTable = &OperTable[i];
      return;
    }  
  return;
} /* End of 'UpdateTokToFunc' function */

/* Start of 'UpdateTokToKeyword' function */
static void UpdateTokToKeyword( TOK *T )
{
  int i;

  for (i = 0; i < sizeof(Kws) / sizeof(Kws[0]); i++)
    if (strcmp(Kws[i].Name, T->Name) == 0)
    {
      T->Id = TOK_KEYW;
      T->KeyW = Kws[i].KeyW; 
      return;
    }
} /* End of 'UpdateTokToKeyword' function */

/* Start of 'Scanner' function */
void Scanner( char *S )
{
  TOK T = {0};
  int i;

  while (*S != 0)
  {
    switch(*S)
    {
    case ' ':
    case '\n':
    case '\r':
    case '\t':
      S++;        
      continue;
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
    case '^':
    case ',':
    case ')':
    case '(':
    case ';':
    case '{':
    case '}':
      T.Name[0] = *S++;
      T.Name[1] = 0;
      UpdateTokToOper(&T);
      break;
    case '=':
    case '!':
    case '>':
    case '<':
      T.Name[0] = *S++;
      if (*S == '=')
      {
        T.Name[1] = *S++;
        T.Name[2] = 0;
      }
      else
        T.Name[1] = 0;
      UpdateTokToOper(&T);
      break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
      T.Id = TOK_NUM;
      T.Num = 0;

      while (isdigit((unsigned char)*S)) 
        T.Num = T.Num * 10 + *S++ - '0';
      if (*S == '.') 
      {
        double denum = 1;
        S++;
        while (isdigit((unsigned char)*S))
          T.Num += (*S++ - '0') / (denum *= 10);
      }
      break;
    default:  
      if (isalpha((unsigned char)*S))
      {
        T.Id = TOK_NAME;
        i = 0;
        do 
        {
          if (i < MAX_NAME - 1)
            T.Name[i++] = *S;
          S++;
        } while (isalpha((unsigned char)*S) || *S == '_');
        T.Name[i] = 0;
        UpdateTokToKeyword(&T);
        UpdateTokToFunc(&T);
        break;
      }
      Error("Unrecognized token: %c", *S);
    }
    Put(&TokList, T);
  }
} /* End of 'Scanner' function */


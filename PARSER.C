/* Lanovaya Alexandra, 10-6, 09.12.20 */
#include "ortgl.h"

#define NextTok()                                                \
  (IsEOTL ?                                                      \
     Error("Unexpected end-of-data") :                           \
     Get(&TokList, &TokCurrent) ?  (void)0 : (void)(IsEOTL = 1))
#define IsTokAnyOp() \
  (!IsEOTL && TokCurrent.Id == TOK_OP)
#define IsTokAnySym() \
  (!IsEOTL && TokCurrent.Id == TOK_SYMBOL)
#define IsTokAnyNum() \
  (!IsEOTL && TokCurrent.Id == TOK_NUM)
#define IsTokAnyName() \
  (!IsEOTL && TokCurrent.Id == TOK_NAME)
#define IsTokAnyFunc() \
  (!IsEOTL && TokCurrent.Id == TOK_FUNC)

#define IsTokOp(C) \
  (IsTokAnyOp() && !strcmp(TokCurrent.OperTable->Name, C))
#define IsTokSym(C) \
  (IsTokAnySym() && !strcmp(TokCurrent.Name, C))
#define IsTokFunc(C) \
  (IsTokAnyFunc() && !strcmp(TokCurrent.OperTable->Name, C))

#define IsTokKw(Keyword) \
  (!IsEOTL && TokCurrent.Id == TOK_KEYW && TokCurrent.KeyW == KW_ ## Keyword)
#define ParseAnyTok() \
  NextTok()
#define ParseAnyOp() \
  (IsTokAnyOp() ? NextTok() : Error("Expected any operator"))
#define ParseOp(C) \
  (IsTokOp(C) ? NextTok() : Error("Expected operator: %c", (C)))
#define ParseSym(C) \
  (IsTokSym(C) ? NextTok() : Error("Expected symbol: %c", (C)))
#define ParseNum(X) \
  (IsTokAnyNum() ? (*(X) = TokCurrent.Num, NextTok()) : Error("Expected number"))
#define ParseKw(Keyword) \
  (IsTokKw(Keyword) ? NextTok() : Error("Expected keyword: \"" #Keyword "\""))

QUEUE TokList;
QUEUE Queue1;
STACK Stack2;
CMD *Proga;
TOK TokCurrent;
INT IsEOTL;

/* Start of 'DropOperators' function */
static VOID DropOpers( OPER *Op )
{
  while (Stack2.Top != NULL && 
        ((Stack2.Top->T.OperTable->Assoc == OP_ASSOC_L && Stack2.Top->T.OperTable->Prior >= Op->Prior) ||
         (Stack2.Top->T.OperTable->Assoc == OP_ASSOC_R && 
          Stack2.Top->T.OperTable->Prior > Op->Prior)))
  {
    TOK T;

    Pop(&Stack2, &T);
    Put(&Queue1, T);
  }
} /* End of 'DropOperators' function */

/* Start of 'ParserExpr' function */
VOID ParserExpr( LIST **Expr )
{
  OPER O;
  INT brk = 0;
  INT func = 0;
  enum
  {
    STATE_PREFIX, STATE_SUFFIX, STATE_DONE, STATE_END
  } state = STATE_PREFIX;

  O.Name[0] = ')';
  O.Name[1] = 0;
  O.Prior = 1;
  while (state != STATE_END)
  {
    if (state <= STATE_SUFFIX)
      if (IsEOTL)
        if (state == STATE_SUFFIX)
          state = STATE_DONE;
        else
          Error("Unexpected and-of-expr");
    switch (state)
    {
    case STATE_PREFIX:
      if (IsTokOp("-"))
        TokCurrent.OperTable = &OperTable[3]; 
      if ((IsTokAnyOp() || IsTokAnyFunc()) && TokCurrent.OperTable->OpType == OP_PREF)
      {
        if (IsTokAnyFunc())
        {
          brk++, func++;
          Push(&Stack2, TokCurrent), ParseAnyTok();
          ParseOp("(");
        }
        break;
      }
      if (IsTokAnyNum() || IsTokAnyName())
        Put(&Queue1, TokCurrent), state = STATE_SUFFIX, ParseAnyTok();
      else if (IsTokOp("("))
      {
        brk++;
        Push(&Stack2, TokCurrent), ParseAnyTok();
      }
      break;
    case STATE_SUFFIX:
      if (!!brk && IsTokOp(")"))
      {
        if (func == brk)
          func--;
        brk--;
        ParseOp(")");
       DropOpers(TokCurrent.OperTable);
      }
      else if (!IsTokOp(")") && IsTokAnyOp())
      {
        if (brk == func && IsTokOp(","))
          TokCurrent.OperTable = &OperTable[17];
        DropOpers(TokCurrent.OperTable);
        Push(&Stack2, TokCurrent), ParseAnyTok();
        state = STATE_PREFIX;
      }
      else
        state = STATE_DONE;
      break;
    case STATE_DONE:
      DropOpers(&O);
      state = STATE_END;
      *Expr = Queue1.Head;
      Queue1.Head = NULL;
      break;
    }
  }
} /* End of 'ParserExpr' function */

/* Start of 'CmdInit' function */
VOID CmdInit( CMD **C )
{
  if ((*C = malloc(sizeof(CMD))) == NULL)
    Error("No memory");
  (*C)->Id = CMD_EXPR;
  (*C)->Expr = NULL;
  (*C)->C1 = (*C)->C2 = (*C)->Next = NULL;
} /* End of 'CmdInit' function */

/* Start of 'ParseCmd' function */
VOID ParseCmd( CMD **C )
{
  *C = NULL;
  if (IsTokKw(IF))
  {
    CmdInit(C);
    (*C)->Id = CMD_IF;
    ParseKw(IF);
    ParseOp("(");
    ParserExpr(&(*C)->Expr);
    ParseOp(")");
    ParseCmd(&(*C)->C1);
    if (IsTokKw(ELSE))
    {
      ParseKw(ELSE);
      ParseCmd(&(*C)->C2);
    }
  }
  else if (IsTokKw(WHILE))
  {
    CmdInit(C);
    (*C)->Id = CMD_WHILE;
    ParseKw(WHILE);
    ParseOp("(");
    ParserExpr(&(*C)->Expr);
    ParseOp(")");
    ParseCmd(&(*C)->C1);
  }
    else if (IsTokKw(FOR))
  {
    CmdInit(C);
    (*C)->Id = CMD_FOR;
    ParseKw(FOR);
    ParseOp("(");
    ParserExpr(&(*C)->Expr);
    ParseSym(";");
    ParserExpr(&(*C)->FExpr);
    ParseSym(";");
    ParserExpr(&(*C)->SExpr);
    ParseOp(")");
    ParseCmd(&(*C)->C1);
  }
  else if (IsTokSym("{"))
  {
    ParseSym("{");
    while (!IsTokSym("}"))
    {
      ParseCmd(C);
      C = &(*C)->Next;
    }
    ParseSym("}");
  }
  else
  {
    CmdInit(C);
    ParserExpr(&(*C)->Expr);
    ParseSym(";");
  }
} /* End of 'ParseCmd' function */

/* Start of 'ParseProgram' function */
VOID ParseProgram( VOID )
{
  CMD **cmd = &Proga;
 
  Proga = NULL;
  IsEOTL = 0;
  NextTok();
  while (!IsEOTL)
  {
    ParseCmd(cmd);
    cmd = &(*cmd)->Next;
  }
} /* End of 'ParseProgram' function */
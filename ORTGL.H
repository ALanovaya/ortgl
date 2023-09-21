/* Lanovaya Alexandra, 10-6, 09.12.20 */

/* Debug memory allocation support */
#ifndef NDEBUG
#  define _CRTDBG_MAP_ALLOC
#  include <crtdbg.h>
#  define SetDbgMemHooks() \
     _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
       _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#else /* _DEBUG */
#  define SetDbgMemHooks() ((void)0)
#endif /* _DEBUG */

#include <commondf.h >

typedef double DBL;
typedef float FLT;
#define MAX_NAME 100        /* max lenght of variable and operation */

typedef enum                /* types of lexemes */
{
  TOK_OP,
  TOK_NUM,
  TOK_NAME,
  TOK_FUNC,
  TOK_KEYW,
  TOK_SYMBOL
} TOKID;

typedef enum
{
  CMD_EXPR, CMD_IF, CMD_WHILE, CMD_FOR
} CMDID;

typedef enum                /* key words */
{
  KW_IF, KW_ELSE, KW_WHILE, KW_FOR
} KEYWORDS;

typedef enum                /* type of operations */
{
  OP_PREF, OP_SUF, OP_INF
} OPERTYPE;

typedef enum                /* type of associations */
{
  OP_ASSOC_L, OP_ASSOC_R
} ASSOCTYPE;

typedef struct tagOPER OPER;
typedef struct tagTOK TOK;
typedef struct tagSTACK STACK;
struct tagOPER
{
  CHAR Name[MAX_NAME];      /* name of operation */
  TOK (*Do)( STACK *S );    /* pointer to function of this operation */
  OPERTYPE OpType;          /* type of operation */
  ASSOCTYPE Assoc;          /* association of operation */
  INT Prior;                /* prior of operation */
  INT Arity;                /* arity for function/operation */
};

extern OPER OperTable[];    /* table of operations */

struct tagTOK               /* lexemes */
{
  TOKID Id;                 /* type of lexem */
  OPER *OperTable;          /* pointer to table of operations */
  CHAR Name[MAX_NAME];      /* name of lexem */
  DBL Num;                  /* value of lexem */
  KEYWORDS KeyW;            /* key word */
};

typedef struct
{
  CHAR Name[MAX_NAME];      /* keyword name */
  KEYWORDS KeyW;            /* keyword constant value */
} KWRD;

typedef struct tagVARLIST VARLIST;
struct tagVARLIST           /* variable list */
{
  CHAR Name[MAX_NAME];      /* name of variable */
  DBL Value;                /* value of variable */
  VARLIST *Next;            /* pointer to next variable */
};
      
typedef struct tagLIST LIST;
struct tagLIST
{
  TOK T;                    /* lexem */
  LIST *Next;               /* pointer to next item of LIST */
};

typedef struct tagCMD CMD;
struct tagCMD
{
  CMDID Id;                 /* type of cmd */
  LIST *Expr,               /* expr for CMD_EXPR, condition for CMD_IF, CMD_WHILE */
       *FExpr,
       *SExpr;
  CMD
    *C1,                    /* ó CMD_IF the first, comand CMD_WHILE */
    *C2,                    /* ó CMD_IF the second, NULL if no 'else' */
    *C3,
    *Next;                  /* pointer to the next element */
};

typedef struct
{
  LIST 
    *Head,                  /* pointer to begin */
    *Tail;                  /* pointer to end */
} QUEUE;

struct tagSTACK
{
  LIST
    *Top;                   /* pointer to top of stack*/
};

extern STACK StackEval;     /* Stack for 'eval' */
extern QUEUE Queue1;
extern STACK Stack2;        /* Stack for 'parser' */
extern QUEUE TokList;       /* Queue for 'parser' */
extern TOK TokCurrent;
extern INT IsEOTL;
extern CMD *Proga;

/* Start of 'Put' function */
VOID Put( QUEUE *Q, TOK NewTOk );

/* Start of 'Get' function */
BOOL Get( QUEUE *Q, TOK *OldTok );

/* Start of 'Push' function */
VOID Push( STACK *S, TOK NewTOk );

/* Start of 'Pop' function */
BOOL Pop( STACK *S, TOK *OldTok );

/* Start of 'DisplayQueue' function */
VOID DisplayQueue( QUEUE *Q );

/* Start of 'DisplayStack' function */
VOID DisplayStack( STACK *S );

/* Start of 'ClearStack' function */
VOID ClearStack( STACK *S );

/* Start of 'ClearQueue' function */
VOID ClearQueue( QUEUE *Q );

/* Start of 'Scanner' function */
VOID Scanner( CHAR *S );

/* Start of 'Eval' function */
DBL Eval( LIST *Expr );

/* Start of 'Error' function */
VOID Error( CHAR *Str, ... );

/* Start of 'ParserExpr' function */
VOID ParserExpr( LIST **Expr );

/* Start of 'SetValue' function */
VOID SetValue( CHAR *Name, DBL Value );

/* Start of 'GetValue' function */
DBL GetValue( CHAR *Name );

/* Start of 'DisplayVarTable' function */
VOID DisplayVarTable( VOID );

/* Start of 'ClearVarTable' function */
VOID ClearVarTable( VOID );

/* Start of 'DoEqual' function */
TOK DoEq( STACK *S );

/* Start of 'DoPlus' function */
TOK DoPlus( STACK *S );

/* Start of 'DoMinus' function */
TOK DoMinus( STACK *S );

/* Start of 'DoNegate' function */
TOK DoNegate( STACK *S );

/* Start of 'DoMult' function */
TOK DoMult( STACK *S );

/* Start of 'DoDev' function */
TOK DoDev( STACK *S );

/* Start of 'DoMod' function */
TOK DoMod( STACK *S );

/* Start of 'DoPow' function */
TOK DoPow( STACK *S );

/* Start of 'DoCom' function */
TOK DoCom( STACK *S );

/* Start of 'DoScan' function */
TOK DoScan( STACK *S );

/* Start of 'DoPrint' function */
TOK DoPrint( STACK *S );

/* Start of 'DoSqrt' function */
TOK DoSqrt( STACK *S );

/* Start of 'DoAdd' function */
TOK DoAdd( STACK *S );

/* Start of 'DoEqCom' function */
TOK DoEqCom( STACK *S );

/* Start of 'DoUneqCom' function */
TOK DoUneqCom( STACK *S );

/* Start of 'DoMoreCom' function */
TOK DoMoreCom( STACK *S );

/* Start of 'DoStrictMoreCom' function */
TOK DoStrictMoreCom( STACK *S );

/* Start of 'DoStrictLessCom' function */
TOK DoStrictLessCom( STACK *S );

/* Start of 'DoLessCom' function */
TOK DoLessCom( STACK *S );

/* Start of 'ParseProgram' function */
VOID ParseProgram( VOID );

/* Start of 'DoCmd' function */
VOID DoCmd( CMD *C );

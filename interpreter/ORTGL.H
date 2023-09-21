/* Lanovaya Alexandra, 10-6, 09.12.20 */

/* Debug memory allocation support */
#ifndef NDEBUG
#  define _CRTDBG_MAP_ALLOC
#ifdef _MSC_VER
#include <crtdbg.h>
#else
#define _ASSERT(expr) ((void)0)

#define _ASSERTE(expr) ((void)0)
#endif
#  define SetDbgMemHooks() \
     _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
       _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#else /* _DEBUG */
#  define SetDbgMemHooks() ((void)0)
#endif /* _DEBUG */

#define MAX_NAME 100        /* max lenght of variable and operation */

#include <stdbool.h>
#include <stdarg.h>
#include <cstdlib>

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
  char Name[MAX_NAME];      /* name of operation */
  TOK (*Do)( STACK *S );    /* pointer to function of this operation */
  OPERTYPE OpType;          /* type of operation */
  ASSOCTYPE Assoc;          /* association of operation */
  int Prior;                /* prior of operation */
  int Arity;                /* arity for function/operation */
};

extern OPER OperTable[];    /* table of operations */

struct tagTOK               /* lexemes */
{
  TOKID Id;                 /* type of lexem */
  OPER *OperTable;          /* pointer to table of operations */
  char Name[MAX_NAME];      /* name of lexem */
  double Num;                  /* value of lexem */
  KEYWORDS KeyW;            /* key word */
};

typedef struct
{
  char Name[MAX_NAME];      /* keyword name */
  KEYWORDS KeyW;            /* keyword constant value */
} KWRD;

typedef struct tagVARLIST VARLIST;
struct tagVARLIST           /* variable list */
{
  char Name[MAX_NAME];      /* name of variable */
  double Value;                /* value of variable */
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
extern int IsEOTL;
extern CMD *Proga;

/* Start of 'Put' function */
void Put( QUEUE *Q, TOK NewTOk );

/* Start of 'Get' function */
bool Get( QUEUE *Q, TOK *OldTok );

/* Start of 'Push' function */
void Push( STACK *S, TOK NewTOk );

/* Start of 'Pop' function */
bool Pop( STACK *S, TOK *OldTok );

/* Start of 'DisplayQueue' function */
void DisplayQueue( QUEUE *Q );

/* Start of 'DisplayStack' function */
void DisplayStack( STACK *S );

/* Start of 'ClearStack' function */
void ClearStack( STACK *S );

/* Start of 'ClearQueue' function */
void ClearQueue( QUEUE *Q );

/* Start of 'Scanner' function */
void Scanner( char *S );

/* Start of 'Eval' function */
double Eval( LIST *Expr );

/* Start of 'Error' function */
void Error( char *Str, ... );

/* Start of 'ParserExpr' function */
void ParserExpr( LIST **Expr );

/* Start of 'SetValue' function */
void SetValue( char *Name, double Value );

/* Start of 'GetValue' function */
double GetValue( char *Name );

/* Start of 'DisplayVarTable' function */
void DisplayVarTable( void );

/* Start of 'ClearVarTable' function */
void ClearVarTable( void );

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
void ParseProgram( void );

/* Start of 'DoCmd' function */
void DoCmd( CMD *C );

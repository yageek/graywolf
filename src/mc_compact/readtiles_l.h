#ifdef linux
#include <stdio.h>
#else
# include "stdio.h"
#endif
#include <yalecad/string.h>
# define U(x) ((x)&0377)
# define NLSTATE yyprevious=YYNEWLINE
# define BEGIN yybgin = yysvec + 1 +
# define INITIAL 0
# define YYLERR yysvec
# define YYSTATE (yyestate-yysvec-1)
# define YYOPTIM 1
# define YYLMAX 200
# define output(c) putc(c,yyout)
# define input() (((yytchar=yysptr>yysbuf?U(*--yysptr):getc(yyin))==10?(yylineno++,yytchar):yytchar)==EOF?0:yytchar)
# define unput(c) {yytchar= (c);if(yytchar=='\n')yylineno--;*yysptr++=yytchar;}
# define yymore() (yymorfg=1)
# define ECHO fprintf(yyout, "%s",yytext)
# define REJECT { nstr = yyreject(); goto yyfussy;}
int yyleng; extern char yytext[];
int yymorfg;
extern char *yysptr, yysbuf[];
int yytchar;
#if defined(__linux__) || defined(__APPLE__)
FILE *yyin = NULL, *yyout = NULL;
#else
FILE *yyin ={stdin}, *yyout ={stdout};
#endif
extern int yylineno;
struct yysvf { 
	struct yywork *yystoff;
	struct yysvf *yyother;
	int *yystops;};
struct yysvf *yyestate;
extern struct yysvf yysvec[], *yybgin;
/* ----------------------------------------------------------------- 
"@(#) readtiles.l version 7.3 5/6/91"
FILE:	    readtiles.l                               
DESCRIPTION:rules for lexical analyzer in readtiles.  This lexical
	    analyzer uses a binary search to reduce the size of 
	    the f.a.  generated by lex.  Thanks to Gary Richey who 
	    showed me the trick.  See chapter 3 of "Introduction
	    to Compiler Construction with UNIX" by Schreiner &
	    Friedman for more details.
CONTENTS:   lex rules -
	    screen()
DATE:	    Aug 07, 1988 - need to add SccsId in comments due to yacc
REVISIONS:  Oct 6, 1988 - fixed sign mistake in INTEGER & FLOAT
	    Feb 12, 1989 - added <> to alphanum to handle DEC case.
	    Apr  4, 1989 - added comments over multiple lines.
	    Apr 18, 1989 - added to compactor.
	    Apr 30, 1989 - modified STDCELL type
	    Thu Apr 18 00:58:28 EDT 1991 - new version featuring
		constraint compaction.
	    Mon May  6 22:35:12 EDT 1991 - added offset keyword.
----------------------------------------------------------------- */
#undef   YYLMAX 
#define  YYLMAX 2000       /* comments may be at most 2000 characters */

#define token(x)      x    /* makes it look like regular lex */
#define END(v) (v-1 + sizeof(v) / sizeof( v[0] ) ) /* for table lookup */

static int screen() ;
static int check_line_count() ;

# define YYNEWLINE 10
yylex(){
int nstr; extern int yyprevious;
while((nstr = yylook()) >= 0)
yyfussy: switch(nstr){
case 0:
if(yywrap()) return(0); break;
case 1:

		      {
			/* C-style comments over multiple lines */
          		check_line_count(yytext) ;
	              }
break;
case 2:
      { 
		         /* convert to an integer */
		         yylval.ival = atoi( yytext ) ;
	  	         return (INTEGER); 
	              }
break;
case 3:
 {
		         /* convert to an integer */
		         yylval.fval = atof( yytext ) ;
	  	         return (FLOAT); 
		      }
break;
case 4:
 {
		         /* convert to an integer */
		         yylval.fval = atof( yytext ) ;
	  	         return (FLOAT); 
		      }
break;
case 5:
              {  return(COLON); }
break;
case 6:
  {  return( screen() ) ; }
break;
case 7:
           {  line_countS++;}
break;
case 8:
             ;
break;
case 9:
              {  return( token(yytext[0]) ) ;}
break;
case -1:
break;
default:
fprintf(yyout,"bad switch yylook %d",nstr);
} return(0); }
/* end of yylex */

/* reserved word screener */
/* ----------------------------------------------------------------- 
    The following is table of the reserved words - Table must be in
    alphabetical order for binary search to work properly.
----------------------------------------------------------------- */
static struct rw_table {  /* reserved word table */
    char *rw_name ;      /* pattern */
    int rw_yylex  ;      /* lex token number */
} rwtable[] = {
    "b",                   token(B),
    "cell",                token(CELL),
    "l",                   token(L),
    "numcells",            token(NUMCELLS),
    "numtiles",            token(NUMTILES),
    "offset",              token(OFFSET),
    "r",                   token(R),
    "stdcell",             token(STDCELL),
    "t",                   token(T),
    "x",                   token(X),
    "y",                   token(Y)
} ;

static int screen() 
{
    int c ;
    struct rw_table  *low = rwtable,        /* ptr to beginning */
		     *mid ,  
		     *high = END(rwtable) ;   /* ptr to end */

    /* binary search to look thru table to find pattern match */
    while( low <= high){
	mid = low + (high-low) / 2 ;
	if( (c = strcmp(mid->rw_name, yytext) ) == STRINGEQ){
	    return( mid->rw_yylex ) ; /* return token number */
	} else if( c < 0 ){
	    low = mid + 1 ;
	} else {
	    high = mid - 1 ;
	}
    }
    /* at this point we haven't found a match so we have a string */
    /* save the string by making copy */
    yylval.string = (char *) Ystrclone( yytext ) ;
    return (STRING); 
		
} /* end screen function */

static int check_line_count( s ) 
char *s ;
{
    if( s ){
	if( strlen(s) >= YYLMAX ){
	    sprintf(YmsgG,"comment beginning at line %d ",line_countS+1 );
	    M( ERRMSG, "lex", YmsgG ) ;
	    sprintf(YmsgG,"exceeds maximum allowed length:%d chars.\n", 
		YYLMAX );
	    M( MSG, NULL, YmsgG ) ;
	    setErrorFlag() ;
	}
	for( ;*s;s++ ){
	    if( *s == '\n'){
		line_countS++;
	    }
	}
    }
} /* end check_line_count */
int yyvstop[] ={
0,

8,
0,

8,
0,

9,
0,

8,
9,
0,

7,
0,

6,
9,
0,

6,
9,
0,

6,
9,
0,

2,
9,
0,

5,
9,
0,

8,
0,

6,
0,

6,
0,

2,
6,
0,

6,
0,

3,
6,
0,

2,
0,

6,
0,

6,
0,

6,
0,

6,
0,

6,
0,

3,
6,
0,

4,
6,
0,

6,
0,

6,
0,

1,
6,
0,

6,
0,

1,
0,

6,
0,

1,
6,
0,

1,
0,
0};
# define YYTYPE unsigned char
struct yywork { YYTYPE verify, advance; } yycrank[] ={
0,0,	0,0,	1,3,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	1,4,	1,5,	
4,11,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	1,6,	4,11,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	1,6,	
1,7,	8,15,	0,0,	1,6,	
1,8,	1,9,	20,20,	28,35,	
29,36,	34,27,	35,28,	7,13,	
36,28,	7,13,	0,0,	1,10,	
7,14,	7,14,	7,14,	7,14,	
7,14,	7,14,	7,14,	7,14,	
7,14,	7,14,	1,6,	6,12,	
6,12,	6,12,	6,12,	6,12,	
6,12,	6,12,	6,12,	6,12,	
6,12,	6,12,	6,12,	6,12,	
6,12,	6,12,	6,12,	6,12,	
6,12,	6,12,	6,12,	6,12,	
6,12,	6,12,	6,12,	6,12,	
0,0,	6,12,	6,12,	6,12,	
6,12,	6,12,	6,12,	6,12,	
6,12,	6,12,	6,12,	6,12,	
6,12,	6,12,	6,12,	6,12,	
6,12,	6,12,	6,12,	6,12,	
6,12,	6,12,	6,12,	6,12,	
6,12,	6,12,	6,12,	6,12,	
6,12,	6,12,	6,12,	6,12,	
6,12,	6,12,	6,12,	6,12,	
6,12,	6,12,	6,12,	6,12,	
6,12,	6,12,	6,12,	6,12,	
6,12,	6,12,	6,12,	6,12,	
6,12,	6,12,	6,12,	6,12,	
6,12,	6,12,	6,12,	6,12,	
6,12,	6,12,	6,12,	6,12,	
6,12,	6,12,	6,12,	6,12,	
6,12,	6,12,	6,12,	6,12,	
6,12,	9,16,	0,0,	9,17,	
9,17,	9,17,	9,17,	9,17,	
9,17,	9,17,	9,17,	9,17,	
9,17,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
13,13,	0,0,	13,13,	0,0,	
9,18,	13,14,	13,14,	13,14,	
13,14,	13,14,	13,14,	13,14,	
13,14,	13,14,	13,14,	14,16,	
0,0,	14,14,	14,14,	14,14,	
14,14,	14,14,	14,14,	14,14,	
14,14,	14,14,	14,14,	0,0,	
0,0,	0,0,	0,0,	15,19,	
0,0,	0,0,	0,0,	0,0,	
9,18,	0,0,	14,18,	15,19,	
15,19,	16,23,	0,0,	16,24,	
16,24,	16,24,	16,24,	16,24,	
16,24,	16,24,	16,24,	16,24,	
16,24,	25,25,	25,25,	25,25,	
25,25,	25,25,	25,25,	25,25,	
25,25,	25,25,	25,25,	15,20,	
16,18,	0,0,	0,0,	0,0,	
0,0,	0,0,	14,18,	0,0,	
15,21,	15,20,	0,0,	0,0,	
15,20,	15,22,	15,20,	0,0,	
19,19,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
15,19,	19,26,	19,19,	0,0,	
0,0,	19,19,	19,19,	19,19,	
16,18,	0,0,	17,16,	15,20,	
17,17,	17,17,	17,17,	17,17,	
17,17,	17,17,	17,17,	17,17,	
17,17,	17,17,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
19,19,	18,18,	0,0,	18,18,	
0,0,	17,18,	18,25,	18,25,	
18,25,	18,25,	18,25,	18,25,	
18,25,	18,25,	18,25,	18,25,	
21,27,	26,27,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
21,27,	21,27,	26,32,	26,27,	
0,0,	0,0,	26,27,	26,33,	
26,27,	0,0,	0,0,	0,0,	
23,23,	17,18,	23,31,	23,31,	
23,31,	23,31,	23,31,	23,31,	
23,31,	23,31,	23,31,	23,31,	
21,28,	0,0,	0,0,	0,0,	
0,0,	26,27,	0,0,	0,0,	
27,19,	21,29,	21,28,	23,18,	
0,0,	21,28,	21,30,	21,28,	
0,0,	27,26,	27,19,	0,0,	
0,0,	27,19,	27,34,	27,19,	
0,0,	21,27,	24,24,	24,24,	
24,24,	24,24,	24,24,	24,24,	
24,24,	24,24,	24,24,	24,24,	
21,28,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	23,18,	
27,19,	0,0,	0,0,	24,18,	
31,31,	31,31,	31,31,	31,31,	
31,31,	31,31,	31,31,	31,31,	
31,31,	31,31,	32,19,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	32,26,	
32,19,	31,18,	0,0,	32,19,	
32,37,	32,19,	0,0,	0,0,	
0,0,	0,0,	0,0,	24,18,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	0,0,	32,19,	0,0,	
0,0,	0,0,	0,0,	0,0,	
0,0,	31,18,	0,0,	0,0,	
0,0};
struct yysvf yysvec[] ={
0,	0,	0,
yycrank+-1,	0,		yyvstop+1,
yycrank+0,	yysvec+1,	yyvstop+3,
yycrank+0,	0,		yyvstop+5,
yycrank+3,	0,		yyvstop+7,
yycrank+0,	0,		yyvstop+10,
yycrank+38,	0,		yyvstop+12,
yycrank+12,	yysvec+6,	yyvstop+15,
yycrank+3,	yysvec+6,	yyvstop+18,
yycrank+119,	yysvec+6,	yyvstop+21,
yycrank+0,	0,		yyvstop+24,
yycrank+0,	yysvec+4,	yyvstop+27,
yycrank+0,	yysvec+6,	yyvstop+29,
yycrank+141,	yysvec+6,	yyvstop+31,
yycrank+153,	yysvec+6,	yyvstop+33,
yycrank+-214,	0,		yyvstop+36,
yycrank+179,	yysvec+6,	yyvstop+38,
yycrank+236,	yysvec+6,	yyvstop+41,
yycrank+258,	yysvec+6,	yyvstop+43,
yycrank+-231,	yysvec+15,	0,	
yycrank+-3,	yysvec+15,	yyvstop+45,
yycrank+-315,	0,		yyvstop+47,
yycrank+0,	yysvec+15,	yyvstop+49,
yycrank+290,	yysvec+6,	yyvstop+51,
yycrank+326,	yysvec+6,	yyvstop+53,
yycrank+189,	yysvec+6,	yyvstop+56,
yycrank+-284,	yysvec+21,	0,	
yycrank+-323,	yysvec+15,	0,	
yycrank+-4,	yysvec+15,	yyvstop+59,
yycrank+-5,	yysvec+15,	yyvstop+61,
yycrank+0,	yysvec+6,	yyvstop+63,
yycrank+348,	yysvec+6,	yyvstop+66,
yycrank+-373,	yysvec+15,	0,	
yycrank+0,	0,		yyvstop+68,
yycrank+6,	0,		0,	
yycrank+7,	yysvec+6,	yyvstop+70,
yycrank+9,	yysvec+6,	yyvstop+72,
yycrank+0,	yysvec+34,	yyvstop+75,
0,	0,	0};
struct yywork *yytop = yycrank+449;
struct yysvf *yybgin = yysvec+1;
char yymatch[] ={
00  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,011 ,012 ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
011 ,'!' ,'!' ,'!' ,'!' ,'!' ,'!' ,'!' ,
'!' ,'!' ,'*' ,'+' ,'!' ,'+' ,'.' ,'/' ,
'0' ,'0' ,'0' ,'0' ,'0' ,'0' ,'0' ,'0' ,
'0' ,'0' ,':' ,'!' ,'!' ,'!' ,'!' ,'!' ,
'!' ,'!' ,'!' ,'!' ,'!' ,'E' ,'!' ,'!' ,
'!' ,'!' ,'!' ,'!' ,'!' ,'!' ,'!' ,'!' ,
'!' ,'!' ,'!' ,'!' ,'!' ,'!' ,'!' ,'!' ,
'!' ,'!' ,'!' ,'!' ,'!' ,'!' ,'!' ,'!' ,
'!' ,'!' ,'!' ,'!' ,'!' ,'E' ,'!' ,'!' ,
'!' ,'!' ,'!' ,'!' ,'!' ,'!' ,'!' ,'!' ,
'!' ,'!' ,'!' ,'!' ,'!' ,'!' ,'!' ,'!' ,
'!' ,'!' ,'!' ,'!' ,'!' ,'!' ,'!' ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
01  ,01  ,01  ,01  ,01  ,01  ,01  ,01  ,
0};
char yyextra[] ={
0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,
0};
/*	ncform	4.1	83/08/11	*/

int yylineno =1;
# define YYU(x) x
# define NLSTATE yyprevious=YYNEWLINE
char yytext[YYLMAX];
struct yysvf *yylstate [YYLMAX], **yylsp, **yyolsp;
char yysbuf[YYLMAX];
char *yysptr = yysbuf;
int *yyfnd;
extern struct yysvf *yyestate;
int yyprevious = YYNEWLINE;
yylook(){
	register struct yysvf *yystate, **lsp;
	register struct yywork *yyt;
	struct yysvf *yyz;
	int yych;
	struct yywork *yyr;
# ifdef LEXDEBUG
	int debug;
# endif
	char *yylastch;
#ifdef linux
	if (yyin == NULL) yyin = stdin;
	if (yyout == NULL) yyout = stdout;
#endif
	/* start off machines */
# ifdef LEXDEBUG
	debug = 0;
# endif
	if (!yymorfg)
		yylastch = yytext;
	else {
		yymorfg=0;
		yylastch = yytext+yyleng;
		}
	for(;;){
		lsp = yylstate;
		yyestate = yystate = yybgin;
		if (yyprevious==YYNEWLINE) yystate++;
		for (;;){
# ifdef LEXDEBUG
			if(debug)fprintf(yyout,"state %d\n",yystate-yysvec-1);
# endif
			yyt = yystate->yystoff;
			if(yyt == yycrank){		/* may not be any transitions */
				yyz = yystate->yyother;
				if(yyz == 0)break;
				if(yyz->yystoff == yycrank)break;
				}
			*yylastch++ = yych = input();
		tryagain:
# ifdef LEXDEBUG
			if(debug){
				fprintf(yyout,"unsigned char ");
				allprint(yych);
				putchar('\n');
				}
# endif
			yyr = yyt;
			if ( (long)yyt > (long)yycrank){
				yyt = yyr + yych;
				if (yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transitions */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					goto contin;
					}
				}
# ifdef YYOPTIM
			else if((long)yyt < (long)yycrank) {		/* r < yycrank */
				yyt = yyr = yycrank+(yycrank-yyt);
# ifdef LEXDEBUG
				if(debug)fprintf(yyout,"compressed state\n");
# endif
				yyt = yyt + yych;
				if(yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transitions */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					goto contin;
					}
				yyt = yyr + YYU(yymatch[yych]);
# ifdef LEXDEBUG
				if(debug){
					fprintf(yyout,"try fall back character ");
					allprint(YYU(yymatch[yych]));
					putchar('\n');
					}
# endif
				if(yyt <= yytop && yyt->verify+yysvec == yystate){
					if(yyt->advance+yysvec == YYLERR)	/* error transition */
						{unput(*--yylastch);break;}
					*lsp++ = yystate = yyt->advance+yysvec;
					goto contin;
					}
				}
			if ((yystate = yystate->yyother) && (yyt= yystate->yystoff) != yycrank){
# ifdef LEXDEBUG
				if(debug)fprintf(yyout,"fall back to state %d\n",yystate-yysvec-1);
# endif
				goto tryagain;
				}
# endif
			else
				{unput(*--yylastch);break;}
		contin:
# ifdef LEXDEBUG
			if(debug){
				fprintf(yyout,"state %d char ",yystate-yysvec-1);
				allprint(yych);
				putchar('\n');
				}
# endif
			;
			}
# ifdef LEXDEBUG
		if(debug){
			fprintf(yyout,"stopped at %d with ",*(lsp-1)-yysvec-1);
			allprint(yych);
			putchar('\n');
			}
# endif
		while (lsp-- > yylstate){
			*yylastch-- = 0;
			if (*lsp != 0 && (yyfnd= (*lsp)->yystops) && *yyfnd > 0){
				yyolsp = lsp;
				if(yyextra[*yyfnd]){		/* must backup */
					while(yyback((*lsp)->yystops,-*yyfnd) != 1 && lsp > yylstate){
						lsp--;
						unput(*yylastch--);
						}
					}
				yyprevious = YYU(*yylastch);
				yylsp = lsp;
				yyleng = yylastch-yytext+1;
				yytext[yyleng] = 0;
# ifdef LEXDEBUG
				if(debug){
					fprintf(yyout,"\nmatch ");
					sprint(yytext);
					fprintf(yyout," action %d\n",*yyfnd);
					}
# endif
				return(*yyfnd++);
				}
			unput(*yylastch);
			}
		if (yytext[0] == 0  /* && feof(yyin) */)
			{
			yysptr=yysbuf;
			return(0);
			}
		yyprevious = yytext[0] = input();
		if (yyprevious>0)
			output(yyprevious);
		yylastch=yytext;
# ifdef LEXDEBUG
		if(debug)putchar('\n');
# endif
		}
	}
yyback(p, m)
	int *p;
{
if (p==0) return(0);
while (*p)
	{
	if (*p++ == m)
		return(1);
	}
return(0);
}
	/* the following are only used in the lex library */
yyinput(){
#ifdef linux
	if (yyin == NULL) yyin = stdin;
#endif
	return(input());
	}
yyoutput(c)
  int c; {
#ifdef linux
	if (yyout == NULL) yyout = stdout;
#endif
	output(c);
	}
yyunput(c)
   int c; {
	unput(c);
	}

#include "klang_lex.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int is_alpha(const char c)
{
    return (('A' <= c && c <= 'Z') ||
            ('a' <= c && c <= 'z') ||
            c == '_');
}

int is_num(const char c)
{
    return '0' <= c && c <= '9';
}

int is_escape(const char *c)
{
    if(c[0]!='\\')
    {
        return 0;
    }
    switch(c[1])
    {
        case('n'):
        case('0'):
        case('r'):
        case('t'):
        case('\\'):
        case('\''):
        case('"'):
            return 1;
        default:
            return 0;
    }
}

int is_comment(const char c)
{
    return (c=='#');
}

int is_quote(const char c)
{
    switch(c)
    {
        case('"'):
            return 1;
        default:
            return 0;
    }
}

int is_alnum(const char c)
{
    if(is_alpha(c) || is_num(c))
    {
        return 1;
    }
    return 0;
}

int is_space(const char c)
{
    switch(c)
    {
        case(' '):
            return 1;
        case('\t'):
            return 1;
        case('\r'):
            return 1;
        default:
            return 0;
    }
}

int is_newline(char c)
{
    if(c=='\n')
    {
        return 1;
    }
    return 0;
}

int is_punct(const char c)
{
    switch(c)
    {
        case('('):
        case(')'):
        case(':'):
        case(','):
        case('.'):
            return 1;
        default:
            return 0;
    }
}

int is_operand(const char c)
{
    switch(c)
    {
        case('+'):
        case('-'):
        case('>'):
        case('<'):
        case('='):
        case('*'):
        case('/'):
        case('|'):
        case('~'):
        case('&'):
        case('!'):
        case('^'):
            return 1;
        default:
            return 0;
    }
}

TokType whichPunct(const char c)
{
    switch(c)
    {
        case('('):
            return TOK_LPAR;
        case(')'):
            return TOK_RPAR;
        case(':'):
            return TOK_COL;
        case(','):
            return TOK_SEP;
        case('.'):
            return TOK_DOT;
        default:
            return TOK_UKNOWN;
    }
}

TokType whichOperand(const char *text)
{
    char c=text[0];
    switch(c)
    {
        case('='):
            switch(text[1])
            {
                case('='):
                return TOK_CMP;
                default:
                return TOK_EQ;
            }
        case('>'):
            switch(text[1])
            {
                case('>'):
                    return TOK_SHR;
                case('='):
                    return TOK_GE;
                default:
                    return TOK_GR;
            }
        case('<'):
            switch(text[1])
            {
                case('<'):
                    return TOK_SHL;
                case('='):
                    return TOK_LE;
                default:
                    return TOK_LS;
            }
        case('+'):
            switch(text[1])
            {
                case('+'):
                    return TOK_INC;
                case('='):
                    return TOK_AEQ;
            }
            return TOK_ADD;
        case('-'):
            switch(text[1])
            {
                case('-'):
                    return TOK_DEC;
                case('='):
                    return TOK_SEQ;
            }
            return TOK_SUB;
        case('*'):
            return TOK_MUL;
        case('/'):
            return TOK_DIV;
        case('~'):
            return TOK_BNOT;
        case('^'):
            return TOK_XOR;
        case('|'):
            switch(text[1])
            {
                case('|'):
                    return TOK_LOR;
            }
            return TOK_BOR;
        case('&'):
            switch(text[1])
            {
                case('&'):
                    return TOK_LAND;
            }
            return TOK_BAND;
        case('!'):
            switch(text[1])
            {
                case('='):
                    return TOK_NCMP;
            }
            return TOK_LNOT;
        default:
            return TOK_UKNOWN;
    }
}

int is_double_operator(TokType type)
{
    switch (type)
    {
        case TOK_CMP:
        case TOK_NCMP:
        case TOK_LE:
        case TOK_GE:
        case TOK_INC:
        case TOK_DEC:
        case TOK_AEQ:
        case TOK_SEQ:
        case TOK_SHL:
        case TOK_SHR:
        case TOK_LAND:
        case TOK_LOR:
            return 1;

        default:
            return 0;
    }
}

lex_state *newLexer(bool Report, char *text)
{
    lex_state *l=malloc(sizeof(lex_state));
    if(l==NULL)
    {
        return NULL;
    }
    l->col=0;
    l->line=0;
    l->cursor=0;
    l->text=text;
    return l;
}


TOKEN *lex(lex_state *l)
{
    if(l==NULL)
    {
        return NULL;
    }
    TOKEN *t=malloc(sizeof(TOKEN));
    if(t==NULL)
    {
        return NULL;
    }
    while(l->text[l->cursor]!='\0')
    {
        if(is_alpha(l->text[l->cursor]))
        {
            t->token_start=l->text+l->cursor;
            uint32_t start_index=l->cursor;
            t->line=l->line;
            t->col=l->col;
            while(is_alnum(l->text[l->cursor]))
            {
                l->col++;
                l->cursor++;
            }
            t->token_size=l->cursor-start_index; // Get token size
            t->token_type=TOK_KEY;
            return t;
        }
        else if(is_num(l->text[l->cursor]))
        {
            t->col=l->col;
            t->line=l->line;
            t->token_start=l->text+l->cursor;
            uint32_t start_index=l->cursor;
            while(is_num(l->text[l->cursor]))
            {
                l->col++;
                l->cursor++;
            }
            t->token_size=l->cursor-start_index;
            t->token_type=TOK_NUM;
            return t;
        }
        else if(is_punct(l->text[l->cursor]))
        {
            t->col=l->col;
            t->line=l->line;
            t->token_start=l->text+l->cursor;
            t->token_size=1;
            t->token_type=whichPunct(l->text[l->cursor]);
            l->col++;
            l->cursor++;
            return t;

        }
        else if(is_newline(l->text[l->cursor]))
        {
            t->token_size=1;
            t->token_start=l->text+l->cursor;
            t->token_type=TOK_NEWLINE;
            t->line=l->line;
            t->col=l->col;
            l->line++;
            l->col=0;
            l->cursor++;
            return t;
        }
        else if(is_operand(l->text[l->cursor]))
        {
            t->token_start=l->text+l->cursor;
            t->col=l->col;
            t->line=l->line;
            t->token_type=whichOperand(l->text+l->cursor);
            if(is_double_operator(t->token_type))
            {
                t->token_size=2;
                l->cursor+=2;
                l->col+=2;
            }
            else{
                t->token_size=1;
                l->cursor++;
                l->col++;
            }
            return t;
        }
        else if(is_quote(l->text[l->cursor]))
        {
            //exclude the first quote
            t->token_start=l->text+l->cursor+1;
            uint32_t start_index=l->cursor+1;
            l->cursor++;
            l->col++;


            t->line=l->line;
            t->col=l->col;
            while(!is_quote(l->text[l->cursor]))
            {
                if(l->text[l->cursor]=='\0')
                {
                    //Implement an report api instead of this
                    fprintf(stderr, "Error: Lexer: quote is not closed: line: %d col: %d", t->line, t->col);
                    free(t);
                    return NULL;
                }
                if(is_escape(l->text+l->cursor))
                {
                    l->col++;
                    l->cursor++;
                }
                l->col++;
                l->cursor++;
            }
            t->token_size=l->cursor-start_index;
            t->token_type=TOK_STR;



            //consume the last quote
            l->col++;
            l->cursor++;


            return t;
        }
        else if(is_comment(l->text[l->cursor]))
        {
            while(l->text[l->cursor]!='\n' && l->text[l->cursor]!='\0')
            {
                //consume the commen
                l->col++;
                l->cursor++;
            }
        }
        else if(is_space(l->text[l->cursor]))
        {
            l->col++;
            l->cursor++;
        }
        else{
            t->token_type=TOK_UKNOWN;
            t->token_size=1;
            t->token_start=l->text+l->cursor;
            t->line=l->line;
            t->col=l->col;
            l->col++;
            l->cursor++;
            return t;
        }
    }
    t->token_type=TOK_EOF;
    t->token_start=l->text+l->cursor;
    t->token_size=0;
    t->col=l->col;
    t->line=l->line;
    return t;
}
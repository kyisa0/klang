#ifndef KLANG_LEX_H
#define KLANG_LEX_H

#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum{
    TOK_KEY,        //keyword
    TOK_STR,        //string
    TOK_NUM,        //number
    TOK_RPAR,       //right par
    TOK_LPAR,       //left par
    TOK_NEWLINE,    //newline
    TOK_GR,         //>
    TOK_SHR,        //>>
    TOK_GE,         //>=
    TOK_LS,         //<
    TOK_SHL,        //<<
    TOK_LE,         //<=
    TOK_EQ,         //=
    TOK_CMP,        //==
    TOK_NCMP,       //!=
    TOK_INC,        //++
    TOK_DEC,        //--
    TOK_ADD,        //+
    TOK_AEQ,        //+=
    TOK_SUB,        //-
    TOK_SEQ,        //-=
    TOK_DIV,        // /
    TOK_MUL,        // *
    TOK_BAND,       //&
    TOK_LAND,       //&&
    TOK_BOR ,       //|
    TOK_LOR,        //||
    TOK_XOR,        //^
    TOK_LNOT,       //!
    TOK_BNOT,       //~
    TOK_SEP,        //,
    TOK_COL,        //:
    TOK_DOT,        // .
    TOK_UKNOWN,
    TOK_EOF         //end of file
}TokType;

typedef struct TOKEN{
    struct TOKEN *next;
    const char *token_start;
    uint32_t token_size;
    uint32_t line;
    uint32_t col;
    TokType token_type;
}TOKEN;

typedef struct{
    const char *text;
    uint32_t cursor;
    uint32_t line;
    uint32_t col;
    bool report_errors;
}lex_state;

const char *token_type_str[] = {
    "TOK_KEY",
    "TOK_STR",
    "TOK_NUM",
    "TOK_RPAR",
    "TOK_LPAR",
    "TOK_NEWLINE",
    "TOK_GR",
    "TOK_SHR",
    "TOK_GE",
    "TOK_LS",
    "TOK_SHL",
    "TOK_LE",
    "TOK_EQ",
    "TOK_CMP",
    "TOK_NCMP",
    "TOK_INC",
    "TOK_DEC",
    "TOK_ADD",
    "TOK_AEQ",
    "TOK_SUB",
    "TOK_SEQ",
    "TOK_DIV",
    "TOK_MUL",
    "TOK_BAND",
    "TOK_LAND",
    "TOK_BOR",
    "TOK_LOR",
    "TOK_XOR",
    "TOK_LNOT",
    "TOK_BNOT",
    "TOK_SEP",
    "TOK_COL",
    "TOK_DOT",
    "TOK_UKNOWN",
    "TOK_EOF"
};

lex_state *newLexer(bool Report, char *text);
TOKEN *lex(lex_state *l);

#endif //KLANG_LEX_H
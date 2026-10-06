#include "klang_parser.h"
#include "klang_lex.h"
#include <stdio.h>

const char built_in_keyword[][32]={
    "var", 
    "if", 
    "elif", 
    "else", 
    "i8", 
    "u8", 
    "i16",
    "u16",
    "i32",
    "u32",
    "f32",
    "str",
    "int",
    "end",
    "struct"
};

const char built_in_types[][32]={
    "u8",
    "i8",
    "u16",
    "i16",
    "u32",
    "i32",
    "f32",
    "int",
    "str"
};

void syntax_error(TOKEN *t,const char *text)
{
    fprintf(stderr, "Syntax Error: line:%d col: %d : %s\n", t->line, t->col, text);
    return;
}

void runtime_error(const char *text)
{
    fprintf(stderr, "Runtime Error: %s\n", text);
}

int token_is(const TOKEN *t,const char *prefix)
{
    size_t prefix_length=strlen(prefix);
    if(t->token_size!=prefix_length)
    {
        return 0;
    }
    for(size_t i=0;i<prefix_length;i++)
    {
        if(t->token_start[i]!=prefix[i])
        {
            return 0;
        }
    }
    return 1;
}

is_builtin(TOKEN *t)
{
    for(int i=0;i<sizeof(built_in_keyword)/32;i++)
    {
        if(token_is(t, built_in_keyword[i]))
        {
            return 1;
        }
    }
    return 0;
}

is_type(TOKEN *t)
{
    for(int i=0;i<sizeof(built_in_types)/32;i++)
    {
        if(token_is(t, built_in_types[i]))
        {
            return 1;
        }
    }
    return 0;
}

ASTnode *parse_var_decl(TOKEN *t)
{
    TOKEN* current=t;
    if(current->token_type!=TOK_KEY)
    {
        syntax_error(current, "Identifier expected");
        return NULL;
    }
    if(is_builtin(current))
    {
        syntax_error(current, "Identifier name cannot be bult-in keyword");
    }
    ASTnode *node=malloc(sizeof(ASTnode));
    node->node_type=NODE_VAR;
    node->symbol=current->token_start;
    node->symbol_length=current->token_size;
    if(node==NULL)
    {
        runtime_error("Failed to allocate AST node");
        return NULL;
    }
    current=current->next;
    if(current->token_type!=TOK_COL)
    {
        syntax_error(current, "Expected \":\"");
        return NULL; //syntax error expected colon
    }
    current=current->next;
    if(current->token_type!=TOK_KEY)
    {
        syntax_error(current, "Type expected");
        return NULL;
    }
    TOKEN *variable_type_token=current; // Store variable type token for later proccessing
    /* can only check at runtime while ast executer is running
    if(!is_type(t))
    {
        
    }*/
    current=current->next;
    if(current->token_type==TOK_EQ)
    {
        current=current->next;
        //Handle expressions if exists
        node->right=parse_expression(current);
    }
    else{
        node->right=malloc(sizeof(ASTnode));
        if(node->right==NULL)
        {
            runtime_error("Failed to allocate AST node");
            return NULL;
        }
    }
}

ASTnode *parse_expression(TOKEN *t)
{
    TOKEN *current=t;
    while(current->token_type!=TOK_EOF && current->token_type!=TOK_NEWLINE)
    {
        switch(current->token_type)
        {
            case(TOK_KEY):
                if(is_builtin(current))
                {
                    syntax_error(current, "keyword cannot be built-in name");
                    return NULL;
                }

        }
    }
}

ASTnode* parse(TOKEN *t)
{
    ASTnode *past=NULL;
    TOKEN *current=t;
    while(current->token_type!=TOK_EOF)
    {
        switch(current->token_type)
        {
            case(TOK_KEY):
                if(token_is(current, "var")) //handle var declarations
                {
                    current=current->next; //advance
                    ASTnode *node=parse_var_decl(current);
                    if(node==NULL)
                    {
                        //goto error exit
                    }
                }
                
        }
    }
}
#ifndef KLANG_PARSER_H
#define KLANG_PARSER_H

#include <stdint.h>


typedef enum{
    NODE_ADD,   // Add node
    NODE_SUB,   //Subtract node
    NODE_VAR,   //variable node
    NODE_DECL,  //variable declaration node
    NODE_CONST,  //Constant value
    NODE_ASSIGN,  //value assign node
    NODE_IF,      //IF node
    NODE_ELIF,    //else if node
    NODE_ELSE     //else node
}NodeType;


// This belongs to ast executer not here
/*
typedef enum{
    TYPE_U8,
    TYPE_I8,
    TYPE_U16,
    TYPE_I16,
    TYPE_U32,
    TYPE_I32,
    TYPE_F32,
    TYPE_STR,
    TYPE_INT,
    TYPE_UNKNOWN,       //For user defined variables
    TYPE_STRUCT         //For structures
    
}VarType;*/


//Implement this inside the AST executer too
typedef union{
    uint8_t U8;
    int8_t I8;
    uint16_t U16;
    int16_t I16;
    uint32_t U32;
    int32_t I32;
    float F32;
    int INTEGER;
    char *STR;
}Value;

typedef int VarID;

typedef struct ASTnode{
    struct ASTnode *next, *prev, *left, *right;
    NodeType node_type;
    const char *symbol;
    uint32_t symbol_length;
    VarID var_id;
    Value constant;
}ASTnode;


#endif //KLANG_PARSER_H
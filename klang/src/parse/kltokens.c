#include "klang/include/parse/kltokens.h"

static const char* descrition[KLTK_NTOKEN] = {
  [KLTK_ID] = "identifier",
  [KLTK_INT] = "integer",
  [KLTK_STRING] = "string",
  [KLTK_BOOLVAL] = "boolean",
  [KLTK_NIL] = "nil",
  [KLTK_VARARG] = "...",
  [KLTK_CONCAT] = "..",
  [KLTK_ADD] = "+",
  [KLTK_MINUS] = "-",
  [KLTK_MUL] = "*",
  [KLTK_DIV] = "/",
  [KLTK_MOD] = "%",
  [KLTK_IDIV] = "//",
  [KLTK_APPEND] = "<<",
  [KLTK_DOT] = ".",
  [KLTK_LT] = "<",
  [KLTK_LE] = "<=",
  [KLTK_GT] = ">",
  [KLTK_GE] = ">=",
  [KLTK_EQ] = "==",
  [KLTK_NE] = "!=",
  [KLTK_IS] = "is",
  [KLTK_ISNOT] = "isnot",
  [KLTK_NOT] = "!",
  [KLTK_AND] = "&&",
  [KLTK_OR] = "||",
  [KLTK_LPAREN] = "(",
  [KLTK_RPAREN] = ")",
  [KLTK_LBRACKET] = "[",
  [KLTK_RBRACKET] = "]",
  [KLTK_LBRACE] = "{",
  [KLTK_RBRACE] = "}",
  [KLTK_COMMA] = ",",
  [KLTK_SEMI] = ";",
  [KLTK_COLON] = ":",
  [KLTK_QUESTION] = "?",
  [KLTK_DARROW] = "=>",
  [KLTK_ARROW] = "->",
  [KLTK_ASSIGN] = "=",
  [KLTK_BAR] = "|",
  [KLTK_WHERE] = "where",
  [KLTK_IF] = "if",
  [KLTK_ELSE] = "else",
  [KLTK_WHILE] = "while",
  [KLTK_REPEAT] = "repeat",
  [KLTK_UNTIL] = "until",
  [KLTK_FOR] = "for",
  [KLTK_IN] = "in",
  [KLTK_LET] = "let",
  [KLTK_LOCAL] = "local",
  [KLTK_SHARED] = "shared",
  [KLTK_RETURN] = "return",
  [KLTK_BREAK] = "break",
  [KLTK_CONTINUE] = "continue",
  [KLTK_NEW] = "new",
  [KLTK_INHERIT] = "inherit",
  [KLTK_METHOD] = "method",
  [KLTK_ASYNC] = "async",
  [KLTK_YIELD] = "yield",
  [KLTK_END] = "EOF",
};

const char* kltoken_desc(KlTokenKind kind) {
  return kind == KLTK_ERR ? "error" : descrition[kind];
}
#ifndef PARSER_H
#define PARSER_H

#include "scanner.hpp"

void parse(vector<Token*> tokens);
void Program();
void Block();
void Vars();
void Expression();
void N();
void A();
void M();
void R();
void Stats();
void Mstat();
void Stat();
void In();
void Out();
void If();
void Loop();
void Assign();
void RO();
void Label();
void Goto();
void printAndIncrement();
void printRootLevelToStdOut(Node* &root, int level);
void error(string grammarLevel, Token* tk);

#endif
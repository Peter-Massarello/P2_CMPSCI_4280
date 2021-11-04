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
void On();
void Out();
void If();
void Loop();
void Assign();
void RO();
void label();
void Goto();
void error();

#endif
#include "parser.hpp"
#include "scanner.hpp"

static Token* tk;
vector<Token*> tokenList;
static int tokenIndex = 0;

void parse(vector<Token*> tokens){
    cout << "parsing" << endl;
    tokenList = tokens;

    tk = scanner(tokenList, tokenIndex);
    tokenIndex++;

    Program();
}

void printAndIncrement(){
    cout << tk->token << " " << tk->tokenType << endl;
    tk = scanner(tokenList, tokenIndex);
    tokenIndex++;
}

void Program(){
    Vars();
    if (tk->tokenType == PROGRAMtk){
        printAndIncrement();
        Block();
    }
    else
        error("Program", tk);
}

void Block(){
    if (tk->tokenType == STARTtk){
        printAndIncrement();
        Vars();
        Stats();

        if (tk->tokenType == STOPtk){
            printAndIncrement();
        } else error("Block", tk);
    } else error("Block", tk);
}

void Vars(){
    if (tk->tokenType == DECLAREtk){
        printAndIncrement();

        if (tk->tokenType == IDtk){
            printAndIncrement();

            if (tk->tokenType == EQUALtk){
                printAndIncrement();

                if (tk->tokenType == DIGITtk) {
                    printAndIncrement();

                    if (tk->tokenType == SEMItk){
                        printAndIncrement();

                        Vars();
                    }
                } else error("Vars", tk);
            } else error("Vars", tk);
        } else error("Vars", tk);
    }
}

void Stats(){
    Stat();
    Mstat();
}

void Stat(){
    In();
}

void Mstat(){
    Stat();
   // Mstat();
}

void In(){
    if (tk->tokenType == LISTENtk) {
        printAndIncrement();

        if (tk->tokenType == IDtk){
            printAndIncrement();

            if (tk->tokenType == SEMItk){
                printAndIncrement();
                
            } else error("In", tk);
        } else error("In", tk);
    }
}

void Out(){

}

void If(){

}

void Loop(){

}

void Assign(){

}

void Goto(){

}

void error(string grammarLevel, Token* tk){
    cout << "PARSER ERROR: Parser has encountered error at on grammar " << grammarLevel << ", token " << tk->token << endl;
    exit(0);
}
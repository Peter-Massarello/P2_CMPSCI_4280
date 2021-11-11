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
    }else error("Program", tk);

    if (tk->tokenType == EOFtk){
        cout << tk->token << " " << tk->tokenType << endl;
    } else error("Program", tk);
}

void Block(){
    cout << "In Block" << endl;
    if (tk->tokenType == STARTtk){
        printAndIncrement();
        Vars();
        Stats();

        if (tk->tokenType == STOPtk){
            printAndIncrement();
            return;
        } else error("Block Stop", tk);
    } else error("Block Start", tk);
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
    cout << "In stat" << endl;
    if (tk->tokenType == LISTENtk){
        In();

        if (tk->tokenType == SEMItk){
            printAndIncrement();
        } else error("In Stat", tk);
    } else if (tk->tokenType == TALKtk){
        Out();

        if (tk->tokenType == SEMItk){
            printAndIncrement();
        } else error("Out Stat", tk);
    } else if (tk->tokenType == STARTtk){
        Block();

    } else if (tk->tokenType == IFtk){
        cout << "in if stat with current tk " << tk->token << endl;
        If();

        if (tk->tokenType == SEMItk){
            printAndIncrement();
        } else error("If Stat", tk);
    } else if (tk->tokenType == WHILEtk){
        Loop();

        if (tk->tokenType == SEMItk){
            printAndIncrement();
        } else error("Loop Stat", tk);
    } else if (tk->tokenType == ASSIGNtk){
        Assign();

        if (tk->tokenType == SEMItk){
            printAndIncrement();
        } else error("Assign Stat", tk);
    } else if (tk->tokenType == LABELtk){
        Label();

        if (tk->tokenType == SEMItk){
            printAndIncrement();
        } else error("Label Stat", tk);
    }  else if (tk->tokenType == JUMPtk){
        Goto();

        if (tk->tokenType == SEMItk){
            printAndIncrement();
        } else error("Goto Stat", tk);
    } else error("Else Stat", tk);
}

void Mstat(){
    if (tk->tokenType == LISTENtk ||
        tk->tokenType == TALKtk || 
        tk->tokenType == IFtk || 
        tk->tokenType == WHILEtk || 
        tk->tokenType == ASSIGNtk || 
        tk->tokenType == LABELtk || 
        tk->tokenType == JUMPtk ||
        tk->tokenType == STARTtk)
    {
        Stat();
        Mstat();    
    }
}

void In(){
    printAndIncrement();

    if (tk->tokenType == IDtk){
        printAndIncrement();

    } else error("In", tk);
}

void Out(){
    printAndIncrement();

    Expression();
}

void If(){
    printAndIncrement();

    if (tk->tokenType == LBRACKtk){
        printAndIncrement();

        Expression();
        RO();
        Expression();

        if (tk->tokenType == RBRACKtk){
            printAndIncrement();

            if (tk->tokenType == THENtk){
                printAndIncrement();

                Stat();

                if (tk->tokenType == ELSEtk){
                    printAndIncrement();

                    Stat();
                }
            } else error("If", tk);
        }else error("If", tk);
    }else error("If", tk);
}

void Loop(){
    printAndIncrement();

    if (tk->tokenType == LBRACKtk){
        printAndIncrement();

        Expression();
        RO();
        Expression();

        if (tk->tokenType == RBRACKtk){
            printAndIncrement();

            Stat();
        }else error("Loop", tk);
    }else error("Loop", tk);
}

void Assign(){
    printAndIncrement();

    if (tk->tokenType == IDtk){
        printAndIncrement();

        if (tk->tokenType == EQUALtk) {
            printAndIncrement();

            Expression();
        } else error("Assign", tk);
    } else error("Assign", tk);
}

void Goto(){
    printAndIncrement();

    if (tk->tokenType == IDtk){
        printAndIncrement();
    } else error("Goto", tk);
}

void Label(){
    printAndIncrement();

    if (tk->tokenType == IDtk){
        printAndIncrement();
    } else error("Label", tk);
}

void RO(){
    if (tk->tokenType == GREATERTHANtk){
        printAndIncrement();
    } else if (tk->tokenType == LESSTHANtk){
        printAndIncrement();
    } else if (tk->tokenType == DEQUALtk){
        printAndIncrement();
    } else if (tk->tokenType == LBRACtk){
        printAndIncrement();

        if (tk->tokenType == DEQUALtk){
            printAndIncrement();

            if (tk->tokenType == RBRACtk){
                printAndIncrement();
            } else error("RO", tk);
        } else error("RO", tk);
    } else if (tk->tokenType == MODtk){
        printAndIncrement();
    } else error("RO", tk);
}

void Expression(){
    N();
    if (tk->tokenType == PLUStk){
        printAndIncrement();

        Expression();
    }
}

void N(){
    A();

    if (tk->tokenType == DIVtk){
        printAndIncrement();

        N();
    } else if (tk->tokenType == MULTtk){
        printAndIncrement();

        N();
    }
}

void A(){
    M();

    if (tk->tokenType == MINUStk){
        printAndIncrement();

        A();
    }
}

void M(){
    if (tk->tokenType == DOTtk){
        printAndIncrement();

        M();
    } else {
        R();
    }
}

void R(){
    if (tk->tokenType == LPARtk){
        printAndIncrement();

        Expression();

        if (tk->tokenType == RPARtk){
            printAndIncrement();
        } else error("R", tk);
    } else if (tk->tokenType == IDtk){
        printAndIncrement();
    } else if (tk->tokenType == DIGITtk){
        printAndIncrement();
    } else error("R", tk);
}

void error(string grammarLevel, Token* tk){
    cout << "PARSER ERROR: Parser has encountered error at on grammar " << grammarLevel << ", token " << tk->token << endl;
    exit(0);
}
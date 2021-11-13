#include "parser.hpp"
#include "scanner.hpp"
#include "node.hpp"

static Token* tk;
vector<Token*> tokenList;
static int tokenIndex = 0;

map<TokenType, string> identifierMap = {
    {DIGITtk,"Digit"},
    {EQUALtk,"="},
    {DEQUALtk,"=="},
    {LESSTHANtk,"<"},
    {GREATERTHANtk,">"},
    {PLUStk,"+"},
    {MINUStk,"-"},
    {MULTtk,"*"},
    {DIVtk,"/"},
    {MODtk,"%"},
    {DOTtk,"."},
    {LBRACtk,"{"},
    {RBRACtk, "}"},
    {IDtk,"Identifier"}
};

void parse(vector<Token*> tokens){
    cout << "parsing" << endl;
    tokenList = tokens;
    Node *root;

    tk = scanner(tokenList, tokenIndex);
    tokenIndex++;

    root = Program();

    tokenIndex = 0;
    printPreorder(root, 0);
}

void printAndIncrement(){
    cout << tk->token << " " << tk->tokenType << endl;
    tk = scanner(tokenList, tokenIndex);
    tokenIndex++;
}

Node* initNode(){
    Node *newNode = new Node();

    newNode->nodeType = "";

    newNode->tk1 = generateToken("", NULLtk, 0, 0);
    newNode->tk2 = generateToken("", NULLtk, 0, 0);
    newNode->tk3 = generateToken("", NULLtk, 0, 0);

    newNode->child1 = NULL;
    newNode->child2 = NULL;
    newNode->child3 = NULL;
    newNode->child4 = NULL;
    newNode->child5 = NULL;

    return newNode;
}

Node* Program(){
    Node *child = initNode();
    child->nodeType = "<PROGRAM>";

    child->child1 = Vars();
    if (tk->tokenType == PROGRAMtk){
        printAndIncrement();
       child->child2 = Block();
    }else error("Program", tk);

    if (tk->tokenType == EOFtk){
        cout << tk->token << " " << tk->tokenType << endl;
    } else error("Program", tk);

    return child;
}

Node* Block(){
    if (tk->tokenType == STARTtk){
        Node *child = initNode();
        child->nodeType = "<BLOCK>";

        printAndIncrement();

        child->child1 = Vars();
        child->child2 = Stats();

        if (tk->tokenType == STOPtk){
            printAndIncrement();
            return child;
        } else error("Block Stop", tk);
    } else error("Block Start", tk);

    return NULL;
}

Node* Vars(){
    Node *child = initNode();
    child->nodeType = "<VARS>";

    if (tk->tokenType == DECLAREtk){
        printAndIncrement();

        if (tk->tokenType == IDtk){
            child->tk1 = tk;
            cout << child->tk1->token << "<- token name" << endl;
            printAndIncrement();
            

            if (tk->tokenType == EQUALtk){
                printAndIncrement();

                if (tk->tokenType == DIGITtk) {
                    child->tk2 = tk;
                    printAndIncrement();

                    if (tk->tokenType == SEMItk){
                        printAndIncrement();

                        child->child1 = Vars();
                        return child;
                    }
                } else error("Vars", tk);
            } else error("Vars", tk);
        } else error("Vars", tk);
    }

    return child;
}

Node* Stats(){
    Node *child = initNode();
    child->nodeType = "<STATS>";

    child->child1 = Stat();
    child->child2 = Mstat();

    return child;
}

Node* Stat(){
    Node *child = initNode();
    child->nodeType = "<STAT>";

    if (tk->tokenType == LISTENtk){
        child->child1 = In();

        if (tk->tokenType == SEMItk){
            printAndIncrement();

            return child;
        } else error("In Stat", tk);
    } else if (tk->tokenType == TALKtk){
        child->child1 = Out();

        if (tk->tokenType == SEMItk){
            printAndIncrement();

            return child;
        } else error("Out Stat", tk);
    } else if (tk->tokenType == STARTtk){
        child->child1 = Block();

        return child;
    } else if (tk->tokenType == IFtk){
        child->child1 = If();

        if (tk->tokenType == SEMItk){
            printAndIncrement();

            return child;
        } else error("If Stat", tk);
    } else if (tk->tokenType == WHILEtk){
        child->child1 = Loop();

        if (tk->tokenType == SEMItk){
            printAndIncrement();

            return child;
        } else error("Loop Stat", tk);
    } else if (tk->tokenType == ASSIGNtk){
        child->child1 = Assign();

        if (tk->tokenType == SEMItk){
            printAndIncrement();

            return child;
        } else error("Assign Stat", tk);
    } else if (tk->tokenType == LABELtk){
        child->child1 = Label();

        if (tk->tokenType == SEMItk){
            printAndIncrement();

            return child;
        } else error("Label Stat", tk);
    }  else if (tk->tokenType == JUMPtk){
        child->child1 = Goto();

        if (tk->tokenType == SEMItk){
            printAndIncrement();

            return child;
        } else error("Goto Stat", tk);
    } else {
        error("Else Stat", tk);
        return NULL;
    } 

    return NULL;
}

Node* Mstat(){
    Node *child = initNode();
    child->nodeType = "<MSTAT>";

    if (tk->tokenType == LISTENtk ||
        tk->tokenType == TALKtk || 
        tk->tokenType == IFtk || 
        tk->tokenType == WHILEtk || 
        tk->tokenType == ASSIGNtk || 
        tk->tokenType == LABELtk || 
        tk->tokenType == JUMPtk ||
        tk->tokenType == STARTtk)
    {
        child->child1 = Stat();
        child->child2 = Mstat();    
    }

    return child;
}

Node* In(){
    Node *child = initNode();
    child->nodeType = "<IN>";

    printAndIncrement();

    if (tk->tokenType == IDtk){
        child->tk1 = tk;
        cout << child->tk1->token << "<- token name" << endl;
        printAndIncrement();
        return child;

    } else {
        error("In", tk);
        return NULL;
    }
}

Node* Out(){
    Node *child = initNode();
    child->nodeType = "<OUT>";

    printAndIncrement();

    child->child1 = Expression();

    return child;
}

Node* If(){
    Node *child = initNode();
    child->nodeType = "<IF>";
    printAndIncrement();

    if (tk->tokenType == LBRACKtk){
        printAndIncrement();

        child->child1 = Expression();
        child->child2 = RO();
        child->child3 = Expression();

        if (tk->tokenType == RBRACKtk){
            printAndIncrement();

            if (tk->tokenType == THENtk){
                printAndIncrement();

                child->child4 = Stat();

                if (tk->tokenType == ELSEtk){
                    printAndIncrement();

                    child->child5 = Stat();

                    return child;
                }
            } else error("If", tk);
        }else error("If", tk);
    }else {
        error("If", tk);

        return NULL;
    }

    return NULL;
}

Node* Loop(){
    Node *child = initNode();
    child->nodeType = "<LOOP>";
    printAndIncrement();

    if (tk->tokenType == LBRACKtk){
        printAndIncrement();

        child->child1 = Expression();
        child->child2 = RO();
        child->child3 = Expression();

        if (tk->tokenType == RBRACKtk){
            printAndIncrement();

            child->child4 = Stat();

            return child;
        }else error("Loop", tk);
    }else {
        error("Loop", tk);

        return NULL;
    }

    return NULL;
}

Node* Assign(){
    Node *child = initNode();
    child->nodeType = "<ASSIGN>";
    printAndIncrement();

    if (tk->tokenType == IDtk){
        child->tk1 = tk;
        printAndIncrement();

        if (tk->tokenType == EQUALtk) {
            printAndIncrement();

            child->child1 = Expression();

            return child;
        } else error("Assign", tk);
    } else {
        error("Assign", tk);

        return NULL;
    }

    return NULL;
}

Node* Goto(){
    Node *child = initNode();
    child->nodeType = "<GOTO>";
    printAndIncrement();

    if (tk->tokenType == IDtk){
        child->tk1 = tk;
        printAndIncrement();

        return child;
    } else {
        error("Goto", tk);

        return NULL;
    }
}

Node* Label(){
    Node *child = initNode();
    child->nodeType = "<LABEL>";
    printAndIncrement();

    if (tk->tokenType == IDtk){
        child->tk1 = tk;
        printAndIncrement();

        return child;
    } else {
        error("Label", tk);

        return NULL;
    }
}

Node* RO(){
    Node *child = initNode();
    child->nodeType = "<RO>";
    if (tk->tokenType == GREATERTHANtk){
        child->tk1 = tk;
        printAndIncrement();

        return child;
    } else if (tk->tokenType == LESSTHANtk){
        child->tk1 = tk;
        printAndIncrement();

        return child;
    } else if (tk->tokenType == DEQUALtk){
        child->tk1 = tk;
        printAndIncrement();

        return child;
    } else if (tk->tokenType == LBRACtk){
        child->tk1 = tk;
        printAndIncrement();

        if (tk->tokenType == DEQUALtk){
            child->tk2 = tk;
            printAndIncrement();

            if (tk->tokenType == RBRACtk){
                child->tk3 = tk;
                printAndIncrement();

                return child;
            } else error("RO", tk);
        } else error("RO", tk);
    } else if (tk->tokenType == MODtk){
        child->tk1 = tk;
        printAndIncrement();

        return child;
    } else {
        error("RO", tk);

        return NULL;
    }

    return NULL;
}

Node* Expression(){
    Node *child = initNode();
    child->nodeType = "<EXPRESSION>";

    child->child1 = N();
    if (tk->tokenType == PLUStk){
        child->tk1 = tk;
        printAndIncrement();

        child->child2 = Expression();

        return child;
    }

    return child;
}

Node* N(){
    Node *child = initNode();
    child->nodeType = "<N>";

    child->child1 = A();

    if (tk->tokenType == DIVtk){
        child->tk1 = tk;
        printAndIncrement();

        child->child2 = N();

        return child;
    } else if (tk->tokenType == MULTtk){
        child->tk1 = tk;
        printAndIncrement();

        child->child2 = N();

        return child;
    }

    return child;
}

Node* A(){
    Node *child = initNode();
    child->nodeType = "<A>";

    child->child1 = M();

    if (tk->tokenType == MINUStk){
        child->tk1 = tk;
        printAndIncrement();

        child->child2 = A();

        return child;
    }

    return child;
}

Node* M(){
    Node *child = initNode();
    child->nodeType = "<M>";

    if (tk->tokenType == DOTtk){
        child->tk1 = tk;
        printAndIncrement();

        child->child1 = M();

        return child;
    } else {
        child->child1 = R();

        return child;
    }

    return child;
}

Node* R(){
    Node *child = initNode();
    child->nodeType = "<R>";

    if (tk->tokenType == LPARtk){
        printAndIncrement();

        child->child1 = Expression();

        if (tk->tokenType == RPARtk){
            printAndIncrement();

            return child;
        } else error("R", tk);
    } else if (tk->tokenType == IDtk){
        child->tk1 = tk;
        printAndIncrement();

        return child;
    } else if (tk->tokenType == DIGITtk){
        child->tk1 = tk;
        printAndIncrement();

        return child;
    } else {
        error("R", tk);

        return NULL;
    }

    return NULL;
}

// Prints current node level to file
void printRootLevelToStdOut(Node* &root, int level){
    tokenIndex++;
    cout << string(level*2, ' ') << root->nodeType << endl;

    if (root->tk1->tokenType != NULLtk){
        cout << string(level*2, ' ') << "{Token" << tokenIndex << ": " << identifierMap[root->tk1->tokenType] << ", " << root->tk1->token << ", Line #: " << root->tk1->lineNum << "}" << endl;
    }
        
    if (root->tk2->tokenType != NULLtk){
        cout << string(level*2, ' ') << "{Token" << tokenIndex << ": " << identifierMap[root->tk2->tokenType] << ", " << root->tk2->token << ", Line #: " << root->tk2->lineNum << "}" << endl;
    }
       

    if (root->tk3->tokenType != NULLtk){
        cout << string(level*2, ' ') << "{Token" << tokenIndex << ": " << identifierMap[root->tk3->tokenType] << ", " << root->tk2->token << ", Line #: " << root->tk2->lineNum << "}" << endl;
    }

}

// root left right
void printPreorder(Node* &root, int level){
    if (root == NULL) return;

    //root
    printRootLevelToStdOut(root, level);  

    //left substree
    printPreorder(root->child1, level+1);  

    //right subtree
    printPreorder(root->child2, level+1);

    //right subtree
    printPreorder(root->child3, level+1);

    //right subtree
    printPreorder(root->child4, level+1);

    //right subtree
    printPreorder(root->child5, level+1);
}

void error(string grammarLevel, Token* tk){
    cout << "PARSER ERROR: Parser has encountered error at on grammar " << grammarLevel << ", token " << tk->token << endl;
    exit(0);
}
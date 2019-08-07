// Copyright 2019, Bradley Peterson, Weber State University, all rights reserved. (8/2019)

#include <iostream>
#include <string>
#include <stack>
#include <sstream>
#include <cmath>

//To prevent those using g++ from trying to use a library
//they don't have
#ifndef __GNUC__
#include <conio.h>
#else
#include <stdio.h>
#endif

using std::stack;
using std::istringstream;
using std::ostringstream;
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::pow;
using std::shared_ptr;
using std::make_shared;

struct Node{
    string data{};
    shared_ptr<Node> lchild;
    shared_ptr<Node> rchild;
};

class TreeParser{
    
private:
    stack<string> mathStack;
    double castStrToDouble(string const &s);
    string castDoubleToStr(const double d);
    void initialize();
    bool isDigit(char c);
    bool isOperator(char c);
    void processExpression (shared_ptr<Node> ptr);
    void computeAnswer(shared_ptr<Node> ptr);
    shared_ptr<Node> root;
    void inOrderTraversal(shared_ptr<Node> ptr);
    void postOrderTraversal(shared_ptr<Node> ptr);
    
protected:
    string expression;
    int position;
    
public:
    TreeParser();
    void displayParseTree();
    void processExpression(string &expression);
    string computeAnswer();
    void inOrderTraversal();
    void postOrderTraversal();
    
    
};

void TreeParser::initialize() {
    expression = "";
    position = 0;
    while (!mathStack.empty()) {
        mathStack.pop();
    }
}

double TreeParser::castStrToDouble(const string &s) {
    istringstream iss(s);
    double x;
    iss >> x;
    return x;
}

string TreeParser::castDoubleToStr(const double d) {
    ostringstream oss;
    oss << d;
    return oss.str();
    
}

TreeParser::TreeParser() {
    initialize();
}


void TreeParser::displayParseTree() {
    cout << "The expression seen using in-order traversal: ";
    inOrderTraversal();
    cout << endl;
    cout << "The expression seen using post-order traversal: ";
    postOrderTraversal();
    cout << endl;
    
}

//public inOrder
void TreeParser::inOrderTraversal(){
    inOrderTraversal(root);
}

//public postOrder
void TreeParser::postOrderTraversal(){
    postOrderTraversal(root);
}

//private inOrder Left Act Right
void TreeParser::inOrderTraversal(shared_ptr<Node> ptr){
    if (ptr) {
        inOrderTraversal(ptr->lchild);
        cout<< ptr->data <<" ";
        inOrderTraversal(ptr->rchild);
    }
    
}

//private postOrder Left Right Act
void TreeParser::postOrderTraversal(shared_ptr<Node> ptr){
    if (ptr) {
        postOrderTraversal(ptr->lchild);
        postOrderTraversal(ptr->rchild);
        cout<< ptr->data <<" ";
    }
    
}

//check if between 0-9
bool TreeParser::isDigit(char c){
    
    if(c >= '0' && c <= '9'){
        return true;
    }
    else{
        return false;
    }
    
}


bool TreeParser::isOperator(char c){
    if(c == '-' || c == '+' || c == '*' || c == '/' || c == '^'){
        return true;
    }
    else{
        return false;
    }
}

void TreeParser::processExpression(string &expression){
    if (expression != "") {
        this->expression = expression;
        position = 0;
        root = make_shared<Node>();
        root->data = expression;
        processExpression(root);
    }
    return;
}

void TreeParser::processExpression(shared_ptr<Node> ptr) {
    while (position < expression.length()) {
        if (expression[position] == '(') {
            shared_ptr<Node> newNode = make_shared<Node>();
            ptr->lchild = newNode;
            position++; //must be post
            processExpression(ptr->lchild);
        }
        else if (isDigit(expression[position])) {
            string holder = "";
            while (isDigit(expression[position])) {
                holder += expression[position];
                ++position; //must be pre
            }
            ptr->data = holder;
            return;
        }
        else if (isOperator(expression[position])) {
            ptr->data = expression[position];
            shared_ptr<Node> newNode = make_shared<Node>();
            ptr->rchild = newNode;
            ++position; //must be pre
            processExpression(ptr->rchild);
        }
        else if (expression[position] == ')') {
            ++position; //must be pre
            return;
        }
        else if (expression[position] == ' ') {
            ++position; //must be pre
        }
    }
}
string TreeParser::computeAnswer(){
    computeAnswer(root);
    string finalNum = mathStack.top();
    mathStack.pop();
    return finalNum;
}

void TreeParser::computeAnswer(shared_ptr<Node> ptr){
    if (ptr) {
        computeAnswer(ptr->lchild);
        computeAnswer(ptr->rchild);
        if (isDigit(ptr->data[0])) {
            mathStack.push(ptr->data);
            return;
        }
        else if (isOperator(ptr->data[0])) {
            double a = castStrToDouble(mathStack.top());
            mathStack.pop();
            double b = castStrToDouble(mathStack.top());
            mathStack.pop();
            if (ptr->data[0] == '+') {
                string answer = castDoubleToStr(b + a);
                mathStack.push(answer);
                return;
            }
            else if (ptr->data[0] == '-') {
                string answer = castDoubleToStr(b - a);
                mathStack.push(answer);
                return;
            }
            else if (ptr->data[0] == '*') {
                string answer = castDoubleToStr(b * a);
                mathStack.push(answer);
                return;
            }
            else if (ptr->data[0] == '/') {
                string answer = castDoubleToStr(b / a);
                mathStack.push(answer);
                return;
            }
            else if (ptr->data[0] == '^') {
                string answer = castDoubleToStr(pow(b, a));
                mathStack.push(answer);
                return;
            }
        }
    }
}


void pressAnyKeyToContinue() {
    printf("Press any key to continue\n");
    
    //Linux and Mac users with g++ don't need this
    //But everyone else will see this message.
#ifndef __GNUC__
    _getch();
#else
    int c;
    fflush(stdout);
    do c = getchar(); while ((c != '\n') && (c != EOF));
#endif
    
}

// Copyright 2019, Bradley Peterson, Weber State University, all rights reserved. (4/2019)

int main() {
    
    TreeParser *tp = new TreeParser;
    
    string expression = "(4+7)";
    tp->processExpression(expression);
    tp->displayParseTree();
    cout << "The result is: " << tp->computeAnswer() << endl; //Should display 11 as a double output
    
    expression = "(7-4)";
    tp->processExpression(expression);
    tp->displayParseTree();
    cout << "The result is: " << tp->computeAnswer() << endl; //Should display 3 as a double output
    
    expression = "(9*5)";
    tp->processExpression(expression);
    tp->displayParseTree();
    cout << "The result is: " << tp->computeAnswer() << endl; //Should display 45 as a double output
    
    expression = "(4^3)";
    tp->processExpression(expression);
    tp->displayParseTree();
    cout << "The result is: " << tp->computeAnswer() << endl; //Should display 64 as a double output
    
    expression = "((2-5)-5)";
    tp->processExpression(expression);
    tp->displayParseTree();
    cout << "The result is: " << tp->computeAnswer() << endl; //Should display -8 as a double output
    
    expression = "(5*(6/2))";
    tp->processExpression(expression);
    tp->displayParseTree();
    cout << "The result is: " << tp->computeAnswer() << endl; //Should display 15 as a double output
    
    expression = "((1 + 2) * (3 + 4))";
    tp->processExpression(expression);
    tp->displayParseTree();
    cout << "The result is: " << tp->computeAnswer() << endl; //Should display 21 as a double output
    
    expression = "(543+321)";
    tp->processExpression(expression);
    tp->displayParseTree();
    cout << "The result is: " << tp->computeAnswer() << endl; //Should display 864 as a double output
    
    expression = "((5*(3+2))+(7*(4+6)))";
    tp->processExpression(expression);
    tp->displayParseTree();
    cout << "The result is: " << tp->computeAnswer() << endl; //Should display 95 as a double output
    
    expression = "(((((3+12)-7)*120)/(2+3))^3)";
    tp->processExpression(expression);
    tp->displayParseTree();
    cout << "The result is: " << tp->computeAnswer() << endl; //Should display close to 7077888 as a double output
    //NOTE, it won't be exact, it will display as scientific notation!
    
    expression = "(((((9+(2*(110-(30/2))))*8)+1000)/2)+(((3^4)+1)/2))";
    tp->processExpression(expression);
    tp->displayParseTree();
    cout << "The result is: " << tp->computeAnswer() << endl; //Should display close to 1337 as a double/decimal output
    
    pressAnyKeyToContinue();
    return 0;
}

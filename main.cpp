//Kenneth Stearns
//Project 10
//Due 11/29/21
//This program will take an input file of in-fix expressions, store that into a expression tree
//and output the expression in pre, in, and post-fix form as well as the value.
#include<iostream>
#include<string>
#include<fstream>
#include<stack>

using namespace std;

//isOperator function
//pre: a character c
//post: a boolean true or false depending on if the character is an operator or not.
bool isOperator(char c)
{
    return c == '-' || c == '+' || c == '*' || c == '/';
}

//isOperand function
//pre: a character c
//post: a boolean true or false depending on if the character is an operand or not.
bool isOperand(char c)
{
    return c >= '0' && c <= '9';
}

//node definition for tree
struct node
{
    char data;
    node *left, *right;
};

class ExpressionTree
{
private:

    //recursive private pre function to generate pre-fix expression.
    void pre(node *t, ostream &out_f)
    {
        if(t != NULL)
        {
            out_f << t -> data << " ";
            pre(t -> left, out_f);
            pre(t -> right, out_f);
        }
    }

    //recursive private in function to generate in-fix expression.
    void in(node *t, ostream &out_f)
    {
        if(t != NULL)
        {
            if(isOperator(t -> data))
                out_f << "( ";
            in(t -> left, out_f);
            out_f << t -> data << " ";
            in(t -> right, out_f);
            if(isOperator(t -> data))
                out_f << ") ";
        }
    }

    //recursive private post function to generate post-fix expression.
    void post(node *t, ostream &out_f)
    {
        if(t != NULL)
        {
            post(t -> left, out_f);
            post(t -> right, out_f);
            out_f << t-> data << " ";
        }
    }

public:
    node *top;

    //constructor
    ExpressionTree(){top = NULL;}

    //destructor
    ~ExpressionTree(){top = NULL;}

    //re-initializer
    void reinitialize(ExpressionTree &t){top = NULL;}
    void build_tree(string& in_f);
    void pre_order(ostream &out_f){pre(top, out_f); return;}
    void in_order(ostream &out_f){in(top, out_f); return;}
    void post_order(ostream &out_f){post(top, out_f); return;}
    int value(node *t);
};

//precedence function
//pre: a character c
//post: a value of 0, 1, or 2 depending on what operator c is.
int precedence(char c)
{
    if(c == '*' || c == '/')
        return 2;
    else if(c == '+' || c == '-')
        return 1;
    else
        return 0;
}

//build_tree function.
//pre: a string in_f
//post: a expression tree built in in-fix notation.
void ExpressionTree::build_tree(string &in_f)
{
    stack<node*> sN;

    stack<char> sC;

    node *t, *u, *v;

    for(int i = 0; i < in_f.length(); i++)
    {
        if(in_f[i] == '(')
        {
            sC.push(in_f[i]);
        }

        else if(isOperand(in_f[i]))
        {
            t = new node;
            t -> data = in_f[i];
            t -> left = t -> right = NULL;
            sN.push(t);
        }

        else if(isOperator(in_f[i]))
        {
            while(!sC.empty() && sC.top() != '(' && precedence(sC.top()) >= precedence(in_f[i]))
            {
                t = new node;
                t -> data = in_f[i];

                u = sN.top();
                sN.pop();

                v = sN.top();
                sN.pop();

                t -> left = v;
                t -> right = u;

                sN.push(t);
            }

            sC.push(in_f[i]);

        }

        else if(in_f[i] == ')')
        {

            while(!sC.empty() && sC.top() != '(')
            {
                t = new node;
                t -> data = sC.top();
                sC.pop();

                u = sN.top();
                sN.pop();

                v = sN.top();
                sN.pop();

                t -> left = v;
                t -> right = u;

                sN.push(t);
            }

            sC.pop();
        }
    }

    top = sN.top();
    return;
}

//calculate function
//pre: a character c and two integers l and r.
//post: returns a value based on the operator c being applied to operands l and r.
int calculate(char c, int l, int r)
{
    int total;

    switch (c)
    {
    case '*':
        {
            total = l * r;
            break;
        }
    case '+':
        {
            total = l + r;
            break;
        }
    case '-':
        {
            total = l - r;
            break;
        }
    case '/':
        {
            total = l / r;
            break;
        }
    }

    return total;
}

//toInteger function.
//pre: a character operand c.
//post: returns the character back as an integer operand.
int toInteger(char c)
{
    int temp = c - '0';
    return temp;
}

//value function.
//pre: a pointer to a node t
//post: the complete value of the tree.
int ExpressionTree::value(node *t)
{
    if(isOperand(t -> data))
        return toInteger(t -> data);

    else if(isOperator(t -> data))
    {
        return calculate(t -> data, value(t -> left), value(t -> right));
    }
}

int main()
{
    string exp, input;
    cout << "Enter the input file name: ";
    cin >> input;
    ExpressionTree et;
    ifstream myfile;
    myfile.open(input);
    if(!myfile.is_open())
        cout << "Error opening file. ";

    else
    {
        while(getline(myfile, exp))
        {
            et.build_tree(exp);
            cout << "Preorder:   ";
            et.pre_order(cout);
            cout << endl << "Inorder:    ";
            et.in_order(cout);
            cout << endl << "Postorder:  ";
            et.post_order(cout);
            cout << endl << "Value = " << et.value(et.top) << endl << endl;
        }
    }
    return 0;
}

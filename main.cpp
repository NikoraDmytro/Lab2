#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <stack>
#include <algorithm>

using namespace std;

map<string, bool> variables;
map<char, int> operators = {{'~', 4}, {'^', 3}, {'*', 2}, {'+', 1}};

pair<bool, bool> takeTwo(stack<bool> &result)
{
    int x = result.top();
    result.pop();
    int y = result.top();
    result.pop();

    return make_pair(x, y);
}

bool takeOne(stack<bool> &result)
{
    int x = result.top();
    result.pop();

    return x;
}

string readVariable(string s, int &index)
{
    string result = "";
    while (s[index] != ' ' && index != s.length())
    {
        result += s[index];
        index++;
    }
    index--;
    return result;
}

void proccessUserInput(string input)
{
    int value = -1;
    string variable = "";
    string::iterator index = input.begin();

    while (*index != '=')
    {
        if (index == input.end())
        {
            cout << "Assignment expected!" << endl;
            return;
        }
        if (*index != ' ')
        {
            variable += *index;
        }
        index++;
    }
    //Skipping all the whitespaces after the assignment
    while (*index == ' ' || *index == '=')
    {
        index++;
    }
    if (variable.empty() || index == input.end())
    {
        cout << "Variable or value is missing!" << endl;
        return;
    }
    //There must be only one character left and it must be 0 or 1
    if ((*index != '0' && *index != '1') || index != input.end() - 1)
    {
        cout << "Expected 0 or 1 as argument!" << endl;
        return;
    }
    value = *index - '0';

    variables[variable] = (bool)value;
}

int doTheOperations(string polishNotation)
{
    stack<bool> result;

    for (int i = 0; i < polishNotation.length(); i++)
    {
        char element = polishNotation[i];

        if (element == '~')
        {
            result.push(!takeOne(result));
        }
        else if (element == '^')
        {
            pair<bool, bool> args = takeTwo(result);
            result.push(args.first ^ args.second);
        }
        else if (element == '*')
        {
            pair<bool, bool> args = takeTwo(result);
            result.push(args.first & args.second);
        }
        else if (element == '+')
        {
            pair<bool, bool> args = takeTwo(result);
            result.push(args.first | args.second);
        }
        else
        {
            result.push(element - '0');
        }
    }

    return result.top();
}

string toPolishNotation(string formula)
{
    string result;
    stack<char> operatorsStack;

    for (int i = 0; i < formula.length(); i++)
    {
        char element = formula[i];
        if (operators.find(element) != operators.end())
        {
            while (!operatorsStack.empty())
            {
                char last = operatorsStack.top();

                if (last == '(' || operators[element] > operators[last])
                {
                    break;
                }

                result += last;
                operatorsStack.pop();
            }

            operatorsStack.push(element);
        }
        else if (element == '(')
        {
            operatorsStack.push(element);
        }
        else if (element == ')')
        {
            while (!operatorsStack.empty())
            {
                char last = operatorsStack.top();
                operatorsStack.pop();

                if (last == '(')
                {
                    break;
                }

                result += last;
            }
        }
        else if (element != ' ')
        {
            string variable = readVariable(formula, i);
            if (variables.find(variable) == variables.end())
            {
                cout << "Unknown variable: " << variable << endl;
                return "error";
            }
            result += variables[variable] ? '1' : '0';
        }
    }

    while (!operatorsStack.empty())
    {
        result += operatorsStack.top();
        operatorsStack.pop();
    }

    return result;
}

int main()
{
    string userInput;
    cout << "Specify as many variables as you like.To move to the next step type 'exit','quit' or empty line." << endl;
    do
    {
        getline(cin, userInput);
        proccessUserInput(userInput);
    } while (!(userInput.empty() || userInput == "exit" || userInput == "quit"));

    cout << "Enter the formula:\n  ~ - negation\n  ^ - exclusive OR\n  + - disjunction\n  * - conjunction\n  () - brackets" << endl;

    string formula;

    getline(cin, formula);

    string polishNotation = toPolishNotation(formula);

    if (polishNotation == "error")
    {
        return 0;
    }

    cout << "\n<-----Result----->" << endl;

    int result = doTheOperations(polishNotation);

    cout << result;

    return 0;
}

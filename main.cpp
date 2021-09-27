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

bool isLatinLetter(char c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool isNumber(string number)
{
    int index = 0;
    if (number[0] == '-')
    {
        index++;
    }
    while (index != number.length())
    {
        int num = number[index] - '0';
        if (num > 9 || num < 0)
        {
            return false;
        }
        index++;
    }
    return true;
}

string readVariable(string s, int &index)
{
    string result = "";
    while (true)
    {
        char element = s[index];
        if (isLatinLetter(element))
        {
            result += element;
            index++;
        }
        else
        {
            return result;
        }
    }
}

void proccessUserInput(string input)
{
    int index = 0;
    string value = "";
    string variable = readVariable(input, index);
    bool afterAssignment = false;

    while (index != input.length())
    {
        char element = input[index];
        if (element == ' ')
        {
            index++;
            continue;
        }
        if (afterAssignment)
        {
            value += element;
        }
        else if (element == '=')
        {
            afterAssignment = true;
        }
        else
        {
            cout << element << "it" << endl;
            cout << "Assignment sign expected after variable!" << endl;
            return;
        }
        index++;
    }

    if (variable.empty() || value.empty())
    {
        cout << "Valid expression expected!" << endl;
        return;
    }
    if (!isNumber(value))
    {
        cout << "Valid integer number expected after assignment!" << endl;
        return;
    }

    variables[variable] = (bool)stoi(value);
}

int doTheOperations(string polishNotation)
{
    stack<bool> result;

    for (int i = 0; i < polishNotation.length(); i++)
    {
        char element = polishNotation[i];

        if (element == '~')
        {
            int x = result.top();
            result.pop();
            result.push(!x);
        }
        else if (element == '^')
        {
            int x = result.top();
            result.pop();
            int y = result.top();
            result.pop();
            result.push(x ^ y);
        }
        else if (element == '*')
        {
            int x = result.top();
            result.pop();
            int y = result.top();
            result.pop();
            result.push(x & y);
        }
        else if (element == '+')
        {
            int x = result.top();
            result.pop();
            int y = result.top();
            result.pop();
            result.push(x | y);
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
        if (element == ' ')
        {
            continue;
        }
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
        else
        {
            string variable = readVariable(formula, i);
            i--;
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
    getline(cin, userInput);
    while (!(userInput.empty() || userInput == "exit" || userInput == "quit"))
    {
        proccessUserInput(userInput);
        getline(cin, userInput);
    }

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

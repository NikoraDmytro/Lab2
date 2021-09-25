#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <stack>
#include <algorithm>

using namespace std;

map<char, int> variables;
map<char, int> operators = {{'~', 3}, {'*', 2}, {'+', 1}};

string normalize(string str)
{
    string normalizedStirng = "";

    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] != ' ')
        {
            normalizedStirng += str[i];
        }
    }

    return normalizedStirng;
}

int doTheOperations(string polishNotation)
{
    stack<int> result;

    for (int i = 0; i < polishNotation.length(); i++)
    {
        char element = polishNotation[i];

        if (element == '~')
        {
            int x = result.top();
            result.pop();

            result.push((x + 1) % 2);
        }
        else if (element == '*')
        {
            int x = result.top();
            result.pop();
            int y = result.top();
            result.pop();

            result.push(min(x, y));
        }
        else if (element == '+')
        {
            int x = result.top();
            result.pop();
            int y = result.top();
            result.pop();

            result.push(max(x, y));
        }
        else
        {
            int value = variables[element];

            result.push(value);
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
        if (element == '(')
        {
            operatorsStack.push(element);
        }
        if (element == ')')
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
        if (variables.find(element) != variables.end())
        {
            result += element;
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

    cout << "Specify as many variables as you like.Type empty string to exit." << endl;
    while (true)
    {
        string userInput;
        getline(cin, userInput);
        userInput = normalize(userInput);

        if (userInput == "")
        {
            break;
        }

        char var = userInput[0];
        int value = userInput[2] - '0';

        variables[var] = value;
    }

    cout << "Enter the formula:\n  ~ - negation\n  + - disjunction\n  * - conjunction\n  () - brackets" << endl;

    string formula;

    getline(cin, formula);
    formula = normalize(formula);

    string polishNotation = toPolishNotation(formula);

    cout << "\n<-----Result----->" << endl;

    int result = doTheOperations(polishNotation);

    cout << result;

    return 0;
}

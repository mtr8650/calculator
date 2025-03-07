#include <bits/stdc++.h>
#include <stack>
#include <cmath>
#include <cctype>
#include <stdexcept>
#include <sstream>
#include <regex>
#include <fstream>
#define M_PI 3.14159265358979323846
using namespace std;
int precedence(char op)
{
    if (op == '+' or op == '-')
        return 1;
    if (op == '*' or op == '/')
        return 2;
    if (op == 'l')
        return 3;
    if (op == '^')
        return 4; // Exponentiation
    return 0;
}
bool isOperator(char c)
{
    return c == '+' or c == '-' or c == '*' or c == '/' or c == '^' or c == 'l';
}
/*

int operatorequaloperand (string str){
    bool onGoingNum = false ;
    int operatorCount =0 , operandCount =0;
    for (int i = 0; i < str.length(); ++i) {
        if (isdigit(str[i]) or str[i] == '.'){
            if(!onGoingNum){
                operandCount++;
                onGoingNum = true;
            }
        }else if (isOperator(str[i])){
                operatorCount++;
                onGoingNum = false ;
        }else {
            onGoingNum = false;
        }
    }
    if(operatorCount > operandCount)
        return 2 ;
    else if (operatorCount == operandCount)
        return 1 ;
    else
        return 0 ;
}
*/

bool isRightAssociative(char op)
{
    return op == '^' || op == 'l'; // Exponentiation is right-associative
}

std::string infixToPostfix(const std::string &infix)
{
    std::stack<char> operators;
    std::string postfix;
    std::string number; // To build multi-digit numbers (including negative numbers)

    for (size_t i = 0; i < infix.length(); i++)
    {
        char token = infix[i];

        if (isdigit(token) or token == '.')
            // Build the number (handle multi-digit and decimal numbers)
            number += token;

        else if (token == '-' and (i == 0 or infix[i - 1] == '(' or isOperator(infix[i - 1])))
            // Handle negative numbers (e.g., -2 or (-2))
            number += token;
        else
        {
            // When we encounter an operator or parenthesis, push the number we've built
            if (!number.empty())
            {
                postfix += number + " ";
                number.clear();
            }

            if (token == '(')
                operators.push(token);
            else if (token == ')')
            {
                while (!operators.empty() and operators.top() != '(')
                {
                    postfix += operators.top();
                    postfix += " ";
                    operators.pop();
                }
                if (!operators.empty())
                    operators.pop(); // Discard '('
            }
            else if (isOperator(token))
            {
                // Handle operators
                while (!operators.empty() and precedence(operators.top()) >= precedence(token) and
                       (!isRightAssociative(token) or precedence(operators.top()) > precedence(token)))
                {
                    postfix += operators.top();
                    postfix += " ";
                    operators.pop();
                }
                operators.push(token);
            }
            else if (token != 's' and token != 'i' and token != 'n' and token != 'c' and token != 'o' and token != 't' and token != 'a')
                throw std::invalid_argument("Invalid character in expression");
        }
    }

    // If there's a number left at the end, add it to the postfix expression
    if (!number.empty())
        postfix += number + " ";

    // Pop any remaining operators to the output
    while (!operators.empty())
    {
        postfix += operators.top();
        postfix += " ";
        operators.pop();
    }

    return postfix;
}

bool isNumber(const std::string &str)
{
    // Regular expression to match a number (integer or floating point, positive or negative)
    std::regex pattern("^[+-]?([0-9]*[.])?[0-9]+$");
    return std::regex_match(str, pattern);
}

// Function to evaluate the postfix expression using stringstream for numbers
long double evaluatePostfix(const std::string &postfix)
{
    std::stack<long double> values;
    std::istringstream ss(postfix);
    std::string token;

    // Process each token in the postfix expression
    while (ss >> token)
    {
        if (isNumber(token))

            // If the token is a number, convert it to double and push it onto the stack
            values.push(std::stod(token));

        else if (token == "l")
        {
            // If the token is the "log" function, apply the operation (base 10)
            long double b = values.top();
            values.pop();
            values.push(log10(b));  // Use log10 for base-10 logarithm
        }
        else if (isOperator(token[0]))
        {
            // If the token is an operator, apply the operation
            long double b = values.top();
            values.pop();
            long double a = values.top();
            values.pop();

            switch (token[0])
            {
                case '+':
                    values.push(a + b);
                    break;
                case '-':
                    values.push(a - b);
                    break;
                case '*':
                    values.push(a * b);
                    break;
                case '/':
                    values.push(a / b);
                    break;
                case '^':
                    values.push(pow(a, b));
                    break;
                default:
                    throw std::invalid_argument("Invalid operator in postfix expression");
            }
        }
        else {
            cout << token << endl ;
            throw std::invalid_argument("Invalid token in postfix expression");
        }
    }

    return values.top();
}

bool OperandEqualOperatorAtTheEnd(string str)
{
    for (int i = str.length(); i >= 0; --i)
    {
        if (isOperator(str[i]))
            throw invalid_argument("operators at the end of the prompt!!!");
        else if (isdigit(str[i]))
            return true;
    }
}

string removeSpaces(string str)
{
    string newStr = "";
    for (int i = 0; i < str.length(); ++i)
        if (str[i] != ' ')
            newStr += str[i];
    return newStr;
}

std::string simplifyOperators(const std::string &input)
{
    std::string result;
    bool lastWasOperator = false;
    char lastOperator = '+';

    for (size_t i = 0; i < input.size(); ++i)
    {
        char currentChar = input[i];

        // If the current character is a number or an operator other than '*' or '/', add it to the result
        if (isdigit(currentChar) or currentChar == '*' or currentChar == '/' or currentChar == '^' or currentChar == 'd' or
            currentChar == '(' or currentChar == ')' or currentChar == 's' or currentChar == '.' or
            currentChar == 'i' or currentChar == 'n' or currentChar == 'c' or currentChar == 'r' or
            currentChar == 'o' or currentChar == 't' or currentChar == 'a' or currentChar == 'l')
        {
            if (lastWasOperator and (lastOperator == '-' or lastOperator == '+'))
            {
                if (lastOperator == '-')
                    result += '-';
                else
                    result += '+';

                lastWasOperator = false;
            }
            result += currentChar;
        }
            // If the current character is a '+' or '-', handle it
        else if (currentChar == '+' or currentChar == '-')
        {
            if (lastWasOperator)
            {
                // Simplify consecutive operators (+ or -)
                if (currentChar == lastOperator)
                    lastOperator = '+'; // Two same operators like "++" or "--" result in "+"

                else
                    lastOperator = '-'; // Different operators like "+-" result in "-"
            }
            else
                lastOperator = currentChar;
            lastWasOperator = true;
        }
    }
    return result;
}

string Sinus(string &str, int index)
{
    if (index + 6 >= str.length())
        throw invalid_argument("didn't enter Sin() correctly!!!");
    if (str[index + 1] != 'i' or str[index + 2] != 'n')
        throw invalid_argument("didn't enter Sin() correctly!!!");

    int parCount = 1;
    int j = index + 4;
    while (parCount)
    {
        if (str[j] == ')')
            parCount--;
        else if (str[j] == '(')
            parCount++;
        if (j > str.length())
            throw invalid_argument("Closing parenthesis missing in Rad()!");
        j++;
    }
    int strIndex = j - 1;    string infix = str.substr(index + 4, strIndex - index - 4);
    string postfix = infixToPostfix(infix);
    long double result = evaluatePostfix(postfix);
    result = sin(M_PI * result / 180);
    str.replace(index, strIndex - index + 1, to_string(result));

    return str;
}

string Cosinus(string &str, int index)
{
    if (index + 6 >= str.length())
        throw invalid_argument("didn't enter Cos() correctly!!!");
    if (str[index + 1] != 'o' or str[index + 2] != 's')
        throw invalid_argument("didn't enter Cos() correctly!!!");

    int parCount = 1;
    int j = index + 4;
    while (parCount)
    {
        if (str[j] == ')')
            parCount--;
        else if (str[j] == '(')
            parCount++;
        if (j > str.length())
            throw invalid_argument("Closing parenthesis missing in Rad()!");
        j++;
    }
    int strIndex = j - 1;    string infix = str.substr(index + 4, strIndex - index - 4);
    string postfix = infixToPostfix(infix);
    long double result = evaluatePostfix(postfix);
    result = cos(M_PI * result / 180);
    str.replace(index, strIndex - index + 1, to_string(result));

    return str;
}

string Tanjant(string &str, int index)
{

    if (index + 6 >= str.length())
        throw invalid_argument("didn't enter Tan() correctly!!!");
    if (str[index + 1] != 'a' or str[index + 2] != 'n')
        throw invalid_argument("didn't enter Tan() correctly!!!");

    int parCount = 1;
    int j = index + 4;
    while (parCount)
    {
        if (str[j] == ')')
            parCount--;
        else if (str[j] == '(')
            parCount++;
        if (j > str.length())
            throw invalid_argument("Closing parenthesis missing in Rad()!");
        j++;
    }
    int strIndex = j - 1;    string infix = str.substr(index + 4, strIndex - index - 4);
    string postfix = infixToPostfix(infix);
    long double result = evaluatePostfix(postfix);
    result = tan(M_PI * result / 180);
    str.replace(index, strIndex - index + 1, to_string(result));

    return str;
}

string Trigonometry(string str)
{
    vector<pair<int, char>> indices;
    for (int i = 0; i < str.length(); ++i)
    {
        if (str[i] == 's')
        {
            indices.push_back({i, 's'});
            i += 3;
        }
        else if (str[i] == 'c')
        {
            indices.push_back({i, 'c'});
            i += 3;
        }
        else if (str[i] == 't')
        {
            indices.push_back({i, 't'});
            i += 3;
        }
    }

    if (!indices.empty())
    {
        reverse(indices.begin(), indices.end());

        for (int i = 0; i < str.size(); ++i)
        {
            if (indices[i].second == 's')
                Sinus(str, indices[i].first);
            else if (indices[i].second == 'c')
                Cosinus(str, indices[i].first);
            else if (indices[i].second == 't')
                Tanjant(str, indices[i].first);
        }
    }
    return str;
}

string Radical(string &str)
{
    vector<int> indices;
    for (int i = 0; i < str.length(); ++i)
        if (str[i] == 'r')
            indices.push_back(i);

    reverse(indices.begin(), indices.end());

    for (int index : indices)
    {
        if (index + 6 > str.length())
            throw invalid_argument("Didn't enter Rad() correctly!");

        if (str[index + 1] != 'a' or str[index + 2] != 'd')
            throw invalid_argument("Didn't enter Rad() correctly!");

        // Find the closing parenthesis for Rad()
        int parCount = 1;
        int j = index + 4;
        while (parCount)
        {
            if (str[j] == ')')
                parCount--;
            else if (str[j] == '(')
                parCount++;
            if (j > str.length())
                throw invalid_argument("Closing parenthesis missing in Rad()!");
            j++;
        }
        int strIndex = j - 1;

        // Extract the part inside the parentheses
        string ebarat = str.substr(index + 4, strIndex - index - 4);
        string sforge = "2"; // Default exponent for square root

        int i = index;
        // Check for the optional number before Rad()
        if (i > 0 and (isdigit(str[i - 1]) or str[i - 1] == '.'))
        {
            sforge = "";
            while (i >= 1 and (isdigit(str[i - 1]) or str[i - 1] == '.'))
            {
                sforge = str[i - 1] + sforge;
                i--;
            }
        }

        // Create the new expression in exponential form
        sforge = to_string(1 / stof(sforge));
        string nstr = "(" + ebarat + ")" + "^" + "(" + sforge + ")";
        cout << ebarat << endl;
        str.replace(index, strIndex - index + 1, nstr);
        cout << str << endl;
    }

    return str;
}



    bool isFileEmpty(const string &fileName)
{
    ifstream file(fileName, ios::binary | ios::ate);
    if (!file.is_open())
    {
        cerr << "Cannot open file: " << fileName << endl;
        return false;
    }
    return file.tellg() == 0;
}


void replaceLogWithL(std::string& str) {
    size_t pos = 0;
    bool replaced = false;  // Flag to check if any replacement is made
    while ((pos = str.find("log", pos)) != std::string::npos) {
        str.replace(pos, 3, "l");
        pos += 1;  // Move past the newly replaced "l"
        replaced = true;  // Set flag when a replacement happens
    }

}


void parenthesese (string str){
    int fpar = 0 , spar = 0 ;
    for (int i = 0; i < str.length(); ++i) {
        if(str[i] == '(')
            fpar++;
        if(str[i] == ')')
            spar++;

    }
    if(fpar != spar){
        throw invalid_argument("you didn't enter the amount of parenthesese correctly !!!");
    }
}



int main()
{
    fstream myFile;
    string infix;
    string lastCal ;
    bool con = false ;


    while (true)
    {
        if(lastCal != ""){
            cout << "do you want to continue your last calculation or the row you have selected from history? if yes please enter -y- and if no please enter -n- ." << endl ;
            string check ;
            getline(cin , check);
            if(check == "y")
                con = true ;
            else if (check == "n")
                con = false ;
            else {
                cout << "please enter correctly !" << endl ;
                continue ;
            }
        }
        else
            con = false ;
        cout << "Enter an infix expression: ";
        getline(cin, infix);
        if(con) infix = lastCal + infix ;
        std::transform(infix.begin(), infix.end(), infix.begin(), [](unsigned char c) {
            return std::tolower(c);
        });
        lastCal = "";
        // if input is h or H:
        if (infix == "h" )
        {

            system("cls");
            myFile.open("History.txt", ios::in);
            if (myFile.is_open())
            {
                string line;
                int i = 1;
                while (getline(myFile, line))
                {
                    if(i % 2 == 1)
                        cout << "\033[1;30m" << i/2+1 << ": " << line << "\033[0m\n";
                    else
                        cout << "\033[1;30m" << line << "\033[0m\n";
                    i++;
                }
                myFile.close();
            }
            if (isFileEmpty("History.txt"))
                cout << "Semnan\n";

             else
             {
                 string rowNumS ;
                 int rowNum;
                 cout << "\nEnter row number: ";
                 getline(cin , rowNumS);
                 rowNum = stoi(rowNumS);
                 int currentRow = 0;
                 string readLine;
                 myFile.open("History.txt", ios::in);
                 while (!myFile.eof())
                 {
                     currentRow++;
                     getline(myFile, readLine);
                     if (currentRow == rowNum*2 -1 )
                         break;
                 }
                 if (currentRow < rowNum)
                     cout << "This became Semnan!\n";
                 else
                 {
                     cout << "Now you can access this part of history: ";
                     cout << "\033[30m" << readLine << "\033[0m\n";
                     lastCal = readLine ;
                 }
                 myFile.close();
             }
        }
            // if input is c or C:
        else if (infix == "c" )
        {
            myFile.open("History.txt", ios::out);
            cout << "History transformed to Semnan!\n";
            myFile.close();
        }

            // if input is off or OFF or Off:
        else if (infix == "off" )
            break;

            // if input is mathematics:
        else
        {
            myFile.open("History.txt", ios::app);
            if (myFile.is_open())
            {
                myFile << infix << " ";
                myFile.close();
            }
            else
                cout << "Can't add infix!";
            infix = removeSpaces(infix);
            lastCal = "";
            try
            {
                /*
                        int a = operatorequaloperand(infix);
                */
                /*cout << a << endl;
                if (a){*/
                parenthesese(infix);
                infix = simplifyOperators(infix);
                Radical(infix);
                infix = Trigonometry(infix);
                OperandEqualOperatorAtTheEnd(infix);
                string postfix = infixToPostfix(infix);
                cout << "\033[92mPostfix expression: " << postfix << "\033[0m\n";

                double result = evaluatePostfix(postfix);
                lastCal = to_string(result);
                cout << "\033[1;36mResult: " << result << "\033[0m\n";
                myFile.open("History.txt", ios::app);
                if (myFile.is_open())
                {
                    myFile << endl <<"= " << result << endl;
                    myFile.close();
                }
                else
                    cout << "Can't add result!";
                /*}else {
                    string postfix = infixToPostfix(infix);
                    cout << "Postfix expression: " << postfix << endl;

                    double result = evaluatePostfix(postfix);
                    cout << "Result: " << result << endl;
                }*/
            }
            catch (const exception &e)
            {
                cerr << "Error: " << e.what() << endl;
            }
        }
    }
    return 0;
}





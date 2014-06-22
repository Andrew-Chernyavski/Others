#include <cstdlib>
#include <iostream>
#include <string>
#include <stack> 

using namespace std;

int solve (int a2, int a1, char c)
{
    switch (c) {
           case '+': return a1 + a2;
           case '-': return a1 - a2;
           case '*': return a1 * a2;
           case '/': return a1 / a2;
           }   
}

int main(int argc, char *argv[])
{
    stack<char> operations;
    stack<int> numbers;
    char str[1024];
    do
    {
    cout << "Input your expression (without spaces) or close window to exit:\n";
    cin >> str;
    cout << "Your expression is " << str << "\n";
    int i = 0;
    double f = false;
    int num = 0;
    do
    {
       if ((str[i] >= '0') and (str[i] <= '9'))
       {
          num *= 10;
          num += (str[i] - 48);
          i++;
       }
       else
       {
           if ((num) > 0)
           {
              numbers.push(num);
              num = 0;
           }
       switch (str[i])
       {
           case '(':
              operations.push(str[i]);
              i++;
              break;
           case '-':
           case '+':
                if ((operations.empty()) or (operations.top() == '('))
                {
                   operations.push(str[i]);
                   i++;
                }
                else if ((operations.top() == '+') or (operations.top() == '-'))
                {
                     int a1 = numbers.top(); numbers.pop();
                     int a2 = numbers.top(); numbers.pop();
                     char s = operations.top(); operations.pop();
                     numbers.push(solve(a1,a2,s));
                     operations.push(str[i]);
                     i++;
                }
                else if ((operations.top() == '*') or (operations.top() == '/'))
                {
                    int a1 = numbers.top(); numbers.pop();
                    int a2 = numbers.top(); numbers.pop();
                    char s = operations.top(); operations.pop();
                    numbers.push(solve(a1,a2,s));      
                }
                else
                {
                   cout << "Error - wrong data.\n";
                   f = true;   
                }
                break;
           case '*':
           case '/':
                if ((operations.empty()) or (operations.top() == '(') or (operations.top() == '+') or (operations.top() == '-'))
                {
                   operations.push(str[i]);
                   i++;
                }
                else if ((operations.top() == '*') or (operations.top() == '/'))
                {
                    int a1 = numbers.top(); numbers.pop();
                    int a2 = numbers.top(); numbers.pop();
                    char s = operations.top(); operations.pop();
                    numbers.push(solve(a1,a2,s));
                    operations.push(str[i]);
                    i++;    
                }
                else
                {
                   cout << "Error - wrong data.\n";
                   f = true;   
                }
                break;
           case ')':
                if (operations.empty())
                {
                   cout << "Error - wrong data.\n";
                   f = true;   
                }
                else if (operations.top() == '(')
                {
                   operations.pop();
                   i++;   
                }
                else if ((operations.top() == '+') or (operations.top() == '-') or (operations.top() == '*') or (operations.top() == '/'))
                {
                   int a1 = numbers.top(); numbers.pop();
                   int a2 = numbers.top(); numbers.pop();
                   char s = operations.top(); operations.pop();
                   numbers.push(solve(a1,a2,s));     
                }
                else
                {
                   cout << "Error - wrong data.\n";
                   f = true;   
                }
                break;
           case '\0':
                if (operations.empty())
                {
                   cout << "Result is " << numbers.top() << "\n";
                   f = true; 
                }
                else if (operations.top() == '(')
                {
                   cout << "Error - wrong data.\n";
                   f = true;
                }
                else if ((operations.top() == '+') or (operations.top() == '-') or (operations.top() == '*') or (operations.top() == '/'))
                {
                   int a1 = numbers.top(); numbers.pop();
                   int a2 = numbers.top(); numbers.pop();
                   char s = operations.top(); operations.pop();
                   numbers.push(solve(a1,a2,s));     
                }
                else
                {
                   cout << "Error - wrong data.\n";
                   f = true;   
                }
                break;
           default:
              cout << "Error - wrong data.\n";
              f = true;
              break;
       }  
       }
    }
    while (!f);
   }
   while (true);
    system("PAUSE");
    return EXIT_SUCCESS;
}

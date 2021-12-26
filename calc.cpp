#include <iostream>
#include <sstream>
#include <concepts>
#include <type_traits>
#include <math.h>
#include <string>
#include <iomanip>
#include <memory>
using namespace std;

template <typename T>
requires requires { is_arithmetic<T>::value; }
class Calculator
{
public:
    Calculator(){};
    double Calculate(stringstream &ss)
    {
        try
        {
            ParseStream(ss);
        }
        catch (std::invalid_argument ex)
        {
            cout << ex.what() << endl;
            first = 0;
            second = 0;
            result = 0;
            return 0;
        }
        DoOperation(op);
        size_t current_precision = ((KnowTens(result) + precision) > 11) ? 11 : KnowTens(result) + precision;
        cout.precision(current_precision);
        ss.clear();
        return result;
    }

    bool IsOperation(char &x)
    {
        if ((x == '+') || (x == '-') || (x == '*') || (x == '/') || (x == '%') || (x == '^'))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    void ParseStream(stringstream &ss)
    {
        char x = ss.peek();
        if (x == 'c')
        {
            throw std::invalid_argument{"Result Deleted!"};
        }
        else if (IsOperation(x))
        {
            first = result;
        }
        else
        {
            ss >> first;
        }
        ss >> op;
        ss >> second;
    }
    void DoOperation(char &op)
    {
        bool is_int = is_integral_v<decltype(first)>;
        double previous_result = result;
        try
        {
            switch (op)
            {
            case ('+'):
            {
                result = first + second;
                break;
            }
            case ('-'):
            {
                result = first - second;
                break;
            }
            case ('*'):
            {
                result = first * second;
                break;
            }
            case ('/'):
            {
                if (second == 0)
                {
                    cout << "Can't divide by zero!" << endl;
                    break;
                }
                result = first / second;
                break;
            }
            case ('%'):
            {
                if (is_int)
                {
                    result = static_cast<int>(first) % static_cast<int>(second);
                }
                else
                {
                    cout << "Can't do this with not int!\n";
                }
                break;
            }
            case ('^'):
            {
                result = pow(static_cast<double>(first), static_cast<double>(second));
                break;
            }
            default:
                break;
            }
            T current_result = result;
            if (current_result != result)
            {
                throw runtime_error{""};
            }
        }
        catch (exception &e)
        {
            cout << "An over/underflow error ocured!" << endl;
            result = previous_result;
        }
    }
    size_t KnowTens(const double &res)
    {
        int r = static_cast<int>(res);
        size_t tens{0};
        while (r != 0)
        {
            r /= 10;
            tens++;
        }
        return tens;
    }
    void SetPrecision(const size_t &p)
    {
        precision = p;
    }

protected:
    char op;
    double result{0};
    T first{0}, second{0};
    size_t precision;
};

class Int_Calculator : public Calculator<int>
{
public:
    Int_Calculator(const size_t &precision)
    {
        SetPrecision(0);
    };

private:
};

class Double_Calculator : public Calculator<double>
{
public:
    Double_Calculator(const size_t &precision)
    {
        SetPrecision(precision);
    };
};

template <typename T>
void RunCalculator(Calculator<T> &calc)
{
    string s;
    cin >> ws; //excluding all whitespaces from cin >>
    while (getline(cin, s))
    {
        stringstream ss{s};
        cout << "Result is: " << calc.Calculate(ss) << endl;
    }
}
main()
{
    cerr << "To delete result type c" << endl;
    cout << "Write number of digits after the point:" << endl;
    size_t zeros;
    cin >> zeros;


    if (zeros == 0)
    {
        cout << "Now you're in tshe int mode, you can use +,-,*,/,%,^ " << endl;
        Int_Calculator calc1(zeros);
        RunCalculator(calc1);
    }
    else
    {
        cout << "Now you're in the double mode, you can use +,-,*,/,^ " << endl;
        Double_Calculator calc2(zeros);
        RunCalculator(calc2);
    }
    return 0;
}
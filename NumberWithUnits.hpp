#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
using namespace std;

namespace ariel{
    class NumberWithUnits{
        public:
            std::string unit;
            double val;
            NumberWithUnits();
            NumberWithUnits(double num, std::string un);
            void setVal(double _val);
            void setUnit(string _unit);
            double getVal();
            string getUnit();
            static void read_units(ifstream& units_file);
            friend bool changeUnit(const NumberWithUnits& num1,  NumberWithUnits& num2);
            NumberWithUnits operator +(const NumberWithUnits& num);
            NumberWithUnits operator +=(const NumberWithUnits& num);
            NumberWithUnits operator +();
            NumberWithUnits operator -(const NumberWithUnits& num);
            NumberWithUnits operator -=(const NumberWithUnits& num);
            NumberWithUnits operator -();
            NumberWithUnits& operator ++();
            NumberWithUnits& operator --();
            NumberWithUnits operator ++(int);
            NumberWithUnits operator --(int);
            NumberWithUnits operator *(double n);
            friend NumberWithUnits operator *(double n, const NumberWithUnits& num);
            friend bool operator > (const NumberWithUnits& num1, const NumberWithUnits& num2);
            friend bool operator < (const NumberWithUnits& num1, const NumberWithUnits& num2);
            friend bool operator >=(const NumberWithUnits& num1, const NumberWithUnits& num2);
            friend bool operator <=(const NumberWithUnits& num1, const NumberWithUnits& num2);
            friend bool operator ==(const NumberWithUnits& num1, const NumberWithUnits& num2);
            friend bool operator !=(const NumberWithUnits& num1, const NumberWithUnits& num2);
            void operator>>(istream& in);
            friend ostream& operator <<(ostream& out, NumberWithUnits const& num);
            friend istream& operator >>(istream& in, NumberWithUnits& num);
    };
}

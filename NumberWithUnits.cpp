#include <stdio.h>
#include <stdlib.h> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <iterator>
#include "NumberWithUnits.hpp"
using namespace std;
using namespace ariel;

double EPS = 0.001;

map<std::string, map<std::string, double>> units;

NumberWithUnits::NumberWithUnits(){}
NumberWithUnits::NumberWithUnits(double num, std::string un){
    this->val = num;
    if(units.find(un) == units.end()){
        throw std::invalid_argument("the unit does not exist!");
    }
    else{
        this->unit = un;
    }
}

vector<string> splitWords(string str)
{
    string word = "";
    vector<string> result;
    for (char x : str) 
    {
        if ((x == ' ' || x == '=' || x == '[' || x == ']'))
        {
            if(word.size() > 0){
                result.push_back(word);
                word = "";
            }
        }
        else {
            word = word + x;
        }
    }
    if(word.size() > 0){
        result.push_back(word);
    }
    return result;
}

bool ariel::changeUnit(const NumberWithUnits& num1,  NumberWithUnits& num2){
    if(units.at(num1.unit).count(num2.unit) != 0){
        if(units.at(num1.unit).at(num2.unit) > 0){
            if(num2.val != 0){
                num2.val = num2.val / units.at(num1.unit).at(num2.unit);
            }
            else{
                num2.val = 0;
            }
        }
        else if(units.at(num1.unit).at(num2.unit) < 0){
            if(num2.val != 0){
                num2.val = units.at(num1.unit).at(num2.unit) * num2.val;
            }
            else{
                num2.val = 0;
            }
        }
        num2.unit = num1.unit;
        return true;
    }
    else{
        throw std::invalid_argument("the values cannot be converted into eachother!");
    }
    return false;
}

void insertToMap(double d1, string str1, double d2, string str2){
    if(units.count(str1) == 0){
        map<std::string, double> tempMap;
        tempMap.insert({str1, 1});
        units.insert({str1, tempMap});
    }
    units.at(str1).insert({str2, d2 / d1});

    if(units.count(str2) == 0){
        map<std::string, double> tempMap;
        tempMap.insert({str2, 1});
        units.insert({str2, tempMap});
    }
    units.at(str2).insert({str1, d1 / d2});
}

void NumberWithUnits::read_units(ifstream& units_file){
    std::string line;
    while (std::getline(units_file, line)){
        vector<string> result = splitWords(line);
        double d1 = std::stod(result.at(0)), d2 = std::stod(result.at(2));
        string str1 = result.at(1), str2 = result.at(3);
        insertToMap(d1, str1, d2, str2);
        if(units[str1].size() > units[str2].size()){
            for (auto it = units[str1].begin();it != units[str1].end(); ++it){ // checking if we can convert to other values
                if(units[str2].count(it->first) == 0){
                    if(it->second > 0){
                        NumberWithUnits tempNum;
                        tempNum.unit = str2;
                        tempNum.val = d2;
                        changeUnit(NumberWithUnits(d1, str1), tempNum);
                        changeUnit(NumberWithUnits(it->second, it->first), tempNum);
                        insertToMap(tempNum.val, tempNum.unit, d2, str2);
                    }
                    else{
                        NumberWithUnits tempNum;
                        tempNum.unit = str2;
                        tempNum.val = d2;
                        changeUnit(NumberWithUnits(d1, str1), tempNum);
                        changeUnit(NumberWithUnits(it->second, it->first), tempNum);
                        insertToMap(d2, str2, tempNum.val, tempNum.unit);
                    }
                }
            }
        }
        else if(units[str1].size() < units[str2].size()){
            for (auto it = units[str2].begin();it != units[str2].end(); ++it){ // checking if we can convert to other values
                if(units[str1].count(it->first) == 0){
                    if(it->second > 0){
                        NumberWithUnits tempNum;
                        tempNum.unit = str1;
                        tempNum.val = d1;
                        changeUnit(NumberWithUnits(d2, str2), tempNum);
                        changeUnit(NumberWithUnits(it->second, it->first), tempNum);
                        insertToMap(tempNum.val, tempNum.unit, d1, str1);
                    }
                    else{
                        NumberWithUnits tempNum;
                        tempNum.unit = str1;
                        tempNum.val = d1;
                        changeUnit(NumberWithUnits(d2, str2), tempNum);
                        changeUnit(NumberWithUnits(it->second, it->first), tempNum);
                        insertToMap(d1, str1, tempNum.val, tempNum.unit);
                    }
                }
            }
        }
    }
}

void NumberWithUnits::setVal(double _val){
    this->val = _val;
}
void NumberWithUnits::setUnit(string _unit){
    this->unit = _unit;
}

double NumberWithUnits::getVal(){
    return this->val;
}
string NumberWithUnits::getUnit(){
    return this->unit;
}

NumberWithUnits NumberWithUnits::operator+(const NumberWithUnits& num){
    NumberWithUnits _num;
    _num.unit = num.unit;
    _num.val = num.val;
    changeUnit(*this, _num);
    return NumberWithUnits(this->val + _num.val, this->unit);
}
NumberWithUnits NumberWithUnits::operator+=(const NumberWithUnits& num){
        NumberWithUnits _num;
    _num.unit = num.unit;
    _num.val = num.val;
    if(this->unit == num.unit){
        this->val += num.val;
    }
    else if(changeUnit(*this, _num) == true){
        this->val += _num.val;
    }
    return *this;
}
NumberWithUnits NumberWithUnits::operator+(){
    return NumberWithUnits(+this->val, this->unit);
}
NumberWithUnits NumberWithUnits::operator-(const NumberWithUnits& num){
    NumberWithUnits _num;
    _num.unit = num.unit;
    _num.val = num.val;
    changeUnit(*this, _num);
    return NumberWithUnits(this->val - _num.val, this->unit);
}
NumberWithUnits NumberWithUnits::operator-=(const NumberWithUnits& num){
    NumberWithUnits _num;
    _num.unit = num.unit;
    _num.val = num.val;
    if(this->unit == num.unit){
        this->val -= num.val;
    }
    else if(changeUnit(*this, _num) == true){
        this->val -= _num.val;
    }
    return *this;
}
NumberWithUnits NumberWithUnits::operator-(){
    return NumberWithUnits(-this->val, this->unit);

}

NumberWithUnits& NumberWithUnits::operator ++(){
    ++this->val;
    return *this;
}
NumberWithUnits& NumberWithUnits::operator --(){
    --this->val;
    return *this;
}

NumberWithUnits NumberWithUnits::operator ++(int){
    return NumberWithUnits(this->val++, this->unit);

}
NumberWithUnits NumberWithUnits::operator --(int){
    return NumberWithUnits(this->val--, this->unit);

}

NumberWithUnits NumberWithUnits::operator *(double n){
    return NumberWithUnits(this->val*n, this->unit);
}
NumberWithUnits ariel::operator *(double n, const NumberWithUnits& num){
    return NumberWithUnits(num.val*n, num.unit);
}


bool ariel::operator >(const NumberWithUnits& num1, const NumberWithUnits& num2){
    NumberWithUnits _num2;
    _num2.unit = num2.unit;
    _num2.val = num2.val;
    bool flag = false;
    bool b = changeUnit(num1, _num2);
    if(num1.unit == _num2.unit){
        double n1 = num1.val, n2 = _num2.val;
        if(num1.val > _num2.val){
            flag = true;
        }
    }
    else{
        throw std::invalid_argument("first value is not equal");
    }
    return flag;
}
bool ariel::operator<(const NumberWithUnits& num1, const NumberWithUnits& num2){
    NumberWithUnits _num2;
    _num2.unit = num2.unit;
    _num2.val = num2.val;
    bool flag = false;
    bool b = changeUnit(num1, _num2);
    if(num1.unit == _num2.unit){
        double n1 = num1.val, n2 = _num2.val;
        if(num1.val < _num2.val){
            flag = true;
        }
    }
    else{
        throw std::invalid_argument("first value is not equal");
    }
    return flag;
}
bool ariel::operator>=(const NumberWithUnits& num1, const NumberWithUnits& num2){
    NumberWithUnits _num2;
    _num2.unit = num2.unit;
    _num2.val = num2.val;
    bool flag = false;
    bool b = changeUnit(num1, _num2);
    if(num1.unit == _num2.unit){
        double n1 = num1.val, n2 = _num2.val;
        if(num1.val >= _num2.val){
            flag = true;
        }
    }
    else{
        throw std::invalid_argument("first value is not equal");
    }
    return flag;
}
bool ariel::operator<=(const NumberWithUnits& num1, const NumberWithUnits& num2){
    NumberWithUnits _num2;
    _num2.unit = num2.unit;
    _num2.val = num2.val;
    bool flag = false;
    bool b = changeUnit(num1, _num2);
    if(num1.unit == _num2.unit){
        double n1 = num1.val, n2 = _num2.val;
        if(num1.val <= _num2.val){
            flag = true;
        }
    }
    else{
        throw std::invalid_argument("first value is not equal");
    }
    return flag;
}
bool ariel::operator==(const NumberWithUnits& num1, const NumberWithUnits& num2){
    NumberWithUnits _num2;
    _num2.unit = num2.unit;
    _num2.val = num2.val;
    bool flag = false;
    bool b = changeUnit(num1, _num2);
    if(num1.unit == _num2.unit){
        double n1 = num1.val, n2 = _num2.val;
        if(abs(num1.val - _num2.val) < EPS){
            flag = true;
        }
    }
    else{
        throw std::invalid_argument("first value is not equal");
    }
    return flag;
}
bool ariel::operator!=(const NumberWithUnits& num1, const NumberWithUnits& num2){
    NumberWithUnits _num2;
    _num2.unit = num2.unit;
    _num2.val = num2.val;
    bool flag = false;
    bool b = changeUnit(num1, _num2);
    if(num1.unit == _num2.unit){
        double n1 = num1.val, n2 = _num2.val;
        if ((num1.val != _num2.val)){
            flag = true;
        }
    }
    else{
        throw std::invalid_argument("first value is not equal");
    }
    return flag;
}

ostream& ariel::operator <<(ostream& out, const NumberWithUnits& num){
    return out << num.val << "[" << num.unit << "]";
}

istream& ariel::operator >>(istream& in, NumberWithUnits& num){
    double num1;
    string str;

    std::string line;
    char c = ' ';
    while (c != ']'){
        in >> c;
        line += c;
    }
   
    vector<string> result = splitWords(line);
    
    if(result.size() > 1){
        str = result[1];
        num1 = std::stod(result[0]);
        num.val = num1;
        num.unit = str;
    }
    else{
        throw std::invalid_argument("the unit does not exist!");
    }

    if(units.find(str) == units.end()){
        throw std::invalid_argument("the unit does not exist!");
    }
    return in;
}


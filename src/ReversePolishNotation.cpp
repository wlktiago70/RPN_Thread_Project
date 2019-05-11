////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2019 Embraer
//
// Project  : RPN Test
//
// File     : ReversePolishNotation.cpp
// Created  : May 2019
// Author   : Wu Liang Kuan
//
////////////////////////////////////////////////////////////////////////////////////////

#include "ReversePolishNotation.hpp"

ReversePolishNotation::ReversePolishNotation(const string& pExpression): mValid(true)
{
    string token;
    double number;
    istringstream iss(pExpression);
    while(iss >> token)
    {
        if(istringstream(token) >> number)
        {
            mOperands.push(number);
        }
        else if(isOperator(token))
        {
            calculate(token);
        }
        else
        {
            mValid = false;
            break;
        }
    }
}

ReversePolishNotation::~ReversePolishNotation(){}

bool ReversePolishNotation::isOperator(const string& pInput)
{
    static const string operators ="-+*/";
    if(1 == pInput.length())
    {
        return operators.find_first_of(pInput[0]) != string::npos;
    }
    return false;
}

void ReversePolishNotation::calculate(const string& pOperator)
{
    if(mOperands.size()>=2)
    {
        double firstOperand, secondOperand;
        secondOperand = mOperands.top();
        mOperands.pop();
        firstOperand = mOperands.top();
        mOperands.pop();
        switch(pOperator[0])
        {
            case '-':
                mOperands.push(firstOperand - secondOperand);
                break;
            case '+':
                mOperands.push(firstOperand + secondOperand);
                break;
            case '*':
                mOperands.push(firstOperand * secondOperand);
                break;
            case '/':
                if(0 == secondOperand)
                {
                    mValid = false;
                }
                mOperands.push(firstOperand / secondOperand);
        }
    }
    else
    {
        mValid = false;
    }
}

string ReversePolishNotation::getStringResult()
{
    return isValid()?to_string(getResult()):"NaN";
}

double ReversePolishNotation::getResult()
{
    if(!mOperands.empty())
    {
        return mOperands.top();
    }
    return 0.0;
}

bool ReversePolishNotation::isValid()
{
    return mValid;
}

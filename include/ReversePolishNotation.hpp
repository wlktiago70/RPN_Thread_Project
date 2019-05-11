////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2019 Embraer
//
// Project  : RPN Test
//
// File     : ReversePolishNotation.hpp
// Created  : May 2019
// Author   : Wu Liang Kuan
//
////////////////////////////////////////////////////////////////////////////////////////

#ifndef REVERSEPOLISHNOTATION_HPP
#define REVERSEPOLISHNOTATION_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <stack>

using namespace std;

class ReversePolishNotation
{
    private:
        stack<double> mOperands; ///< A stack to store all operands
        bool mValid; ///< For result validity

        /**
         * Checks whether given string is an operator or not
         * @param pInput An string to be validated
         */
        bool isOperator(const string& pInput);

        /**
         * Calculates the result based on the given operator
         * @param pOperator Calculation operator
         */
        void calculate(const string& pOperator);

    public:

        /**
         * Class constructor
         * @param pExpression A expression using Reverse Polish Notation
         */
        ReversePolishNotation(const string& pExpression);

        /**
         * Class destructor
         */
        virtual ~ReversePolishNotation();

        /**
         * Get the RPN expression result
         * @return <double> When it is a valid result
         * @return NaN When the RPN expression is invalid
         */
        string getStringResult();

        /**
         * Get the RPN expression numeric result (without checking its validity)
         * @return <double> The numeric value of the result
         */
        double getResult();

        /**
         * Get the validity of numeric value
         * @return true The numeric value is valid
         * @return false The numeric value is invalid
         */
        bool isValid();
};

#endif // REVERSEPOLISHNOTATION_HPP

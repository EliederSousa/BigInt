/** 
 *  bigint.h
 *  
 *  Copyright (c) 2022, Elieder Sousa
 *  eliedersousa<at>gmail<dot>com
 *  
 *  Distributed under the MIT license.
 *  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *  
 *  @date 28/11/22
 *  
 *  @brief A simple BigInt library, slower than standards, faster than other simple ones. "It does the job".
 */

#ifndef _BIGINT_ELI_
#define _BIGINT_ELI_

#include <iostream>
#include <string>

using namespace std;
 
 class BigInt {
    public:
        int *num; // The number itself.
        int size; // Its real size.
        int MAXDIGITS; // Its vector size.
        int tens;
        int *nonTenFactors;
        int factorsSize;
        string text;

    public:
        // ctors
        BigInt ( int initialValue);
        BigInt ( string initialValue);

        // Handle manipulators, printing and stuff like that.
        void reset( int initialValue );
        void copy( BigInt value );
        int digit( int n, bool isRightSide );
        void print();
        void print( bool useSeparator );
        void debug();
        string toString();
        int toInt();

        // Arithmetic functions.
        void add( BigInt valor );
        void subtract( BigInt valor );
        void multiply(int x);
        void multiply( BigInt value );
        void multiplyByTens( int numberOfZeros );        
        void divide(long divisor);
        void divideByTens( int numberOfZeros );
        
        void pow( int n );
        void factorial( int n );

        // Misc Math functions.
        long long factorialDigits( long long n );
};

#endif
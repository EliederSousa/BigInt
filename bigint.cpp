/** 
 *  bigint.cpp
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
#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <omp.h>

#include "bigint.h"

int _MAXDIGITS = 1000000;
long base = 10;

BigInt::BigInt( int initialValue) {
    MAXDIGITS = _MAXDIGITS;
    tens = 0;
    nonTenFactors = new int[MAXDIGITS];
    factorsSize = 0;
    num = new int[MAXDIGITS];
    if( initialValue > 0 ) {
        size = floor(log10(initialValue) + 1);
    } else {
        size = 1;
    }
    for ( int w=0; w<MAXDIGITS; w++ ) {
        num[w] = 0;
    }
    num[MAXDIGITS-1] = initialValue;
    for( int w=0; w<size; w++ ) {
        num[MAXDIGITS-1-w] = (int)(initialValue / std::pow(10, w)) % 10;
    }
}

BigInt::BigInt( string strValue) {
    MAXDIGITS = _MAXDIGITS;
    tens = 0;
    nonTenFactors = new int[MAXDIGITS];
    factorsSize = 0;
    num = new int[MAXDIGITS];
    size = strValue.length();
    for ( int w=0; w<MAXDIGITS; w++ ) {
        num[w] = 0;
    }
    for( int w=0; w<size; w++ ) {
        num[MAXDIGITS-1-w] = strValue.at(strValue.length() - 1 - w) - 48;
    }
}

void BigInt::reset( int initialValue ) {
    tens = 0;
    nonTenFactors = new int[MAXDIGITS];
    factorsSize = 0;
    if( initialValue > 0 ) {
        size = floor(log10(initialValue) + 1);
    } else {
        size = 1;
    }
    for ( int w=0; w<MAXDIGITS; w++ ) {
        num[w] = 0;
    }
    num[MAXDIGITS-1] = initialValue;
    for( int w=0; w < size; w++ ) {
        num[MAXDIGITS-1-w] = (int)(initialValue / std::pow(10, w)) % 10;
    }
}

void BigInt::copy( BigInt value ) {
    for ( int w=0;w<MAXDIGITS; w++ ) {
        num[MAXDIGITS-1-w] = value.num[MAXDIGITS-1-w];
    }
    size = value.size;
}

int BigInt::digit( int n, bool isRightSide ) {
    if ( isRightSide ) {
        return num[MAXDIGITS-1-n];
    } else {
        return num[MAXDIGITS-size+n];
    }
}

void BigInt::print() {
    for( int w=MAXDIGITS-size; w<MAXDIGITS; w++ ) {
        std::cout << num[w];
    }
    std::cout << std::endl;
}

void BigInt::debug() {
    std::cout << "----------------------\n";
    std::cout << "Size: " << size << "\n";
    std::cout << "Maximum size: " << MAXDIGITS << "\n";
    std::cout << "Value: ";
    print();
    std::cout << "----------------------\n";
}

void BigInt::print( bool useSeparator ) {
    if( useSeparator && size > 3 ) {
        for( int w=MAXDIGITS-size; w<MAXDIGITS; w++ ) {
            std::cout << num[w];
            if( w % 3 == 0 && (w != MAXDIGITS-1) ) {
                std::cout << ".";
            }
        }
        std::cout << std::endl;
    } else {
        for( int w=MAXDIGITS-size; w<MAXDIGITS; w++ ) {
            std::cout << num[w];
        }
        std::cout << std::endl;
    } 
}

string BigInt::toString() {
    std::string str;
    for( int w=MAXDIGITS-size; w<MAXDIGITS; w++ ) {
        str.push_back(num[w] + '0');
    }
    return str;
}

int BigInt::toInt() {
    return stoi(toString());
}

void BigInt::multiplyByTens( int numberOfZeros ) {
    if( numberOfZeros > 0 ) {
        for (int actual=MAXDIGITS-size; actual<MAXDIGITS; actual++) {
            num[actual-numberOfZeros] = num[actual];
        }
        for( int w=0;w<numberOfZeros; w++ ) {
            num[MAXDIGITS - 1 - w] = 0;
        }
        size += numberOfZeros;
    }
}

void BigInt::multiply(int x) {
    int carry = 0;
    int actual;
    for (int i=0; i<size; i++) {
        actual = MAXDIGITS - 1 - i;
        int prod = num[actual] * x + carry;
        num[actual] = prod % 10;
        carry  = prod/10;
    }

    while (carry) {
        num[MAXDIGITS - 1 - size] = carry%10;
        carry = carry/10;
        size++;
    }
}

void BigInt::multiply( BigInt value ) {
    BigInt tempsum = BigInt( 0 );
    BigInt copied = BigInt( 0 );
    for( int w=0; w<value.size; w++ ) {
        copied.copy( *this );
        copied.multiply( value.digit(w, true) );
        copied.multiplyByTens( w );
        tempsum.add( copied );
    }
    copy( tempsum );
}

void BigInt::pow( int n ) {
    int copied = toInt();
    reset(1);
    for( int w=0; w<n; w++ ) {
        multiply( copied );
    }
}

void BigInt::divideByTens( int numberOfZeros ) {
    if( numberOfZeros < size ) {
        for (int actual=0; actual<size; actual++) {
            num[MAXDIGITS-1-actual] = num[MAXDIGITS-1-actual-numberOfZeros];
        }
        size -= numberOfZeros;
    }
}

void BigInt::divide(long divisor) {
    long carry=0, xi, q;
    for (long i=MAXDIGITS-size; i<MAXDIGITS; i++) {
        xi    = num[i]+carry*base;
        q     = xi/divisor;
        carry = xi-q*divisor;   
        num[i]  = q;        
    }  
    bool running = true;
    int newSizeToSubtract = 0;
    for (long i=MAXDIGITS-size; i<MAXDIGITS; i++) {
    if( running && (num[i] == 0) ) {
    newSizeToSubtract++;
    } else {
    running = false;
    }
    }
    size -= newSizeToSubtract;
}

void BigInt::add( BigInt valor ) {
    long carry=0;
    if( valor.size > size ) {
        size = valor.size;
    }
    for (int i=MAXDIGITS-1; i>=MAXDIGITS-size; i--) {
        num[i] += valor.num[i]+carry;
        if (num[i]<base) {
            carry = 0;
        } else {
            carry = 1;
            num[i] -= base;
        }
    }
    if ( carry ) {
        num[MAXDIGITS-size-1] = 1;
        size++;
    }
}

void BigInt::subtract( BigInt valor ) {
    int borrow = 0;
    for(int i=MAXDIGITS-size; i < MAXDIGITS; i++) {
        int result= (num[i] - valor.num[i] - borrow);
        if(result < 0) {
            num[i] = result + 10;
            borrow = 1;
        } else {
            num[i] = result;
            borrow = 0;
        }
    }
}

long long BigInt::factorialDigits( long long n ) {
    return std::ceil( std::log10(std::sqrt(2*3.1415926535897932384*n)) + n * std::log10(n/std::exp(1)));
}

void BigInt::factorial( int n ) {
    reset( 1 );
    for( int w=2; w<=n; w++ ) {
        multiply(w);
    }
}

/*void BigInt::factorial( int n ) {
    reset( 1 );
    int twos = 0;
    int fives = 0;
    tens = 0;
    #pragma omp parallel num_threads(2)
    {
        int idThread = omp_get_thread_num();
        int candidate = 0;
        int temptwos = 0;
        int tempfives = 0;
        for( int w=2+idThread;w<=n;w+=2 ) {
            candidate = w;
            while( candidate % 2 == 0 ) {
                candidate /= 2;
                temptwos++;
            }
            while( candidate % 5 == 0 ) {
                candidate /= 5;
                tempfives++;
            }
            if( candidate != 1 ) {
                #pragma omp critical
                {
                    nonTenFactors[factorsSize] = candidate;
                    factorsSize++;
                    multiply(candidate);
                }
            }
        }
        #pragma omp critical 
        {
            twos += temptwos;
            fives += tempfives;
        }
    }
    tens = fives;
    multiplyByTens( tens );
    twos -= fives;
    nonTenFactors[factorsSize] = 1;

    for( int w=0; w<twos; w++ ) {
        if( nonTenFactors[factorsSize] < 8388608 ) {
            nonTenFactors[factorsSize] *= 2;
        } else {
            factorsSize++;
            nonTenFactors[factorsSize] = 2;
        }
        multiply(2);
    }
}*/
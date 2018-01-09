//
// Created by cagatay on 27.07.2016.
//
#include "LinkedList.h"

#ifndef BIGINTEGER_H
#define BIGINTEGER_H

using namespace std;

class BigInteger {

public:
    BigInteger(int number=0);                       //done

    BigInteger(const string& bigInteger);           //done

    BigInteger operator+(const BigInteger &list);           //done

    BigInteger operator*(const BigInteger &list);       //done

    BigInteger operator*(int i);

    BigInteger(const BigInteger &other);
    BigInteger& operator=(const BigInteger &list);
    ~BigInteger();

public:
    LinkedList *num = 0;

};


#endif //
BIGINTEGER_H

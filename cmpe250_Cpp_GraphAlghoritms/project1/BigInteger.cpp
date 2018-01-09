/*
Student Name: BARIN ÖZMEN
Student Number: 2012400045
Project Number: 1
Operating System: Linux(xUbuntu)
Compile Status: DONE
Program Status: DONE
Notes: Anything you want to say about your code that will be helpful in the grading process.

*/

#include "BigInteger.h"
#include <algorithm>
/*
ostream &operator<<(ostream &out, const BigInteger &bigInteger)  {
    string str = "";
    Node *head = bigInteger.num->head;
    while (head) {
        str += to_string(head->data);
        head = head->next;
    }
    reverse(str.begin(), str.end());
    if (str == "")
        str = "0";
    out << str;
    return out;

}
*/

BigInteger::BigInteger(int number){
    this->num = new LinkedList;
    while(number!= 0){
        num->pushTail(number%10);
        number/10;

    }

}

BigInteger::BigInteger(const BigInteger &other){

    this->num = new LinkedList;
    *num  = *other.num;

}


BigInteger::BigInteger(const string& bigInteger){
    this->num = new LinkedList;
    int number;
    for(int i = bigInteger.size()-1; i>= 0; i--){
        number = bigInteger[i] -'0';
        num->pushTail(number);
    }

}

BigInteger BigInteger::operator+(const BigInteger &list){
    BigInteger big;
    Node* ptr1 = this->num->head;
    Node* ptr2 = list.num->head;
    while (ptr1!=NULL && ptr2!=NULL) {
        if((ptr1->data + ptr2->data) >= 10){
            if(ptr1->next!=NULL){
                big.num->pushTail((ptr1->data + ptr2->data)%10);
                ptr1 = ptr1->next;
                ptr2 = ptr2->next;
                ptr1->data += 1;
            }
            else if(ptr2->next!=NULL){
                big.num->pushTail((ptr1->data + ptr2->data)%10);
                ptr1 = ptr1->next;
                ptr2 = ptr2->next;
                ptr2->data +=1;
            }
            else{
                big.num->pushTail((ptr1->data + ptr2->data)%10);
                big.num->pushTail((ptr1->data + ptr2->data)/10);
                ptr1 = ptr1->next;
                ptr2 = ptr2->next;
            }

        }
        else{
            big.num->pushTail((ptr1->data + ptr2->data));
            ptr1 = ptr1->next;
            ptr2 = ptr2->next;
        }
    }
    Node* bigger = 0;
    if (ptr1!=NULL) bigger = ptr1;
    else if (ptr2!=NULL) bigger = ptr2;
    while (bigger!=NULL) {
        big.num->pushTail(bigger->data);
        bigger = bigger->next;
    }
    return big;
}

BigInteger BigInteger::operator*(const BigInteger &list){
    BigInteger big;
    BigInteger other;


    int t = 0;
    int count =0;
    Node* ptr1 = this->num->head;
    Node* ptr2 = list.num->head;


    while (ptr1!=NULL) {
        count++;
        ptr1 = ptr1->next;
    }

    ptr1 = this->num->head;
    for(int i=0; i<count ; i++){
        while(ptr2 != NULL && ptr1 != NULL){
            other.num->pushTail(((ptr1->data*ptr2->data) +t ) %10);
            t = (ptr1->data*ptr2->data + t)/10;

            if(ptr2->next == NULL){
                if(ptr2->data*ptr1->data>=10){
                    other.num->pushTail(t);
                }
                for(int push =2*i ; push > i ; push--){
                    other.num->pushHead(0);
                }
            }
            ptr2 = ptr2->next;
        }
        t=0;
        ptr2 = list.num->head;
        ptr1 = ptr1->next;

        big = big + other;
        other.num->~LinkedList();
        other.num = new LinkedList;

    }

    return big;
}

BigInteger BigInteger::operator*(int i){
    BigInteger big;
    Node* ptr1 = this->num->head;
    int t=0;

    while(ptr1 != NULL){
        big.num->pushTail(((ptr1->data*i)%10) + t);
        t = (ptr1->data*i)/10;
        ptr1 = ptr1->next;
    }

    return big;
}


BigInteger& BigInteger::operator=(const BigInteger &list){
    this->num->~LinkedList();
    Node *temp = list.num->head;
    while (temp != NULL) {
        this->num->pushTail(temp->data);
        temp = temp->next;
    }
    return *this;
}

BigInteger::~BigInteger(){
    if(num) delete(num);
}



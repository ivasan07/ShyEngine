#include <iostream>
#include "CFlatBasics.h"

using CFlat::IBox;

IBox* initialiseSetup() {

   
    IBox* a = new CFlat::Float(2);
    IBox* b = new CFlat::Float(1);
    IBox* d = new CFlat::Float(3);


    IBox* suma = new CFlat::Add();
    IBox* resta = new CFlat::Subtract();
    IBox* mult = new CFlat::Multiply();

    IBox* print = new CFlat::Print();

    suma->addInput(a);
    suma->addInput(b);

    resta->addInput(a);
    resta->addInput(d);

    mult->addInput(suma);
    mult->addInput(resta);

    print->addInput(mult);

    return print;
}


int main()
{
    IBox* currentBox = initialiseSetup();

    while (currentBox != nullptr)
    {
        currentBox->processBox();
        currentBox = currentBox->nextBox;
    }

    return 0;
}

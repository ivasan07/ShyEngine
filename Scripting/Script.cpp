#include "Script.h"
#include "CFlat.h"
#include "FlowManager.h"

void Script::iteration(CFlat::IBox* iteration)
{
    while (iteration != nullptr)
    {
        iteration->processBox();
        iteration = iteration->nextBox;
    }
}

void Script::setupScript(std::string script)
{
    //TODO: el flow manager no deberia estar en el script??? 
    FlowManager manager;
    FlowManager::Script* scriptInputs = manager.loadScript(script);
    initBox = scriptInputs->init;
    updateBox = scriptInputs->update;
}

void Script::init()
{
    if (!initBox)
        return;

    iteration(initBox);
}

void Script::update()
{
    if (!updateBox)
        return;
    iteration(updateBox);
}

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
    CFlat::FlowManager::ScriptInfo* scriptInputs = CFlat::FlowManager::instance->loadScript(script, this);

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

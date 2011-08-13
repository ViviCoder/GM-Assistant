#include "Model.h"

// iterator's methods
Model::iterator::iterator(Model::IterationType type, State state): itType(type), sState(state)
{
}

Model::IterationType Model::iterator::type() const
{
    return itType;
}

State Model::iterator::state() const
{
    return sState;
}

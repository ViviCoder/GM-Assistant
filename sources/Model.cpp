#include "Model.h"

Model::iterator::iterator(Model::IterationType type, State state): std::iterator<std::random_access_iterator_tag,std::pair<std::string,State> >(), itType(type), sState(state)
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

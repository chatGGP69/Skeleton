#include "Settlement.h"
#include <sstream>
#include <unordered_map>
#include <stdexcept>
using namespace std;

Settlement::Settlement(const string &name, SettlementType type)
    : name(name), type(type) {}

const string &Settlement::getName() const
{
    return name;
}

SettlementType Settlement::getType() const
{
    return type;
}

const string Settlement::toString() const
{
    ostringstream oss;
    oss << "Settlement Name: " << name << 

    switch (this->getType())
    {
    case SettlementType::VILLAGE:
        oss << "Village";
        break;
    case SettlementType::CITY:
        oss << "City";
        break;
    case SettlementType::METROPOLIS:
        oss << "Metropolis";
        break;
    default:
        oss << "Unknown Settlement Type";
    }

    return oss.str();
}

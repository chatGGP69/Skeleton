#include "Facility.h"
#include <sstream>
#include <unordered_map>

using namespace std;

FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score)
    : name(name), category(category), price(price), lifeQuality_score(lifeQuality_score), economy_score(economy_score), environment_score(environment_score) {}

const string &FacilityType::getName() const
{
    return name;
}

int FacilityType::getCost() const
{
    return price;
}

int FacilityType::getLifeQualityScore() const
{
    return lifeQuality_score;
}

int FacilityType::getEnvironmentScore() const
{
    return environment_score;
}

int FacilityType::getEconomyScore() const
{
    return economy_score;
}

FacilityCategory FacilityType::getCategory() const
{
    return category;
}

Facility::Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score)
    : FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score), settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(price) {}

Facility::Facility(const FacilityType &type, const string &settlementName)
    : FacilityType(type),
      settlementName(settlementName),
      status(FacilityStatus::UNDER_CONSTRUCTIONS),
      timeLeft(type.getCost()) {}

const string &Facility::getSettlementName() const
{
    return settlementName;
}

const int Facility::getTimeLeft() const
{
    return timeLeft;
}

FacilityStatus Facility::step()
{
    if (timeLeft == 0)
    {
        setStatus(FacilityStatus::OPERATIONAL);
    }
    
    if (timeLeft > 0)
    {
        timeLeft--;
    }

    return status;
}

void Facility::setStatus(FacilityStatus status)
{
    this->status = status;
}

const FacilityStatus &Facility::getStatus() const
{
    return status;
}

const string Facility::toString() const
{
    ostringstream oss;
    oss << "Facility Name: " << name << ", Category: ";

    switch (category)
    {
    case FacilityCategory::LIFE_QUALITY:
        oss << "Life Quality";
        break;
    case FacilityCategory::ECONOMY:
        oss << "Economy";
        break;
    case FacilityCategory::ENVIRONMENT:
        oss << "Environment";
        break;
    default:
        oss << "Unknown Category";
    }

    oss << ", Status: ";

    switch (status)
    {
    case FacilityStatus::UNDER_CONSTRUCTIONS:
        oss << "Under Construction";
        break;
    case FacilityStatus::OPERATIONAL:
        oss << "Operational";
        break;
    default:
        oss << "Unknown Status";
    }

    oss << ", Associated Settlement: " << settlementName;

    return oss.str();
}

Facility *Facility::clone() const
{
    return new Facility(*this);
}

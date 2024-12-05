
#include <vector>
#include "Plan.h"
using std::vector;

enum class PlanStatus
{
    AVALIABLE,
    BUSY,
};

Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
    : plan_id(planId),
      settlement(settlement),
      selectionPolicy(selectionPolicy),
      facilityOptions(facilityOptions),
      status(PlanStatus::AVALIABLE),
      facilities(),
      underConstruction(),
      life_quality_score(0),
      economy_score(0),
      environment_score(0) {}

Plan::Plan(const Plan &other)
    : plan_id(other.plan_id),
      settlement(other.settlement),
      selectionPolicy(other.selectionPolicy->clone()),
      facilityOptions(other.facilityOptions),
      status(other.status),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score)
{

    for (const auto facility : other.facilities)
    {
        facilities.push_back(facility->clone());
    }

    for (const auto facility : other.underConstruction)
    {
        underConstruction.push_back(facility->clone());
    }
}

const int Plan::getlifeQualityScore() const
{
    return life_quality_score;
}

const int Plan::getEconomyScore() const
{
    return economy_score;
}

const int Plan::getEnvironmentScore() const
{
    return environment_score;
}

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy)
{
    if (this->selectionPolicy != nullptr)
    {
        delete this->selectionPolicy;
    }

    this->selectionPolicy = selectionPolicy->clone();
}

void Plan::step()
{
    for (size_t i = 0; i < underConstruction.size(); ++i)
    {
        Facility *currFacility = underConstruction[i];
        currFacility->step();

        if (currFacility->getStatus() == FacilityStatus::OPERATIONAL)
        {
            facilities.push_back(currFacility);
            underConstruction[i] = underConstruction.back();
            underConstruction.pop_back();
            --i;
        }
    }
}

void Plan::printStatus()
{
    ostringstream oss;

    oss << "PlanID: " << plan_id << endl;

    oss << "SettlementName: " << settlement << endl;

    oss << "PlanStatus: ";
    switch (status)
    {
    case PlanStatus::BUSY:
        oss << "BUSY";
        break;
    case PlanStatus::AVALIABLE:
        oss << "AVALIABLE";
        break;
    default:
        oss << "UNKNOWN";
    }
    oss << endl;

    if (selectionPolicy)
    {
        oss << "SelectionPolicy: " << selectionPolicy->toString() << endl;
    }
    else
    {
        oss << "SelectionPolicy: None" << endl;
    }

    oss << "LifeQualityScore: " << lifeQualityScore << endl;
    oss << "EconomyScore: " << economyScore << endl;
    oss << "EnvironmentScore: " << environmentScore << endl;

    for (const auto &facility : facilities)
    {
        oss << "FacilityName: " << facility->getName() << endl;
        oss << "FacilityStatus: ";
        switch (facility->getStatus())
        {
        case FacilityStatus::UNDER_CONSTRUCTION:
            oss << "UNDER_CONSTRUCTION";
            break;
        case FacilityStatus::OPERATIONAL:
            oss << "OPERATIONAL";
            break;
        default:
            oss << "UNKNOWN";
        }
        oss << endl;
    }

    cout << oss.str();
}

const vector<Facility *> &Plan::getFacilities() const
{
    return facilities;
}

void Plan::addFacility(Facility *facility)
{
    facilityOptions.push_back(facility);
}

const string Plan::toString() const
{
    return string();
}

private:
int plan_id;
const Settlement &settlement;
SelectionPolicy *selectionPolicy; // What happens if we change this to a reference?
PlanStatus status;
vector<Facility *> facilities;
vector<Facility *> underConstruction;
const vector<FacilityType> &facilityOptions;
int life_quality_score, economy_score, environment_score;

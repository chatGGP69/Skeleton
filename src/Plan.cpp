
#include <vector>
#include "Plan.h"
using std::vector;


//Constructor
Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
    : plan_id(planId),
      settlement(settlement),
      selectionPolicy(selectionPolicy->clone()),
      facilityOptions(facilityOptions),
      status(PlanStatus::AVALIABLE),
      facilities(),
      underConstruction(),
      life_quality_score(0),
      economy_score(0),
      environment_score(0) {
        if (settlement.getType() == SettlementType::VILLAGE)
        {
            constructionLimit = 1;
        }
        if (settlement.getType() == SettlementType::CITY)
        {
            constructionLimit = 2;
        }

        if (settlement.getType() == SettlementType::METROPOLIS)
        {
            constructionLimit = 3;
        }
      }

//Copy Constructor
Plan::Plan(const Plan &other)
    : plan_id(other.plan_id),
      settlement(other.settlement),
      selectionPolicy(other.selectionPolicy->clone()),
      facilityOptions(other.facilityOptions),
      status(other.status),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score),
      constructionLimit(other.constructionLimit)
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

//Move Constructor
Plan::Plan(Plan &&other)
    : plan_id(other.plan_id),
      settlement(other.settlement),
      selectionPolicy(other.selectionPolicy),
      facilityOptions(other.facilityOptions),
      status(other.status),
      facilities(move(other.facilities)),
      underConstruction(move(other.underConstruction)),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score),
      constructionLimit(other.constructionLimit)
{
    other.selectionPolicy = nullptr;
    other.facilities.clear();
    other.underConstruction.clear();
}

//Destructor
Plan::~Plan()
{
    clearResources();
}


//Getters
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

const vector<Facility *> &Plan::getFacilities() const
{
    return facilities;
}

const SelectionPolicy *Plan::getSelectionPolicy() const
{
    return selectionPolicy;
}

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy)
{
    if (!(selectionPolicy == this->selectionPolicy))
        if (this->selectionPolicy)
        {
            delete this->selectionPolicy;
        }

        {
            this->selectionPolicy = selectionPolciy ? selectionPolicy->clone() : nullptr;
        }
}

void Plan::step()
{
    if (status == PlanStatus::AVALIABLE)
    {
        while (underConstruction.size() < constructionLimit)
        {
            FacilityType facilityType = selectionPolicy->selectFacility(facilityOptions, life_quality_score, economy_score, environment_score);
            Facility *facility = new Facility(facilityType);
            underConstruction.push_back(facility);
        }
        
    }
    
    for (size_t i = 0; i < underConstruction.size(); ++i)
    {
        Facility *currFacility = underConstruction[i];
        currFacility->step();

        if (currFacility->getStatus() == FacilityStatus::OPERATIONAL)
        {
            addFacility(currFacility);
            life_quality_score += currFacility->getLifeQualityScore();
            economy_score += currFacility->getEconomyScore();
            environment_score += currFacility->getEnvironmentScore();
            underConstruction[i] = underConstruction.back();
            underConstruction.pop_back();
            i--;
        }
    }

    if (underConstruction.size() == constructionLimit)
    {
        status = PlanStatus::BUSY;
    }
    else
    {
        status = PlanStatus::AVALIABLE;
    }

void Plan::printStatus()
{
    cout << toString() << endl;
}


void Plan::addFacility(Facility *facility)
{
    if(facility != nullptr)
        facilities.push_back(facility);
}

const string Plan::toString() const {
    ostringstream oss;

    oss << "PlanID: " << plan_id << "\n";
    oss << "SettlementName: " << settlement << "\n";

    oss << "PlanStatus: ";
    switch (status) {
    case PlanStatus::BUSY:
        oss << "BUSY";
        break;
    case PlanStatus::AVALIABLE:
        oss << "AVALIABLE";
        break;
    }
    oss << "\n";

    if (selectionPolicy) {
        oss << "SelectionPolicy: " << selectionPolicy->toString() << "\n";
    }

    oss << "LifeQualityScore: " << life_quality_score << "\n";
    oss << "EconomyScore: " << economy_score << "\n";
    oss << "EnvironmentScore: " << environment_score << "\n";

    for (const auto &facility : facilities) {
        oss << "FacilityName: " << facility->getName() << "\n";
        oss << "FacilityStatus: ";
        switch (facility->getStatus()) {
        case FacilityStatus::UNDER_CONSTRUCTION:
            oss << "UNDER_CONSTRUCTION";
            break;
        case FacilityStatus::OPERATIONAL:
            oss << "OPERATIONAL";
            break;
        }
        oss << "\n";
    }

    return oss.str();
}

//Helper method to clear resources
void Plan::clearResources()
{
    for (auto facility : facilities)
    {
        delete facility;
    }
    facilities.clear(); 

    for (auto facility : underConstruction)
    {
        delete facility;
    }
    underConstruction.clear();

    if (selectionPolicy)
    {
        delete selectionPolicy;
        selectionPolicy = nullptr;
    }
}


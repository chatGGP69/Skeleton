
#include "SelectionPolicy.h"
#include <algorithm>
#include <climits>
using std::vector;
using namespace std;

// NaiveSelection
NaiveSelection::NaiveSelection()
    : lastSelectedIndex(0)
{
}

const FacilityType &NaiveSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    if (facilitiesOptions.empty())
    {
        throw runtime_error("No facilities available for selection.");
    }

    const FacilityType &selectedFacility = facilitiesOptions[lastSelectedIndex];
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    return selectedFacility;
}

const string NaiveSelection::toString() const
{
    return "Naive Selection Policy";
}

NaiveSelection *NaiveSelection::clone() const
{
    return new NaiveSelection(*this);
}

// BalancedSelection
BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore)
    : LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore) {}

const FacilityType &BalancedSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{

    if (facilitiesOptions.empty())
    {
        throw runtime_error("No facilities available for selection.");
    }

    const FacilityType *bestFacility = &facilitiesOptions[0];
    int bestBalance = INT_MAX;
    for (const FacilityType &curr : facilitiesOptions)
    {
        int possibleLifeQualityScore = curr.getLifeQualityScore() + this->LifeQualityScore;
        int possibleEconomyScore = curr.getEconomyScore() + this->EconomyScore;
        int possibleEnvironmentScore = curr.getEnvironmentScore() + this->EnvironmentScore;

        int maxScore = max({possibleLifeQualityScore, possibleEnvironmentScore, possibleEconomyScore});
        int minScore = min({possibleLifeQualityScore, possibleEnvironmentScore, possibleEconomyScore});
        int balance = maxScore - minScore;

        if (balance < bestBalance)
        {
            bestBalance = balance;
            bestFacility = &curr;
        }
    }

    return *bestFacility;
}

const string BalancedSelection::toString() const
{
    return "Balanced Selection Policy";
}

BalancedSelection *BalancedSelection::clone() const
{
    return new BalancedSelection(*this);
}

// EconomySelection

EconomySelection::EconomySelection()
    : lastSelectedIndex(0) {}

const FacilityType &EconomySelection::selectFacility(const std::vector<FacilityType> &facilitiesOptions)
{
    if (facilitiesOptions.empty())
    {
        throw std::runtime_error("No facilities available for selection.");
    }

    size_t startIndex = lastSelectedIndex;
    do
    {
        const FacilityType &selectedFacility = facilitiesOptions[lastSelectedIndex];
        lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();

        if (selectedFacility.getCategory() == FacilityCategory::ECONOMY)
        {
            return selectedFacility;
        }
    } while (lastSelectedIndex != startIndex);

    throw std::runtime_error("No ECONOMY facility found.");
}

const string EconomySelection::toString() const
{
    return "Economy Selection Policy";
}

EconomySelection *EconomySelection::clone() const
{
    return new EconomySelection(*this);
}

// SustainabiltiySelection
SustainabilitySelection::SustainabilitySelection()
    : lastSelectedIndex(0) {}

const FacilityType &SustainabilitySelection::selectFacility(const std::vector<FacilityType> &facilitiesOptions)
{
    if (facilitiesOptions.empty())
    {
        throw std::runtime_error("No facilities available for selection.");
    }

    size_t startIndex = lastSelectedIndex;
    do
    {
        const FacilityType &selectedFacility = facilitiesOptions[lastSelectedIndex];
        lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();

        if (selectedFacility.getCategory() == FacilityCategory::ENVIRONMENT)
        {
            return selectedFacility;
        }
    } while (lastSelectedIndex != startIndex);

    throw std::runtime_error("No Enviroment facility found.");
}

const string SustainabilitySelection::toString() const
{
    return "Sustanability Selection Policy";
}

SustainabilitySelection *SustainabilitySelection::clone() const
{
    return new SustainabilitySelection(*this);
}

#include "SelectionPolicy.h"
#include "Facility.h"
#include <cassert>
#include <iostream>
#include <vector>

// Test NaiveSelection
void testNaiveSelection()
{
    NaiveSelection naive;
    std::vector<FacilityType> facilities = {
        FacilityType("Facility1", FacilityCategory::LIFE_QUALITY, 100, 10, 20, 30),
        FacilityType("Facility2", FacilityCategory::ECONOMY, 150, 15, 25, 35),
    };

    assert(naive.selectFacility(facilities).getName() == "Facility1");
    assert(naive.selectFacility(facilities).getName() == "Facility2");
    assert(naive.selectFacility(facilities).getName() == "Facility1"); // Wrap-around
}

// Test BalancedSelection
void testBalancedSelection()
{
    BalancedSelection balanced(5, 5, 5);
    std::vector<FacilityType> facilities = {
        FacilityType("Facility1", FacilityCategory::LIFE_QUALITY, 100, 10, 20, 30),
        FacilityType("Facility2", FacilityCategory::ECONOMY, 150, 15, 25, 35),
    };

    assert(balanced.selectFacility(facilities).getName() == "Facility1"); // Best-balanced facility
}

// Test EconomySelection
void testEconomySelection()
{
    EconomySelection economy;
    std::vector<FacilityType> facilities = {
        FacilityType("Facility1", FacilityCategory::LIFE_QUALITY, 100, 10, 20, 30),
        FacilityType("Facility2", FacilityCategory::ECONOMY, 150, 15, 25, 35),
        FacilityType("Facility3", FacilityCategory::ENVIRONMENT, 200, 20, 10, 40),
    };

    assert(economy.selectFacility(facilities).getName() == "Facility2");

    try
    {
        economy.selectFacility({});
        assert(false); // This should not be reached
    }
    catch (const std::runtime_error &)
    {
        // Expected exception
    }
}

// Test SustainabilitySelection
void testSustainabilitySelection()
{
    SustainabilitySelection sustainability;
    std::vector<FacilityType> facilities = {
        FacilityType("Facility1", FacilityCategory::ECONOMY, 100, 10, 20, 30),
        FacilityType("Facility2", FacilityCategory::ENVIRONMENT, 150, 15, 25, 35),
    };

    assert(sustainability.selectFacility(facilities).getName() == "Facility2");

    try
    {
        sustainability.selectFacility({});
        assert(false); // This should not be reached
    }
    catch (const std::runtime_error &)
    {
        // Expected exception
    }
}

int main()
{
    try
    {
        testNaiveSelection();
        std::cout << "[PASSED] NaiveSelection Test" << std::endl;

        testBalancedSelection();
        std::cout << "[PASSED] BalancedSelection Test" << std::endl;

        testEconomySelection();
        std::cout << "[PASSED] EconomySelection Test" << std::endl;

        testSustainabilitySelection();
        std::cout << "[PASSED] SustainabilitySelection Test" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[FAILED] Test failed: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "[FAILED] Test failed: Unknown error" << std::endl;
        return 1;
    }

    return 0;
}

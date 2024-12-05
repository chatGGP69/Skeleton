#include "Facility.h"
#include <iostream>

void testFacility()
{
    // Test Case 1: Valid Facility Initialization
    std::cout << "Testing Valid Facilities:" << std::endl;
    Facility hospital("Hospital", "Green Village", FacilityCategory::LIFE_QUALITY, 10, 5, 0, 0);
    Facility factory("Factory", "Sun City", FacilityCategory::ECONOMY, 15, 0, 8, 0);
    Facility park("Park", "Mega Metropolis", FacilityCategory::ENVIRONMENT, 12, 0, 0, 10);

    std::cout << hospital.toString() << std::endl;
    std::cout << factory.toString() << std::endl;
    std::cout << park.toString() << std::endl;

    // Test Case 2: Facility Lifecycle (step method)
    std::cout << "\nTesting Facility Lifecycle:" << std::endl;
    for (int i = 0; i < 10; ++i)
    {
        std::cout << "Step " << i + 1 << ": " << hospital.toString() << std::endl;
        hospital.step(); // Simulate construction progress
    }

    // Ensure status is now OPERATIONAL
    std::cout << "Final Status: " << hospital.toString() << " (Expected: OPERATIONAL)" << std::endl;

    // Test Case 3: Unknown Category and Status
    std::cout << "\nTesting Unknown Facility:" << std::endl;
    Facility unknown("Unknown Facility", "Unknown Settlement", static_cast<FacilityCategory>(-1), 0, 0, 0, 0);
    std::cout << unknown.toString() << " (Expected: Unknown Category, Status: UNDER_CONSTRUCTIONS)" << std::endl;

    // Test Case 4: Manual Status Change
    std::cout << "\nTesting Manual Status Change:" << std::endl;
    park.setStatus(FacilityStatus::OPERATIONAL);
    std::cout << park.toString() << " (Expected: OPERATIONAL)" << std::endl;
}

int main()
{
    testFacility();
    return 0;
}

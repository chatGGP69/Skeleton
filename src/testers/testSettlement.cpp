#include "Settlement.h"
#include <iostream>

void testSettlement()
{
    // Test Case 1: Valid Settlements
    Settlement village("Green Village", SettlementType::VILLAGE);
    Settlement city("Sun City", SettlementType::CITY);
    Settlement metropolis("Mega Metropolis", SettlementType::METROPOLIS);

    std::cout << "Testing Valid Settlements:" << std::endl;
    std::cout << village.toString() << " (Expected: Settlement Name: Green Village, Type: Village)" << std::endl;
    std::cout << city.toString() << " (Expected: Settlement Name: Sun City, Type: City)" << std::endl;
    std::cout << metropolis.toString() << " (Expected: Settlement Name: Mega Metropolis, Type: Metropolis)" << std::endl;

    // Test Case 2: Unknown Settlement Type
    Settlement unknown("Mystery Settlement", static_cast<SettlementType>(-1));
    std::cout << "Testing Unknown Settlement Type:" << std::endl;
    std::cout << unknown.toString() << " (Expected: Settlement Name: Mystery Settlement, Type: Unknown Settlement type)" << std::endl;
}

int main()
{
    testSettlement();
    return 0;
}

#include "double_unit_costs.h"
#include "random_costs.h"
#include <algorithm>
#include <random>
#include <string>
#include <vector>
#include <list>
#include "genie/dat/DatFile.h"
#include "ids.h"

// These cost functions (and those in random_costs.h/cpp) probably ought to
// migrate to a shared "costs.h"

// typedef genie::ResourceUsage<int16_t, int16_t, int16_t> ResourceCost; // defined in random_costs.h
ResourceCost toDoubledResourceCost(const ResourceCost &resourceCost) {
    ResourceCost doubledCost;
    doubledCost.Type = resourceCost.Type;
    doubledCost.Amount = resourceCost.Amount * 2;
    doubledCost.Flag = (bool)resourceCost.Flag;
    return doubledCost;
}

std::vector<ResourceCost> toDoubledResourceCosts(const std::vector<ResourceCost> &resourceCosts) {
    std::vector<ResourceCost> doubledCosts;
    doubledCosts.reserve(resourceCosts.size());
    for (const ResourceCost &resourceCost : resourceCosts) {
        doubledCosts.push_back(toDoubledResourceCost(resourceCost));
    }
    return doubledCosts;
}

void doubleUnitCosts(genie::DatFile *df) {
    std::vector<int> unitIds;

    for (genie::Unit unit : df->Civs.at(0).Units) {
        if (hasNaturalResourceCost(unit)) {
            unitIds.push_back(unit.ID);
        }
    }

    for (int unitId : unitIds) {

        std::vector<ResourceCost> res = df->Civs.at(0).Units.at(unitId).Creatable.ResourceCosts;
        std::vector<ResourceCost> doubled = toDoubledResourceCosts(res);

        std::cout << "Setting cost of unit with id " << unitId << " to " << costToString(doubled)
                  << std::endl;
        if (!bothRequirePopulationHeadroom(unitId, doubled, df->Civs.at(0))) {
            copyResourceCostAt(unitId, 2, doubled, df->Civs.at(0));
        }
        for (genie::Civ &civ : df->Civs) {
            civ.Units.at(unitId).Creatable.ResourceCosts = doubled;
        }
    }
}

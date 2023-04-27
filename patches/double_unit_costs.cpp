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
//ResourceCost toDoubledResourceCost(const ResourceCost &resourceCost) {
ResourceCost scaledResourceCost(const ResourceCost &resourceCost, const float scale) {
    ResourceCost scaledCost;
    scaledCost.Type = resourceCost.Type;
    scaledCost.Amount = resourceCost.Amount * scale;
    scaledCost.Flag = (bool)resourceCost.Flag;
    return scaledCost;
}

std::vector<ResourceCost> scaledResourceCosts(const std::vector<ResourceCost> &resourceCosts, const float scale) {
    std::vector<ResourceCost> scaledCosts;
    scaledCosts.reserve(resourceCosts.size());
    for (const ResourceCost &resourceCost : resourceCosts) {
        scaledCosts.push_back(scaledResourceCost(resourceCost, scale));
    }
    return scaledCosts;
}


// TODO: Generic this to "scaleUnitCosts(genie::DatFile *df, scale), and then
// implement "2" or whatever as doubleUnitCosts.  We should have separate building
// and/or unit scaling.
void doubleUnitCosts(genie::DatFile *df) {
    std::vector<int> unitIds;

    for (genie::Unit unit : df->Civs.at(0).Units) {
        //if (hasNaturalResourceCost(unit) and unit.Type == genie::UnitType::UT_Building) { // works
        if (hasNaturalResourceCost(unit) and unit.Type == genie::UnitType::UT_Creatable){  // Units
            //std::cout << "Type" + unit.Type << std::endl;
            unitIds.push_back(unit.ID);
        }
    }

    for (int unitId : unitIds) {

        std::vector<ResourceCost> res = df->Civs.at(0).Units.at(unitId).Creatable.ResourceCosts;
        std::vector<ResourceCost> doubled = scaledResourceCosts(res, 2.0); // Scale here

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

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

/*
std::string costToString(const std::vector<ResourceCost> &costs) {
    std::string s;
    for (const ResourceCost &cost : costs) {
        if (cost.Flag == 1) {
            s += std::to_string(cost.Amount);
            s += " ";
            switch (cost.Type) {
            case TYPE_FOOD:
                s += "Food ";
                break;
            case TYPE_WOOD:
                s += "Wood ";
                break;
            case TYPE_GOLD:
                s += "Gold ";
                break;
            case TYPE_STONE:
                s += "Stone ";
                break;
            case TYPE_POPULATION_HEADROOM:
                s += "Pop ";
                break;
            default:
                s += "vat?";
            }
        }
    }
    return s;
}


std::string costToString(const std::vector<ResearchResourceCost> &costs) {
    std::string s;
    for (const ResearchResourceCost &cost : costs) {
        if (cost.Flag == 1) {
            s += std::to_string(cost.Amount);
            s += " ";
            switch (cost.Type) {
            case TYPE_FOOD:
                s += "Food ";
                break;
            case TYPE_WOOD:
                s += "Wood ";
                break;
            case TYPE_GOLD:
                s += "Gold ";
                break;
            case TYPE_STONE:
                s += "Stone ";
                break;
            case TYPE_POPULATION_HEADROOM:
                s += "Pop ";
                break;
            default:
                s += "vat?";
            }
        }
    }
    return s;
}


bool bothRequirePopulationHeadroom(int unitId, std::vector<ResourceCost> &resourceCosts, const genie::Civ &civ) {
    return ((civ.Units.at(unitId).Creatable.ResourceCosts.at(2).Type == TYPE_POPULATION_HEADROOM &&
             resourceCosts.at(2).Type == TYPE_POPULATION_HEADROOM) ||
            (civ.Units.at(unitId).Creatable.ResourceCosts.at(2).Type != TYPE_POPULATION_HEADROOM &&
             resourceCosts.at(2).Type != TYPE_POPULATION_HEADROOM));
}


void copyResourceCostAt(int unitId, int index, std::vector<ResourceCost> &target, const genie::Civ &civ) {
    genie::ResourceUsage<int16_t, int16_t, int16_t> source = civ.Units.at(unitId).Creatable.ResourceCosts.at(index);
    target.at(index).Type = source.Type;
    target.at(index).Amount = source.Amount;
    target.at(index).Flag = source.Flag;
}

*/
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

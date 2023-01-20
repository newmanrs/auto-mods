#ifndef RANDOM_COSTS_H
#define RANDOM_COSTS_H

#include "genie/dat/DatFile.h"

typedef genie::ResourceUsage<int16_t, int16_t, int16_t> ResourceCost;
typedef genie::ResourceUsage<int16_t, int16_t, uint8_t> ResearchResourceCost;

bool isNaturalResourceCost(const ResourceCost &cost);
bool hasNaturalResourceCost(const genie::Unit &unit);
int getSumOfNaturalResourceCosts(const std::vector<ResourceCost> &resourceCosts);
std::string costToString(const std::vector<ResourceCost> &costs);
bool bothRequirePopulationHeadroom(int unitId, std::vector<ResourceCost> &resourceCosts, const genie::Civ &civ);
void copyResourceCostAt(int unitId, int index, std::vector<ResourceCost> &target, const genie::Civ &civ);

void jumbleCosts(genie::DatFile *df);
void enableStablesForMesoCivs(genie::DatFile *df);
void jumbleUnitCosts(genie::DatFile *df);
void jumbleTechCosts(genie::DatFile *df);

#endif  // RANDOM_COSTS_H

#include "L1Trigger/L1TMuonBarrel/interface/L1TMuonBarrelParamsAccessible.h"

const L1TMuonBarrelParamsAccessible& cast_to_L1TMuonBarrelParamsAccessible(const L1TMuonBarrelParams& a)
{
	assert(sizeof(L1TMuonBarrelParamsAccessible) == sizeof(L1TMuonBarrelParams));
	const void * pa = &a;
	const L1TMuonBarrelParamsAccessible * py = static_cast<const L1TMuonBarrelParamsAccessible *>(pa);
	return *py;
}

const L1TMuonBarrelParams& cast_to_L1TMuonBarrelParams(const L1TMuonBarrelParamsAccessible& a)
{
	assert(sizeof(L1TMuonBarrelParamsAccessible) == sizeof(L1TMuonBarrelParams));
	const void * pa = &a;
	const L1TMuonBarrelParams * py = static_cast<const L1TMuonBarrelParams *>(pa);
	return *py;
}
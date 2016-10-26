#include "FWCore/Framework/interface/MakerMacros.h"
#include "EventFilter/L1TRawToDigi/interface/Unpacker.h"

#include "BMTFCollections.h"

namespace l1t{
	namespace stage2{
		struct qualityHits
		{
			int linkNo, secNo, wheel;
			int hits[3][7];
		};
		class BMTFUnpackerInputsOldQual : public Unpacker
		{
			public:
				virtual bool unpack(const Block& block, UnpackerCollections *coll) override;
			private:
				//qualityHits linkAndQual_;
				std::map<std::string, qualityHits> linkAndQual_;
		};

		class BMTFUnpackerInputsNewQual : public Unpacker
		{
			public:
				virtual bool unpack(const Block& block, UnpackerCollections *coll) override;
			private:
				//qualityHits linkAndQual_;
				std::map<std::string, qualityHits> linkAndQual_;
		};

	}
}

DEFINE_L1T_UNPACKER(l1t::stage2::BMTFUnpackerInputsOldQual);
DEFINE_L1T_UNPACKER(l1t::stage2::BMTFUnpackerInputsNewQual);

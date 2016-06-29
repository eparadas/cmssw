#include "BMTFPackerOutput.h"

// Implementation
namespace l1t 
{
   namespace stage2 
   {
      Blocks BMTFPackerOutput::pack(const edm::Event& event, const PackerTokens* toks)
      {	 

	 	auto muonToken = static_cast<const BMTFTokens*>(toks)->getOutputMuonToken();

        Blocks blocks;
        
		edm::Handle<RegionalMuonCandBxCollection> muons;
        event.getByToken(muonToken, muons);

        for(int ibx = muons->getFirstBX(); ibx <= muons->getLastBX(); ibx++)
        {
        	for (auto imu = muons->begin(); imu != muons->end(); imu++)
        	{
        		uint32_t firstWord(0), lastWord(0);
        		RegionalMuonRawDigiTranslator::generatePackedDataWords(*imu, firstWord, lastWord);
        		payloadMap_[imu->link()*2+1].push_back(firstWord);
        		payloadMap_[imu->link()*2+1].push_back(lastWord);
        	}
        	if (muons->size()<3)
        	{
        		for(unsigned int j = 0; j < 3-muons->size(); j++)
        			payloadMap_[123].push_back(0);
        	}

        }

        for (auto &ip: payloadMap_)
        	blocks.push_back(Block(ip.first, ip.second));
         

         return blocks;
      }

	}//ns stage2
}//ns l1t
DEFINE_L1T_PACKER(l1t::stage2::BMTFPackerOutput);
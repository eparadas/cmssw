#include "BMTFUnpackerInputs.h"

#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambPhContainer.h"
#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambThContainer.h"

namespace l1t
{
	namespace stage2
	{
		void numWheelSectorTrTag(int& wheelNo, int& sectorNo, int& tagSegID, int linkNo, int amcNo)
		{
			if (linkNo >= 0 && linkNo < 6)
				wheelNo = -2;
			else if (linkNo >= 8 && linkNo < 14)
				wheelNo = -1;
			else if (linkNo >= 16 && linkNo < 22)
				wheelNo = 0;	
			else if (linkNo >= 22 && linkNo < 28)
				wheelNo = 1;
			else if ( (linkNo >= 28 && linkNo < 29) || (linkNo >= 32 && linkNo < 36))
				wheelNo = 2;
			
			sectorNo = amcNo;
			
			if ( linkNo%2 == 0 )
				tagSegID = 0;
			else
				tagSegID = 1;
		}
			
		bool BMTFUnpackerInputs::unpack(const Block& block, UnpackerCollections *coll)
		{
			unsigned int blockId = block.header().getID();
			LogDebug("L1T") << "Block ID: " << blockId << " size: " << block.header().getSize();
			auto payload = block.payload();
			int nBX, firstBX, lastBX;
		
			nBX = int(ceil(block.header().getSize()/6));
			getBXRange(nBX, firstBX, lastBX);

			LogDebug("L1T") << "BX override. Set firstBX = lastBX = 0";
			
			L1MuDTChambPhContainer *resPhi;
			// std::cout << "Checking link: " << blockId << std::endl;
			// std::cout << "Address before is: " << resPhi << std::endl;
			L1MuDTChambThContainer *resThe;
			resPhi = static_cast<BMTFCollections*>(coll)->getInMuonsPh();
			resThe = static_cast<BMTFCollections*>(coll)->getInMuonsTh();
			// std::cout << "Address after is: " << resPhi << std::endl;
			L1MuDTChambPhContainer::Phi_Container phi_data = *(resPhi->getContainer()); 
			
			
			L1MuDTChambThContainer::The_Container the_data;
			
			for(int ibx = firstBX; ibx <= lastBX; ibx++)
			{
				uint32_t inputWords[block.header().getSize()/nBX];
				
				for(unsigned int iw = 0; iw < block.header().getSize()/nBX; iw++)
					inputWords[iw] = payload[iw+(ibx+lastBX)*6];
			
				int wheel, sector, trTag;
				numWheelSectorTrTag(wheel, sector, trTag, blockId/2, block.amc().getAMCNumber());
				
				int mbPhi[4], mbPhiB[4], mbQual[4], mbBxC[4], mbRPC[4];
				mbPhiB[2] = 0;
				
				for (int iw = 0; iw < 4; iw++)
				{
					if ( ((inputWords[iw] & 0xfffffff) == 0) || (inputWords[iw] == 0x505050bc) ) 
						continue;
					else if ( (inputWords[iw] != 0x505050bc) && (inputWords[iw+2] == 0x505050bc) )
						continue;
						
					
					if ( ((inputWords[iw] >> 11) & 0x1) == 1 )
						mbPhi[iw] = (inputWords[iw] & 0x7FF ) - 2048;
					else
						mbPhi[iw] = (inputWords[iw] & 0xFFF );
						
					if ( iw != 2)
					{
						if ( ((inputWords[iw] >> 21) & 0x1) == 1 )
							mbPhiB[iw] = ( (inputWords[iw] >> 12) & 0x1FF ) - 512;
						else
							mbPhiB[iw] = (inputWords[iw] >> 12) & 0x3FF;
						mbQual[iw] = (inputWords[iw] >> 22) & 0xF;
						mbRPC[iw] = (inputWords[iw] >> 26) & 0x1;
						mbBxC[iw] = (inputWords[iw] >> 30) & 0x3;
					}
					else	
					{
						mbQual[iw] = (inputWords[iw] >> 12) & 0xF;
						mbRPC[iw] = (inputWords[iw] >> 16) & 0x1;
						mbBxC[iw] = (inputWords[iw] >> 20) & 0x3;
					}
	
					if (mbQual[iw] == 0)
						continue;
					
					// std::cout << iw+1 << "\tWord: " << std::hex << inputWords[iw] << std::dec << "\tLink: " << blockId/2 << "\tamc: " << block.amc().getAMCNumber() << "\twheel: " << wheel << "\tsector: " << sector << std::endl;
					phi_data.push_back( L1MuDTChambPhDigi( ibx, wheel, sector, iw+1, mbPhi[iw], mbPhiB[iw], mbQual[iw], trTag, mbBxC[iw], mbRPC[iw] ) );
					// std::cout << iw+1 << "\tAfter push: " << std::hex << inputWords[iw] << std::dec << "\tLink: " << blockId/2 << "\tamc: " << block.amc().getAMCNumber() << "\twheel: " << phi_data.back().whNum() << "\tsector: " << phi_data.back().scNum() << std::endl;
				}//iw
				int mbEta[3];//, mbEtaBxC;
				for (int i = 0; i < 3; i++)
					mbEta[i] = (inputWords[4] >> (i*7 + 1)) & 0xFF; 
				
				the_data.push_back(L1MuDTChambThDigi( ibx, wheel, sector, 3, mbEta) );
				

				// std::cout << "phi_data size: " << phi_data.size() << std::endl;

			}//ibx
			// std::cout << "Final size: " << phi_data.size() << std::endl;
			// std::cout << "Address before set is: " << resPhi << std::endl;
			resPhi->setContainer(phi_data);
			// std::cout << "Address after set is: " << resPhi << std::endl;
			if (phi_data.size() != 0)
				// std::cout << "\tAfter set: " << "\twheel: " << phi_data.back().whNum() << "\tsector: " << phi_data.back().scNum() << std::endl;
			resThe->setContainer(the_data);
			
		return true;
		}//unpack
	}//ns2
}//ns l1t;
			

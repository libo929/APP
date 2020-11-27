/**
 *  @file   ExampleContent/src/ExampleAlgorithms/PrepareCaloHitsAlgorithm.cc
 * 
 *  @brief  Implementation of the create additional calo hits algorithm class.
 * 
 *  $Log: $
 */

#include "TFile.h"
#include "TTree.h"

#include "Pandora/AlgorithmHeaders.h"
#include "ExampleAlgorithms/PrepareCaloHitsAlgorithm.h"
#include "ExampleObjects/ExampleCaloHit.h"



using namespace pandora;

namespace example_content
{

april_content::CaloHitFactory PrepareCaloHitsAlgorithm::m_pCaloHitFactory;

int PrepareCaloHitsAlgorithm::m_nEvent = 0;

PrepareCaloHitsAlgorithm::PrepareCaloHitsAlgorithm() :
    m_setCurrentListToInputList(false),
    m_worldSideLength(1000.f),
    m_groupSideLength(10.f),
	m_energyThreshold(0.),
	m_timeThreshold(0.),
    m_randomEngine(12345)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

pandora::StatusCode PrepareCaloHitsAlgorithm::Run()
{
#if 1
	TFile* f = new TFile(m_inputFileName.c_str());
	TTree* inTree = (TTree*)gDirectory->Get("ce");

	int evtIndex;
	int hitSize;

	std::vector<int>* cellVecX = 0;
	std::vector<int>* cellVecY = 0;
	std::vector<int>* cellVecZ = 0;

	std::vector<float>* posVecX = 0;
	std::vector<float>* posVecY = 0;
	std::vector<float>* posVecZ = 0;

	std::vector<float>* energyVec = 0;
	//std::vector<float>* timeVec = 0;

#if 0
	std::vector<int>* fromIDVec  = new std::vector<int>;
	std::vector<float>* fromEVec = new std::vector<float>;
#endif

	/////////////////////////////////////////////////////
	inTree->SetBranchAddress("evtIndex",  &evtIndex);
	inTree->SetBranchAddress("hitSize",   &hitSize);

#if 1
	inTree->SetBranchAddress("cellVecX",  &cellVecX);
	inTree->SetBranchAddress("cellVecY",  &cellVecY);
	inTree->SetBranchAddress("cellVecZ",  &cellVecZ);
#endif

	inTree->SetBranchAddress("posVecX",   &posVecX);
	inTree->SetBranchAddress("posVecY",   &posVecY);
	inTree->SetBranchAddress("posVecZ",   &posVecZ);

	inTree->SetBranchAddress("energyVec", &energyVec);
	//inTree->SetBranchAddress("timeVec",   &timeVec);

#if 0
	inTree->SetBranchAddress("fromIDVec",  &fromIDVec);
	inTree->SetBranchAddress("fromIDEVec", &fromEVec);
#endif


	//for(unsigned int i= 0; i < inTree->GetEntries(); ++i)
	if(m_nEvent<inTree->GetEntries()) 
	{
		inTree->GetEntry(m_nEvent++);

#if 1
		std::cout << " ~~~~~~~~~~~~~~~~~~****************** evtIndex: " << evtIndex 
			      << ", hit size: " << hitSize << std::endl;
#endif

		for(unsigned int ihit = 0; ihit < hitSize; ++ ihit)
		{
#if 0
			std::cout << "  --- hit cell: " << cellVecX->at(ihit) << ", " << cellVecY->at(ihit) << ", " << cellVecZ->at(ihit)
					  << ", pos: "  << posVecX->at(ihit) << ", " << posVecY->at(ihit) << ", " <<  posVecZ->at(ihit)
					  << ", energy: " << energyVec->at(ihit) 
				      << ", time: " << timeVec->at(ihit) << std::endl;
#endif

			//if(int(posVecZ->at(ihit)/5.5)>=2) continue;

			if(energyVec->at(ihit) < m_energyThreshold) continue;
			//if(timeVec->at(ihit) < m_timeThreshold) continue;

            const CartesianVector hitPosition( posVecX->at(ihit), posVecY->at(ihit), posVecZ->at(ihit) ); 

            // Mainly dummy parameters
            ExampleCaloHitParameters parameters;
            parameters.m_positionVector = hitPosition;
            parameters.m_expectedDirection = CartesianVector(0.f, 0.f, 1.f);
            parameters.m_cellNormalVector = CartesianVector(0.f, 0.f, 1.f);
            parameters.m_cellGeometry = RECTANGULAR;
			
			// -> real parameters
            //parameters.m_cellSize0 = 10.1f;
            //parameters.m_cellSize1 = 10.1f;
            //parameters.m_cellThickness = 5.5f;
			// -> for display
            parameters.m_cellSize0 = 5.f;
            parameters.m_cellSize1 = 5.f;
            parameters.m_cellThickness = 3.f;

			//const float toGeV = 0.001;

			//float alpha = 0.06071;
			float alpha = 57.292472;
			float beta = 0.;
			float hitE = energyVec->at(ihit) * alpha + beta;

            parameters.m_nCellRadiationLengths = 1.f;
            parameters.m_nCellInteractionLengths = 1.f;
            parameters.m_time = 0.001;//timeVec->at(ihit);
            parameters.m_inputEnergy = hitE;
            parameters.m_mipEquivalentEnergy = 1.f;
            parameters.m_electromagneticEnergy = hitE;
			//std::cout << __LINE__ << ", " << hitE  << std::endl;
#if 0
            //parameters.m_hadronicEnergy = fromIDVec->at(ihit);
#endif

			//if(cellVecZ->at(ihit) > 2) continue;

            parameters.m_hadronicEnergy = hitE;
            parameters.m_isDigital = false;
            parameters.m_hitType = pandora::ECAL;
            parameters.m_hitRegion = SINGLE_REGION;
            parameters.m_layer = int(cellVecZ->at(ihit));
            parameters.m_isInOuterSamplingLayer = false;
            parameters.m_pParentAddress = (void*)(static_cast<uintptr_t>(ihit));

			//std::cout << " --- " << posVecX->at(ihit) << " " << posVecY->at(ihit) << " " << 12520 + posVecZ->at(ihit) << std::endl;

			//parameters.m_fromID = fromIDVec->at(ihit);
			//parameters.m_fromE = fromEVec->at(ihit);

            const CaloHit *pCaloHit(nullptr);
            PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::CaloHit::Create(*this, parameters, pCaloHit, m_pCaloHitFactory));

			//std::cout << "hit PseudoLayer: " << pCaloHit->GetPseudoLayer() << std::endl;
		}
	}

#if 0
	delete fromIDVec;
	delete fromEVec;
#endif

	delete f;
#endif

    // Newly created calo hits always appear in the Input list. This list may not necessarily be the current list. The API below
    // will replace the current calo hit list with the Input list for this and subsequent algorithms.
    if (m_setCurrentListToInputList)
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::ReplaceCurrentList<CaloHit>(*this, "Input"));

    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode PrepareCaloHitsAlgorithm::ReadSettings(const TiXmlHandle xmlHandle)
{
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle,
        "SetCurrentListToInputList", m_setCurrentListToInputList));

    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "WorldSideLength", m_worldSideLength));

    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "GroupSideLength", m_groupSideLength));

    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "EnergyThreshold", m_energyThreshold));

    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "TimeThreshold", m_timeThreshold));

    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle,
        "InputFileName", m_inputFileName));

    return STATUS_CODE_SUCCESS;
}

} // namespace example_content

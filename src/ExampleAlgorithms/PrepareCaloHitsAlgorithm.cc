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
	TFile* f = new TFile("calo.root");
	TTree* inTree = (TTree*)gDirectory->Get("ce");

	int evtIndex;
	int hitSize;

	std::vector<int>* cellVecX = new std::vector<int>;
	std::vector<int>* cellVecY = new std::vector<int>;
	std::vector<int>* cellVecZ = new std::vector<int>;

	std::vector<float>* posVecX = new std::vector<float>;
	std::vector<float>* posVecY = new std::vector<float>;
	std::vector<float>* posVecZ = new std::vector<float>;

	std::vector<float>* energyVec = new std::vector<float>;
	std::vector<float>* timeVec = new std::vector<float>;

	/////////////////////////////////////////////////////
	inTree->SetBranchAddress("evtIndex",  &evtIndex);
	inTree->SetBranchAddress("hitSize",   &hitSize);

	inTree->SetBranchAddress("cellVecX",  &cellVecX);
	inTree->SetBranchAddress("cellVecY",  &cellVecY);
	inTree->SetBranchAddress("cellVecZ",  &cellVecZ);

	inTree->SetBranchAddress("posVecX",   &posVecX);
	inTree->SetBranchAddress("posVecY",   &posVecY);
	inTree->SetBranchAddress("posVecZ",   &posVecZ);

	inTree->SetBranchAddress("energyVec", &energyVec);
	inTree->SetBranchAddress("timeVec",   &timeVec);

    //ExampleCaloHitFactory exampleCaloHitFactory;
    std::uniform_real_distribution<float> randomDistribution(0.f, 1.f);

	//for(unsigned int i= 0; i < 1; ++i)

	//for(unsigned int i= 0; i < inTree->GetEntries(); ++i)
	{
		inTree->GetEntry(m_nEvent++);

#if 0
		std::cout << "evtIndex: " << evtIndex << ", hit size: " << hitSize << 
			", timeVec size: " << timeVec->size() << std::endl;
#endif

		for(unsigned int ihit = 0; ihit < timeVec->size(); ++ ihit)
		{
#if 0
			std::cout << "  --- hit cell: " << cellVecX->at(ihit) << ", " << cellVecY->at(ihit) << ", " << cellVecZ->at(ihit)
					  << ", pos: "  << posVecX->at(ihit) << ", " << posVecY->at(ihit) << ", " <<  posVecZ->at(ihit)
					  << ", energy: " << energyVec->at(ihit) 
				      << ", time: " << timeVec->at(ihit) << std::endl;
#endif

			if(energyVec->at(ihit) < m_energyThreshold) continue;
			if(timeVec->at(ihit) < m_timeThreshold) continue;

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
            parameters.m_cellSize0 = 9.f;
            parameters.m_cellSize1 = 9.f;
            parameters.m_cellThickness = 3.f;

            parameters.m_nCellRadiationLengths = 1.f;
            parameters.m_nCellInteractionLengths = 1.f;
            parameters.m_time = timeVec->at(ihit);
            parameters.m_inputEnergy = energyVec->at(ihit) * 0.001; // to GeV
            parameters.m_mipEquivalentEnergy = 1.f;
            parameters.m_electromagneticEnergy = energyVec->at(ihit) * 0.001;
            parameters.m_hadronicEnergy = energyVec->at(ihit) * 0.001;
            parameters.m_isDigital = false;
            parameters.m_hitType = pandora::ECAL;
            parameters.m_hitRegion = SINGLE_REGION;
            parameters.m_layer = cellVecZ->at(ihit);
            parameters.m_isInOuterSamplingLayer = false;
            parameters.m_pParentAddress = (void*)(static_cast<uintptr_t>(ihit));

            const CaloHit *pCaloHit(nullptr);
            PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::CaloHit::Create(*this, parameters, pCaloHit, m_pCaloHitFactory));

			//std::cout << "hit PseudoLayer: " << pCaloHit->GetPseudoLayer() << std::endl;
		}
	}

	delete cellVecX;
	delete cellVecY;
	delete cellVecZ;

	delete posVecX;
	delete posVecY;
	delete posVecZ;

	delete energyVec;
	delete timeVec;

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

    return STATUS_CODE_SUCCESS;
}

} // namespace example_content

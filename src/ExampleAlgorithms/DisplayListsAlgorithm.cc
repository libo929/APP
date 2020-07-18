/**
 *  @file   ExampleContent/src/ExampleAlgorithms/DisplayListsAlgorithm.cc
 * 
 *  @brief  Implementation of the display lists algorithm class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "ExampleAlgorithms/DisplayListsAlgorithm.h"

#include "ExampleObjects/ExampleCaloHit.h"

using namespace pandora;

namespace example_content
{

DisplayListsAlgorithm::DisplayListsAlgorithm() :
    m_displayCurrentCaloHits(false),
    m_displayCurrentTracks(false),
    m_displayCurrentMCParticles(false),
    m_displayCurrentClusters(false),
    m_displayCurrentVertices(false),
    m_displayCurrentPfos(false),
	m_energyThreshold(0.),
	m_timeThreshold(1000.)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode DisplayListsAlgorithm::Run()
{
    // Useful event display functionality for visualizing the operations performed in other example algorithms.

    // The PANDORA_MONITORING_API preprocessor macro automatically checks whether monitoring has been enabled via the
    // PandoraSettings xml file. If the MONITORING preprocessor flag has not been defined, its arguments will collapse to no code.

    if (m_displayCurrentCaloHits)
    {
        const CaloHitList *pCaloHitList(nullptr);
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pCaloHitList));
        std::cout << "---" << pCaloHitList->size() << " calo hits in current list " << std::endl;

		CaloHitList selectedHitList;

		for(auto hit : *pCaloHitList)
		{
			if(hit->GetInputEnergy() < m_energyThreshold || hit->GetTime() > m_timeThreshold) continue;
			selectedHitList.push_back(hit);
		}

		std::cout << "selected hit: " << selectedHitList.size() << std::endl;

        // Display additional properties for any example user-defined calo hits
        for (const CaloHit *const pCaloHit : selectedHitList)
        {
            const ExampleCaloHit *const pExampleCaloHit(dynamic_cast<const ExampleCaloHit*>(pCaloHit));

            if (pExampleCaloHit)
            {
                std::cout << "------ExampleCaloHit " << pExampleCaloHit << ", additional property: " << pExampleCaloHit->GetAdditionalProperty() << std::endl;
            }
        }

		OrderedCaloHitList orderedCaloHitList;
		orderedCaloHitList.Add(selectedHitList);
		//std::cout << "OrderedCaloHitList size: " << orderedCaloHitList.size() << std::endl;

		for(auto& iter : orderedCaloHitList)
		{
			//std::cout << "layer: " << iter.first << std::endl;
			auto& caloHitListAtLayer = iter.second;

            //PANDORA_MONITORING_API(VisualizeCaloHits(this->GetPandora(), pCaloHitList, "CurrentCaloHits", GRAY));
			std::string caloListName("CurrentCaloHits@layer_");
			caloListName += std::to_string(iter.first);

            PANDORA_MONITORING_API(VisualizeCaloHits(this->GetPandora(), caloHitListAtLayer, caloListName.c_str(), 
						::AUTOITER));
	    }
    }

    if (m_displayCurrentTracks)
    {
        const TrackList *pTrackList(nullptr);
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pTrackList));
        std::cout << "---" << pTrackList->size() << " tracks in current list " << std::endl;
        PANDORA_MONITORING_API(VisualizeTracks(this->GetPandora(), pTrackList, "CurrentTracks", MAGENTA));
    }

    if (m_displayCurrentMCParticles)
    {
        const MCParticleList *pMCParticleList(nullptr);
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pMCParticleList));
        std::cout << "---" << pMCParticleList->size() << " mc particles in current list " << std::endl;
        PANDORA_MONITORING_API(VisualizeMCParticles(this->GetPandora(), pMCParticleList, "CurrentMCParticles", CYAN));
    }

    if (m_displayCurrentClusters)
    {
        const ClusterList *pClusterList(nullptr);
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pClusterList));
        std::cout << "---" << pClusterList->size() << " clusters in current list " << std::endl;

        for (const Cluster *const pCluster : *pClusterList)
        {
            std::cout << "------Cluster " << pCluster << ", nHits: " << pCluster->GetNCaloHits() << std::endl;
        }

        PANDORA_MONITORING_API(VisualizeClusters(this->GetPandora(), pClusterList, "CurrentClusterList", AUTOITER));
    }

    if (m_displayCurrentVertices)
    {
        const VertexList *pVertexList(nullptr);
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pVertexList));
        std::cout << "---" << pVertexList->size() << " vertices in current list " << std::endl;
        PANDORA_MONITORING_API(VisualizeVertices(this->GetPandora(), pVertexList, "CurrentVertexList", GREEN));
    }

    if (m_displayCurrentPfos)
    {
        const PfoList *pPfoList(nullptr);
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pPfoList));
        std::cout << "---" << pPfoList->size() << " pfos in current list " << std::endl;

        for (const Pfo *const pPfo : *pPfoList)
        {
            std::cout << "------Pfo " << pPfo << ", nClusters: " << pPfo->GetClusterList().size() << ", nVertices: " << pPfo->GetVertexList().size() << std::endl;
        }

        PANDORA_MONITORING_API(VisualizeParticleFlowObjects(this->GetPandora(), pPfoList, "CurrentPfoList", BLUE));
    }

    PANDORA_MONITORING_API(ViewEvent(this->GetPandora()));

    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode DisplayListsAlgorithm::ReadSettings(const TiXmlHandle xmlHandle)
{
    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "DisplayCurrentCaloHits", m_displayCurrentCaloHits));

    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "DisplayCurrentTracks", m_displayCurrentTracks));

    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "DisplayCurrentMCParticles", m_displayCurrentMCParticles));

    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "DisplayCurrentClusters", m_displayCurrentClusters));

    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "DisplayCurrentVertices", m_displayCurrentVertices));

    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "DisplayCurrentPfos", m_displayCurrentPfos));

    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "EnergyThreshold", m_energyThreshold));

    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "TimeThreshold", m_timeThreshold));

    return STATUS_CODE_SUCCESS;
}

} // namespace example_content

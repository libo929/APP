/**
 *  @file   ExampleContent/src/ExampleAlgorithms/AccessListsAlgorithm.cc
 * 
 *  @brief  Implementation of the access lists algorithm class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "ExampleAlgorithms/AccessListsAlgorithm.h"

using namespace pandora;

namespace example_content
{

StatusCode AccessListsAlgorithm::Run()
{
    // Access the current calo hit list. Pandora lists are std::sets, keyed on the address of the relevant objects.
    // It is possible for algorithms to access either the current list, or any saved lists with a specified name.
    const CaloHitList *pCurrentCaloHitList(nullptr);
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pCurrentCaloHitList));

	std::cout << "calo hit list: " << pCurrentCaloHitList->size() << std::endl;

    for (const CaloHit *pCaloHit : *pCurrentCaloHitList)
    {
        // Process calo hits, e.g. ...
        if (pCaloHit->GetElectromagneticEnergy() > 0.f)
        {
			const CartesianVector& hitPos = pCaloHit->GetPositionVector();

			std::cout << "hit: " << pCaloHit 
				<< ", layer: " << pCaloHit->GetLayer()
				<< ", pos: " << hitPos.GetX() << ", " << hitPos.GetY() << ", " << hitPos.GetZ()
				<< ", energy: " << pCaloHit->GetElectromagneticEnergy()
				<< ", time: " << pCaloHit->GetTime()
				<< std::endl;
        }
    }

    // Access to other current lists is almost identical. Some objects (CaloHits, Tracks, MCParticles) are "input objects".
    // These are the basic building blocks and are typically all created via the client app (can also be created in-algorithm).
    const MCParticleList *pMCParticleList(nullptr);
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pMCParticleList));

	std::cout << "MCP list: " << pMCParticleList->size() << std::endl;

    const TrackList *pTrackList(nullptr);
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pTrackList));
	std::cout << "track list: " << pTrackList->size() << std::endl;

    // Other objects (Clusters, Vertices, Pfos) are "algorithm objects" and can only be formed via algorithms. Clusters are
    // containers of CaloHits, whilst Pfos are containers of Clusters, Tracks and Vertices alongside particle metadata.
    const ClusterList *pCurrentClusterList(nullptr);
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pCurrentClusterList));
	std::cout << "cluster list: " << pCurrentClusterList->size() << std::endl;

    const VertexList *pCurrentVertexList(nullptr);
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pCurrentVertexList));
	std::cout << "vertex list: " << pCurrentVertexList->size() << std::endl;

    const PfoList *pCurrentPfoList(nullptr);
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pCurrentPfoList));
	std::cout << "PFO list: " << pCurrentPfoList->size() << std::endl;

    // Access to a named list is demonstrated below. This access only proceeds if a list name has been specified within the
    // algorithm xml tags. This list name is an optional parameter, rather than mandatory.
    if (!m_requestedCaloHitListName.empty())
    {
        const CaloHitList *pNamedCaloHitList(nullptr);
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetList(*this, m_requestedCaloHitListName, pNamedCaloHitList));

        // Use the named calo hit list...
    }

    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode AccessListsAlgorithm::ReadSettings(const TiXmlHandle xmlHandle)
{
    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "RequestedCaloHitListName", m_requestedCaloHitListName));

    return STATUS_CODE_SUCCESS;
}

} // namespace example_content

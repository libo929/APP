/**
 *  @file   ExampleContent/src/ExampleAlgorithms/CreateClustersAlgorithm.cc
 * 
 *  @brief  Implementation of the create clusters algorithm class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "ExampleAlgorithms/CreateClustersAlgorithm.h"

#include "ExampleHelpers/ExampleHelper.h"

using namespace pandora;

namespace example_content
{

CreateClustersAlgorithm::CreateClustersAlgorithm() :
    m_maxClusterHitDistance(20.f)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode CreateClustersAlgorithm::Run()
{
    const CaloHitList *pCaloHitList(nullptr);
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pCaloHitList));

    const ClusterList *pTemporaryList(nullptr);
    std::string temporaryListName;
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::CreateTemporaryListAndSetCurrent(*this, pTemporaryList, temporaryListName));

	std::map<int, CaloHitList> hitMap;

    for (const CaloHit *const pCaloHit : *pCaloHitList)
    {
        // Once a calo hit has been added to a cluster, it is flagged as unavailable.
        if (!PandoraContentApi::IsAvailable(*this, pCaloHit))
            continue;

		//std::cout << "    ---  hit: " << pCaloHit->GetHadronicEnergy() << std::endl;

		// ad hoc: energy is for mc id
		int mcID = int(pCaloHit->GetHadronicEnergy());

		if(hitMap.find(mcID) == hitMap.end())
		{
			CaloHitList  caloHitList;
			caloHitList.push_back(pCaloHit);
			hitMap[mcID] = caloHitList;
		}
		else
		{
			hitMap[mcID].push_back(pCaloHit);
		}
    }

	std::cout << "hit map size: " << hitMap.size() << std::endl;

	for(const auto& hitMapIter : hitMap)
	{
		const auto& caloHitList = hitMapIter.second; 
        const Cluster *pCluster(nullptr);
        PandoraContentApi::Cluster::Parameters parameters;
        parameters.m_caloHitList = caloHitList;
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::Cluster::Create(*this, parameters, pCluster));

		std::cout << "cluster energy: " << pCluster->GetElectromagneticEnergy() << ", from mc id: " << hitMapIter.first << std::endl;
	}

    // Choose to save the temporary clusters under a specified name and to set the cluster list to be the current list.
    if (!pTemporaryList->empty())
    {
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::SaveList<Cluster>(*this, m_outputListName));
        PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::ReplaceCurrentList<Cluster>(*this, m_outputListName));
    }

    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode CreateClustersAlgorithm::ReadSettings(const TiXmlHandle xmlHandle)
{
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle,
        "MaxClusterHitDistance", m_maxClusterHitDistance));

    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle,
        "OutputListName", m_outputListName));

    return STATUS_CODE_SUCCESS;
}

} // namespace example_content

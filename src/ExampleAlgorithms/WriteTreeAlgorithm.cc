/**
 *  @file   ExampleContent/src/ExampleAlgorithms/WriteTreeAlgorithm.cc
 * 
 *  @brief  Implementation of the write tree algorithm class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "ExampleAlgorithms/WriteTreeAlgorithm.h"

using namespace pandora;

namespace example_content
{

WriteTreeAlgorithm::~WriteTreeAlgorithm()
{
    try
    {
        // Tree is written to file when the algorithm is deleted (i.e. when the parent Pandora instance is deleted)
        PANDORA_MONITORING_API(SaveTree(this->GetPandora(), m_treeName, m_fileName, "UPDATE"));
    }
    catch (const StatusCodeException &)
    {
        std::cout << "WriteTreeAlgorithm: Unable to write tree " << m_treeName << " to file " << m_fileName << std::endl;
    }
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode WriteTreeAlgorithm::Run()
{
    // Create a simple root tree containing, for each event, the number of clusters in the current list and a vector
    // of all the cluster hadronic energy estimators.
    const ClusterList *pClusterList(nullptr);
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pClusterList));

    int nClusters(0);
    int nMainClusters(0);
    FloatVector clusterEnergies;
    float mainClustersEnergy(0.);
    float eventEnergy(0.);

    for (const Cluster *const pCluster : *pClusterList)
    {
		float clusterEnergy = pCluster->GetHadronicEnergy();

        clusterEnergies.push_back(clusterEnergy);
		eventEnergy += clusterEnergy;

		if(clusterEnergy > 0.3) 
		{ 
			mainClustersEnergy += clusterEnergy;
			++nMainClusters;
		}

        ++nClusters;
    }

    PANDORA_MONITORING_API(SetTreeVariable(this->GetPandora(), m_treeName, "nClusters",          nClusters));
    PANDORA_MONITORING_API(SetTreeVariable(this->GetPandora(), m_treeName, "nMainClusters",      nMainClusters));
    PANDORA_MONITORING_API(SetTreeVariable(this->GetPandora(), m_treeName, "clusterEnergies",    &clusterEnergies));
    PANDORA_MONITORING_API(SetTreeVariable(this->GetPandora(), m_treeName, "mainClustersEnergy", mainClustersEnergy));
    PANDORA_MONITORING_API(SetTreeVariable(this->GetPandora(), m_treeName, "eventEnergy",        eventEnergy));
    PANDORA_MONITORING_API(FillTree(this->GetPandora(), m_treeName));

    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode WriteTreeAlgorithm::ReadSettings(const TiXmlHandle xmlHandle)
{
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle,
        "FileName", m_fileName));

    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle,
        "TreeName", m_treeName));

    return STATUS_CODE_SUCCESS;
}

} // namespace example_content

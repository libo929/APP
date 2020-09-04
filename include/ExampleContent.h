/**
 *  @file   ExampleContent/include/ExampleContent.h
 * 
 *  @brief  Header file detailing example pandora content
 * 
 *  $Log: $
 */
#ifndef EXAMPLE_CONTENT_H
#define EXAMPLE_CONTENT_H 1

#include "ExampleAlgorithms/AccessListsAlgorithm.h"
#include "ExampleAlgorithms/PrepareCaloHitsAlgorithm.h"
#include "ExampleAlgorithms/CreateAdditionalCaloHitsAlgorithm.h"
#include "ExampleAlgorithms/CreateClustersAlgorithm.h"
#include "ExampleAlgorithms/CreateClustersDaughterAlgorithm.h"
#include "ExampleAlgorithms/CreatePfosAlgorithm.h"
#include "ExampleAlgorithms/CreateVerticesAlgorithm.h"
#include "ExampleAlgorithms/DeleteClustersAlgorithm.h"
#include "ExampleAlgorithms/DisplayListsAlgorithm.h"
#include "ExampleAlgorithms/FragmentClustersAlgorithm.h"
#include "ExampleAlgorithms/MergeClustersAlgorithm.h"
#include "ExampleAlgorithms/ReconfigureClustersAlgorithm.h"
#include "ExampleAlgorithms/SelectHitSubsetAlgorithm.h"
#include "ExampleAlgorithms/UseAlgorithmToolAlgorithm.h"
#include "ExampleAlgorithms/UsePluginsAlgorithm.h"
#include "ExampleAlgorithms/WriteTreeAlgorithm.h"

#include "ExampleAlgorithmTools/ExampleAlgorithmTool.h"

#include "ExamplePlugins/ExampleEnergyCorrectionPlugin.h"
#include "ExamplePlugins/ExampleParticleIdPlugin.h"
#include "ExamplePlugins/ExamplePseudoLayerPlugin.h"

// APRIL
#include "APRILClustering/APRILClusteringAlgorithm.h"
#include "APRILTopologicalAssociation/ClosebySeedMergingAlgorithm.h"
#include "APRILTopologicalAssociation/ClustersMergingAlgorithm.h"
#include "APRILTopologicalAssociation/NearbyFragmentsMergingAlgorithm.h"
#include "APRILTools/ConnectorSeedingTool.h"
#include "APRILTools/ConnectorCleaningTool.h"
#include "APRILTools/NearbyHitsConnectingTool.h"
#include "APRILUtility/CaloHitPreparationAlgorithm.h"
#include "APRILUtility/NearbyHitRecoveryAlgorithm.h"
#include "APRILMonitoring/VisualMonitoringAlgorithm.h"


/**
 *  @brief  ExampleContent class
 */
class ExampleContent
{
public:
    #define EXAMPLE_ALGORITHM_LIST(d)                                                                                   \
        d("AccessListsExample",                         example_content::AccessListsAlgorithm::Factory)                 \
        d("PrepareCaloHits",                            example_content::PrepareCaloHitsAlgorithm::Factory)             \
        d("CreateAdditionalCaloHitsExample",            example_content::CreateAdditionalCaloHitsAlgorithm::Factory)    \
        d("CreateClustersExample",                      example_content::CreateClustersAlgorithm::Factory)              \
        d("CreateClustersDaughterExample",              example_content::CreateClustersDaughterAlgorithm::Factory)      \
        d("CreatePfosExample",                          example_content::CreatePfosAlgorithm::Factory)                  \
        d("CreateVerticesExample",                      example_content::CreateVerticesAlgorithm::Factory)              \
        d("DeleteClustersExample",                      example_content::DeleteClustersAlgorithm::Factory)              \
        d("DisplayListsExample",                        example_content::DisplayListsAlgorithm::Factory)                \
        d("FragmentClustersExample",                    example_content::FragmentClustersAlgorithm::Factory)            \
        d("MergeClustersExample",                       example_content::MergeClustersAlgorithm::Factory)               \
        d("ReconfigureClustersExample",                 example_content::ReconfigureClustersAlgorithm::Factory)         \
        d("SelectHitSubsetExample",                     example_content::SelectHitSubsetAlgorithm::Factory)             \
        d("UseAlgorithmToolExample",                    example_content::UseAlgorithmToolAlgorithm::Factory)            \
        d("UsePluginsExample",                          example_content::UsePluginsAlgorithm::Factory)                  \
        d("WriteTreeExample",                           example_content::WriteTreeAlgorithm::Factory)                   \
        d("VisualMonitoring",                           april_content::VisualMonitoringAlgorithm::Factory)              \
        d("APRILClustering",                            april_content::APRILClusteringAlgorithm::Factory)               \
	    d("ClosebySeedMerging",                         april_content::ClosebySeedMergingAlgorithm::Factory)            \
	    d("CaloHitPreparation",                         april_content::CaloHitPreparationAlgorithm::Factory)            \
	    d("ClustersMerging",                            april_content::ClustersMergingAlgorithm::Factory) \
	    d("NearbyHitRecovery",                          april_content::NearbyHitRecoveryAlgorithm::Factory) \
	    d("NearbyFragmentsMerging",                     april_content::NearbyFragmentsMergingAlgorithm::Factory)

    #define EXAMPLE_ALGORITHM_TOOL_LIST(d)                                                                              \
        d("AlgorithmToolExample",                       example_content::ExampleAlgorithmTool::Factory)                 \
        d("ConnectorSeeding",                           april_content::ConnectorSeedingTool::Factory)                   \
	    d("NearbyHitsConnecting",                       april_content::NearbyHitsConnectingTool::Factory) \
        d("ConnectorCleaning",                          april_content::ConnectorCleaningTool::Factory)

    #define EXAMPLE_ENERGY_CORRECTION_LIST(d)                                                                           \
        d("EnergyCorrectionExample", pandora::HADRONIC, example_content::ExampleEnergyCorrectionPlugin)

    #define EXAMPLE_PARTICLE_ID_LIST(d)                                                                                 \
        d("ParticleIdExample",                          example_content::ExampleParticleIdPlugin)

    /**
     *  @brief  Register all the example content algorithms and tools with pandora
     * 
     *  @param  pandora the pandora instance with which to register content
     */
    static pandora::StatusCode RegisterExampleAlgorithms(const pandora::Pandora &pandora);

    /**
     *  @brief  Register the example content plugins with pandora
     * 
     *  @param  pandora the pandora instance with which to register content
     */
    static pandora::StatusCode RegisterExamplePlugins(const pandora::Pandora &pandora);
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::StatusCode ExampleContent::RegisterExampleAlgorithms(const pandora::Pandora &pandora)
{
    EXAMPLE_ALGORITHM_LIST(PANDORA_REGISTER_ALGORITHM);
    EXAMPLE_ALGORITHM_TOOL_LIST(PANDORA_REGISTER_ALGORITHM_TOOL);

    return pandora::STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::StatusCode ExampleContent::RegisterExamplePlugins(const pandora::Pandora &pandora)
{
    EXAMPLE_ENERGY_CORRECTION_LIST(PANDORA_REGISTER_ENERGY_CORRECTION);
    EXAMPLE_PARTICLE_ID_LIST(PANDORA_REGISTER_PARTICLE_ID);
    PANDORA_RETURN_RESULT_IF(pandora::STATUS_CODE_SUCCESS, !=, PandoraApi::SetPseudoLayerPlugin(pandora, new example_content::ExamplePseudoLayerPlugin));

    return pandora::STATUS_CODE_SUCCESS;
}

#endif // #ifndef EXAMPLE_CONTENT_H

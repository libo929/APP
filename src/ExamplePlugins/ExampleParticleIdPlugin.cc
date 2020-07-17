/**
 *  @file   ExampleContent/src/ExamplePlugins/ExampleParticleIdPlugin.cc
 * 
 *  @brief  Implementation of the example particle id plugin class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "ExamplePlugins/ExampleParticleIdPlugin.h"

using namespace pandora;

namespace example_content
{

ExampleParticleIdPlugin::ExampleParticleIdPlugin() :
    m_exampleParameter(0)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool ExampleParticleIdPlugin::IsMatch(const Cluster *const /*pCluster*/) const
{
    // Particle id plugins are instantiated and registed (with the Pandora plugin manager) via the client app. They are then
    // associated with particular particle id "slots" via the PandoraSettings xml file e.g. MuonPlugin, ElectronPlugin
    // Each plugin can have configurable parameters and must provide an implementation of an IsMatch(const Cluster *const) function.

    return false;
}

//------------------------------------------------------------------------------------------------------------------------------------------

bool ExampleParticleIdPlugin::IsMatch(const ParticleFlowObject *const /*pPfo*/) const
{
    // Require implementation for both a cluster and a pfo. Could choose to simply redirect pfo implementation to cluster implementation,
    // provide completely different implementations for each, or raise an exception rather than provide an implementation for both.

    return false;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode ExampleParticleIdPlugin::ReadSettings(const TiXmlHandle xmlHandle)
{
    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "ExampleParameter", m_exampleParameter));

    return STATUS_CODE_SUCCESS;
}

} // namespace example_content

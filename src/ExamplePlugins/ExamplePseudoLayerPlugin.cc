/**
 *  @file   ExampleContent/src/ExamplePlugins/ExamplePseudoLayerPlugin.cc
 * 
 *  @brief  Implementation of the example pseudo layer plugin class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "ExamplePlugins/ExamplePseudoLayerPlugin.h"

using namespace pandora;

namespace example_content
{

ExamplePseudoLayerPlugin::ExamplePseudoLayerPlugin() :
    m_zPitch(1.f),
    m_zOffset(500.f)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

unsigned int ExamplePseudoLayerPlugin::GetPseudoLayer(const CartesianVector &positionVector) const
{
    // The pseudo layer plugin is instantiated and registed (with the Pandora plugin manager) via the client app. It is responsible
    // for binning world coordinates into layers. In a 4-pi detector with collisions at a central interaction point, the layers typically
    // group together positions of equivalent depth in the detector. For fixed target collisions, the layer structure will typically
    // represent a binning of distance from the target.

	//return static_cast<unsigned int>( (positionVector.GetZ() - 12520. + 4.)/5.5 );  
	//std::cout << " ****** " << static_cast<unsigned int> ( positionVector.GetZ()/5.5 ) << std::endl;
	return static_cast<unsigned int> ( (positionVector.GetZ()-12520.)/5.5 + 1);  
	//cellID.Z() * 5.5 - 4.
    //return static_cast<unsigned int>((m_zOffset + positionVector.GetZ()) / m_zPitch);
    //return static_cast<unsigned int>((m_zOffset + positionVector.GetZ()) / m_zPitch);
}

//------------------------------------------------------------------------------------------------------------------------------------------

unsigned int ExamplePseudoLayerPlugin::GetPseudoLayerAtIp() const
{
    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode ExamplePseudoLayerPlugin::ReadSettings(const TiXmlHandle xmlHandle)
{
    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "ZPitch", m_zPitch));

    if (m_zPitch < std::numeric_limits<float>::epsilon())
        return STATUS_CODE_INVALID_PARAMETER;

    PANDORA_RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::ReadValue(xmlHandle,
        "ZOffset", m_zOffset));

    return STATUS_CODE_SUCCESS;
}

} // namespace example_content

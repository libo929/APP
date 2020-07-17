/**
 *  @file   ExampleContent/src/ExampleAlgorithms/SelectHitSubsetAlgorithm.cc
 * 
 *  @brief  Implementation of the select hit subset algorithm class.
 * 
 *  $Log: $
 */

#include "Pandora/AlgorithmHeaders.h"

#include "ExampleAlgorithms/SelectHitSubsetAlgorithm.h"

using namespace pandora;

namespace example_content
{

SelectHitSubsetAlgorithm::SelectHitSubsetAlgorithm() :
    m_hitSelectionFraction(1.f),
    m_randomEngine(12345)
{
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode SelectHitSubsetAlgorithm::Run()
{
    // Take the current list of calo hits and select some fraction of them, saving a new subset calo hit list with a specified
    // name and setting the new list to be the current calo hit list for all subsequent algorithms.
    const CaloHitList *pCaloHitList(nullptr);
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::GetCurrentList(*this, pCaloHitList));

    CaloHitList selectedCaloHitList;
    std::uniform_real_distribution<float> randomDistribution(0.f, 1.f);

    for (const CaloHit *const pCaloHit : *pCaloHitList)
    {
        if (randomDistribution(m_randomEngine) < m_hitSelectionFraction)
            selectedCaloHitList.push_back(pCaloHit);
    }

    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::SaveList(*this, selectedCaloHitList, m_outputListName));
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, PandoraContentApi::ReplaceCurrentList<CaloHit>(*this, m_outputListName));

    return STATUS_CODE_SUCCESS;
}

//------------------------------------------------------------------------------------------------------------------------------------------

StatusCode SelectHitSubsetAlgorithm::ReadSettings(const TiXmlHandle xmlHandle)
{
    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle,
        "HitSelectionFraction", m_hitSelectionFraction));

    if ((m_hitSelectionFraction < std::numeric_limits<float>::epsilon()) || (m_hitSelectionFraction - 1.f > std::numeric_limits<float>::epsilon()))
    {
        std::cout << "SelectHitSubsetAlgorithm: invalid hit selection fraction specified " << m_hitSelectionFraction << std::endl;
        return STATUS_CODE_INVALID_PARAMETER;
    }

    PANDORA_RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::ReadValue(xmlHandle,
        "OutputListName", m_outputListName));

    return STATUS_CODE_SUCCESS;
}

} // namespace example_content

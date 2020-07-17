/**
 *  @file   ExampleContent/include/ExampleAlgorithms/PrepareCaloHitsAlgorithm.h
 * 
 *  @brief  Header file for the create calo hits algorithm class.
 * 
 *  $Log: $
 */
#ifndef PREPARE_CALOHITS_ALGORITHM 
#define PREPARE_CALOHITS_ALGORITHM 1

#include "Pandora/Algorithm.h"
#include "ArborObjects/CaloHit.h"

#include <random>

namespace example_content
{

/**
 *  @brief  PrepareCaloHitsAlgorithm class
 */
class PrepareCaloHitsAlgorithm : public pandora::Algorithm
{
public:
    /**
     *  @brief  Factory class for instantiating algorithm
     */
    class Factory : public pandora::AlgorithmFactory
    {
    public:
        pandora::Algorithm *CreateAlgorithm() const;
    };

    /**
     *  @brief  Default constructor
     */
    PrepareCaloHitsAlgorithm();

private:
    pandora::StatusCode Run();
    pandora::StatusCode ReadSettings(const pandora::TiXmlHandle xmlHandle);

    unsigned int                m_nCaloHitsToMake;                  ///< The number of new calo hits to make
    bool                        m_setCurrentListToInputList;        ///< Whether to return calo hit list to the input list after hit creation

    float                       m_worldSideLength;                  ///< The world volume cube side length
    float                       m_groupSideLength;                  ///< The group volume cube side length

    std::default_random_engine  m_randomEngine;                     ///< The engine for random number generation

	static arbor_content::CaloHitFactory   m_pCaloHitFactory;
    static int                  m_nEvent;
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::Algorithm *PrepareCaloHitsAlgorithm::Factory::CreateAlgorithm() const
{
    return new PrepareCaloHitsAlgorithm();
}

} // namespace example_content

#endif // #ifndef PREPARE_CALOHITS_ALGORITHM

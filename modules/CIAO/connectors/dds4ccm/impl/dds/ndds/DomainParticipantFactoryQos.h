/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_DOMAINPARTICIPANTFACTORYQOSPOLICY_H
#define DDS4CCM_DOMAINPARTICIPANTFACTORYQOSPOLICY_H

#include "EntityFactoryQosPolicy.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

#if (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::DomainParticipantFactoryQos DDS_DomainParticipantFactoryQos;
#endif

#if (CIAO_DDS4CCM_NDDS==1)
inline void
operator<<= (::DDS::DomainParticipantFactoryQos &ddsqos, const ::DDS_DomainParticipantFactoryQos & qos)
{
  ddsqos.entity_factory <<= qos.entity_factory;
}

inline void
operator<<= (::DDS_DomainParticipantFactoryQos &ddsqos, const ::DDS::DomainParticipantFactoryQos & qos)
{
  ddsqos.entity_factory <<= qos.entity_factory;
}

#endif

#endif /* DDS4CCM_DOMAINPARTICIPANTFACTORYQOSPOLICY_H */

//==============================================================
/**
 *  @file  IDD_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_IDD_HANDLER_H
#define CIAO_CONFIG_HANDLERS_IDD_HANDLER_H
#include /**/ "ace/pre.h"

#include "Config_Handlers/Config_Handlers_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Deployment
{
  struct InstanceDeploymentDescription;
  class InstanceDeploymentDescriptions;
  struct InstanceResourceDeploymentDescription;
}

namespace CIAO
{
  namespace Config_Handlers
  {
    class DeploymentPlan;
    class InstanceDeploymentDescription;
    class InstanceResourceDeploymentDescription;

   /*
    * @class IDD_Handler
    *
    * @brief Handler class for <InstanceDeploymentDescription> types.
    *
    * This class defines handler methods to map values from
    * XSC InstanceDeploymentDescriptionn objects, parsed from
    * the descriptor files, to the corresponding CORBA IDL type.
    *
    */

    class Config_Handlers_Export IDD_Handler
    {
    public:
      static bool instance_deployment_descrs (
          const DeploymentPlan &src,
          ::Deployment::InstanceDeploymentDescriptions& dest);

    private:
      static bool instance_deployment_descr (
          const InstanceDeploymentDescription &src,
          ::Deployment::InstanceDeploymentDescription &dest);

      static void instance_resource_depl_descr (
        const InstanceResourceDeploymentDescription &src,
        Deployment::InstanceResourceDeploymentDescription &dest);
    };
  }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_IDD_HANDLER_H*/

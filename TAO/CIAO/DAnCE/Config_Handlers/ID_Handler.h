
//==============================================================
/**
 *  @file  ID_Handler.h
 *
 *  $Id$
 *
 *  @author Jules White <jules@dre.vanderbilt.edu>
 */
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_ID_Handler_H
#define CIAO_CONFIG_HANDLERS_ID_Handler_H
#include /**/ "ace/pre.h"

#include "Config_Handlers_export.h"
#include "ace/config-lite.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */



namespace Deployment
{
  class ImplementationDependency;
}


namespace CIAO
{

  namespace Config_Handlers
  {

   class ImplementationDependency;


   /*
    * @class ID_Handler
    *
    * @brief Handler class for <ImplementationDependency> types.
    *
    * This class defines handler methods to map values from
    * XSC ImplementationDependency objects, parsed from the descriptor files, to the
    * corresponding CORBA IDL Any type.
    *
    */
    class Config_Handlers_Export ID_Handler
    {
    public:

      ID_Handler (void);
      virtual ~ID_Handler (void);

      void get_ImplementationDependency (
          Deployment::ImplementationDependency& toconfig,
          ImplementationDependency& desc);

      ImplementationDependency impl_dependency (
          const ::Deployment::ImplementationDependency& src);
    };
  }
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_ID_Handler_H */

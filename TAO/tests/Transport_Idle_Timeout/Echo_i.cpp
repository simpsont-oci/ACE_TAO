#include "Echo_i.h"
#include "tao/debug.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"

Echo_i::Echo_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
Echo_i::ping (const char *msg)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Echo_i::ping (%C)\n"), msg));
  return CORBA::string_dup (msg);
}

void
Echo_i::shutdown ()
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Echo_i::shutdown ()\n")));
  orb_->shutdown (false);
}

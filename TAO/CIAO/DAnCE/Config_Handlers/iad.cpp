// $Id$
#include "iad.hpp"

namespace Deployment
{
  // NamedImplementationArtifact
  // 

  NamedImplementationArtifact::
  NamedImplementationArtifact (::XMLSchema::string< char > const& name__,
  ::Deployment::ImplementationArtifactDescription const& referencedArtifact__)
  :
  name_ (new ::XMLSchema::string< char > (name__)),
  referencedArtifact_ (new ::Deployment::ImplementationArtifactDescription (referencedArtifact__)),
  regulator__ ()
  {
    name_->container (this);
    referencedArtifact_->container (this);
  }

  NamedImplementationArtifact::
  NamedImplementationArtifact (::Deployment::NamedImplementationArtifact const& s)
  :
  name_ (new ::XMLSchema::string< char > (*s.name_)),
  referencedArtifact_ (new ::Deployment::ImplementationArtifactDescription (*s.referencedArtifact_)),
  regulator__ ()
  {
    name_->container (this);
    referencedArtifact_->container (this);
  }

  ::Deployment::NamedImplementationArtifact& NamedImplementationArtifact::
  operator= (::Deployment::NamedImplementationArtifact const& s)
  {
    name (s.name ());

    referencedArtifact (s.referencedArtifact ());

    return *this;
  }


  // NamedImplementationArtifact
  // 
  ::XMLSchema::string< char > const& NamedImplementationArtifact::
  name () const
  {
    return *name_;
  }

  ::XMLSchema::string< char >& NamedImplementationArtifact::
  name ()
  {
    return *name_;
  }

  void NamedImplementationArtifact::
  name (::XMLSchema::string< char > const& e)
  {
    *name_ = e;
  }

  // NamedImplementationArtifact
  // 
  ::Deployment::ImplementationArtifactDescription const& NamedImplementationArtifact::
  referencedArtifact () const
  {
    return *referencedArtifact_;
  }

  ::Deployment::ImplementationArtifactDescription& NamedImplementationArtifact::
  referencedArtifact ()
  {
    return *referencedArtifact_;
  }

  void NamedImplementationArtifact::
  referencedArtifact (::Deployment::ImplementationArtifactDescription const& e)
  {
    *referencedArtifact_ = e;
  }


  // ImplementationArtifactDescription
  // 

  ImplementationArtifactDescription::
  ImplementationArtifactDescription ()
  :
  regulator__ ()
  {
  }

  ImplementationArtifactDescription::
  ImplementationArtifactDescription (::Deployment::ImplementationArtifactDescription const& s)
  :
  label_ (s.label_.get () ? new ::XMLSchema::string< char > (*s.label_) : 0),
  UUID_ (s.UUID_.get () ? new ::XMLSchema::string< char > (*s.UUID_) : 0),
  execParameter_ (s.execParameter_.get () ? new ::Deployment::Property (*s.execParameter_) : 0),
  deployRequirement_ (s.deployRequirement_.get () ? new ::Deployment::Requirement (*s.deployRequirement_) : 0),
  infoProperty_ (s.infoProperty_.get () ? new ::Deployment::Property (*s.infoProperty_) : 0),
  contentLocation_ (s.contentLocation_.get () ? new ::XMLSchema::string< char > (*s.contentLocation_) : 0),
  regulator__ ()
  {
    if (label_.get ()) label_->container (this);
    if (UUID_.get ()) UUID_->container (this);
    location_.reserve (s.location_.size ());
    {
      for (location_const_iterator i (s.location_.begin ());
      i != s.location_.end ();
      ++i) add_location (*i);
    }

    if (execParameter_.get ()) execParameter_->container (this);
    if (deployRequirement_.get ()) deployRequirement_->container (this);
    dependsOn_.reserve (s.dependsOn_.size ());
    {
      for (dependsOn_const_iterator i (s.dependsOn_.begin ());
      i != s.dependsOn_.end ();
      ++i) add_dependsOn (*i);
    }

    if (infoProperty_.get ()) infoProperty_->container (this);
    if (contentLocation_.get ()) contentLocation_->container (this);
  }

  ::Deployment::ImplementationArtifactDescription& ImplementationArtifactDescription::
  operator= (::Deployment::ImplementationArtifactDescription const& s)
  {
    if (s.label_.get ()) label (*(s.label_));
    else label_ = ::std::auto_ptr< ::XMLSchema::string< char > > (0);

    if (s.UUID_.get ()) UUID (*(s.UUID_));
    else UUID_ = ::std::auto_ptr< ::XMLSchema::string< char > > (0);

    location_.clear ();
    location_.reserve (s.location_.size ());
    {
      for (location_const_iterator i (s.location_.begin ());
      i != s.location_.end ();
      ++i) add_location (*i);
    }

    if (s.execParameter_.get ()) execParameter (*(s.execParameter_));
    else execParameter_ = ::std::auto_ptr< ::Deployment::Property > (0);

    if (s.deployRequirement_.get ()) deployRequirement (*(s.deployRequirement_));
    else deployRequirement_ = ::std::auto_ptr< ::Deployment::Requirement > (0);

    dependsOn_.clear ();
    dependsOn_.reserve (s.dependsOn_.size ());
    {
      for (dependsOn_const_iterator i (s.dependsOn_.begin ());
      i != s.dependsOn_.end ();
      ++i) add_dependsOn (*i);
    }

    if (s.infoProperty_.get ()) infoProperty (*(s.infoProperty_));
    else infoProperty_ = ::std::auto_ptr< ::Deployment::Property > (0);

    if (s.contentLocation_.get ()) contentLocation (*(s.contentLocation_));
    else contentLocation_ = ::std::auto_ptr< ::XMLSchema::string< char > > (0);

    return *this;
  }


  // ImplementationArtifactDescription
  // 
  bool ImplementationArtifactDescription::
  label_p () const
  {
    return label_.get () != 0;
  }

  ::XMLSchema::string< char > const& ImplementationArtifactDescription::
  label () const
  {
    return *label_;
  }

  ::XMLSchema::string< char >& ImplementationArtifactDescription::
  label ()
  {
    return *label_;
  }

  void ImplementationArtifactDescription::
  label (::XMLSchema::string< char > const& e)
  {
    if (label_.get ())
    {
      *label_ = e;
    }

    else
    {
      label_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
      label_->container (this);
    }
  }

  // ImplementationArtifactDescription
  // 
  bool ImplementationArtifactDescription::
  UUID_p () const
  {
    return UUID_.get () != 0;
  }

  ::XMLSchema::string< char > const& ImplementationArtifactDescription::
  UUID () const
  {
    return *UUID_;
  }

  ::XMLSchema::string< char >& ImplementationArtifactDescription::
  UUID ()
  {
    return *UUID_;
  }

  void ImplementationArtifactDescription::
  UUID (::XMLSchema::string< char > const& e)
  {
    if (UUID_.get ())
    {
      *UUID_ = e;
    }

    else
    {
      UUID_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
      UUID_->container (this);
    }
  }

  // ImplementationArtifactDescription
  // 
  ImplementationArtifactDescription::location_iterator ImplementationArtifactDescription::
  begin_location ()
  {
    return location_.begin ();
  }

  ImplementationArtifactDescription::location_iterator ImplementationArtifactDescription::
  end_location ()
  {
    return location_.end ();
  }

  ImplementationArtifactDescription::location_const_iterator ImplementationArtifactDescription::
  begin_location () const
  {
    return location_.begin ();
  }

  ImplementationArtifactDescription::location_const_iterator ImplementationArtifactDescription::
  end_location () const
  {
    return location_.end ();
  }

  void ImplementationArtifactDescription::
  add_location (::XMLSchema::string< char > const& e)
  {
    if (location_.capacity () < location_.size () + 1)
    {
      ::std::vector< ::XMLSchema::string< char > > v;
      v.reserve (location_.size () + 1);

      while (location_.size ())
      {
        //@@ VC6
        ::XMLSchema::string< char >& t = location_.back ();
        t.container (0);
        v.push_back (t);
        v.back ().container (this);
        location_.pop_back ();
      }

      location_.swap (v);
    }

    location_.push_back (e);
    location_.back ().container (this);
  }

  // ImplementationArtifactDescription
  // 
  bool ImplementationArtifactDescription::
  execParameter_p () const
  {
    return execParameter_.get () != 0;
  }

  ::Deployment::Property const& ImplementationArtifactDescription::
  execParameter () const
  {
    return *execParameter_;
  }

  ::Deployment::Property& ImplementationArtifactDescription::
  execParameter ()
  {
    return *execParameter_;
  }

  void ImplementationArtifactDescription::
  execParameter (::Deployment::Property const& e)
  {
    if (execParameter_.get ())
    {
      *execParameter_ = e;
    }

    else
    {
      execParameter_ = ::std::auto_ptr< ::Deployment::Property > (new ::Deployment::Property (e));
      execParameter_->container (this);
    }
  }

  // ImplementationArtifactDescription
  // 
  bool ImplementationArtifactDescription::
  deployRequirement_p () const
  {
    return deployRequirement_.get () != 0;
  }

  ::Deployment::Requirement const& ImplementationArtifactDescription::
  deployRequirement () const
  {
    return *deployRequirement_;
  }

  ::Deployment::Requirement& ImplementationArtifactDescription::
  deployRequirement ()
  {
    return *deployRequirement_;
  }

  void ImplementationArtifactDescription::
  deployRequirement (::Deployment::Requirement const& e)
  {
    if (deployRequirement_.get ())
    {
      *deployRequirement_ = e;
    }

    else
    {
      deployRequirement_ = ::std::auto_ptr< ::Deployment::Requirement > (new ::Deployment::Requirement (e));
      deployRequirement_->container (this);
    }
  }

  // ImplementationArtifactDescription
  // 
  ImplementationArtifactDescription::dependsOn_iterator ImplementationArtifactDescription::
  begin_dependsOn ()
  {
    return dependsOn_.begin ();
  }

  ImplementationArtifactDescription::dependsOn_iterator ImplementationArtifactDescription::
  end_dependsOn ()
  {
    return dependsOn_.end ();
  }

  ImplementationArtifactDescription::dependsOn_const_iterator ImplementationArtifactDescription::
  begin_dependsOn () const
  {
    return dependsOn_.begin ();
  }

  ImplementationArtifactDescription::dependsOn_const_iterator ImplementationArtifactDescription::
  end_dependsOn () const
  {
    return dependsOn_.end ();
  }

  void ImplementationArtifactDescription::
  add_dependsOn (::Deployment::NamedImplementationArtifact const& e)
  {
    if (dependsOn_.capacity () < dependsOn_.size () + 1)
    {
      ::std::vector< ::Deployment::NamedImplementationArtifact > v;
      v.reserve (dependsOn_.size () + 1);

      while (dependsOn_.size ())
      {
        //@@ VC6
        ::Deployment::NamedImplementationArtifact& t = dependsOn_.back ();
        t.container (0);
        v.push_back (t);
        v.back ().container (this);
        dependsOn_.pop_back ();
      }

      dependsOn_.swap (v);
    }

    dependsOn_.push_back (e);
    dependsOn_.back ().container (this);
  }

  // ImplementationArtifactDescription
  // 
  bool ImplementationArtifactDescription::
  infoProperty_p () const
  {
    return infoProperty_.get () != 0;
  }

  ::Deployment::Property const& ImplementationArtifactDescription::
  infoProperty () const
  {
    return *infoProperty_;
  }

  ::Deployment::Property& ImplementationArtifactDescription::
  infoProperty ()
  {
    return *infoProperty_;
  }

  void ImplementationArtifactDescription::
  infoProperty (::Deployment::Property const& e)
  {
    if (infoProperty_.get ())
    {
      *infoProperty_ = e;
    }

    else
    {
      infoProperty_ = ::std::auto_ptr< ::Deployment::Property > (new ::Deployment::Property (e));
      infoProperty_->container (this);
    }
  }

  // ImplementationArtifactDescription
  // 
  bool ImplementationArtifactDescription::
  contentLocation_p () const
  {
    return contentLocation_.get () != 0;
  }

  ::XMLSchema::string< char > const& ImplementationArtifactDescription::
  contentLocation () const
  {
    return *contentLocation_;
  }

  ::XMLSchema::string< char >& ImplementationArtifactDescription::
  contentLocation ()
  {
    return *contentLocation_;
  }

  void ImplementationArtifactDescription::
  contentLocation (::XMLSchema::string< char > const& e)
  {
    if (contentLocation_.get ())
    {
      *contentLocation_ = e;
    }

    else
    {
      contentLocation_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
      contentLocation_->container (this);
    }
  }
}

namespace Deployment
{
  // NamedImplementationArtifact
  //

  NamedImplementationArtifact::
  NamedImplementationArtifact (::XSCRT::XML::Element< char > const& e)
  :Base__ (e), regulator__ ()
  {

    ::XSCRT::Parser< char > p (e);

    while (p.more_elements ())
    {
      ::XSCRT::XML::Element< char > e (p.next_element ());
      ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

      if (n == "name")
      {
        name_ = ::std::auto_ptr< ::XMLSchema::string< char > > (new ::XMLSchema::string< char > (e));
        name_->container (this);
      }

      else if (n == "referencedArtifact")
      {
        referencedArtifact_ = ::std::auto_ptr< ::Deployment::ImplementationArtifactDescription > (new ::Deployment::ImplementationArtifactDescription (e));
        referencedArtifact_->container (this);
      }

      else 
      {
      }
    }
  }

  // ImplementationArtifactDescription
  //

  ImplementationArtifactDescription::
  ImplementationArtifactDescription (::XSCRT::XML::Element< char > const& e)
  :Base__ (e), regulator__ ()
  {

    ::XSCRT::Parser< char > p (e);

    while (p.more_elements ())
    {
      ::XSCRT::XML::Element< char > e (p.next_element ());
      ::std::basic_string< char > n (::XSCRT::XML::uq_name (e.name ()));

      if (n == "label")
      {
        ::XMLSchema::string< char > t (e);
        label (t);
      }

      else if (n == "UUID")
      {
        ::XMLSchema::string< char > t (e);
        UUID (t);
      }

      else if (n == "location")
      {
        ::XMLSchema::string< char > t (e);
        add_location (t);
      }

      else if (n == "execParameter")
      {
        ::Deployment::Property t (e);
        execParameter (t);
      }

      else if (n == "deployRequirement")
      {
        ::Deployment::Requirement t (e);
        deployRequirement (t);
      }

      else if (n == "dependsOn")
      {
        ::Deployment::NamedImplementationArtifact t (e);
        add_dependsOn (t);
      }

      else if (n == "infoProperty")
      {
        ::Deployment::Property t (e);
        infoProperty (t);
      }

      else if (n == "contentLocation")
      {
        ::XMLSchema::string< char > t (e);
        contentLocation (t);
      }

      else 
      {
      }
    }
  }
}

namespace Deployment
{
}

#include <XMLSchema/TypeInfo.hpp>

namespace Deployment
{
  namespace
  {
    ::XMLSchema::TypeInfoInitializer < char > XMLSchemaTypeInfoInitializer_ (::XSCRT::extended_type_info_map ());

    struct NamedImplementationArtifactTypeInfoInitializer
    {
      NamedImplementationArtifactTypeInfoInitializer ()
      {
        ::XSCRT::TypeId id (typeid (NamedImplementationArtifact));
        ::XSCRT::ExtendedTypeInfo nf (id);

        nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
        ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
      }
    };

    NamedImplementationArtifactTypeInfoInitializer NamedImplementationArtifactTypeInfoInitializer_;

    struct ImplementationArtifactDescriptionTypeInfoInitializer
    {
      ImplementationArtifactDescriptionTypeInfoInitializer ()
      {
        ::XSCRT::TypeId id (typeid (ImplementationArtifactDescription));
        ::XSCRT::ExtendedTypeInfo nf (id);

        nf.add_base (::XSCRT::ExtendedTypeInfo::Access::public_, false, typeid (::XSCRT::Type));
        ::XSCRT::extended_type_info_map ().insert (::std::make_pair (id, nf));
      }
    };

    ImplementationArtifactDescriptionTypeInfoInitializer ImplementationArtifactDescriptionTypeInfoInitializer_;
  }
}

namespace Deployment
{
  namespace Traversal
  {
    // NamedImplementationArtifact
    //
    //

    void NamedImplementationArtifact::
    traverse (Type& o)
    {
      pre (o);
      name (o);
      referencedArtifact (o);
      post (o);
    }

    void NamedImplementationArtifact::
    traverse (Type const& o)
    {
      pre (o);
      name (o);
      referencedArtifact (o);
      post (o);
    }

    void NamedImplementationArtifact::
    pre (Type&)
    {
    }

    void NamedImplementationArtifact::
    pre (Type const&)
    {
    }

    void NamedImplementationArtifact::
    name (Type& o)
    {
      dispatch (o.name ());
    }

    void NamedImplementationArtifact::
    name (Type const& o)
    {
      dispatch (o.name ());
    }

    void NamedImplementationArtifact::
    referencedArtifact (Type& o)
    {
      dispatch (o.referencedArtifact ());
    }

    void NamedImplementationArtifact::
    referencedArtifact (Type const& o)
    {
      dispatch (o.referencedArtifact ());
    }

    void NamedImplementationArtifact::
    post (Type&)
    {
    }

    void NamedImplementationArtifact::
    post (Type const&)
    {
    }

    // ImplementationArtifactDescription
    //
    //

    void ImplementationArtifactDescription::
    traverse (Type& o)
    {
      pre (o);
      if (o.label_p ()) label (o);
      else label_none (o);
      if (o.UUID_p ()) UUID (o);
      else UUID_none (o);
      location (o);
      if (o.execParameter_p ()) execParameter (o);
      else execParameter_none (o);
      if (o.deployRequirement_p ()) deployRequirement (o);
      else deployRequirement_none (o);
      dependsOn (o);
      if (o.infoProperty_p ()) infoProperty (o);
      else infoProperty_none (o);
      if (o.contentLocation_p ()) contentLocation (o);
      else contentLocation_none (o);
      post (o);
    }

    void ImplementationArtifactDescription::
    traverse (Type const& o)
    {
      pre (o);
      if (o.label_p ()) label (o);
      else label_none (o);
      if (o.UUID_p ()) UUID (o);
      else UUID_none (o);
      location (o);
      if (o.execParameter_p ()) execParameter (o);
      else execParameter_none (o);
      if (o.deployRequirement_p ()) deployRequirement (o);
      else deployRequirement_none (o);
      dependsOn (o);
      if (o.infoProperty_p ()) infoProperty (o);
      else infoProperty_none (o);
      if (o.contentLocation_p ()) contentLocation (o);
      else contentLocation_none (o);
      post (o);
    }

    void ImplementationArtifactDescription::
    pre (Type&)
    {
    }

    void ImplementationArtifactDescription::
    pre (Type const&)
    {
    }

    void ImplementationArtifactDescription::
    label (Type& o)
    {
      dispatch (o.label ());
    }

    void ImplementationArtifactDescription::
    label (Type const& o)
    {
      dispatch (o.label ());
    }

    void ImplementationArtifactDescription::
    label_none (Type&)
    {
    }

    void ImplementationArtifactDescription::
    label_none (Type const&)
    {
    }

    void ImplementationArtifactDescription::
    UUID (Type& o)
    {
      dispatch (o.UUID ());
    }

    void ImplementationArtifactDescription::
    UUID (Type const& o)
    {
      dispatch (o.UUID ());
    }

    void ImplementationArtifactDescription::
    UUID_none (Type&)
    {
    }

    void ImplementationArtifactDescription::
    UUID_none (Type const&)
    {
    }

    void ImplementationArtifactDescription::
    location (Type& o)
    {
      // VC6 anathema strikes again
      //
      ImplementationArtifactDescription::Type::location_iterator b (o.begin_location()), e (o.end_location());

      if (b != e)
      {
        location_pre (o);
        for (;
         b != e;
        )
        {
          dispatch (*b);
          if (++b != e) location_next (o);
        }

        location_post (o);
      }

      else location_none (o);
    }

    void ImplementationArtifactDescription::
    location (Type const& o)
    {
      // VC6 anathema strikes again
      //
      ImplementationArtifactDescription::Type::location_const_iterator b (o.begin_location()), e (o.end_location());

      if (b != e)
      {
        location_pre (o);
        for (;
         b != e;
        )
        {
          dispatch (*b);
          if (++b != e) location_next (o);
        }

        location_post (o);
      }

      else location_none (o);
    }

    void ImplementationArtifactDescription::
    location_pre (Type&)
    {
    }

    void ImplementationArtifactDescription::
    location_pre (Type const&)
    {
    }

    void ImplementationArtifactDescription::
    location_next (Type&)
    {
    }

    void ImplementationArtifactDescription::
    location_next (Type const&)
    {
    }

    void ImplementationArtifactDescription::
    location_post (Type&)
    {
    }

    void ImplementationArtifactDescription::
    location_post (Type const&)
    {
    }

    void ImplementationArtifactDescription::
    location_none (Type&)
    {
    }

    void ImplementationArtifactDescription::
    location_none (Type const&)
    {
    }

    void ImplementationArtifactDescription::
    execParameter (Type& o)
    {
      dispatch (o.execParameter ());
    }

    void ImplementationArtifactDescription::
    execParameter (Type const& o)
    {
      dispatch (o.execParameter ());
    }

    void ImplementationArtifactDescription::
    execParameter_none (Type&)
    {
    }

    void ImplementationArtifactDescription::
    execParameter_none (Type const&)
    {
    }

    void ImplementationArtifactDescription::
    deployRequirement (Type& o)
    {
      dispatch (o.deployRequirement ());
    }

    void ImplementationArtifactDescription::
    deployRequirement (Type const& o)
    {
      dispatch (o.deployRequirement ());
    }

    void ImplementationArtifactDescription::
    deployRequirement_none (Type&)
    {
    }

    void ImplementationArtifactDescription::
    deployRequirement_none (Type const&)
    {
    }

    void ImplementationArtifactDescription::
    dependsOn (Type& o)
    {
      // VC6 anathema strikes again
      //
      ImplementationArtifactDescription::Type::dependsOn_iterator b (o.begin_dependsOn()), e (o.end_dependsOn());

      if (b != e)
      {
        dependsOn_pre (o);
        for (;
         b != e;
        )
        {
          dispatch (*b);
          if (++b != e) dependsOn_next (o);
        }

        dependsOn_post (o);
      }

      else dependsOn_none (o);
    }

    void ImplementationArtifactDescription::
    dependsOn (Type const& o)
    {
      // VC6 anathema strikes again
      //
      ImplementationArtifactDescription::Type::dependsOn_const_iterator b (o.begin_dependsOn()), e (o.end_dependsOn());

      if (b != e)
      {
        dependsOn_pre (o);
        for (;
         b != e;
        )
        {
          dispatch (*b);
          if (++b != e) dependsOn_next (o);
        }

        dependsOn_post (o);
      }

      else dependsOn_none (o);
    }

    void ImplementationArtifactDescription::
    dependsOn_pre (Type&)
    {
    }

    void ImplementationArtifactDescription::
    dependsOn_pre (Type const&)
    {
    }

    void ImplementationArtifactDescription::
    dependsOn_next (Type&)
    {
    }

    void ImplementationArtifactDescription::
    dependsOn_next (Type const&)
    {
    }

    void ImplementationArtifactDescription::
    dependsOn_post (Type&)
    {
    }

    void ImplementationArtifactDescription::
    dependsOn_post (Type const&)
    {
    }

    void ImplementationArtifactDescription::
    dependsOn_none (Type&)
    {
    }

    void ImplementationArtifactDescription::
    dependsOn_none (Type const&)
    {
    }

    void ImplementationArtifactDescription::
    infoProperty (Type& o)
    {
      dispatch (o.infoProperty ());
    }

    void ImplementationArtifactDescription::
    infoProperty (Type const& o)
    {
      dispatch (o.infoProperty ());
    }

    void ImplementationArtifactDescription::
    infoProperty_none (Type&)
    {
    }

    void ImplementationArtifactDescription::
    infoProperty_none (Type const&)
    {
    }

    void ImplementationArtifactDescription::
    contentLocation (Type& o)
    {
      dispatch (o.contentLocation ());
    }

    void ImplementationArtifactDescription::
    contentLocation (Type const& o)
    {
      dispatch (o.contentLocation ());
    }

    void ImplementationArtifactDescription::
    contentLocation_none (Type&)
    {
    }

    void ImplementationArtifactDescription::
    contentLocation_none (Type const&)
    {
    }

    void ImplementationArtifactDescription::
    post (Type&)
    {
    }

    void ImplementationArtifactDescription::
    post (Type const&)
    {
    }
  }
}

namespace Deployment
{
  namespace Writer
  {
    // NamedImplementationArtifact
    //
    //

    NamedImplementationArtifact::
    NamedImplementationArtifact (::XSCRT::XML::Element< char >& e)
    : ::XSCRT::Writer< char > (e)
    {
    }

    NamedImplementationArtifact::
    NamedImplementationArtifact ()
    {
    }

    void NamedImplementationArtifact::
    traverse (Type const& o)
    {
      Traversal::NamedImplementationArtifact::traverse (o);
    }

    void NamedImplementationArtifact::
    name (Type const& o)
    {
      push_ (::XSCRT::XML::Element< char > ("name", top_ ()));
      Traversal::NamedImplementationArtifact::name (o);
      pop_ ();
    }

    void NamedImplementationArtifact::
    referencedArtifact (Type const& o)
    {
      push_ (::XSCRT::XML::Element< char > ("referencedArtifact", top_ ()));
      Traversal::NamedImplementationArtifact::referencedArtifact (o);
      pop_ ();
    }

    // ImplementationArtifactDescription
    //
    //

    ImplementationArtifactDescription::
    ImplementationArtifactDescription (::XSCRT::XML::Element< char >& e)
    : ::XSCRT::Writer< char > (e)
    {
    }

    ImplementationArtifactDescription::
    ImplementationArtifactDescription ()
    {
    }

    void ImplementationArtifactDescription::
    traverse (Type const& o)
    {
      Traversal::ImplementationArtifactDescription::traverse (o);
    }

    void ImplementationArtifactDescription::
    label (Type const& o)
    {
      push_ (::XSCRT::XML::Element< char > ("label", top_ ()));
      Traversal::ImplementationArtifactDescription::label (o);
      pop_ ();
    }

    void ImplementationArtifactDescription::
    UUID (Type const& o)
    {
      push_ (::XSCRT::XML::Element< char > ("UUID", top_ ()));
      Traversal::ImplementationArtifactDescription::UUID (o);
      pop_ ();
    }

    void ImplementationArtifactDescription::
    location_pre (Type const&)
    {
      push_ (::XSCRT::XML::Element< char > ("location", top_ ()));
    }

    void ImplementationArtifactDescription::
    location_next (Type const& o)
    {
      location_post (o);
      location_pre (o);
    }

    void ImplementationArtifactDescription::
    location_post (Type const&)
    {
      pop_ ();
    }

    void ImplementationArtifactDescription::
    execParameter (Type const& o)
    {
      push_ (::XSCRT::XML::Element< char > ("execParameter", top_ ()));
      Traversal::ImplementationArtifactDescription::execParameter (o);
      pop_ ();
    }

    void ImplementationArtifactDescription::
    deployRequirement (Type const& o)
    {
      push_ (::XSCRT::XML::Element< char > ("deployRequirement", top_ ()));
      Traversal::ImplementationArtifactDescription::deployRequirement (o);
      pop_ ();
    }

    void ImplementationArtifactDescription::
    dependsOn_pre (Type const&)
    {
      push_ (::XSCRT::XML::Element< char > ("dependsOn", top_ ()));
    }

    void ImplementationArtifactDescription::
    dependsOn_next (Type const& o)
    {
      dependsOn_post (o);
      dependsOn_pre (o);
    }

    void ImplementationArtifactDescription::
    dependsOn_post (Type const&)
    {
      pop_ ();
    }

    void ImplementationArtifactDescription::
    infoProperty (Type const& o)
    {
      push_ (::XSCRT::XML::Element< char > ("infoProperty", top_ ()));
      Traversal::ImplementationArtifactDescription::infoProperty (o);
      pop_ ();
    }

    void ImplementationArtifactDescription::
    contentLocation (Type const& o)
    {
      push_ (::XSCRT::XML::Element< char > ("contentLocation", top_ ()));
      Traversal::ImplementationArtifactDescription::contentLocation (o);
      pop_ ();
    }
  }
}

namespace Deployment
{
}


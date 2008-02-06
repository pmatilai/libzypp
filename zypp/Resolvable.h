/*---------------------------------------------------------------------\
|                          ____ _   __ __ ___                          |
|                         |__  / \ / / . \ . \                         |
|                           / / \ V /|  _/  _/                         |
|                          / /__ | | | | | |                           |
|                         /_____||_| |_| |_|                           |
|                                                                      |
\---------------------------------------------------------------------*/
/** \file zypp/Resolvable.h
 *
*/
#ifndef ZYPP_RESOLVABLE_H
#define ZYPP_RESOLVABLE_H

#include <iosfwd>
#include <string>

#include "zypp/base/Deprecated.h"

#include "zypp/base/ReferenceCounted.h"
#include "zypp/base/NonCopyable.h"
#include "zypp/base/PtrTypes.h"

#include "zypp/sat/Solvable.h"

#include "zypp/Dependencies.h"

///////////////////////////////////////////////////////////////////
namespace zypp
{ /////////////////////////////////////////////////////////////////

  struct NVRAD;

  ///////////////////////////////////////////////////////////////////
  //
  //	CLASS NAME : Resolvable
  //
  /** Interface base for resolvable objects (identification and dependencies).
   * \todo Merge with ResObject
  */
  class Resolvable : protected sat::Solvable,
                     public base::ReferenceCounted, private base::NonCopyable
  {
    friend std::ostream & operator<<( std::ostream & str, const Resolvable & obj );

  public:
    typedef Resolvable               Self;
    typedef ResTraits<Self>          TraitsType;
    typedef TraitsType::KindType     Kind;
    typedef TraitsType::PtrType      Ptr;
    typedef TraitsType::constPtrType constPtr;

  public:
    /** Whether this represents a valid- or no-solvable. */
    using sat::Solvable::operator bool_type;
    /** Whether this represents an installed solvable. */
    using sat::Solvable::isSystem;

    using sat::Solvable::ident;

    using sat::Solvable::kind;
    using sat::Solvable::name;
    using sat::Solvable::edition;
    using sat::Solvable::arch;

    /** \name Dependencies. */
    //@{
    /** Select by Dep. */
    Capabilities dep( Dep which_r ) const
    { return operator[]( which_r ); }
    using sat::Solvable::operator[];
    using sat::Solvable::provides;
    using sat::Solvable::requires;
    using sat::Solvable::conflicts;
    using sat::Solvable::obsoletes;
    using sat::Solvable::recommends;
    using sat::Solvable::suggests;
    using sat::Solvable::freshens;
    using sat::Solvable::enhances;
    using sat::Solvable::supplements;
    using sat::Solvable::prerequires;
    //@}

  public:
    const sat::Solvable & satSolvable() const { return *this; }

  protected:
    /** Ctor */
    Resolvable( const sat::Solvable & solvable_r );
    /** Dtor */
    virtual ~Resolvable();
    /** Helper for stream output */
    virtual std::ostream & dumpOn( std::ostream & str ) const;
 };
 ///////////////////////////////////////////////////////////////////

 /** \relates Resolvable Stream output */
 inline std::ostream & operator<<( std::ostream & str, const Resolvable & obj )
 { return obj.dumpOn( str ); }

 /** \relates Resolvable More verbose stream output including dependencies */
 inline std::ostream & dumpOn( std::ostream & str, const Resolvable & obj )
 { return dumpOn( str, obj.satSolvable() ); }

  /** Test whether a Resolvable::Ptr is of a certain Kind.
   * \return \c Ture iff \a p is not \c NULL and points to a Resolvable
   * of the specified Kind.
   * \relates Resolvable
   * \code
   * isKind<Package>(resPtr);
   * \endcode
  */
  template<class _Res>
    inline bool isKind( const Resolvable::constPtr & p )
    { return p && p->kind() == ResTraits<_Res>::kind; }

  // Specialization for Resolvable: Always true.
  template<>
    inline bool isKind<Resolvable>( const Resolvable::constPtr & p )
    { return p; }

  // Specialization for ResObject: Always true.
  template<>
    inline bool isKind<ResObject>( const Resolvable::constPtr & p )
    { return p; }


  /** Convert Resolvable::Ptr into Ptr of a certain Kind.
   * \return \c NULL iff \a p is \c NULL or points to a Resolvable
   * not of the specified Kind.
   * \relates Resolvable
   * \code
   * asKind<Package>(resPtr);
   * \endcode
  */
  template<class _Res>
    inline typename ResTraits<_Res>::PtrType asKind( const Resolvable::Ptr & p )
    { return dynamic_pointer_cast<_Res>(p); }

  template<class _Res>
    inline typename ResTraits<_Res>::constPtrType asKind( const Resolvable::constPtr & p )
    { return dynamic_pointer_cast<const _Res>(p); }

  ///////////////////////////////////////////////////////////////////

  /** \relates Resolvable Compare Resolvable::constPtr according to
   *  \a kind and \a name.
  */
  inline int compareByN( const Resolvable::constPtr & lhs,
                         const Resolvable::constPtr & rhs )
  {
    if ( lhs == rhs )
      return 0;
    if ( ! (lhs && rhs) )
      return lhs ? 1 : -1;
    int res = 0;
    if ( (res = lhs->kind().compare( rhs->kind() )) )
      return res;
    return lhs->name().compare( rhs->name() );
  }

  /** \relates Resolvable Compare Resolvable::constPtr according to
   *  \a kind, \a name and \a edition(compare!).
  */
  inline int compareByNVR( const Resolvable::constPtr & lhs,
                           const Resolvable::constPtr & rhs )
  {
    if ( lhs == rhs )
      return 0;
    if ( ! (lhs && rhs) )
      return lhs ? 1 : -1;
    int res = 0;
    if ( (res = lhs->kind().compare( rhs->kind() )) )
      return res;
    if ( (res = lhs->name().compare( rhs->name() )) )
      return res;
    return lhs->edition().compare( rhs->edition() );
  }

  /** \relates Resolvable Compare Resolvable::constPtr according to
   *  \a kind, \a name, \a edition(compare!) and \a arch.
  */
  inline int compareByNVRA( const Resolvable::constPtr & lhs,
                            const Resolvable::constPtr & rhs )
  {
    if ( lhs == rhs )
      return 0;
    if ( ! (lhs && rhs) )
      return lhs ? 1 : -1;
    int res = 0;
    if ( (res = lhs->kind().compare( rhs->kind() )) )
      return res;
    if ( (res = lhs->name().compare( rhs->name() )) )
      return res;
    if ( (res = lhs->edition().compare( rhs->edition() )) )
      return res;
    return lhs->arch().compare( rhs->arch() );
  }

  /////////////////////////////////////////////////////////////////
} // namespace zypp
///////////////////////////////////////////////////////////////////
#endif // ZYPP_RESOLVABLE_H

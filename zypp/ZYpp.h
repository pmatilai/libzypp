/*---------------------------------------------------------------------\
|                          ____ _   __ __ ___                          |
|                         |__  / \ / / . \ . \                         |
|                           / / \ V /|  _/  _/                         |
|                          / /__ | | | | | |                           |
|                         /_____||_| |_| |_|                           |
|                                                                      |
\---------------------------------------------------------------------*/
/** \file	zypp/ZYpp.h
 *
*/
#ifndef ZYPP_ZYPP_H
#define ZYPP_ZYPP_H

#include <iosfwd>

#include "zypp/base/ReferenceCounted.h"
#include "zypp/base/NonCopyable.h"
#include "zypp/base/PtrTypes.h"
#include "zypp/base/Deprecated.h"

#include "zypp/ZConfig.h"

#include "zypp/ZYppCommit.h"
#include "zypp/ResTraits.h"

#include "zypp/Target.h"
#include "zypp/Resolver.h"
#include "zypp/KeyRing.h"
#include "zypp/DiskUsageCounter.h"

///////////////////////////////////////////////////////////////////
namespace zypp
{ /////////////////////////////////////////////////////////////////

  namespace zypp_detail
  {
    class ZYppImpl;
  }

  class ZYppFactory;
  class ResPool;
  class ResPoolProxy;
  class Locale;
  class KeyRing;

  ///////////////////////////////////////////////////////////////////
  //
  //	CLASS NAME : ZYpp
  //
  /**
   * \todo define Exceptions
   * ZYpp API main interface
   */
  class ZYpp : public base::ReferenceCounted, private base::NonCopyable
  {
  public:

    typedef intrusive_ptr<ZYpp>       Ptr;
    typedef intrusive_ptr<const ZYpp> constPtr;

  public:

    /**
     * Access to the global resolvable pool.
     * Same as \ref zypp::ResPool::instance
     */
    ResPool pool() const;

    /** Pool of ui::Selectable.
     * Based on the ResPool, ui::Selectable groups ResObjetcs of
     * same kind and name.
    */
    ResPoolProxy poolProxy() const;

    DiskUsageCounter::MountPointSet diskUsage();

    void setPartitions(const DiskUsageCounter::MountPointSet &mp);

    DiskUsageCounter::MountPointSet getPartitions() const;

  public:
    /**
     * \throws Exception
     */
    Target_Ptr target() const;

    /**
     * \throws Exception
     * just init the target, dont populate store or pool
     */
    void initializeTarget(const Pathname & root);

    /**
     * \throws Exception
     */
    void finishTarget();


  public:
    typedef ZYppCommitResult CommitResult;

    /** Commit changes and transactions.
     * \param \ref CommitPolicy
     * \return \ref CommitResult
     * \throws Exception
    */
    ZYppCommitResult commit( const ZYppCommitPolicy & policy_r );

    /** Install a source package on the Target.
     * \throws Exception
     */
    void installSrcPackage( const SrcPackage_constPtr & srcPackage_r );

  public:
    /** */
    Resolver_Ptr resolver() const;
    KeyRing_Ptr keyRing() const;
  public:
    /** Set the preferred locale for translated labels, descriptions,
     *  etc. passed to the UI.
     * \deprecated Use ZConfig diretcly.
     */
    ZYPP_DEPRECATED void setTextLocale( const Locale & textLocale_r )
    { ZConfig::instance().setTextLocale( textLocale_r ); }
    /** \deprecated Use ZConfig diretcly. */
    ZYPP_DEPRECATED Locale getTextLocale() const
    { return ZConfig::instance().textLocale(); }

  public:
    /** \name move to pool
    */
    //@{
    typedef std::set<Locale> LocaleSet;
    /** Set the requested locales.
     * Languages to be supported by the system, e.g. language specific
     * packages to be installed. This function operates on the pool,
     * so only the locales that are available as resolvables
     * are marked as requested. The rest is ignored.
    */
    void setRequestedLocales( const LocaleSet & locales_r );
    /** */
    LocaleSet getRequestedLocales() const;

    /**
     * Get the set of available locales.
     * This is computed from the package data so it actually
     * represents all locales packages claim to support.
     */
    LocaleSet getAvailableLocales() const;

    /**
     * internal use only
     **/
    void availableLocale( const Locale & locale_r );
    //@}

  public:
    /** Get the path where zypp related plugins store persistent data and caches   */
    Pathname homePath() const;

    /** Get the path where zypp related plugins store temp data   */
    Pathname tmpPath() const;

    /** set the home, if you need to change it */
    void setHomePath( const Pathname & path );

    /** Get the system architecture.
      * \deprecated Use ZConfig diretcly.
    */
    ZYPP_DEPRECATED Arch architecture() const
    { return ZConfig::instance().systemArchitecture(); }
    /** Set the system architecture.
     * This should be used for testing/debugging only since the Target backend
     * won't be able to install incompatible packages ;-)
     * \deprecated Use ZConfig diretcly.
    */
    ZYPP_DEPRECATED void setArchitecture( const Arch & arch )
    { ZConfig::instance().setSystemArchitecture( arch ); }

  public:

   /**
    * \short Apply persistant locks to current pool.
    * Call this before solving
    *
    * \returns Number of items locked
    */
   int applyLocks();

  protected:
    /** Dtor */
    virtual ~ZYpp();
    /** Stream output */
    virtual std::ostream & dumpOn( std::ostream & str ) const;
  private:
    /** Factory */
    friend class ZYppFactory;

    /** */
    typedef zypp_detail::ZYppImpl Impl;
    typedef shared_ptr<Impl>      Impl_Ptr;
    /** Factory ctor */
    explicit
    ZYpp( const Impl_Ptr & impl_r );
  private:
    /** Pointer to implementation */
    RW_pointer<Impl> _pimpl;
  };
  ///////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////
} // namespace zypp
///////////////////////////////////////////////////////////////////
#endif // ZYPP_ZYPP_H

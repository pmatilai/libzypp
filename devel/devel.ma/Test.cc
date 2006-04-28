#include <ctime>
#include <iostream>
#include <fstream>
#include "Tools.h"

#include <zypp/base/IOStream.h>
#include <zypp/base/PtrTypes.h>
#include <zypp/base/Exception.h>
#include <zypp/base/ProvideNumericId.h>

#include "zypp/ZYppFactory.h"
#include "zypp/ResPoolProxy.h"
#include <zypp/SourceManager.h>
#include <zypp/SourceFactory.h>

#include "zypp/NVRAD.h"
#include "zypp/ResPool.h"
#include "zypp/ResFilters.h"
#include "zypp/CapFilters.h"
#include "zypp/Package.h"
#include "zypp/Language.h"
#include "zypp/NameKindProxy.h"


using namespace std;
using namespace zypp;
using namespace zypp::ui;
using namespace zypp::functor;

///////////////////////////////////////////////////////////////////

static const Pathname sysRoot( "/Local/ROOT" );

///////////////////////////////////////////////////////////////////

struct DebugStats : public std::unary_function<ResObject::constPtr, bool>
{
  bool operator()( const ResObject::constPtr & ptr )
  {
    Source_Ref::NumericId srcid = ptr->source().numericId();
    unsigned mediaid = mediaId( ptr );
    USR << "S"  << srcid
        << "/M" << mediaid
        << " "  << ptr
        << endl;
    return true;
  }

  unsigned mediaId( const ResObject::constPtr & ptr ) const
  {
    Package::constPtr pkg( asKind<Package>(ptr) );
    return pkg ? pkg->mediaId() : 0;
  }


  public:
    typedef std::map<ResolvableTraits::KindType,Counter<unsigned> > KindMap;
    Counter<unsigned> _total;
    KindMap           _perKind;
};

inline std::ostream & operator<<( std::ostream & str, const DebugStats & obj )
{ return str; }

template <class _Iterator>
  std::ostream & vdumpPoolStats( std::ostream & str,
                                 _Iterator begin_r, _Iterator end_r )
  {
    DebugStats stats;
    std::for_each( begin_r, end_r,
                   functor::functorRef<bool,ResObject::constPtr>(stats) );
    return str << stats;
  }

///////////////////////////////////////////////////////////////////

inline bool selectForTransact( const NameKindProxy & nkp )
{
  if ( nkp.availableEmpty() )
    return false;

  return nkp.availableBegin()->status().setTransact( true, ResStatus::USER );
}
///////////////////////////////////////////////////////////////////
namespace zypp
{ /////////////////////////////////////////////////////////////////
#if 0
  ///////////////////////////////////////////////////////////////////
  //
  //	CLASS NAME : EnumerationClass<_Derived>
  //
  /** EnumerationClass.
  */
  template<class _Derived>
    class EnumerationClass
    {
    public:
      /** The enum */
      typedef _Derived::for_use_in_switch for_use_in_switch;

      /** String representation of enumarator. */
      const std::string & asString() const;

      /** Enumarator provided for use in \c switch statement. */
      for_use_in_switch inSwitch() const
      { return _op };

    protected:
      /** Ctor */
      EnumerationClass( for_use_in_switch op_r )
      : _op( op_r )
      {}
    private:
      /** The enum value. */
      for_use_in_switch _op;
    };
  ///////////////////////////////////////////////////////////////////

  /** \relates EnumerationClass Stream output. */
  template<class _Derived>
    inline std::ostream & operator<<( std::ostream & str, const EnumerationClass<_Derived> & obj )
    { return str << obj.asString(); }

  /** \relates EnumerationClass Stream output. */
  template<class _Derived>
    inline bool operator==( const EnumerationClass<_Derived> & lhs, const EnumerationClass<_Derived> & rhs )
    { return lhs.inSwitch() == rhs.inSwitch(); }

  /** \relates PatchCategory */
  template<class _Derived>
    inline bool operator!=( const EnumerationClass<_Derived> & lhs, const EnumerationClass<_Derived> & rhs )
    { return ! ( lhs == rhs ); }
#endif
  /////////////////////////////////////////////////////////////////
} // namespace zypp
///////////////////////////////////////////////////////////////////

/******************************************************************
**
**      FUNCTION NAME : main
**      FUNCTION TYPE : int
*/
int main( int argc, char * argv[] )
{
  //zypp::base::LogControl::instance().logfile( "xxx" );
  INT << "===[START]==========================================" << endl;

  ResPool pool( getZYpp()->pool() );

  if ( 0 )
    {
      getZYpp()->initTarget( sysRoot );
      getZYpp()->addResolvables( getZYpp()->target()->resolvables(), true );
      INT << "Added target: " << pool << endl;
    }

  Source_Ref src1( createSource( "nfs://machcd2/CDs/SLES-10-CD-i386-Beta10/CD1" ) );

  getZYpp()->initTarget( "/" );

  if ( 1 )
    {
      Source_Ref src1( createSource( "dir:/Local/TEST" ) );
      getZYpp()->addResolvables( src1.resolvables() );
      INT << "Pool: " << pool << endl;
    }

  selectForTransact( nameKindProxy<Package>( pool, "ccur-su-nslm" ) );

  vdumpPoolStats( INT << "Transacting: ",
                  make_filter_begin<resfilter::ByTransact>(pool),
                  make_filter_end<resfilter::ByTransact>(pool) ) << endl;

  getZYpp()->commit( ZYppCommitPolicy() );

  INT << "===[END]============================================" << endl << endl;
  return 0;
}

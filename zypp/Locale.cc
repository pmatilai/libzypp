/*---------------------------------------------------------------------\
|                          ____ _   __ __ ___                          |
|                         |__  / \ / / . \ . \                         |
|                           / / \ V /|  _/  _/                         |
|                          / /__ | | | | | |                           |
|                         /_____||_| |_| |_|                           |
|                                                                      |
\---------------------------------------------------------------------*/
/** \file	zypp/Locale.cc
 *
*/
#include <iostream>
//#include "zypp/base/Logger.h"

#include "zypp/Locale.h"

using std::endl;

///////////////////////////////////////////////////////////////////
namespace zypp
{ /////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////
  //
  //	CLASS NAME : Locale::Impl
  //
  /** Locale implementation. */
  struct Locale::Impl
  {
    Impl()
    {}

    Impl( const std::string & code_r )
    {
      std::string t;
      std::string::size_type sep = code_r.find_first_of( "@." );
      if ( sep == std::string::npos ) {
        t = code_r;
      } else {
        t = code_r.substr( 0, sep );
      }

      sep = t.find( '_' );
      if ( sep == std::string::npos ) {
        _language = LanguageCode( t );
      } else {
        _language = LanguageCode( t.substr( 0, sep ) );
        _country = CountryCode( t.substr( sep+1 ) );
      }
    }

    Impl( const LanguageCode & language_r,
          const CountryCode & country_r )
    : _language( language_r )
    , _country( country_r )
    {}

    const LanguageCode & language() const
    { return _language; }

    const CountryCode & country() const
    { return _country; }

    std::string code() const
    {
      std::string ret( _language.code() );
      if ( _country.hasCode() )
        ret += "_" + _country.code();
      return ret;
    }

    std::string name() const
    {
      std::string ret( _language.name() );
      if ( _country.hasCode() )
        ret += " (" + _country.name() + ")";
      return ret;
    }

    Locale fallback() const
    {  
      // blah_BLAH
      if ( _country.hasCode() )
      {
        return Locale(_language.code());
      }
      else
      {
        if (_language.code() == "en" )
        { // no fallback for english
          return Locale();
        }
        else if ( (_language.code() == "") )
        { // for empty locale, we give up
          return Locale();
        }
        else
        { // for all others, english
          return Locale("en");
        }
      }
      return Locale();        
    }
    
  private:

    LanguageCode _language;
    CountryCode _country;

  public:
    /** Offer default Impl. */
    static shared_ptr<Impl> nullimpl()
    {
      static shared_ptr<Impl> _nullimpl( new Impl );
      return _nullimpl;
    }
  };
  ///////////////////////////////////////////////////////////////////

  /** \relates Locale::Impl Stream output */
  inline std::ostream & operator<<( std::ostream & str, const Locale::Impl & obj )
  {
    return str << "Locale::Impl";
  }

  ///////////////////////////////////////////////////////////////////
  //
  //	CLASS NAME : Locale
  //
  ///////////////////////////////////////////////////////////////////

  const Locale Locale::noCode;

  ///////////////////////////////////////////////////////////////////
  //
  //	METHOD NAME : Locale::Locale
  //	METHOD TYPE : Ctor
  //
  Locale::Locale()
  : _pimpl( Impl::nullimpl() )
  {}

  ///////////////////////////////////////////////////////////////////
  //
  //	METHOD NAME : Locale::Locale
  //	METHOD TYPE : Ctor
  //
  Locale::Locale( const std::string & code_r )
  : _pimpl( new Impl( code_r ) )
  {}

  ///////////////////////////////////////////////////////////////////
  //
  //	METHOD NAME : Locale::Locale
  //	METHOD TYPE : Ctor
  //
  Locale::Locale( const LanguageCode & language_r,
                  const CountryCode & country_r )
  : _pimpl( new Impl( language_r, country_r ) )
  {}

  ///////////////////////////////////////////////////////////////////
  //
  //	METHOD NAME : Locale::~Locale
  //	METHOD TYPE : Dtor
  //
  Locale::~Locale()
  {}

  ///////////////////////////////////////////////////////////////////
  //
  //	METHOD NAME : Locale::
  //	METHOD TYPE :
  //
  const LanguageCode & Locale::language() const
  { return _pimpl->language(); }

  ///////////////////////////////////////////////////////////////////
  //
  //	METHOD NAME : Locale::
  //	METHOD TYPE :
  //
  const CountryCode & Locale::country() const
  { return _pimpl->country(); }

  ///////////////////////////////////////////////////////////////////
  //
  //	METHOD NAME : Locale::
  //	METHOD TYPE :
  //
  std::string Locale::code() const
  { return _pimpl->code(); }

  ///////////////////////////////////////////////////////////////////
  //
  //	METHOD NAME : Locale::
  //	METHOD TYPE :
  //
  std::string Locale::name() const
  { return _pimpl->name(); }

  ///////////////////////////////////////////////////////////////////
  //
  //    METHOD NAME : Locale::
  //    METHOD TYPE :
  //
  Locale Locale::fallback() const
  { return _pimpl->fallback(); }
  /////////////////////////////////////////////////////////////////
} // namespace zypp
///////////////////////////////////////////////////////////////////

/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                       Paraver Main Computing Library                      *
 *****************************************************************************
 *     ___     This library is free software; you can redistribute it and/or *
 *    /  __         modify it under the terms of the GNU LGPL as published   *
 *   /  /  _____    by the Free Software Foundation; either version 2.1      *
 *  /  /  /     \   of the License, or (at your option) any later version.   *
 * (  (  ( B S C )                                                           *
 *  \  \  \_____/   This library is distributed in hope that it will be      *
 *   \  \__         useful but WITHOUT ANY WARRANTY; without even the        *
 *    \___          implied warranty of MERCHANTABILITY or FITNESS FOR A     *
 *                  PARTICULAR PURPOSE. See the GNU LGPL for more details.   *
 *                                                                           *
 * You should have received a copy of the GNU Lesser General Public License  *
 * along with this library; if not, write to the Free Software Foundation,   *
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA          *
 * The GNU LEsser General Public License is contained in the file COPYING.   *
 *                                 ---------                                 *
 *   Barcelona Supercomputing Center - Centro Nacional de Supercomputacion   *
\*****************************************************************************/

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#ifndef SEMANTICCOMPOSEFUNCTIONS_H_INCLUDED
#define SEMANTICCOMPOSEFUNCTIONS_H_INCLUDED

#include <stack>
#include "semanticcompose.h"

class ComposeAsIs: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeAsIs()
    {
      setDefaultParam();
    }

    ~ComposeAsIs()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}


    virtual std::string getName()
    {
      return ComposeAsIs::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeAsIs( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return SAME_TYPE;
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return ( TParamValue ) 0;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "";
    }
  private:
    static const bool initFromBegin = false;
    static std::string name;
};


class ComposeSign: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeSign()
    {
      setDefaultParam();
    }

    ~ComposeSign()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

    virtual std::string getName()
    {
      return ComposeSign::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeSign( *this );
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return ( TParamValue ) 0;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;

};


class ComposeUnsign: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeUnsign()
    {
      setDefaultParam();
    }

    ~ComposeUnsign()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

    virtual std::string getName()
    {
      return ComposeUnsign::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeUnsign( *this );
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return ( TParamValue ) 0;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;

};


class ComposeMod: public SemanticCompose
{
  public:
    typedef enum
    {
      DIVIDER = 0,
      MAXPARAM
    } TParam;

    ComposeMod()
    {
      setDefaultParam();
    }

    ~ComposeMod()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

    virtual std::string getName()
    {
      return ComposeMod::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeMod( *this );
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if ( whichParam == DIVIDER )
        tmp.push_back( 1 );
      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "Divider";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;

};


class ComposeModPlus1: public SemanticCompose
{
  public:
    typedef enum
    {
      DIVIDER = 0,
      MAXPARAM
    } TParam;

    ComposeModPlus1()
    {
      setDefaultParam();
    }

    ~ComposeModPlus1()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

    virtual std::string getName()
    {
      return ComposeModPlus1::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeModPlus1( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if ( whichParam == DIVIDER )
        tmp.push_back( 1 );
      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "Divider";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;

};


class ComposeDivide: public SemanticCompose
{
  public:
    typedef enum
    {
      DIVIDER = 0,
      MAXPARAM
    } TParam;

    ComposeDivide()
    {
      setDefaultParam();
    }

    ~ComposeDivide()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

    virtual std::string getName()
    {
      return ComposeDivide::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeDivide( *this );
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if ( whichParam == DIVIDER )
        tmp.push_back( 1 );
      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "Divider";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;

};


class ComposeProduct: public SemanticCompose
{
  public:
    typedef enum
    {
      FACTOR = 0,
      MAXPARAM
    } TParam;

    ComposeProduct()
    {
      setDefaultParam();
    }

    ~ComposeProduct()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

    virtual std::string getName()
    {
      return ComposeProduct::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeProduct( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if ( whichParam == FACTOR )
        tmp.push_back( 0 );
      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "Factor";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;

};


class ComposeAdding: public SemanticCompose
{
  public:
    typedef enum
    {
      FACTOR = 0,
      MAXPARAM
    } TParam;

    ComposeAdding()
    {
      setDefaultParam();
    }

    ~ComposeAdding()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

    virtual std::string getName()
    {
      return ComposeAdding::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeAdding( *this );
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if ( whichParam == FACTOR )
        tmp.push_back( 0 );
      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "Value";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;

};


class ComposeSubstract: public SemanticCompose
{
  public:
    typedef enum
    {
      FACTOR = 0,
      MAXPARAM
    } TParam;

    ComposeSubstract()
    {
      setDefaultParam();
    }

    ~ComposeSubstract()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

    virtual std::string getName()
    {
      return ComposeSubstract::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeSubstract( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if ( whichParam == FACTOR )
        tmp.push_back( 0 );
      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "Value";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;

};


class ComposeSelectRange: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXVALUE = 0,
      MINVALUE,
      MAXPARAM
    } TParam;

    ComposeSelectRange()
    {
      setDefaultParam();
    }

    ~ComposeSelectRange()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

    virtual std::string getName()
    {
      return ComposeSelectRange::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeSelectRange( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return SAME_TYPE;
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if ( whichParam == MAXVALUE )
        tmp.push_back( 1 );
      else if ( whichParam == MINVALUE )
        tmp.push_back( 0 );

      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      if( whichParam == MAXVALUE )
        return "Max value";
      else if( whichParam == MINVALUE )
        return "Min value";
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;
};


class ComposeSelectRangeOpen: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXVALUE = 0,
      MINVALUE,
      MAXPARAM
    } TParam;

    ComposeSelectRangeOpen()
    {
      setDefaultParam();
    }

    ~ComposeSelectRangeOpen()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

    virtual std::string getName()
    {
      return ComposeSelectRangeOpen::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeSelectRangeOpen( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return SAME_TYPE;
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if ( whichParam == MAXVALUE )
        tmp.push_back( 1 );
      else if ( whichParam == MINVALUE )
        tmp.push_back( 0 );

      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      if( whichParam == MAXVALUE )
        return "Max value";
      else if( whichParam == MINVALUE )
        return "Min value";
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;
};


class ComposeIsInRange: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXVALUE = 0,
      MINVALUE,
      MAXPARAM
    } TParam;

    ComposeIsInRange()
    {
      setDefaultParam();
    }

    ~ComposeIsInRange()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

    virtual std::string getName()
    {
      return ComposeIsInRange::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeIsInRange( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if ( whichParam == MAXVALUE )
        tmp.push_back( 1 );
      else if ( whichParam == MINVALUE )
        tmp.push_back( 0 );

      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      if( whichParam == MAXVALUE )
        return "Max value";
      else if( whichParam == MINVALUE )
        return "Min value";
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;

};


class ComposeIsInRangeOpen: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXVALUE = 0,
      MINVALUE,
      MAXPARAM
    } TParam;

    ComposeIsInRangeOpen()
    {
      setDefaultParam();
    }

    ~ComposeIsInRangeOpen()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

    virtual std::string getName()
    {
      return ComposeIsInRangeOpen::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeIsInRangeOpen( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if ( whichParam == MAXVALUE )
        tmp.push_back( 1 );
      else if ( whichParam == MINVALUE )
        tmp.push_back( 0 );

      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      if( whichParam == MAXVALUE )
        return "Max value";
      else if( whichParam == MINVALUE )
        return "Min value";
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;
};


class ComposeIsEqual: public SemanticCompose
{
  public:
    typedef enum
    {
      VALUES = 0,
      MAXPARAM
    } TParam;

    ComposeIsEqual()
    {
      setDefaultParam();
    }

    ~ComposeIsEqual()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

    virtual std::string getName()
    {
      return ComposeIsEqual::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeIsEqual( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return SAME_TYPE;
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if ( whichParam == VALUES )
        tmp.push_back( 1 );

      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "Values";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;

};


class ComposeIsEqualSign: public SemanticCompose
{
  public:
    typedef enum
    {
      VALUES = 0,
      MAXPARAM
    } TParam;

    ComposeIsEqualSign()
    {
      setDefaultParam();
    }

    ~ComposeIsEqualSign()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

    virtual std::string getName()
    {
      return ComposeIsEqualSign::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeIsEqualSign( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if ( whichParam == VALUES )
        tmp.push_back( 1 );

      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "Values";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;

};


class ComposeFloor: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeFloor()
    {
      setDefaultParam();
    }

    ~ComposeFloor()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}


    virtual std::string getName()
    {
      return ComposeFloor::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeFloor( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return SAME_TYPE;
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return ( TParamValue ) 0;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "";
    }
  private:
    static const bool initFromBegin = false;
    static std::string name;
};


class ComposeCeil: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeCeil()
    {
      setDefaultParam();
    }

    ~ComposeCeil()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}


    virtual std::string getName()
    {
      return ComposeCeil::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeCeil( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return SAME_TYPE;
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return ( TParamValue ) 0;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "";
    }
  private:
    static const bool initFromBegin = false;
    static std::string name;
};


class ComposeRound: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeRound()
    {
      setDefaultParam();
    }

    ~ComposeRound()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}


    virtual std::string getName()
    {
      return ComposeRound::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeRound( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return SAME_TYPE;
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return ( TParamValue ) 0;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "";
    }
  private:
    static const bool initFromBegin = false;
    static std::string name;
};


class ComposeAbs: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeAbs()
    {
      setDefaultParam();
    }

    ~ComposeAbs()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}


    virtual std::string getName()
    {
      return ComposeAbs::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeAbs( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return SAME_TYPE;
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return ( TParamValue ) 0;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "";
    }
  private:
    static const bool initFromBegin = false;
    static std::string name;
};


class ComposeStackedValue: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeStackedValue()
    {
      setDefaultParam();
    }

    ~ComposeStackedValue()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow );

    virtual std::string getName()
    {
      return ComposeStackedValue::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeStackedValue( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return SAME_TYPE;
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );

      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "";
    }

    virtual std::vector<std::vector<TSemanticValue> > *getStack()
    {
      return &myStack;
    }

  private:
    static const bool initFromBegin = true;
    static std::string name;

    std::vector<std::vector<TSemanticValue> > myStack;
};


class ComposeInStackedValue: public SemanticCompose
{
  public:
    typedef enum
    {
      VALUE = 0,
      MAXPARAM
    } TParam;

    ComposeInStackedValue()
    {
      setDefaultParam();
    }

    ~ComposeInStackedValue()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow );

    virtual std::string getName()
    {
      return ComposeInStackedValue::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeInStackedValue( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      else if ( whichParam == VALUE )
        tmp.push_back( 1 );

      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "Value";
    }

    virtual std::vector<std::vector<TSemanticValue> > *getStack()
    {
      return &myStack;
    }

  private:
    static const bool initFromBegin = true;
    static std::string name;

    std::vector<std::vector<TSemanticValue> > myStack;
};


class ComposeNestingLevel: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeNestingLevel()
    {
      setDefaultParam();
    }

    ~ComposeNestingLevel()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow );

    virtual std::string getName()
    {
      return ComposeNestingLevel::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeNestingLevel( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );

      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = true;
    static std::string name;

    std::vector<TSemanticValue> myStack;
};


class ComposeEnumerate: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeEnumerate()
    {
      setDefaultParam();
    }

    ~ComposeEnumerate()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow );

    virtual std::string getName()
    {
      return ComposeEnumerate::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeEnumerate( *this );
    }


  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );

      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = true;
    static std::string name;

    std::vector<TSemanticValue> myEnumerate;
};


class ComposeDelta: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeDelta()
    {
      setDefaultParam();
    }

    ~ComposeDelta()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow );

    virtual std::string getName()
    {
      return ComposeDelta::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeDelta( *this );
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );

      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = true;
    static std::string name;

    std::vector<TSemanticValue> semPrevValue;
};


class ComposeBurstTime: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeBurstTime()
    {
      setDefaultParam();
    }

    ~ComposeBurstTime()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

    virtual std::string getName()
    {
      return ComposeBurstTime::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeBurstTime( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return TIME_TYPE;
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );

      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;

};


class ComposeJoinBursts: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeJoinBursts()
    {
      setDefaultParam();
    }

    ~ComposeJoinBursts()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

    virtual std::string getName()
    {
      return ComposeJoinBursts::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeJoinBursts( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return SAME_TYPE;
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );

      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = true;
    static std::string name;

};

class ComposeBeginTime: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeBeginTime()
    {
      setDefaultParam();
    }

    ~ComposeBeginTime()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

    virtual std::string getName()
    {
      return ComposeBeginTime::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeBeginTime( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return TIME_TYPE;
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );

      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;

};

class ComposeEndTime: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeEndTime()
    {
      setDefaultParam();
    }

    ~ComposeEndTime()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

    virtual std::string getName()
    {
      return ComposeEndTime::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeEndTime( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return TIME_TYPE;
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );

      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;

};


class ComposeTranslate: public SemanticCompose
{
  public:
    typedef enum
    {
      TRANSLATION_LIST = 0,
      MAXPARAM
    } TParam;

    ComposeTranslate()
    {
      setDefaultParam();
    }

    ~ComposeTranslate()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}

    virtual std::string getName()
    {
      return ComposeTranslate::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeTranslate( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return SAME_TYPE; // Some doubts
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      TParamValue tmp;

      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );

      return tmp;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "Translation List";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;

};


class ComposeSine: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeSine()
    {
      setDefaultParam();
    }

    ~ComposeSine()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}


    virtual std::string getName()
    {
      return ComposeSine::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeSine( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return NO_TYPE;
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return ( TParamValue ) 0;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "";
    }
  private:
    static const bool initFromBegin = false;
    static std::string name;
};


class ComposeCosine: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeCosine()
    {
      setDefaultParam();
    }

    ~ComposeCosine()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}


    virtual std::string getName()
    {
      return ComposeCosine::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeCosine( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return NO_TYPE;
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return ( TParamValue ) 0;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "";
    }
  private:
    static const bool initFromBegin = false;
    static std::string name;
};


class ComposeSqrtAbs: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeSqrtAbs()
    {
      setDefaultParam();
    }

    ~ComposeSqrtAbs()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}


    virtual std::string getName()
    {
      return ComposeSqrtAbs::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeSqrtAbs( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return NO_TYPE;
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return ( TParamValue ) 0;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "";
    }
  private:
    static const bool initFromBegin = false;
    static std::string name;
};


class ComposeArcTan: public SemanticCompose
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    ComposeArcTan()
    {
      setDefaultParam();
    }

    ~ComposeArcTan()
    {}

    virtual TParamIndex getMaxParam() const
    {
      return MAXPARAM;
    }

    virtual TSemanticValue execute( const SemanticInfo *info );

    virtual void init( KWindow *whichWindow )
    {}


    virtual std::string getName()
    {
      return ComposeArcTan::name;
    }

    virtual SemanticFunction *clone()
    {
      return new ComposeArcTan( *this );
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return NO_TYPE;
    }

  protected:
    virtual const bool getMyInitFromBegin()
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return ( TParamValue ) 0;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return "";
    }
  private:
    static const bool initFromBegin = false;
    static std::string name;
};

#endif // SEMANTICCOMPOSEFUNCTIONS_H_INCLUDED

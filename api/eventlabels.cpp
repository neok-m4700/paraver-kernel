/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                      API Library for libparaver-kernel                    *
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

#include "eventlabels.h"

#ifdef OLD_PCFPARSER
#include "utils/pcfparser/old/ParaverEventType.h"
#include "utils/pcfparser/old/ParaverEventValue.h"
#endif

using namespace std;

const string EventLabels::unknownLabel = "Unknown";

EventLabels::EventLabels()
{}

#ifdef OLD_PCFPARSER

EventLabels::EventLabels( const set<TEventType>& eventsLoaded )
{
/*  for ( set<TEventType>::const_iterator it = eventsLoaded.begin();
        it != eventsLoaded.end(); ++it )
    eventType2Label[ *it ] = unknownLabel + " type ";*/
}

EventLabels::EventLabels( const libparaver::ParaverTraceConfig& config,
                          const set<TEventType>& eventsLoaded )
{
/*  for ( set<TEventType>::const_iterator it = eventsLoaded.begin();
        it != eventsLoaded.end(); ++it )
    eventType2Label[ *it ] = unknownLabel + " type ";*/

  const vector<ParaverEventType *>& types = config.get_eventTypes();
  for ( vector<ParaverEventType *>::const_iterator it = types.begin();
        it != types.end(); ++it )
  {
    eventType2Label[ ( *it )->get_key() ] = ( *it )->get_description();
    label2eventType[ ( *it )->get_description() ] = ( *it )->get_key();
    eventValue2Label[ ( *it )->get_key() ] = map<TEventValue, string>();
    const vector<ParaverEventValue *>& values = ( *it )->get_eventValues();
    for ( vector<ParaverEventValue *>::const_iterator itVal = values.begin();
          itVal != values.end(); ++itVal )
    {
      ( eventValue2Label[ ( *it )->get_key() ] )[ ( *itVal )->get_key() ] =
        ( *itVal )->get_value();

      multimap< TEventType, TEventValue >& tmpEventMap = label2eventValue[ ( *itVal )->get_value() ];
      tmpEventMap.insert( make_pair( ( *it )->get_key(), ( *itVal )->get_key() ) );
    }
  }
}

#else


EventLabels::EventLabels( const set<TEventType>& eventsLoaded )
{
/*  for ( set<TEventType>::const_iterator it = eventsLoaded.begin();
        it != eventsLoaded.end(); ++it )
    eventType2Label[ *it ] = unknownLabel + " type ";*/
}
#include <iostream>
EventLabels::EventLabels( const libparaver::UIParaverTraceConfig& config,
                          const set<TEventType>& eventsLoaded )
{
/*  for ( set<TEventType>::const_iterator it = eventsLoaded.begin();
        it != eventsLoaded.end(); ++it )
    eventType2Label[ *it ] = unknownLabel + " type ";*/

  try
  {
    const vector<unsigned int>& types = config.getEventTypes();
    for ( vector<unsigned int>::const_iterator it = types.begin(); it != types.end(); ++it )
    {
      eventType2Label[ *it ] = config.getEventType( *it );
      label2eventType[ config.getEventType( *it ) ] = *it;
      eventValue2Label[ *it ] = map<TEventValue, string>();
      try
      {
        vector<unsigned int> values = config.getEventValues( *it );
        for ( vector<unsigned int>::const_iterator itVal = values.begin(); itVal != values.end(); ++itVal )
        {
          ( eventValue2Label[ *it ] )[ *itVal ] = config.getEventValue( *it, *itVal );

          multimap< TEventType, TEventValue >& tmpEventMap = label2eventValue[ config.getEventValue( *it, *itVal ) ];
          tmpEventMap.insert( make_pair( *it, *itVal ) );
        }
      }
      catch( libparaver::UIParaverTraceConfig::value_not_found )
      {
      }
    }
  }
  catch( libparaver::UIParaverTraceConfig::value_not_found )
  {
  }

}

#endif

EventLabels::~EventLabels()
{}

void EventLabels::getTypes( vector<TEventType>& onVector ) const
{
  for ( map<TEventType, string>::const_iterator it = eventType2Label.begin();
        it != eventType2Label.end(); ++it )
    onVector.push_back( ( *it ).first );
}

bool EventLabels::getEventTypeLabel( TEventType type, string& onStr ) const
{
  map<TEventType, string>::const_iterator it = eventType2Label.find( type );
  if ( it == eventType2Label.end() )
  {
    onStr = unknownLabel;
    return false;
  }
  onStr = ( *it ).second;
  return true;
}

bool EventLabels::getEventValueLabel( TEventType type, TEventValue value, string& onStr ) const
{
  map<TEventType, map<TEventValue, string> >::const_iterator it = eventValue2Label.find( type );
  if ( it == eventValue2Label.end() )
  {
    onStr = unknownLabel;
    return false;
  }
  map<TEventValue, string>::const_iterator itVal = ( *it ).second.find( value );
  if ( itVal == ( *it ).second.end() )
  {
    onStr = unknownLabel;
    return false;
  }
  onStr = ( *itVal ).second;
  return true;
}

bool EventLabels::getEventValueLabel( TEventValue value, string& onStr ) const
{
  bool found = false;
  onStr = unknownLabel;

  map<TEventType, map<TEventValue, string> >::const_iterator it = eventValue2Label.begin();
  while ( !found && it != eventValue2Label.end() )
  {
    map<TEventValue, string>::const_iterator itVal = ( *it ).second.find( value );
    if ( itVal != ( *it ).second.end() )
    {
      onStr = ( *itVal ).second;
      found = true;
    }
    ++it;
  }

  return found;
}


bool EventLabels::getValues( TEventType type, vector<string> &values ) const
{
  map< TEventType, map< TEventValue, string > >::const_iterator it = eventValue2Label.find( type );
  if ( it == eventValue2Label.end() )
  {
    return false;
  }
  else
  {
    for ( map< TEventValue, string >::const_iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2 )
      values.push_back((*it2).second );
  }

  return true;
}


bool EventLabels::getValues( TEventType type, map<TEventValue, string> &values ) const
{
  map< TEventType, map< TEventValue, string > >::const_iterator it = eventValue2Label.find( type );
  if ( it == eventValue2Label.end() )
  {
    return false;
  }
  else
  {
    for ( map<TEventValue, string>::const_iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2 )
      values[ (*it2).first ] = (*it2).second;
  }

  return true;
}


bool EventLabels::getEventType( const string& whichTypeLabel, TEventType& onType ) const
{
  map<string, TEventType>::const_iterator it = label2eventType.find( whichTypeLabel );
  if( it == label2eventType.end() )
    return false;

  onType = (*it).second;
  return true;
}


bool EventLabels::getEventValue( const string& whichValueLabel, multimap< TEventType, TEventValue >& onMap ) const
{
  map<string, multimap< TEventType, TEventValue > >::const_iterator it = label2eventValue.find( whichValueLabel );

  if( it == label2eventValue.end() )
    return false;

  onMap = (*it).second;
  return true;
}

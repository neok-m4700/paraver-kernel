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

#include <set>

#include "traceeditactions.h"
#include "eventtranslator.h"
#ifdef OLD_PCFPARSER
#include "utils/pcfparser/old/ParaverTraceConfig.h"
#else
#include "utils/pcfparser/UIParaverTraceConfig.h"
#endif

using std::set;

/****************************************************************************
 ********                  PCFEventMergerAction                      ********
 ****************************************************************************/

void cloneValuesIntoGroup( vector< unsigned int >::iterator begin,
                           vector< unsigned int >::iterator end,
                           unsigned int oldValue,
                           unsigned int newValue,
                           map< TTypeValuePair, TTypeValuePair > &translation )
{
  for ( vector< unsigned int >::iterator it = begin; it != end; ++it )
  {
    translation[ TTypeValuePair( *it, oldValue ) ] = TTypeValuePair( *it, newValue );
  }
}


vector<TraceEditSequence::TSequenceStates> PCFEventMergerAction::getStateDependencies() const
{
  vector<TraceEditSequence::TSequenceStates> tmpStates;
  tmpStates.push_back( TraceEditSequence::eventTranslationTableState );
  tmpStates.push_back( TraceEditSequence::outputTraceFileNameState );
  tmpStates.push_back( TraceEditSequence::outputDirSuffixState );
  tmpStates.push_back( TraceEditSequence::pcfMergerReferenceState );

  return tmpStates;
}


bool PCFEventMergerAction::execute( std::string whichTrace )
{
  TraceEditSequence *tmpSequence = mySequence;
  bool translationOk = false;

#ifndef OLD_PCFPARSER
  // Get new tracename
  std::string newName = ( (OutputTraceFileNameState *)tmpSequence->getState( TraceEditSequence::outputTraceFileNameState ) )->getData();
  if ( newName.empty() )
  {
    std::string tmpSuffix = ( (OutputDirSuffixState *)tmpSequence->getState( TraceEditSequence::outputDirSuffixState ) )->getData();
    std::string outputPath = whichTrace.substr( 0, whichTrace.find_last_of( mySequence->getKernelConnection()->getPathSeparator() ) ) +
                           mySequence->getKernelConnection()->getPathSeparator() + tmpSuffix;

    vector< std::string > tmpID;
    tmpID.push_back( EventTranslator::getID() );

    newName = mySequence->getKernelConnection()->getNewTraceName( whichTrace, outputPath, tmpID, false );
  }

  std::string referenceTrace = ( (PCFMergerReferenceState *)tmpSequence->getState( TraceEditSequence::pcfMergerReferenceState ) )->getData();
  std::string referencePCFFile = LocalKernel::composeName( referenceTrace, "pcf" );
  std::string sourceTrace = LocalKernel::composeName( whichTrace, "pcf" );

  // Do pcf file exist? If not, exit sequence
  bool verbose = true;
  bool keepOpen = false;
  bool exitProgram = false;
  if ( !mySequence->getKernelConnection()->isFileReadable( referencePCFFile,
                                                           "PCFEventMergerAction::execute",
                                                           verbose, keepOpen, exitProgram ) ||
       !mySequence->getKernelConnection()->isFileReadable( sourceTrace,
                                                           "PCFEventMergerAction::execute",
                                                           verbose, keepOpen, exitProgram ) )
    return false;

  // Read reference pcf
#ifdef OLD_PCFPARSER
  ParaverTraceConfig *referenceTraceConfig = new ParaverTraceConfig( referencePCFFile );
  referenceTraceConfig->parse();
#else
  UIParaverTraceConfig *referenceTraceConfig = new UIParaverTraceConfig();
  referenceTraceConfig->parse( referencePCFFile );
#endif

  // Read source pcf
#ifdef OLD_PCFPARSER
  ParaverTraceConfig *sourceTraceConfig = new ParaverTraceConfig( sourceTrace );
  sourceTraceConfig->parse();
#else
  UIParaverTraceConfig *sourceTraceConfig = new UIParaverTraceConfig();
  sourceTraceConfig->parse( sourceTrace );
#endif

  // Translation algorithm
  map< TTypeValuePair, TTypeValuePair > translation;

  vector< vector< unsigned int > > sourceGroupedTypes = sourceTraceConfig->getGroupedEventTypes();
  vector< unsigned int > tmpCodes = referenceTraceConfig->getEventTypes();
  set< unsigned int > referenceTypes;
  referenceTypes.insert( tmpCodes.begin(), tmpCodes.end() );

  for ( vector< vector< unsigned int > >::iterator itGroupType = sourceGroupedTypes.begin(); itGroupType != sourceGroupedTypes.end(); ++itGroupType )
  {
    vector< unsigned int >::iterator itSourceType;
    itSourceType = (*itGroupType).begin();

    if ( referenceTypes.find( *itSourceType ) != referenceTypes.end() )
    {
      vector< unsigned int > sourceValues;
      try
      {
        sourceValues = sourceTraceConfig->getEventValues( *itSourceType );
      }
#ifdef OLD_PCFPARSER
      catch( libparaver::ParaverTraceConfig::value_not_found )
#else
      catch( libparaver::UIParaverTraceConfig::value_not_found )
#endif
      {}

      if ( sourceValues.empty() )
        continue;

      map< std::string, unsigned int > referenceValues;
      try
      {
        tmpCodes = referenceTraceConfig->getEventValues( *itSourceType );
      }
#ifdef OLD_PCFPARSER
      catch( libparaver::ParaverTraceConfig::value_not_found )
#else
      catch( libparaver::UIParaverTraceConfig::value_not_found )
#endif
      {}

      if ( tmpCodes.empty() )
        continue;

      for ( vector< unsigned int >::iterator itReferenceValue = tmpCodes.begin(); itReferenceValue != tmpCodes.end(); ++itReferenceValue )
      {
        referenceValues[ referenceTraceConfig->getEventValue( *itSourceType, *itReferenceValue ) ] = *itReferenceValue;
      }

      map< unsigned int, std::string > valuesColliding;
      map< unsigned int, std::string > valuesFinal;

      for ( vector< unsigned int >::iterator itSourceValue = sourceValues.begin(); itSourceValue != sourceValues.end(); ++itSourceValue )
      {
        std::string sourceTag = sourceTraceConfig->getEventValue( *itSourceType, *itSourceValue );
        map< std::string, unsigned int >::iterator itRefValue = referenceValues.find( sourceTag );
        if ( itRefValue != referenceValues.end() )
        {
          if ( valuesFinal.find( (*itRefValue).second ) != valuesFinal.end() )
          {
            valuesColliding[ (*itRefValue).second ] = valuesFinal[ (*itRefValue).second ];
          }
          else
          {
            if ( *itSourceValue != (*itRefValue).second )
              cloneValuesIntoGroup( itSourceType, (*itGroupType).end(), *itSourceValue, (*itRefValue).second, translation );
          }

          valuesFinal[ (*itRefValue).second ] = sourceTag;
        }
        else
        {
          if ( valuesFinal.find( *itSourceValue ) != valuesFinal.end() )
          {
            valuesColliding[ *itSourceValue ] = sourceTag;
          }
          else
          {
            valuesFinal[ *itSourceValue ] = sourceTag;
          }
        }
      }

      unsigned int maxValue = (--valuesFinal.end())->first;
      for ( map< unsigned int, std::string >::iterator itCollision = valuesColliding.begin(); itCollision != valuesColliding.end(); ++itCollision )
      {
        valuesFinal[ ++maxValue ] = (*itCollision).second;
        if ( (*itCollision).first != maxValue )
          cloneValuesIntoGroup( itSourceType, (*itGroupType).end(), (*itCollision).first, maxValue, translation );
      }

      sourceTraceConfig->setEventValues( *itSourceType, valuesFinal );
    }
  }

  // Write files
  ( (EventTranslationTableState *)tmpSequence->getState( TraceEditSequence::eventTranslationTableState ) )->setData( translation );

  if ( !translation.empty() )
  {
    mySequence->getKernelConnection()->copyROW( whichTrace, newName );
    std::fstream tmpFileDestiny;
    tmpFileDestiny.open( LocalKernel::composeName( newName, std::string( "pcf" ) ).c_str(), std::ios::out );
    tmpFileDestiny << sourceTraceConfig->toString();
    tmpFileDestiny.close();
  }

  delete sourceTraceConfig;
  delete referenceTraceConfig;
#endif

  tmpSequence->executeNextAction( whichTrace );

  return true;
}

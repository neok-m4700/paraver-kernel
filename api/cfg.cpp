#include <string>
#include <fstream>
#include <sstream>

#include "kernelconnection.h"
#include "cfg.h"
#include "window.h"
#include "trace.h"

#include "histogram.h"
#include "histogramstatistic.h"
#include "functionmanagement.h"
#include "semanticfunction.h"
#include "filter.h"
#include "paraverlabels.h"
#include "drawmode.h"


using namespace std;

map<string, TagFunction *> CFGLoader::cfgTagFunctions;

string currentWindowName;
string CFGLoader::errorLine = "";

TWindowLevel stringToLevel( const string& strLevel )
{
  TWindowLevel level = NONE;

  if ( strLevel.compare( OLDCFG_LVL_APPL ) == 0 )
    level = APPLICATION;
  else if ( strLevel.compare( OLDCFG_LVL_PTASK ) == 0 )
    level = APPLICATION;
  else if ( strLevel.compare( OLDCFG_LVL_WORKLOAD ) == 0 )
    level = WORKLOAD;
  else if ( strLevel.compare( OLDCFG_LVL_TASK ) == 0 )
    level = TASK;
  else if ( strLevel.compare( OLDCFG_LVL_THREAD ) == 0 )
    level = THREAD;
  else if ( strLevel.compare( OLDCFG_LVL_CPU ) == 0 )
    level = CPU;
  else if ( strLevel.compare( OLDCFG_LVL_NODE ) == 0 )
    level = NODE;
  else if ( strLevel.compare( OLDCFG_LVL_SYSTEM ) == 0 )
    level = SYSTEM;
  else if ( strLevel.compare( OLDCFG_LVL_TOPCOMPOSE ) == 0 )
    level = TOPCOMPOSE2;
  else if ( strLevel.compare( OLDCFG_LVL_TOPCOMPOSE1 ) == 0 )
    level = TOPCOMPOSE1;
  else if ( strLevel.compare( OLDCFG_LVL_TOPCOMPOSE2 ) == 0 )
    level = TOPCOMPOSE2;
  else if ( strLevel.compare( OLDCFG_LVL_COMPOSE_WORKLOAD ) == 0 )
    level = COMPOSEWORKLOAD;
  else if ( strLevel.compare( OLDCFG_LVL_COMPOSE_APPL ) == 0 )
    level = COMPOSEAPPLICATION;
  else if ( strLevel.compare( OLDCFG_LVL_COMPOSE_TASK ) == 0 )
    level = COMPOSETASK;
  else if ( strLevel.compare( OLDCFG_LVL_COMPOSE_THREAD ) == 0 )
    level = COMPOSETHREAD;
  else if ( strLevel.compare( OLDCFG_LVL_COMPOSE_CPU ) == 0 )
    level = COMPOSECPU;
  else if ( strLevel.compare( OLDCFG_LVL_COMPOSE_NODE ) == 0 )
    level = COMPOSENODE;
  else if ( strLevel.compare( OLDCFG_LVL_COMPOSE_SYSTEM ) == 0 )
    level = COMPOSESYSTEM;
  // Old semantic composes
  else if ( strLevel.compare( OLDCFG_LVL_COMPOSE1 ) == 0 )
    level = TOPCOMPOSE1;
  else if ( strLevel.compare( OLDCFG_LVL_COMPOSE2 ) == 0 )
    level = TOPCOMPOSE2;

  return level;
}

string levelToString( TWindowLevel whichLevel )
{
  switch ( whichLevel )
  {
    case NONE:
      break;
    case DERIVED:
      break;
    case WORKLOAD:
      return OLDCFG_LVL_WORKLOAD;
      break;
    case APPLICATION:
      return OLDCFG_LVL_APPL;
      break;
    case TASK:
      return OLDCFG_LVL_TASK;
      break;
    case THREAD:
      return OLDCFG_LVL_THREAD;
      break;
    case SYSTEM:
      return OLDCFG_LVL_SYSTEM;
      break;
    case NODE:
      return OLDCFG_LVL_NODE;
      break;
    case CPU:
      return OLDCFG_LVL_CPU;
      break;
    case TOPCOMPOSE1:
      return OLDCFG_LVL_TOPCOMPOSE1;
      break;
    case TOPCOMPOSE2:
      return OLDCFG_LVL_TOPCOMPOSE2;
      break;
    case COMPOSEWORKLOAD:
      return OLDCFG_LVL_COMPOSE_WORKLOAD;
      break;
    case COMPOSEAPPLICATION:
      return OLDCFG_LVL_COMPOSE_APPL;
      break;
    case COMPOSETASK:
      return OLDCFG_LVL_COMPOSE_TASK;
      break;
    case COMPOSETHREAD:
      return OLDCFG_LVL_COMPOSE_THREAD;
      break;
    case COMPOSESYSTEM:
      return OLDCFG_LVL_COMPOSE_SYSTEM;
      break;
    case COMPOSENODE:
      return OLDCFG_LVL_COMPOSE_NODE;
      break;
    case COMPOSECPU:
      return OLDCFG_LVL_COMPOSE_CPU;
      break;
  }

  return "";
}

string levelToStringHisto( TWindowLevel whichLevel )
{
  switch ( whichLevel )
  {
    case WORKLOAD:
      return OLDCFG_LVL_HISTO_WORKLOAD;
      break;
    case APPLICATION:
      return OLDCFG_LVL_HISTO_APPL;
      break;
    case TASK:
      return OLDCFG_LVL_HISTO_TASK;
      break;
    case THREAD:
      return OLDCFG_LVL_HISTO_THREAD;
      break;
    case SYSTEM:
      return OLDCFG_LVL_HISTO_SYSTEM;
      break;
    case NODE:
      return OLDCFG_LVL_HISTO_NODE;
      break;
    case CPU:
      return OLDCFG_LVL_HISTO_CPU;
      break;
    default:
      break;
  }
  return "";
}

bool CFGLoader::isCFGFile( const string& filename )
{
  string cfgExt;

  cfgExt = filename.substr( filename.length() - 4 );

  return ( cfgExt.compare( ".cfg" ) == 0 );
}

bool CFGLoader::loadCFG( KernelConnection *whichKernel,
                         const string& filename,
                         Trace *whichTrace,
                         vector<Window *>& windows,
                         vector<Histogram *>& histograms )
{
  ifstream cfgFile( filename.c_str() );
  if ( !cfgFile )
    return false;

  loadMap();

  windows.push_back( NULL );
  histograms.push_back( NULL );

  while ( !cfgFile.eof() )
  {
    string strLine;
    string cfgTag;

    getline( cfgFile, strLine );
    if ( strLine.length() == 0 )
      continue;
    else if ( strLine[ 0 ] == '#' )
      continue;

    istringstream auxStream( strLine );
    if ( strLine[ 0 ] == '<' )
    {
      cfgTag = strLine;
    }
    else
    {
      getline( auxStream, cfgTag, ' ' );
    }

    map<string, TagFunction *>::iterator it = cfgTagFunctions.find( cfgTag );
    if ( it != cfgTagFunctions.end() )
    {
      if ( !it->second->parseLine( whichKernel, auxStream, whichTrace, windows,
                                   histograms ) )
      {
        if ( histograms.begin() != histograms.end() &&
             histograms[ histograms.size() - 1 ] != NULL )
        {
          delete histograms[ histograms.size() - 1 ];
          histograms[ histograms.size() - 1 ] = NULL;
          CFGLoader::errorLine = strLine;
        }
        else
        {
          if ( windows[ windows.size() - 1 ] != NULL )
          {
            delete windows[ windows.size() - 1 ];
            CFGLoader::errorLine = strLine;
          }
          windows[ windows.size() - 1 ] = NULL;
        }
      }
    }
  }

  cfgFile.close();

  unLoadMap();

  if ( histograms[ histograms.size() -1 ] == NULL )
    histograms.pop_back();

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  // Because old paraver set window_open to false for all windows
  if ( histograms.begin() == histograms.end() )
    windows[ windows.size() - 1 ]->setShowWindow( true );

  return true;
}

void CFGLoader::pushbackWindow( Window *whichWindow,
                                vector<Window *>& allWindows )
{
  if ( whichWindow->isDerivedWindow() )
  {
    pushbackWindow( whichWindow->getParent( 0 ), allWindows );
    pushbackWindow( whichWindow->getParent( 1 ), allWindows );
  }
  allWindows.push_back( whichWindow );
}

void CFGLoader::pushbackAllWindows( const vector<Window *>& selectedWindows,
                                    const vector<Histogram *>& selectedHistos,
                                    vector<Window *>& allWindows )
{
  for ( vector<Window *>::const_iterator it = selectedWindows.begin();
        it != selectedWindows.end(); ++it )
  {
    pushbackWindow( ( *it ), allWindows );
  }

  for ( vector<Histogram *>::const_iterator it = selectedHistos.begin();
        it != selectedHistos.end(); ++it )
  {
    pushbackWindow( ( *it )->getControlWindow(), allWindows );
    if ( ( *it )->getControlWindow() != ( *it )->getDataWindow() )
      pushbackWindow( ( *it )->getDataWindow(), allWindows );
    if ( ( *it )->getThreeDimensions() &&
         ( *it )->getExtraControlWindow() != ( *it )->getControlWindow() &&
         ( *it )->getExtraControlWindow() != ( *it )->getDataWindow() )
      pushbackWindow( ( *it )->getExtraControlWindow(), allWindows );
  }
}

bool CFGLoader::saveCFG( const string& filename,
                         const SaveOptions& options,
                         const vector<Window *>& windows,
                         const vector<Histogram *>& histograms )
{
  vector<Window *> allWindows;

  ofstream cfgFile( filename.c_str() );
  if ( !cfgFile )
    return false;

  pushbackAllWindows( windows, histograms, allWindows );

  cfgFile << fixed;
  cfgFile.precision( 6 );

  cfgFile << "ConfigFile.Version: 3.4" << endl;
  cfgFile << "ConfigFile.NumWindows: " << allWindows.size() << endl;
  cfgFile << options.description << endl;
  cfgFile << endl;

  int id = 1;
  for ( vector<Window *>::const_iterator it = allWindows.begin();
        it != allWindows.end(); ++it )
  {
    cfgFile << "################################################################################" << endl;
    cfgFile << "< NEW DISPLAYING WINDOW " << ( *it )->getName() << " >" << endl;
    cfgFile << "################################################################################" << endl;
    WindowName::printLine( cfgFile, it );
    WindowType::printLine( cfgFile, it );
    cfgFile << OLDCFG_TAG_WNDW_ID << " " << id << endl;
    if ( ( *it )->isDerivedWindow() )
    {
      WindowFactors::printLine( cfgFile, it );
      WindowOperation::printLine( cfgFile, it );
      WindowIdentifiers::printLine( cfgFile, allWindows, it );
    }
    WindowPositionX::printLine( cfgFile, it );
    WindowPositionY::printLine( cfgFile, it );
    WindowWidth::printLine( cfgFile, it );
    WindowHeight::printLine( cfgFile, it );
    WindowCommLines::printLine( cfgFile, it );
    WindowColorMode::printLine( cfgFile, it );
    if ( !( *it )->isDerivedWindow() )
    {
      WindowFilterLogical::printLine( cfgFile, it );
      WindowFilterPhysical::printLine( cfgFile, it );
      WindowFilterBoolOpFromTo::printLine( cfgFile, it );
      WindowFilterBoolOpTagSize::printLine( cfgFile, it );
      WindowFilterBoolOpTypeVal::printLine( cfgFile, it );
    }
    WindowUnits::printLine( cfgFile, it );
    WindowMaximumY::printLine( cfgFile, it );
    WindowMinimumY::printLine( cfgFile, it );
    WindowComputeYMax::printLine( cfgFile, options, it );
    WindowLevel::printLine( cfgFile, it );
    WindowScaleRelative::printLine( cfgFile, options, it );
    WindowEndTimeRelative::printLine( cfgFile, options, it );
    WindowObject::printLine( cfgFile, it );
    WindowBeginTime::printLine( cfgFile, options, it );
    WindowEndTime::printLine( cfgFile, options, it );
    WindowStopTime::printLine( cfgFile, options, it );
    WindowBeginTimeRelative::printLine( cfgFile, options, it );
    WindowDrawMode::printLine( cfgFile, it );
    WindowDrawModeRows::printLine( cfgFile, it );
    if ( !( *it )->isDerivedWindow() )
      WindowSelectedFunctions::printLine( cfgFile, it );
    WindowComposeFunctions::printLine( cfgFile, it );
    WindowSemanticModule::printLine( cfgFile, it );
    if ( !( *it )->isDerivedWindow() )
      WindowFilterModule::printLine( cfgFile, it );

    cfgFile << endl;
    ++id;
  }

  for ( vector<Histogram *>::const_iterator it = histograms.begin();
        it != histograms.end(); ++it )
  {
    Analyzer2DCreate::printLine( cfgFile, it );
    Analyzer2DName::printLine( cfgFile, it );
    Analyzer2DX::printLine( cfgFile, it );
    Analyzer2DY::printLine( cfgFile, it );
    Analyzer2DWidth::printLine( cfgFile, it );
    Analyzer2DHeight::printLine( cfgFile, it );
    Analyzer2DControlWindow::printLine( cfgFile, allWindows, it );
    Analyzer2DDataWindow::printLine( cfgFile, allWindows, it );
    Analyzer2DAccumulator::printLine( cfgFile, it );
    Analyzer2DStatistic::printLine( cfgFile, it );
    Analyzer2DCalculateAll::printLine( cfgFile, it );
    Analyzer2DHideColumns::printLine( cfgFile, it );
    Analyzer2DHorizontal::printLine( cfgFile, it );
    Analyzer2DColor::printLine( cfgFile, it );
    Analyzer2DZoom::printLine( cfgFile, it );
    Analyzer2DAccumulateByControlWindow::printLine( cfgFile, it );
    Analyzer2DSortCols::printLine( cfgFile, it );
    Analyzer2DSortCriteria::printLine( cfgFile, it );
    Analyzer2DParameters::printLine( cfgFile, it );
    Analyzer2DAnalysisLimits::printLine( cfgFile, options, it );
    Analyzer2DRelativeTime::printLine( cfgFile, it );
    Analyzer2DComputeYScale::printLine( cfgFile, options, it );
    Analyzer2DMinimum::printLine( cfgFile, it );
    Analyzer2DMaximum::printLine( cfgFile, it );
    Analyzer2DDelta::printLine( cfgFile, it );
    Analyzer2DComputeGradient::printLine( cfgFile, options, it );
    Analyzer2DMinimumGradient::printLine( cfgFile, it );
    Analyzer2DMaximumGradient::printLine( cfgFile, it );
    if ( ( *it )->getThreeDimensions() )
    {
      Analyzer3DControlWindow::printLine( cfgFile, allWindows, it );
      Analyzer3DMinimum::printLine( cfgFile, it );
      Analyzer3DMaximum::printLine( cfgFile, it );
      Analyzer3DDelta::printLine( cfgFile, it );
      Analyzer3DFixedValue::printLine( cfgFile, it );
    }

    cfgFile << endl;
  }

  cfgFile.close();

  return true;
}

int CFGLoader::findWindow( const Window *whichWindow,
                           const vector<Window *>& allWindows )
{
  unsigned int i = 0;

  if ( allWindows.begin() == allWindows.end() )
    return -1;

  while ( i < allWindows.size() )
  {
    if ( whichWindow == allWindows[ i ] )
      break;
    ++i;
  }

  if ( i == allWindows.size() )
    return -1;

  return i;
}

void CFGLoader::loadMap()
{
  cfgTagFunctions[OLDCFG_TAG_WNDW_NAME]                = new WindowName();
  cfgTagFunctions[OLDCFG_TAG_WNDW_TYPE]                = new WindowType();
  cfgTagFunctions[OLDCFG_TAG_WNDW_FACTORS]             = new WindowFactors();
  cfgTagFunctions[OLDCFG_TAG_WNDW_POSX]                = new WindowPositionX();
  cfgTagFunctions[OLDCFG_TAG_WNDW_POSY]                = new WindowPositionY();
  cfgTagFunctions[OLDCFG_TAG_WNDW_WIDTH]               = new WindowWidth();
  cfgTagFunctions[OLDCFG_TAG_WNDW_HEIGHT]              = new WindowHeight();
  cfgTagFunctions[OLDCFG_TAG_WNDW_COMM_LINES]          = new WindowCommLines();
  cfgTagFunctions[OLDCFG_TAG_WNDW_UNITS]               = new WindowUnits();
  cfgTagFunctions[OLDCFG_TAG_WNDW_COLOR_MODE]          = new WindowColorMode();
  cfgTagFunctions[OLDCFG_TAG_WNDW_OPERATION]           = new WindowOperation();
  cfgTagFunctions[OLDCFG_TAG_WNDW_MAXIMUM_Y]           = new WindowMaximumY();
  cfgTagFunctions[OLDCFG_TAG_WNDW_MINIMUM_Y]           = new WindowMinimumY();
  cfgTagFunctions[OLDCFG_TAG_WNDW_COMPUTE_Y_MAX]       = new WindowComputeYMax();
  cfgTagFunctions[OLDCFG_TAG_WNDW_LEVEL]               = new WindowLevel();
  cfgTagFunctions[OLDCFG_TAG_WNDW_SCALE_RELATIVE]      = new WindowScaleRelative();
  cfgTagFunctions[CFG_TAG_WNDW_END_TIME_RELATIVE]      = new WindowEndTimeRelative();
  cfgTagFunctions[OLDCFG_TAG_WNDW_OBJECT]              = new WindowObject();
  cfgTagFunctions[OLDCFG_TAG_WNDW_IDENTIFIERS]         = new WindowIdentifiers();

  cfgTagFunctions[OLDCFG_TAG_WNDW_BEGIN_TIME]          = new WindowBeginTime();
  cfgTagFunctions[OLDCFG_TAG_WNDW_STOP_TIME]           = new WindowStopTime();
  cfgTagFunctions[OLDCFG_TAG_WNDW_END_TIME]            = new WindowEndTime();
  cfgTagFunctions[OLDCFG_TAG_WNDW_BEGIN_TIME_RELATIVE] = new WindowBeginTimeRelative();

  cfgTagFunctions[OLDCFG_TAG_WNDW_NUMBER_OF_ROWS]      = new WindowNumberOfRow();
  cfgTagFunctions[OLDCFG_TAG_WNDW_SELECTED_FUNCTIONS]  = new WindowSelectedFunctions();
  cfgTagFunctions[OLDCFG_TAG_WNDW_SEMANTIC_MODULE]     = new WindowSemanticModule();
  cfgTagFunctions[OLDCFG_TAG_WNDW_COMPOSE_FUNCTIONS]   = new WindowComposeFunctions();

  // Filter options
  cfgTagFunctions[OLDCFG_TAG_WNDW_FILTER_MODULE]       = new WindowFilterModule();
  cfgTagFunctions[OLDCFG_TAG_WNDW_LOGICAL_FILTERED]    = new WindowFilterLogical();
  cfgTagFunctions[OLDCFG_TAG_WNDW_PHYSICAL_FILTERED]   = new WindowFilterPhysical();
  cfgTagFunctions[OLDCFG_TAG_WNDW_FROMTO]              = new WindowFilterBoolOpFromTo();
  cfgTagFunctions[OLDCFG_TAG_WNDW_COMM_TAGSIZE]        = new WindowFilterBoolOpTagSize();
  cfgTagFunctions[OLDCFG_TAG_WNDW_TYPEVAL]             = new WindowFilterBoolOpTypeVal();

  cfgTagFunctions[OLDCFG_TAG_WNDW_OPEN]                = new WindowOpen();
  cfgTagFunctions[OLDCFG_TAG_WNDW_DRAW_MODE]           = new WindowDrawMode();
  cfgTagFunctions[OLDCFG_TAG_WNDW_DRAW_MODE_ROWS]      = new WindowDrawModeRows();

  // Histogram options

  cfgTagFunctions[OLDCFG_TAG_AN2D_NEW]                 = new Analyzer2DCreate();
  cfgTagFunctions[OLDCFG_TAG_AN2D_NAME]                = new Analyzer2DName();
  cfgTagFunctions[OLDCFG_TAG_AN2D_X]                   = new Analyzer2DX();
  cfgTagFunctions[OLDCFG_TAG_AN2D_Y]                   = new Analyzer2DY();
  cfgTagFunctions[OLDCFG_TAG_AN2D_WIDTH]               = new Analyzer2DWidth();
  cfgTagFunctions[OLDCFG_TAG_AN2D_HEIGHT]              = new Analyzer2DHeight();
  cfgTagFunctions[OLDCFG_TAG_AN2D_CONTROL_WINDOW]      = new Analyzer2DControlWindow();
  cfgTagFunctions[OLDCFG_TAG_AN2D_DATA_WINDOW]         = new Analyzer2DDataWindow();
  cfgTagFunctions[OLDCFG_TAG_AN2D_STATISTIC]           = new Analyzer2DStatistic();
  cfgTagFunctions[OLDCFG_TAG_AN2D_CALCULATE_ALL]       = new Analyzer2DCalculateAll();
  // --> Analyzer2D.hide_empty_cols:
  cfgTagFunctions[OLDCFG_TAG_AN2D_HIDE_COLS]           = new Analyzer2DHideColumns();
  cfgTagFunctions[OLDCFG_TAG_AN2D_HORIZONTAL]          = new Analyzer2DHorizontal();
  cfgTagFunctions[OLDCFG_TAG_AN2D_COLOR]               = new Analyzer2DColor();
  // --> Analyzer2D.SemanticColor:
  // --> Analyzer2D.TextMode:
  cfgTagFunctions[OLDCFG_TAG_AN2D_ZOOM]               = new Analyzer2DZoom();
  // --> Analyzer2D.Expanded:
  // --> Analyzer2D.expanded:
  cfgTagFunctions[OLDCFG_TAG_AN2D_ACCUMULATOR]          = new Analyzer2DAccumulator();
  cfgTagFunctions[OLDCFG_TAG_AN2D_ACCUM_BY_CTRL_WINDOW] = new Analyzer2DAccumulateByControlWindow();
  cfgTagFunctions[OLDCFG_TAG_AN2D_SORTCOLS]             = new Analyzer2DSortCols();
  cfgTagFunctions[OLDCFG_TAG_AN2D_SORTCRITERIA]         = new Analyzer2DSortCriteria();

  cfgTagFunctions[OLDCFG_TAG_AN2D_PARAMETERS]           = new Analyzer2DParameters();
  cfgTagFunctions[OLDCFG_TAG_AN2D_ANALYSISLIMITS]       = new Analyzer2DAnalysisLimits();
  cfgTagFunctions[OLDCFG_TAG_AN2D_RELATIVETIME]         = new Analyzer2DRelativeTime();
  // --> cfgTagFunctions["Analyzer2D.RelativeXScale:"] = new Analyzer2DRelativeXScale();
  // --> Analyzer2D.ShowWindows:
  cfgTagFunctions[OLDCFG_TAG_AN2D_COMPUTEYSCALE]        = new Analyzer2DComputeYScale();
  cfgTagFunctions[OLDCFG_TAG_AN2D_MINIMUM]              = new Analyzer2DMinimum();
  cfgTagFunctions[OLDCFG_TAG_AN2D_MAXIMUM]              = new Analyzer2DMaximum();
  cfgTagFunctions[OLDCFG_TAG_AN2D_DELTA]                = new Analyzer2DDelta();
  cfgTagFunctions[OLDCFG_TAG_AN2D_COMPUTEGRADIENT]      = new Analyzer2DComputeGradient();
  cfgTagFunctions[OLDCFG_TAG_AN2D_MINIMUMGRADIENT]      = new Analyzer2DMinimumGradient();
  cfgTagFunctions[OLDCFG_TAG_AN2D_MAXIMUMGRADIENT]      = new Analyzer2DMaximumGradient();

  // 3D Histogram
  cfgTagFunctions[OLDCFG_TAG_AN3D_CONTROLWINDOW]        = new Analyzer3DControlWindow();
  cfgTagFunctions[OLDCFG_TAG_AN3D_MINIMUM]              = new Analyzer3DMinimum();
  cfgTagFunctions[OLDCFG_TAG_AN3D_MAXIMUM]              = new Analyzer3DMaximum();
  cfgTagFunctions[OLDCFG_TAG_AN3D_DELTA]                = new Analyzer3DDelta();
  cfgTagFunctions[OLDCFG_TAG_AN3D_FIXEDVALUE]           = new Analyzer3DFixedValue();
}

void CFGLoader::unLoadMap()
{
  for ( map<string, TagFunction *>::iterator it = cfgTagFunctions.begin();
        it != cfgTagFunctions.end();
        ++it )
    delete ( *it ).second;
}


bool WindowName::parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms )
{
  string name;

  getline( line, name );

  currentWindowName = name;

  return true;
}

void WindowName::printLine( ofstream& cfgFile,
                            const vector<Window *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_WNDW_NAME << " " << ( *it )->getName() << endl;
}

bool WindowType::parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms )
{
  string type;
  Window *tmpWin;

  getline( line, type );
  if ( type.compare( OLDCFG_VAL_WNDW_TYPE_SINGLE ) == 0 )
  {
    tmpWin = Window::create( whichKernel, whichTrace );
  }
  else if ( type.compare( OLDCFG_VAL_WNDW_TYPE_COMPOSED ) == 0 )
  {
    tmpWin = Window::create( whichKernel );
  }
  else
    return false;

  if ( windows.size() == 1 && windows[ 0 ] == NULL )
    windows[ 0 ] = tmpWin;
  else
    windows.push_back( tmpWin );

  windows[ windows.size() - 1 ]->setName( currentWindowName );

  // Default value from old paraver code
  windows[ windows.size() - 1 ]->setTimeUnit( US );

  return true;
}

void WindowType::printLine( ofstream& cfgFile,
                            const vector<Window *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_WNDW_TYPE << " ";
  if ( ( *it )->isDerivedWindow() )
    cfgFile << OLDCFG_VAL_WNDW_TYPE_COMPOSED << endl;
  else
    cfgFile << OLDCFG_VAL_WNDW_TYPE_SINGLE << endl;
}

bool WindowFactors::parseLine( KernelConnection *whichKernel, istringstream& line,
                               Trace *whichTrace,
                               vector<Window *>& windows,
                               vector<Histogram *>& histograms )
{
  string strFactor;
  UINT16 numFactor = 0;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  if ( !windows[ windows.size() - 1 ]->isDerivedWindow() )
    return true;

  while ( !line.eof() )
  {
    getline( line, strFactor, ' ' );
    istringstream tmpStream( strFactor );
    TSemanticValue factorValue;

    if ( !( tmpStream >> factorValue ) )
      return false;

    windows[ windows.size() - 1 ]->setFactor( numFactor, factorValue );

    numFactor++;
  }
  return true;
}

void WindowFactors::printLine( ofstream& cfgFile,
                               const vector<Window *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_WNDW_FACTORS << " " << ( *it )->getFactor( 0 ) <<
  " " << ( *it )->getFactor( 1 ) << endl;
}

bool WindowPositionX::parseLine( KernelConnection *whichKernel, istringstream& line,
                                 Trace *whichTrace,
                                 vector<Window *>& windows,
                                 vector<Histogram *>& histograms )
{
  string strPos;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strPos, ' ' );
  istringstream tmpStream( strPos );
  UINT16 pos;

  if ( !( tmpStream >> pos ) )
    return false;

  windows[ windows.size() - 1 ]->setPosX( pos );

  return true;
}

void WindowPositionX::printLine( ofstream& cfgFile,
                                 const vector<Window *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_WNDW_POSX << " " << ( *it )->getPosX() << endl;
}

bool WindowPositionY::parseLine( KernelConnection *whichKernel, istringstream& line,
                                 Trace *whichTrace,
                                 vector<Window *>& windows,
                                 vector<Histogram *>& histograms )
{
  string strPos;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strPos, ' ' );
  istringstream tmpStream( strPos );
  UINT16 pos;

  if ( !( tmpStream >> pos ) )
    return false;

  windows[ windows.size() - 1 ]->setPosY( pos );

  return true;
}

void WindowPositionY::printLine( ofstream& cfgFile,
                                 const vector<Window *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_WNDW_POSY << " " << ( *it )->getPosY() << endl;
}

bool WindowWidth::parseLine( KernelConnection *whichKernel, istringstream& line,
                             Trace *whichTrace,
                             vector<Window *>& windows,
                             vector<Histogram *>& histograms )
{
  string strWidth;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strWidth, ' ' );
  istringstream tmpStream( strWidth );
  UINT16 width;

  if ( !( tmpStream >> width ) )
    return false;

  windows[ windows.size() - 1 ]->setWidth( width );

  return true;
}

void WindowWidth::printLine( ofstream& cfgFile,
                             const vector<Window *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_WNDW_WIDTH << " " << ( *it )->getWidth() << endl;
}

bool WindowHeight::parseLine( KernelConnection *whichKernel, istringstream& line,
                              Trace *whichTrace,
                              vector<Window *>& windows,
                              vector<Histogram *>& histograms )
{
  string strHeight;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strHeight, ' ' );
  istringstream tmpStream( strHeight );
  UINT16 height;

  if ( !( tmpStream >> height ) )
    return false;

  windows[ windows.size() - 1 ]->setHeight( height );

  return true;
}

void WindowHeight::printLine( ofstream& cfgFile,
                              const vector<Window *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_WNDW_HEIGHT << " " << ( *it )->getHeight() << endl;
}

bool WindowCommLines::parseLine( KernelConnection *whichKernel, istringstream& line,
                                 Trace *whichTrace,
                                 vector<Window *>& windows,
                                 vector<Histogram *>& histograms )
{
  string strBool;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strBool, ' ' );

  if ( strBool.compare( OLDCFG_VAL_FALSE ) == 0 )
    windows[ windows.size() - 1 ]->setDrawCommLines( false );
  else if ( strBool.compare( OLDCFG_VAL_TRUE ) == 0 )
    windows[ windows.size() - 1 ]->setDrawCommLines( true );
  else
    return false;

  return true;
}

void WindowCommLines::printLine( ofstream& cfgFile,
                                 const vector<Window *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_WNDW_COMM_LINES << " " << ( ( *it )->getDrawCommLines() ?
      OLDCFG_VAL_TRUE : OLDCFG_VAL_FALSE ) << endl;
}

bool WindowColorMode::parseLine( KernelConnection *whichKernel, istringstream& line,
                                 Trace *whichTrace,
                                 vector<Window *>& windows,
                                 vector<Histogram *>& histograms )
{
  string strMode;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strMode, ' ' );

  if ( strMode.compare( OLDCFG_VAL_COLOR_MODE_GRADIENT ) == 0 )
  {
    windows[ windows.size() - 1 ]->setGradientColorMode();
    windows[ windows.size() - 1 ]->getGradientColor().allowOutOfScale( true );
  }
  else if ( strMode.compare( OLDCFG_VAL_COLOR_MODE_NULL_GRADIENT ) == 0 )
  {
    windows[ windows.size() - 1 ]->setGradientColorMode();
    windows[ windows.size() - 1 ]->getGradientColor().allowOutOfScale( false );
  }

  return true;
}

void WindowColorMode::printLine( ofstream& cfgFile,
                                 const vector<Window *>::const_iterator it )
{
  if ( ( *it )->IsGradientColorSet() )
  {
    cfgFile << OLDCFG_TAG_WNDW_COLOR_MODE << " " <<
    ( ( *it )->getGradientColor().getAllowOutOfScale() ?
      OLDCFG_VAL_COLOR_MODE_GRADIENT : OLDCFG_VAL_COLOR_MODE_NULL_GRADIENT ) << endl;
  }
}

bool WindowUnits::parseLine( KernelConnection *whichKernel, istringstream& line,
                             Trace *whichTrace,
                             vector<Window *>& windows,
                             vector<Histogram *>& histograms )
{
  string strUnits;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strUnits, ' ' );

  if ( strUnits.compare( OLDCFG_VAL_TIMEUNIT_NS ) == 0 )
    windows[ windows.size() - 1 ]->setTimeUnit( NS );
  else if ( strUnits.compare( OLDCFG_VAL_TIMEUNIT_US ) == 0 )
    windows[ windows.size() - 1 ]->setTimeUnit( US );
  else if ( strUnits.compare( OLDCFG_VAL_TIMEUNIT_MS ) == 0 )
    windows[ windows.size() - 1 ]->setTimeUnit( MS );
  else if ( strUnits.compare( OLDCFG_VAL_TIMEUNIT_S ) == 0 )
    windows[ windows.size() - 1 ]->setTimeUnit( SEC );
  else if ( strUnits.compare( OLDCFG_VAL_TIMEUNIT_M ) == 0 )
    windows[ windows.size() - 1 ]->setTimeUnit( MIN );
  else if ( strUnits.compare( OLDCFG_VAL_TIMEUNIT_H ) == 0 )
    windows[ windows.size() - 1 ]->setTimeUnit( HOUR );
  else if ( strUnits.compare( OLDCFG_VAL_TIMEUNIT_D ) == 0 )
    windows[ windows.size() - 1 ]->setTimeUnit( DAY );

  return true;
}

void WindowUnits::printLine( ofstream& cfgFile,
                             const vector<Window *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_WNDW_UNITS << " ";
  switch ( ( *it )->getTimeUnit() )
  {
    case NS:
      cfgFile << OLDCFG_VAL_TIMEUNIT_NS;
      break;
    case US:
      cfgFile << OLDCFG_VAL_TIMEUNIT_US;
      break;
    case MS:
      cfgFile << OLDCFG_VAL_TIMEUNIT_MS;
      break;
    case SEC:
      cfgFile << OLDCFG_VAL_TIMEUNIT_S;
      break;
    case MIN:
      cfgFile << OLDCFG_VAL_TIMEUNIT_M;
      break;
    case HOUR:
      cfgFile << OLDCFG_VAL_TIMEUNIT_H;
      break;
    case DAY:
      cfgFile << OLDCFG_VAL_TIMEUNIT_D;
      break;
  }

  cfgFile << endl;
}

bool WindowOperation::parseLine( KernelConnection *whichKernel, istringstream& line,
                                 Trace *whichTrace,
                                 vector<Window *>& windows,
                                 vector<Histogram *>& histograms )
{
  string strFunction;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  if ( !windows[ windows.size() - 1 ]->isDerivedWindow() )
    return true;

  getline( line, strFunction );

  return windows[ windows.size() - 1 ]->setLevelFunction( DERIVED, strFunction );
}

void WindowOperation::printLine( ofstream& cfgFile,
                                 const vector<Window *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_WNDW_OPERATION << " " << ( *it )->getLevelFunction( DERIVED ) << endl;
}

bool WindowMaximumY::parseLine( KernelConnection *whichKernel, istringstream& line,
                                Trace *whichTrace,
                                vector<Window *>& windows,
                                vector<Histogram *>& histograms )
{
  string strMaximum;
  TSemanticValue maximum;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strMaximum, ' ' );
  istringstream tmpStream( strMaximum );
  if ( !( tmpStream >> maximum ) )
    return false;

  windows[ windows.size() - 1 ]->setMaximumY( maximum );

  return true;
}

void WindowMaximumY::printLine( ofstream& cfgFile,
                                const vector<Window *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_WNDW_MAXIMUM_Y << " " << ( *it )->getMaximumY() << endl;
}

bool WindowMinimumY::parseLine( KernelConnection *whichKernel, istringstream& line,
                                Trace *whichTrace,
                                vector<Window *>& windows,
                                vector<Histogram *>& histograms )
{
  string strMinimum;
  TSemanticValue minimum;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strMinimum, ' ' );
  istringstream tmpStream( strMinimum );
  if ( !( tmpStream >> minimum ) )
    return false;

  windows[ windows.size() - 1 ]->setMinimumY( minimum );

  return true;
}

void WindowMinimumY::printLine( ofstream& cfgFile,
                                const vector<Window *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_WNDW_MINIMUM_Y << " " << ( *it )->getMinimumY() << endl;
}

bool WindowComputeYMax::parseLine( KernelConnection *whichKernel, istringstream& line,
                                   Trace *whichTrace,
                                   vector<Window *>& windows,
                                   vector<Histogram *>& histograms )
{
  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  windows[ windows.size() - 1 ]->setComputeYMaxOnInit( true );

  return true;
}

void WindowComputeYMax::printLine( ofstream& cfgFile,
                                   const SaveOptions& options,
                                   const vector<Window *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_WNDW_COMPUTE_Y_MAX << " ";
  if ( options.windowComputeYMaxOnLoad )
    cfgFile << OLDCFG_VAL_TRUE;
  else
    cfgFile << OLDCFG_VAL_FALSE;
  cfgFile << endl;
}

bool WindowLevel::parseLine( KernelConnection *whichKernel, istringstream& line,
                             Trace *whichTrace,
                             vector<Window *>& windows,
                             vector<Histogram *>& histograms )
{
  string strLevel;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  if ( windows[ windows.size() - 1 ]->isDerivedWindow() )
    return true;

  getline( line, strLevel, ' ' );

  windows[ windows.size() - 1 ]->setLevel( stringToLevel( strLevel ) );

  return true;
}

void WindowLevel::printLine( ofstream& cfgFile,
                             const vector<Window *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_WNDW_LEVEL << " " << levelToString( ( *it )->getLevel() ) << endl;
}

bool WindowIdentifiers::parseLine( KernelConnection *whichKernel, istringstream& line,
                                   Trace *whichTrace,
                                   vector<Window *>& windows,
                                   vector<Histogram *>& histograms )
{
  string strID;
  UINT16 id;
  UINT16 numID = 0;
  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  if ( !windows[ windows.size() - 1 ]->isDerivedWindow() )
    return true;

  while ( !line.eof() )
  {
    getline( line, strID, ' ' );
    istringstream tmpStream( strID );
    if ( !( tmpStream >> id ) )
      return false;

    if ( windows[ id - 1 ] == NULL )
      return false;

    windows[ windows.size() - 1 ]->setParent( numID, windows[ id - 1 ] );
    numID++;
  }

  return true;
}

void WindowIdentifiers::printLine( ofstream& cfgFile,
                                   const vector<Window *>& allWindows,
                                   const vector<Window *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_WNDW_IDENTIFIERS << " ";
  cfgFile << CFGLoader::findWindow( ( *it )->getParent( 0 ), allWindows ) + 1 << " ";
  cfgFile << CFGLoader::findWindow( ( *it )->getParent( 1 ), allWindows ) + 1;
  cfgFile << endl;
}

bool WindowScaleRelative::parseLine( KernelConnection *whichKernel, istringstream& line,
                                     Trace *whichTrace,
                                     vector<Window *>& windows,
                                     vector<Histogram *>& histograms )
{
  string strEndTime;
  double percEndTime;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strEndTime );
  istringstream tmpStream( strEndTime );
  if ( !( tmpStream >> percEndTime ) )
    return false;

  if ( percEndTime > 1.0 ) percEndTime = 1.0;

  windows[ windows.size() - 1 ]->setWindowEndTime(
    ( windows[ windows.size() - 1 ]->getTrace() )->getEndTime() * percEndTime );

  return true;
}

void WindowScaleRelative::printLine( ofstream& cfgFile,
                                     const SaveOptions& options,
                                     const vector<Window *>::const_iterator it )
{
  if ( options.windowScaleRelative )
  {
    cfgFile << OLDCFG_TAG_WNDW_SCALE_RELATIVE << " ";
    double maxScale = ( *it )->getTrace()->getEndTime() / ( double )( *it )->getWidth();
    cfgFile << ( ( *it )->getWindowEndTime() / ( *it )->getWidth() ) / maxScale;
    cfgFile << endl;
  }
}

bool WindowEndTimeRelative::parseLine( KernelConnection *whichKernel, istringstream& line,
                                       Trace *whichTrace,
                                       vector<Window *>& windows,
                                       vector<Histogram *>& histograms )
{
  string strEndTime;
  double percEndTime;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strEndTime );
  istringstream tmpStream( strEndTime );
  if ( !( tmpStream >> percEndTime ) )
    return false;

  windows[ windows.size() - 1 ]->setWindowEndTime(
    ( windows[ windows.size() - 1 ]->getTrace() )->getEndTime() * percEndTime );

  return true;
}

void WindowEndTimeRelative::printLine( ofstream& cfgFile,
                                       const SaveOptions& options,
                                       const vector<Window *>::const_iterator it )
{
  if ( options.windowScaleRelative )
  {
    cfgFile << CFG_TAG_WNDW_END_TIME_RELATIVE << " ";
    cfgFile << ( *it )->getWindowEndTime() / ( *it )->getTrace()->getEndTime();
    cfgFile << endl;
  }
}

bool WindowObject::parseLine( KernelConnection *whichKernel, istringstream& line, Trace *whichTrace,
                              vector<Window *>& windows,
                              vector<Histogram *>& histograms )
{
#ifndef WIN32
#warning WindowObject::parseLine
#endif
  return true;
}

void WindowObject::printLine( ofstream& cfgFile,
                              const vector<Window *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_WNDW_OBJECT << " appl { 1, { All } }" << endl;
#ifndef WIN32
#warning WindowObject::printLine
#endif
}

bool WindowBeginTime::parseLine( KernelConnection *whichKernel, istringstream& line,
                                 Trace *whichTrace,
                                 vector<Window *>& windows,
                                 vector<Histogram *>& histograms )
{
  string strTime;
  TRecordTime auxTime;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strTime, ' ' );
  istringstream tmpTime( strTime );

  if ( !( tmpTime >> auxTime ) )
    return false;

  if ( auxTime < whichTrace->getEndTime() )
    windows[ windows.size() - 1 ]->setWindowBeginTime( auxTime );
  else
    windows[ windows.size() - 1 ]->setWindowBeginTime( 0.0 );

  return true;
}

void WindowBeginTime::printLine( ofstream& cfgFile,
                                 const SaveOptions& options,
                                 const vector<Window *>::const_iterator it )
{
  if ( !options.windowBeginTimeRelative )
    cfgFile << OLDCFG_TAG_WNDW_BEGIN_TIME << " " << ( *it )->getWindowBeginTime() << endl;
}

bool WindowEndTime::parseLine( KernelConnection *whichKernel, istringstream& line,
                               Trace *whichTrace,
                               vector<Window *>& windows,
                               vector<Histogram *>& histograms )
{
  string strTime;
  TRecordTime auxTime;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strTime, ' ' );
  istringstream tmpTime( strTime );

  if ( !( tmpTime >> auxTime ) )
    return false;

  // endTime = auxTime;

  return true;
}

void WindowEndTime::printLine( ofstream& cfgFile,
                               const SaveOptions& options,
                               const vector<Window *>::const_iterator it )
{
#ifndef WIN32
#warning WindowEndTime::printLine
#endif
}

bool WindowStopTime::parseLine( KernelConnection *whichKernel, istringstream& line,
                                Trace *whichTrace,
                                vector<Window *>& windows,
                                vector<Histogram *>& histograms )
{
  string strTime;
  TRecordTime auxTime;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strTime, ' ' );
  istringstream tmpTime( strTime );

  if ( !( tmpTime >> auxTime ) )
    return false;

  if ( auxTime <= whichTrace->getEndTime() )
    windows[ windows.size() - 1 ]->setWindowEndTime( auxTime );
  else
    windows[ windows.size() - 1 ]->setWindowEndTime( whichTrace->getEndTime() );
  return true;
}

void WindowStopTime::printLine( ofstream& cfgFile,
                                const SaveOptions& options,
                                const vector<Window *>::const_iterator it )
{
  if ( !options.windowScaleRelative )
    cfgFile << OLDCFG_TAG_WNDW_STOP_TIME << " " << ( *it )->getWindowEndTime() << endl;
}

bool WindowBeginTimeRelative::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<Window *>& windows,
    vector<Histogram *>& histograms )
{
  string strPercentage;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strPercentage, ' ' );
  istringstream tmpStream( strPercentage );
  TSemanticValue percentage;

  if ( !( tmpStream >> percentage ) )
    return false;

  windows[ windows.size() - 1 ]->setWindowBeginTime(
    ( windows[ windows.size() - 1 ]->getTrace() )->getEndTime() * percentage );

  return true;
}

void WindowBeginTimeRelative::printLine( ofstream& cfgFile,
    const SaveOptions& options,
    const vector<Window *>::const_iterator it )
{
  if ( options.windowBeginTimeRelative )
  {
    cfgFile << OLDCFG_TAG_WNDW_BEGIN_TIME_RELATIVE << " ";
    cfgFile << ( *it )->getWindowBeginTime() / ( *it )->getTrace()->getEndTime();
    cfgFile << endl;
  }
}

bool WindowNumberOfRow::parseLine( KernelConnection *whichKernel, istringstream& line,
                                   Trace *whichTrace,
                                   vector<Window *>& windows,
                                   vector<Histogram *>& histograms )
{
#ifndef WIN32
#warning WindowNumberOfRow::parseLine
#endif
  return true;
}

void WindowNumberOfRow::printLine( ofstream& cfgFile,
                                   const vector<Window *>::const_iterator it )
{
#ifndef WIN32
#warning WindowNumberOfRow::printLine
#endif
}

bool WindowSelectedFunctions::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<Window *>& windows,
    vector<Histogram *>& histograms )
{
  string tmpString;
  string strNumFunctions;
  UINT16 numFunctions;
  string strLevel;
  TWindowLevel level;
  string strFunction;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  if ( windows[ windows.size() - 1 ]->isDerivedWindow() )
    return true;

  getline( line, tmpString, ' ' );
  getline( line, strNumFunctions, ',' );
  istringstream tmpNumFunctions( strNumFunctions );

  if ( !( tmpNumFunctions >> numFunctions ) )
    return false;

  getline( line, tmpString, '{' );
  for ( UINT16 i = 0; i < numFunctions; i++ )
  {
    getline( line, tmpString, '{' );
    getline( line, strLevel, ',' );
    getline( line, tmpString, ' ' );
    getline( line, strFunction, '}' );
    level = stringToLevel( strLevel );

    // It's a semantic function
    if ( level != NONE )
    {
      bool result = windows[ windows.size() - 1 ]->setLevelFunction( level, strFunction );
      if ( !result )
        return false;
    }
    // It's a filter function
    else
    {
      Filter *filter = windows[ windows.size() - 1 ]->getFilter();

      if ( strLevel.compare( OLDCFG_VAL_FILTER_OBJ_FROM ) == 0 )
        filter->setCommFromFunction( strFunction );
      else if ( strLevel.compare( OLDCFG_VAL_FILTER_OBJ_TO ) == 0 )
        filter->setCommToFunction( strFunction );
      else if ( strLevel.compare( OLDCFG_VAL_FILTER_COM_TAG ) == 0 )
        filter->setCommTagFunction( strFunction );
      else if ( strLevel.compare( OLDCFG_VAL_FILTER_COM_SIZE ) == 0 )
        filter->setCommSizeFunction( strFunction );
      else if ( strLevel.compare( OLDCFG_VAL_FILTER_COM_BW ) == 0 )
        filter->setBandWidthFunction( strFunction );
      else if ( strLevel.compare( OLDCFG_VAL_FILTER_EVT_TYPE ) == 0 )
        filter->setEventTypeFunction( strFunction );
      else if ( strLevel.compare( OLDCFG_VAL_FILTER_EVT_VALUE ) == 0 )
        filter->setEventValueFunction( strFunction );
    }
  }

  return true;
}

void WindowSelectedFunctions::printLine( ofstream& cfgFile,
    const vector<Window *>::const_iterator it )
{
  Filter *filter = ( *it )->getFilter();

  cfgFile << OLDCFG_TAG_WNDW_SELECTED_FUNCTIONS << " { 14, { ";
  cfgFile << "{" << OLDCFG_LVL_CPU << ", " << ( *it )->getLevelFunction( CPU ) << "}, ";
  cfgFile << "{" << OLDCFG_LVL_APPL << ", " << ( *it )->getLevelFunction( APPLICATION ) << "}, ";
  cfgFile << "{" << OLDCFG_LVL_TASK << ", " << ( *it )->getLevelFunction( TASK ) << "}, ";
  cfgFile << "{" << OLDCFG_LVL_THREAD << ", " << ( *it )->getLevelFunction( THREAD ) << "}, ";
  cfgFile << "{" << OLDCFG_LVL_NODE << ", " << ( *it )->getLevelFunction( NODE ) << "}, ";
  cfgFile << "{" << OLDCFG_LVL_SYSTEM << ", " << ( *it )->getLevelFunction( SYSTEM ) << "}, ";
  cfgFile << "{" << OLDCFG_LVL_WORKLOAD << ", " << ( *it )->getLevelFunction( WORKLOAD ) << "}, ";
  cfgFile << "{" << OLDCFG_VAL_FILTER_OBJ_FROM << ", " << filter->getCommFromFunction() << "}, ";
  cfgFile << "{" << OLDCFG_VAL_FILTER_OBJ_TO << ", " << filter->getCommToFunction() << "}, ";
  cfgFile << "{" << OLDCFG_VAL_FILTER_COM_TAG << ", " << filter->getCommTagFunction() << "}, ";
  cfgFile << "{" << OLDCFG_VAL_FILTER_COM_SIZE << ", " << filter->getCommSizeFunction() << "}, ";
  cfgFile << "{" << OLDCFG_VAL_FILTER_COM_BW << ", " << filter->getBandWidthFunction() << "}, ";
  cfgFile << "{" << OLDCFG_VAL_FILTER_EVT_TYPE << ", " << filter->getEventTypeFunction() << "}, ";
  cfgFile << "{" << OLDCFG_VAL_FILTER_EVT_VALUE << ", " << filter->getEventValueFunction() << "} ";
  cfgFile << "} }" << endl;
}

bool WindowComposeFunctions::parseLine( KernelConnection *whichKernel, istringstream& line,
                                        Trace *whichTrace,
                                        vector<Window *>& windows,
                                        vector<Histogram *>& histograms )
{
  string tmpString;
  string strNumFunctions;
  UINT16 numFunctions;
  string strLevel;
  TWindowLevel level;
  string strFunction;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, tmpString, ' ' );
  getline( line, strNumFunctions, ',' );
  istringstream tmpNumFunctions( strNumFunctions );

  if ( !( tmpNumFunctions >> numFunctions ) )
    return false;

  getline( line, tmpString, '{' );
  for ( UINT16 i = 0; i < numFunctions; i++ )
  {
    getline( line, tmpString, '{' );
    getline( line, strLevel, ',' );
    getline( line, tmpString, ' ' );
    getline( line, strFunction, '}' );
    level = stringToLevel( strLevel );

    // It's a semantic function
    if ( level != NONE )
    {
      bool result = windows[ windows.size() - 1 ]->setLevelFunction( level, strFunction );
      if ( !result )
        return false;
    }
    else
      return false;
  }

  return true;
}

void WindowComposeFunctions::printLine( ofstream& cfgFile,
                                        const vector<Window *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_WNDW_COMPOSE_FUNCTIONS << " { 9, { ";
  cfgFile << "{" << OLDCFG_LVL_COMPOSE_CPU << ", ";
  if ( !( *it )->isDerivedWindow() )
    cfgFile << ( *it )->getLevelFunction( COMPOSECPU ) << "}, ";
  else
    cfgFile << "As Is}, ";

  cfgFile << "{" << OLDCFG_LVL_COMPOSE_APPL << ", ";
  if ( !( *it )->isDerivedWindow() )
    cfgFile << ( *it )->getLevelFunction( COMPOSEAPPLICATION ) << "}, ";
  else
    cfgFile << "As Is}, ";

  cfgFile << "{" << OLDCFG_LVL_COMPOSE_TASK << ", ";
  if ( !( *it )->isDerivedWindow() )
    cfgFile << ( *it )->getLevelFunction( COMPOSETASK ) << "}, ";
  else
    cfgFile << "As Is}, ";

  cfgFile << "{" << OLDCFG_LVL_COMPOSE_THREAD << ", ";
  if ( !( *it )->isDerivedWindow() )
    cfgFile << ( *it )->getLevelFunction( COMPOSETHREAD ) << "}, ";
  else
    cfgFile << "As Is}, ";

  cfgFile << "{" << OLDCFG_LVL_COMPOSE_NODE << ", ";
  if ( !( *it )->isDerivedWindow() )
    cfgFile << ( *it )->getLevelFunction( COMPOSENODE ) << "}, ";
  else
    cfgFile << "As Is}, ";

  cfgFile << "{" << OLDCFG_LVL_COMPOSE_SYSTEM << ", ";
  if ( !( *it )->isDerivedWindow() )
    cfgFile << ( *it )->getLevelFunction( COMPOSESYSTEM ) << "}, ";
  else
    cfgFile << "As Is}, ";

  cfgFile << "{" << OLDCFG_LVL_COMPOSE_WORKLOAD << ", ";
  if ( !( *it )->isDerivedWindow() )
    cfgFile << ( *it )->getLevelFunction( COMPOSEWORKLOAD ) << "}, ";
  else
    cfgFile << "As Is}, ";

  cfgFile << "{" << OLDCFG_LVL_TOPCOMPOSE1 << ", " << ( *it )->getLevelFunction( TOPCOMPOSE1 ) << "}, ";
  cfgFile << "{" << OLDCFG_LVL_TOPCOMPOSE2 << ", " << ( *it )->getLevelFunction( TOPCOMPOSE2 ) << "} ";
  cfgFile << "} }" << endl;
}

bool WindowSemanticModule::parseLine( KernelConnection *whichKernel, istringstream& line,
                                      Trace *whichTrace,
                                      vector<Window *>& windows,
                                      vector<Histogram *>& histograms )
{
  string strLevel;
  TWindowLevel level;
  string strFunction;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strLevel, ' ' );
  level = stringToLevel( strLevel );
  if ( level == NONE )
    return false;

  getline( line, strFunction, '{' );
  strFunction.erase( strFunction.length() - 1 ); // Final space.

  if ( windows[ windows.size() - 1 ]->getLevelFunction( level ) == strFunction )
  {
    string tmpString;
    string strNumParam;
    TParamIndex numParam;

    getline( line, tmpString, ' ' );
    getline( line, strNumParam, ',' );
    istringstream tmpNumParam( strNumParam );
    if ( !( tmpNumParam >> numParam ) )
      return false;

    getline( line, tmpString, '{' );

    for ( TParamIndex i = 0; i < numParam; i++ )
    {
      string strNumValues;
      UINT32 numValues;
      TParamValue values;

      getline( line, tmpString, ' ' );

      getline( line, strNumValues, ' ' );
      istringstream tmpNumValues( strNumValues );
      if ( !( tmpNumValues >> numValues ) )
        return false;

      for ( UINT32 j = 0; j < numValues; j++ )
      {
        string strParamValue;
        double paramValue;

        if ( j < numValues - 1 )
          getline( line, strParamValue, ' ' );
        else if ( j == numValues - 1 )
        {
          if ( i < numParam - 1 )
            getline( line, strParamValue, ',' );
          else if ( i == numParam - 1 )
            getline( line, strParamValue, ' ' );
        }
        istringstream tmpParamValue( strParamValue );
        if ( !( tmpParamValue >> paramValue ) )
          return false;
        values.push_back( paramValue );
      }

      windows[ windows.size() - 1 ]->setFunctionParam( level, i, values );
    }

  }

  return true;
}

void WindowSemanticModule::printLine( ofstream& cfgFile,
                                      const vector<Window *>::const_iterator it )
{
  for ( int levelIdx = TOPCOMPOSE1; levelIdx <= TOPCOMPOSE2; ++levelIdx )
  {
    TWindowLevel topLevel = ( TWindowLevel ) levelIdx;
    for ( TParamIndex parIdx = 0; parIdx < ( *it )->getFunctionNumParam( topLevel ); ++parIdx )
    {
      if ( parIdx == 0 )
      {
        cfgFile << OLDCFG_TAG_WNDW_SEMANTIC_MODULE << " " << levelToString( topLevel );
        cfgFile << " " << ( *it )->getLevelFunction( topLevel ) << " { ";
        cfgFile << ( *it )->getFunctionNumParam( topLevel ) << ", ";
        cfgFile << "{ ";
      }
      vector<double> v = ( *it )->getFunctionParam( topLevel, parIdx );
      cfgFile << v.size();
      for ( vector<double>::iterator itVec = v.begin(); itVec != v.end(); ++itVec )
        cfgFile << " " << ( *itVec );
      if ( parIdx == ( *it )->getFunctionNumParam( topLevel ) - 1 )
        cfgFile << ", ";
      else
        cfgFile << " } }" << endl;
    }
  }

  if ( ( *it )->isDerivedWindow() )
  {
    for ( TParamIndex parIdx = 0; parIdx < ( *it )->getFunctionNumParam( DERIVED ); ++parIdx )
    {
      if ( parIdx == 0 )
      {
        cfgFile << OLDCFG_TAG_WNDW_SEMANTIC_MODULE << " " << levelToString( DERIVED );
        cfgFile << " " << ( *it )->getLevelFunction( DERIVED ) << " { ";
        cfgFile << ( *it )->getFunctionNumParam( DERIVED ) << ", ";
        cfgFile << "{ ";
      }
      vector<double> v = ( *it )->getFunctionParam( DERIVED, parIdx );
      cfgFile << v.size();
      for ( vector<double>::iterator itVec = v.begin(); itVec != v.end(); ++itVec )
        cfgFile << " " << ( *itVec );
      if ( parIdx < ( *it )->getFunctionNumParam( DERIVED ) - 1 )
        cfgFile << ", ";
      else
        cfgFile << " } }" << endl;
    }
  }
  else
  {
    for ( int levelIdx = WORKLOAD; levelIdx <= CPU; ++levelIdx )
    {
      TWindowLevel level = ( TWindowLevel ) levelIdx;
      for ( TParamIndex parIdx = 0; parIdx < ( *it )->getFunctionNumParam( level ); ++parIdx )
      {
        if ( parIdx == 0 )
        {
          cfgFile << OLDCFG_TAG_WNDW_SEMANTIC_MODULE << " " << levelToString( level );
          cfgFile << " " << ( *it )->getLevelFunction( level ) << " { ";
          cfgFile << ( *it )->getFunctionNumParam( level ) << ", ";
          cfgFile << "{ ";
        }
        vector<double> v = ( *it )->getFunctionParam( level, parIdx );
        cfgFile << v.size();
        for ( vector<double>::iterator itVec = v.begin(); itVec != v.end(); ++itVec )
          cfgFile << " " << ( *itVec );
        if ( parIdx < ( *it )->getFunctionNumParam( level ) - 1 )
          cfgFile << ", ";
        else
          cfgFile << " } }" << endl;
      }
    }
    for ( int levelIdx = COMPOSEWORKLOAD; levelIdx <= COMPOSECPU; ++levelIdx )
    {
      TWindowLevel level = ( TWindowLevel ) levelIdx;
      for ( TParamIndex parIdx = 0; parIdx < ( *it )->getFunctionNumParam( level ); ++parIdx )
      {
        if ( parIdx == 0 )
        {
          cfgFile << OLDCFG_TAG_WNDW_SEMANTIC_MODULE << " " << levelToString( level );
          cfgFile << " " << ( *it )->getLevelFunction( level ) << " { ";
          cfgFile << ( *it )->getFunctionNumParam( level ) << ", ";
          cfgFile << "{ ";
        }
        vector<double> v = ( *it )->getFunctionParam( level, parIdx );
        cfgFile << v.size();
        for ( vector<double>::iterator itVec = v.begin(); itVec != v.end(); ++itVec )
          cfgFile << " " << ( *itVec );
        if ( parIdx < ( *it )->getFunctionNumParam( level ) - 1 )
          cfgFile << ", ";
        else
          cfgFile << " } }" << endl;
      }
    }

  }
}

bool WindowFilterModule::parseLine( KernelConnection *whichKernel, istringstream& line,
                                    Trace *whichTrace,
                                    vector<Window *>& windows,
                                    vector<Histogram *>& histograms )
{
  string strTag, strNumberParams, strValue;
  UINT16 numParams;
  Filter *filter;
  TObjectOrder fromObject;
  TObjectOrder toObject;
  TCommTag commTag;
  TCommSize commSize;
  TSemanticValue bandWidth;
  TEventType eventType;
  TEventValue eventValue;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  if ( windows[ windows.size() - 1 ]->isDerivedWindow() )
    return true;

  getline( line, strTag, ' ' );          // Parameter type.
  getline( line, strNumberParams, ' ' ); // Number of following parameters.
  istringstream tmpNumberParams( strNumberParams );

  if ( !( tmpNumberParams >> numParams ) )
    return false;

  filter = windows[ windows.size() - 1 ]->getFilter();

  for ( UINT16 ii = 0; ii < numParams; ii++ )
  {
    if ( strTag.compare( OLDCFG_VAL_FILTER_OBJ_FROM ) == 0 )
    {
      getline( line, strValue, ' ' );
      istringstream tmpValue( strValue );

      if ( !( tmpValue >> fromObject ) )
        return false;

      filter->insertCommFrom( fromObject - 1 );
    }
    else if ( strTag.compare( OLDCFG_VAL_FILTER_OBJ_TO ) == 0 )
    {
      getline( line, strValue, ' ' );
      istringstream tmpValue( strValue );

      if ( !( tmpValue >> toObject ) )
        return false;

      filter->insertCommTo( toObject - 1 );
    }
    else if ( strTag.compare( OLDCFG_VAL_FILTER_COM_TAG ) == 0 )
    {
      getline( line, strValue, ' ' );
      istringstream tmpValue( strValue );

      if ( !( tmpValue >> commTag ) )
        return false;

      filter->insertCommTag( commTag );
    }
    else if ( strTag.compare( OLDCFG_VAL_FILTER_COM_SIZE ) == 0 )
    {
      getline( line, strValue, ' ' );
      istringstream tmpValue( strValue );

      if ( !( tmpValue >> commSize ) )
        return false;

      filter->insertCommSize( commSize );
    }
    else if ( strTag.compare( OLDCFG_VAL_FILTER_COM_BW ) == 0 )
    {
      getline( line, strValue, ' ' );
      istringstream tmpValue( strValue );

      if ( !( tmpValue >> bandWidth ) )
        return false;

      filter->insertBandWidth( bandWidth );
    }
    else if ( strTag.compare( OLDCFG_VAL_FILTER_EVT_TYPE ) == 0 )
    {
      getline( line, strValue, ' ' );
      istringstream tmpValue( strValue );

      if ( !( tmpValue >> eventType ) )
        return false;

      filter->insertEventType( eventType );
    }
    else if ( strTag.compare( OLDCFG_VAL_FILTER_EVT_VALUE ) == 0 )
    {
      getline( line, strValue, ' ' );
      istringstream tmpValue( strValue );

      if ( !( tmpValue >> eventValue ) )
        return false;

      filter->insertEventValue( eventValue );
    }
  }

  return true;
}

void WindowFilterModule::printLine( ofstream& cfgFile,
                                    const vector<Window *>::const_iterator it )
{
  Filter *filter = ( *it )->getFilter();
  vector<TObjectOrder> objVec;
  vector<TCommTag> tagVec;
  vector<TCommSize> sizeVec;
  vector<TSemanticValue> bwVec;
  vector<TEventType> typeVec;
  vector<TEventValue> valueVec;

  filter->getCommFrom( objVec );
  if ( objVec.begin() != objVec.end() )
  {
    cfgFile << OLDCFG_TAG_WNDW_FILTER_MODULE << " " << OLDCFG_VAL_FILTER_OBJ_FROM << " ";
    cfgFile << objVec.size();
    for ( vector<TObjectOrder>::iterator itObj = objVec.begin();
          itObj != objVec.end(); ++itObj )
    {
      cfgFile << " " << ( *itObj );
    }
    cfgFile << endl;
  }

  objVec.clear();
  filter->getCommTo( objVec );
  if ( objVec.begin() != objVec.end() )
  {
    cfgFile << OLDCFG_TAG_WNDW_FILTER_MODULE << " " << OLDCFG_VAL_FILTER_OBJ_TO << " ";
    cfgFile << objVec.size();
    for ( vector<TObjectOrder>::iterator itObj = objVec.begin();
          itObj != objVec.end(); ++itObj )
    {
      cfgFile << " " << ( *itObj );
    }
    cfgFile << endl;
  }

  filter->getCommTag( tagVec );
  if ( tagVec.begin() != tagVec.end() )
  {
    cfgFile << OLDCFG_TAG_WNDW_FILTER_MODULE << " " << OLDCFG_VAL_FILTER_COM_TAG << " ";
    cfgFile << tagVec.size();
    for ( vector<TCommTag>::iterator itTag = tagVec.begin();
          itTag != tagVec.end(); ++itTag )
    {
      cfgFile << " " << ( *itTag );
    }
    cfgFile << endl;
  }

  filter->getCommSize( sizeVec );
  if ( sizeVec.begin() != sizeVec.end() )
  {
    cfgFile << OLDCFG_TAG_WNDW_FILTER_MODULE << " " << OLDCFG_VAL_FILTER_COM_SIZE << " ";
    cfgFile << sizeVec.size();
    for ( vector<TCommSize>::iterator itSize = sizeVec.begin();
          itSize != sizeVec.end(); ++itSize )
    {
      cfgFile << " " << ( *itSize );
    }
    cfgFile << endl;
  }

  filter->getBandWidth( bwVec );
  if ( bwVec.begin() != bwVec.end() )
  {
    cfgFile << OLDCFG_TAG_WNDW_FILTER_MODULE << " " << OLDCFG_VAL_FILTER_COM_BW << " ";
    cfgFile << bwVec.size();
    for ( vector<TSemanticValue>::iterator itBW = bwVec.begin();
          itBW != bwVec.end(); ++itBW )
    {
      cfgFile << " " << ( *itBW );
    }
    cfgFile << endl;
  }

  filter->getEventType( typeVec );
  if ( typeVec.begin() != typeVec.end() )
  {
    cfgFile << OLDCFG_TAG_WNDW_FILTER_MODULE << " " << OLDCFG_VAL_FILTER_EVT_TYPE << " ";
    cfgFile << typeVec.size();
    for ( vector<TEventType>::iterator itType = typeVec.begin();
          itType != typeVec.end(); ++itType )
    {
      cfgFile << " " << ( *itType );
    }
    cfgFile << endl;
  }

  filter->getEventValue( valueVec );
  if ( valueVec.begin() != valueVec.end() )
  {
    cfgFile << OLDCFG_TAG_WNDW_FILTER_MODULE << " " << OLDCFG_VAL_FILTER_EVT_VALUE << " ";
    cfgFile << valueVec.size();
    for ( vector<TEventValue>::iterator itValue = valueVec.begin();
          itValue != valueVec.end(); ++itValue )
    {
      cfgFile << " " << ( *itValue );
    }
    cfgFile << endl;
  }

}

bool WindowFilterLogical::parseLine( KernelConnection *whichKernel, istringstream& line,
                                     Trace *whichTrace,
                                     vector<Window *>& windows,
                                     vector<Histogram *>& histograms )
{
  string strBool;
  Filter *filter;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  if ( windows[ windows.size() - 1 ]->isDerivedWindow() )
    return true;

  getline( line, strBool, ' ' );

  filter = windows[ windows.size() - 1 ]->getFilter();

  if ( strBool.compare( OLDCFG_VAL_FALSE ) == 0 )
    filter->setLogical( false );
  else if ( strBool.compare( OLDCFG_VAL_TRUE ) == 0 )
    filter->setLogical( true );
  else
    return false;

  return true;
}

void WindowFilterLogical::printLine( ofstream& cfgFile,
                                     const vector<Window *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_WNDW_LOGICAL_FILTERED << " ";
  if ( ( *it )->getFilter()->getLogical() )
    cfgFile << OLDCFG_VAL_TRUE;
  else
    cfgFile << OLDCFG_VAL_FALSE;
  cfgFile << endl;
}

bool WindowFilterPhysical::parseLine( KernelConnection *whichKernel, istringstream& line,
                                      Trace *whichTrace,
                                      vector<Window *>& windows,
                                      vector<Histogram *>& histograms )
{
  string strBool;
  Filter *filter;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  if ( windows[ windows.size() - 1 ]->isDerivedWindow() )
    return true;

  getline( line, strBool, ' ' );

  filter = windows[ windows.size() - 1 ]->getFilter();

  if ( strBool.compare( OLDCFG_VAL_FALSE ) == 0 )
    filter->setPhysical( false );
  else if ( strBool.compare( OLDCFG_VAL_TRUE ) == 0 )
    filter->setPhysical( true );
  else
    return false;

  return true;
}

void WindowFilterPhysical::printLine( ofstream& cfgFile,
                                      const vector<Window *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_WNDW_PHYSICAL_FILTERED << " ";
  if ( ( *it )->getFilter()->getPhysical() )
    cfgFile << OLDCFG_VAL_TRUE;
  else
    cfgFile << OLDCFG_VAL_FALSE;
  cfgFile << endl;
}

bool WindowFilterBoolOpFromTo::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<Window *>& windows,
    vector<Histogram *>& histograms )
{
  string strBool;
  Filter *filter;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  if ( windows[ windows.size() - 1 ]->isDerivedWindow() )
    return true;

  filter = windows[ windows.size() - 1 ]->getFilter();

  getline( line, strBool, ' ' );

  if ( strBool.compare( OLDCFG_VAL_FALSE ) == 0 )
    filter->setOpFromToOr();
  else if ( strBool.compare( OLDCFG_VAL_TRUE ) == 0 )
    filter->setOpFromToAnd();
  else
    return false;

  return true;
}

void WindowFilterBoolOpFromTo::printLine( ofstream& cfgFile,
    const vector<Window *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_WNDW_FROMTO << " ";
  if ( ( *it )->getFilter()->getOpFromTo() )
    cfgFile << OLDCFG_VAL_TRUE;
  else
    cfgFile << OLDCFG_VAL_FALSE;
  cfgFile << endl;
}

bool WindowFilterBoolOpTagSize::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<Window *>& windows,
    vector<Histogram *>& histograms )
{
  string strBool;
  Filter *filter;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  if ( windows[ windows.size() - 1 ]->isDerivedWindow() )
    return true;

  getline( line, strBool, ' ' );

  filter = windows[ windows.size() - 1 ]->getFilter();

  if ( strBool.compare( OLDCFG_VAL_FALSE ) == 0 )
    filter->setOpTagSizeOr();
  else if ( strBool.compare( OLDCFG_VAL_TRUE ) == 0 )
    filter->setOpTagSizeAnd();
  else
    return false;

  return true;
}

void WindowFilterBoolOpTagSize::printLine( ofstream& cfgFile,
    const vector<Window *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_WNDW_COMM_TAGSIZE << " ";
  if ( ( *it )->getFilter()->getOpTagSize() )
    cfgFile << OLDCFG_VAL_TRUE;
  else
    cfgFile << OLDCFG_VAL_FALSE;
  cfgFile << endl;
}

bool WindowFilterBoolOpTypeVal::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<Window *>& windows,
    vector<Histogram *>& histograms )
{
  string strBool;
  Filter *filter;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  if ( windows[ windows.size() - 1 ]->isDerivedWindow() )
    return true;

  getline( line, strBool, ' ' );

  filter = windows[ windows.size() - 1 ]->getFilter();

  if ( strBool.compare( OLDCFG_VAL_FALSE ) == 0 )
    filter->setOpTypeValueOr();
  else if ( strBool.compare( OLDCFG_VAL_TRUE ) == 0 )
    filter->setOpTypeValueAnd();
  else
    return false;

  return true;
}

void WindowFilterBoolOpTypeVal::printLine( ofstream& cfgFile,
    const vector<Window *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_WNDW_TYPEVAL << " ";
  if ( ( *it )->getFilter()->getOpTypeValue() )
    cfgFile << OLDCFG_VAL_TRUE;
  else
    cfgFile << OLDCFG_VAL_FALSE;
  cfgFile << endl;
}

bool WindowOpen::parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms )
{
  string strBool;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strBool, ' ' );

  if ( strBool.compare( OLDCFG_VAL_FALSE ) == 0 )
    windows[ windows.size() - 1 ]->setShowWindow( false );
  else if ( strBool.compare( OLDCFG_VAL_TRUE ) == 0 )
    windows[ windows.size() - 1 ]->setShowWindow( true );
  else
    return false;

  return true;
}

void WindowOpen::printLine( ofstream& cfgFile,
                            const vector<Window *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_WNDW_OPEN << " ";
  if ( ( *it )->getShowWindow() )
    cfgFile << OLDCFG_VAL_TRUE;
  else
    cfgFile << OLDCFG_VAL_FALSE;
  cfgFile << endl;
}

bool WindowDrawMode::parseLine( KernelConnection *whichKernel, istringstream& line,
                                Trace *whichTrace,
                                vector<Window *>& windows,
                                vector<Histogram *>& histograms )
{
  string strMode;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strMode, ' ' );

  if ( strMode.compare( OLDCFG_VAL_DRAW_MODE_LAST ) == 0 )
    windows[ windows.size() - 1 ]->setDrawModeTime( DRAW_LAST );
  else if ( strMode.compare( OLDCFG_VAL_DRAW_MODE_MAXIMUM ) == 0 )
    windows[ windows.size() - 1 ]->setDrawModeTime( DRAW_MAXIMUM );
  else if ( strMode.compare( OLDCFG_VAL_DRAW_MODE_MINNOT0 ) == 0 )
    windows[ windows.size() - 1 ]->setDrawModeTime( DRAW_MINNOTZERO );
  else if ( strMode.compare( OLDCFG_VAL_DRAW_MODE_RANDOM ) == 0 )
    windows[ windows.size() - 1 ]->setDrawModeTime( DRAW_RANDOM );
  else if ( strMode.compare( OLDCFG_VAL_DRAW_MODE_RANDOMNOT0 ) == 0 )
    windows[ windows.size() - 1 ]->setDrawModeTime( DRAW_RANDNOTZERO );
  else if ( strMode.compare( OLDCFG_VAL_DRAW_MODE_AVERAGE ) == 0 )
    windows[ windows.size() - 1 ]->setDrawModeTime( DRAW_AVERAGE );

  return true;
}

void WindowDrawMode::printLine( ofstream& cfgFile,
                                const vector<Window *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_WNDW_DRAW_MODE << " ";
  switch ( ( *it )->getDrawModeTime() )
  {
    case DRAW_LAST:
      cfgFile << OLDCFG_VAL_DRAW_MODE_LAST;
      break;
    case DRAW_MAXIMUM:
      cfgFile << OLDCFG_VAL_DRAW_MODE_MAXIMUM;
      break;
    case DRAW_MINNOTZERO:
      cfgFile << OLDCFG_VAL_DRAW_MODE_MINNOT0;
      break;
    case DRAW_RANDOM:
      cfgFile << OLDCFG_VAL_DRAW_MODE_RANDOM;
      break;
    case DRAW_RANDNOTZERO:
      cfgFile << OLDCFG_VAL_DRAW_MODE_RANDOMNOT0;
      break;
    case DRAW_AVERAGE:
      cfgFile << OLDCFG_VAL_DRAW_MODE_AVERAGE;
      break;
    default:
      break;
  }
  cfgFile << endl;
}

bool WindowDrawModeRows::parseLine( KernelConnection *whichKernel, istringstream& line,
                                    Trace *whichTrace,
                                    vector<Window *>& windows,
                                    vector<Histogram *>& histograms )
{
  string strMode;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;

  getline( line, strMode, ' ' );

  if ( strMode.compare( OLDCFG_VAL_DRAW_MODE_LAST ) == 0 )
    windows[ windows.size() - 1 ]->setDrawModeObject( DRAW_LAST );
  else if ( strMode.compare( OLDCFG_VAL_DRAW_MODE_MAXIMUM ) == 0 )
    windows[ windows.size() - 1 ]->setDrawModeObject( DRAW_MAXIMUM );
  else if ( strMode.compare( OLDCFG_VAL_DRAW_MODE_MINNOT0 ) == 0 )
    windows[ windows.size() - 1 ]->setDrawModeObject( DRAW_MINNOTZERO );
  else if ( strMode.compare( OLDCFG_VAL_DRAW_MODE_RANDOM ) == 0 )
    windows[ windows.size() - 1 ]->setDrawModeObject( DRAW_RANDOM );
  else if ( strMode.compare( OLDCFG_VAL_DRAW_MODE_RANDOMNOT0 ) == 0 )
    windows[ windows.size() - 1 ]->setDrawModeObject( DRAW_RANDNOTZERO );
  else if ( strMode.compare( OLDCFG_VAL_DRAW_MODE_AVERAGE ) == 0 )
    windows[ windows.size() - 1 ]->setDrawModeObject( DRAW_AVERAGE );

  return true;
}

void WindowDrawModeRows::printLine( ofstream& cfgFile,
                                    const vector<Window *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_WNDW_DRAW_MODE_ROWS << " ";
  switch ( ( *it )->getDrawModeObject() )
  {
    case DRAW_LAST:
      cfgFile << OLDCFG_VAL_DRAW_MODE_LAST;
      break;
    case DRAW_MAXIMUM:
      cfgFile << OLDCFG_VAL_DRAW_MODE_MAXIMUM;
      break;
    case DRAW_MINNOTZERO:
      cfgFile << OLDCFG_VAL_DRAW_MODE_MINNOT0;
      break;
    case DRAW_RANDOM:
      cfgFile << OLDCFG_VAL_DRAW_MODE_RANDOM;
      break;
    case DRAW_RANDNOTZERO:
      cfgFile << OLDCFG_VAL_DRAW_MODE_RANDOMNOT0;
      break;
    case DRAW_AVERAGE:
      cfgFile << OLDCFG_VAL_DRAW_MODE_AVERAGE;
      break;
    default:
      break;
  }
  cfgFile << endl;
}

bool Analyzer2DCreate::parseLine( KernelConnection *whichKernel, istringstream& line,
                                  Trace *whichTrace,
                                  vector<Window *>& windows,
                                  vector<Histogram *>& histograms )
{
  if ( histograms[ histograms.size() - 1 ] == NULL )
  {
    histograms[ histograms.size() - 1 ] = Histogram::create( whichKernel );
  }
  else
  {
    histograms.push_back( Histogram::create( whichKernel ) );
  }

  return true;
}

void Analyzer2DCreate::printLine( ofstream& cfgFile,
                                  const vector<Histogram *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_AN2D_NEW << endl;
}

bool Analyzer2DName::parseLine( KernelConnection *whichKernel, istringstream& line,
                                Trace *whichTrace,
                                vector<Window *>& windows,
                                vector<Histogram *>& histograms )
{
  string strName;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histograms[ histograms.size() - 1 ] == NULL )
    return false;

  getline( line, strName );
  histograms[ histograms.size() - 1 ]->setName( strName );

  return true;
}

void Analyzer2DName::printLine( ofstream& cfgFile,
                                const vector<Histogram *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_AN2D_NAME << " " << ( *it )->getName() << endl;
}

bool Analyzer2DX::parseLine( KernelConnection *whichKernel, istringstream& line,
                             Trace *whichTrace,
                             vector<Window *>& windows,
                             vector<Histogram *>& histograms )
{
  string strX;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histograms[ histograms.size() - 1 ] == NULL )
    return false;

  getline( line, strX, ' ' );
  istringstream tmpStream( strX );
  UINT16 x;

  if ( !( tmpStream >> x ) )
    return false;

  histograms[ histograms.size() - 1 ]->setPosX( x );

  return true;
}

void Analyzer2DX::printLine( ofstream& cfgFile,
                             const vector<Histogram *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_AN2D_X << " " << ( *it )->getPosX() << endl;
}

bool Analyzer2DY::parseLine( KernelConnection *whichKernel, istringstream& line,
                             Trace *whichTrace,
                             vector<Window *>& windows,
                             vector<Histogram *>& histograms )
{
  string strY;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histograms[ histograms.size() - 1 ] == NULL )
    return false;

  getline( line, strY, ' ' );
  istringstream tmpStream( strY );
  UINT16 y;

  if ( !( tmpStream >> y ) )
    return false;

  histograms[ histograms.size() - 1 ]->setPosY( y );

  return true;
}

void Analyzer2DY::printLine( ofstream& cfgFile,
                             const vector<Histogram *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_AN2D_Y << " " << ( *it )->getPosY() << endl;
}

bool Analyzer2DWidth::parseLine( KernelConnection *whichKernel, istringstream& line,
                                 Trace *whichTrace,
                                 vector<Window *>& windows,
                                 vector<Histogram *>& histograms )
{
  string strWidth;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histograms[ histograms.size() - 1 ] == NULL )
    return false;

  getline( line, strWidth, ' ' );
  istringstream tmpStream( strWidth );
  UINT16 width;

  if ( !( tmpStream >> width ) )
    return false;

  histograms[ histograms.size() - 1 ]->setWidth( width );

  return true;
}

void Analyzer2DWidth::printLine( ofstream& cfgFile,
                                 const vector<Histogram *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_AN2D_WIDTH << " " << ( *it )->getWidth() << endl;
}

bool Analyzer2DHeight::parseLine( KernelConnection *whichKernel, istringstream& line,
                                  Trace *whichTrace,
                                  vector<Window *>& windows,
                                  vector<Histogram *>& histograms )
{
  string strHeight;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histograms[ histograms.size() - 1 ] == NULL )
    return false;

  getline( line, strHeight, ' ' );
  istringstream tmpStream( strHeight );
  UINT16 height;

  if ( !( tmpStream >> height ) )
    return false;

  histograms[ histograms.size() - 1 ]->setHeight( height );

  return true;
}

void Analyzer2DHeight::printLine( ofstream& cfgFile,
                                  const vector<Histogram *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_AN2D_HEIGHT << " " << ( *it )->getHeight() << endl;
}

bool Analyzer2DControlWindow::parseLine( KernelConnection *whichKernel, istringstream& line, Trace *whichTrace,
    vector<Window *>& windows,
    vector<Histogram *>& histograms )
{
  string strIndexControlWindow;
  UINT32 indexControlWindow;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histograms[ histograms.size() - 1 ] == NULL )
    return false;

  getline( line, strIndexControlWindow );
  istringstream tmpWindow( strIndexControlWindow );
  if ( !( tmpWindow >> indexControlWindow ) )
    return false;

  if ( indexControlWindow > windows.size() ||
       windows[ indexControlWindow - 1 ] == NULL )
    return false;

  histograms[ histograms.size() - 1 ]->setControlWindow( windows[ indexControlWindow - 1 ] );

  return true;
}

void Analyzer2DControlWindow::printLine( ofstream& cfgFile,
    const vector<Window *>& allWindows,
    const vector<Histogram *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_AN2D_CONTROL_WINDOW << " ";
  cfgFile << CFGLoader::findWindow( ( *it )->getControlWindow(), allWindows ) + 1;
  cfgFile << endl;
}

bool Analyzer2DDataWindow::parseLine( KernelConnection *whichKernel, istringstream& line,
                                      Trace *whichTrace,
                                      vector<Window *>& windows,
                                      vector<Histogram *>& histograms )
{
  string strIndexDataWindow;
  UINT32 indexDataWindow;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histograms[ histograms.size() - 1 ] == NULL )
    return false;

  getline( line, strIndexDataWindow );
  istringstream tmpWindow( strIndexDataWindow );
  if ( !( tmpWindow >> indexDataWindow ) )
    return false;

  if ( indexDataWindow > windows.size() ||
       windows[ indexDataWindow - 1 ] == NULL )
    return false;

  histograms[ histograms.size() - 1 ]->setDataWindow( windows[ indexDataWindow - 1 ] );

  return true;
}

void Analyzer2DDataWindow::printLine( ofstream& cfgFile,
                                      const vector<Window *>& allWindows,
                                      const vector<Histogram *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_AN2D_DATA_WINDOW << " ";
  cfgFile << CFGLoader::findWindow( ( *it )->getDataWindow(), allWindows ) + 1;
  cfgFile << endl;
}

bool Analyzer2DStatistic::parseLine( KernelConnection *whichKernel, istringstream& line,
                                     Trace *whichTrace,
                                     vector<Window *>& windows,
                                     vector<Histogram *>& histograms )
{
  string strStatistic;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histograms[ histograms.size() - 1 ] == NULL )
    return false;

  getline( line, strStatistic );

  if ( !histograms[ histograms.size() - 1 ]->getCalculateAll() )
    histograms[ histograms.size() - 1 ]->pushbackStatistic( strStatistic );

  histograms[ histograms.size() - 1 ]->setCurrentStat( strStatistic );

  return true;
}

void Analyzer2DStatistic::printLine( ofstream& cfgFile,
                                     const vector<Histogram *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_AN2D_STATISTIC << " ";
  cfgFile << ( *it )->getCurrentStat() << endl;
}

bool Analyzer2DCalculateAll::parseLine( KernelConnection *whichKernel, istringstream& line,
                                        Trace *whichTrace,
                                        vector<Window *>& windows,
                                        vector<Histogram *>& histograms )
{
  string strBoolAll;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histograms[ histograms.size() - 1 ] == NULL )
    return false;

  getline( line, strBoolAll, ' ' );

  if ( strBoolAll.compare( OLDCFG_VAL_TRUE2 ) == 0 )
    histograms[ histograms.size() - 1 ]->setCalculateAll( true );
  else if ( strBoolAll.compare( OLDCFG_VAL_FALSE2 ) == 0 )
    histograms[ histograms.size() - 1 ]->setCalculateAll( false );
  else
    return false;

  return true;
}

void Analyzer2DCalculateAll::printLine( ofstream& cfgFile,
                                        const vector<Histogram *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_AN2D_CALCULATE_ALL << " ";
  if ( ( *it )->getCalculateAll() )
    cfgFile << OLDCFG_VAL_TRUE2;
  else
    cfgFile << OLDCFG_VAL_FALSE2;
  cfgFile << endl;
}

bool Analyzer2DHideColumns::parseLine( KernelConnection *whichKernel, istringstream& line,
                                       Trace *whichTrace,
                                       vector<Window *>& windows,
                                       vector<Histogram *>& histograms )
{
  string strBool;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histograms[ histograms.size() - 1 ] == NULL )
    return false;

  getline( line, strBool, ' ' );

  if ( strBool.compare( OLDCFG_VAL_TRUE2 ) == 0 )
    histograms[ histograms.size() - 1 ]->setHideColumns( true );
  else if ( strBool.compare( OLDCFG_VAL_FALSE2 ) == 0 )
    histograms[ histograms.size() - 1 ]->setHideColumns( false );
  else
    return false;

  return true;
}

void Analyzer2DHideColumns::printLine( ofstream& cfgFile,
                                       const vector<Histogram *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_AN2D_HIDE_COLS << " ";
  if ( ( *it )->getHideColumns() )
    cfgFile << OLDCFG_VAL_TRUE2;
  else
    cfgFile << OLDCFG_VAL_FALSE2;
  cfgFile << endl;
}

bool Analyzer2DHorizontal::parseLine( KernelConnection *whichKernel, istringstream& line,
                                      Trace *whichTrace,
                                      vector<Window *>& windows,
                                      vector<Histogram *>& histograms )
{
  string strBool;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histograms[ histograms.size() - 1 ] == NULL )
    return false;

  getline( line, strBool, ' ' );

  if ( strBool.compare( OLDCFG_VAL_HORIZONTAL ) == 0 )
    histograms[ histograms.size() - 1 ]->setHorizontal( true );
  else if ( strBool.compare( OLDCFG_VAL_VERTICAL ) == 0 )
    histograms[ histograms.size() - 1 ]->setHorizontal( false );
  else
    return false;

  return true;
}

void Analyzer2DHorizontal::printLine( ofstream& cfgFile,
                                      const vector<Histogram *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_AN2D_HORIZONTAL << " ";
  if ( ( *it )->getHorizontal() )
    cfgFile << OLDCFG_VAL_HORIZONTAL;
  else
    cfgFile << OLDCFG_VAL_VERTICAL;
  cfgFile << endl;
}

bool Analyzer2DColor::parseLine( KernelConnection *whichKernel, istringstream& line,
                                 Trace *whichTrace,
                                 vector<Window *>& windows,
                                 vector<Histogram *>& histograms )
{
  string strBool;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histograms[ histograms.size() - 1 ] == NULL )
    return false;

  getline( line, strBool, ' ' );

  if ( strBool.compare( OLDCFG_VAL_TRUE2 ) == 0 )
    histograms[ histograms.size() - 1 ]->setShowColor( true );
  else if ( strBool.compare( OLDCFG_VAL_FALSE2 ) == 0 )
    histograms[ histograms.size() - 1 ]->setShowColor( false );

  return true;
}

void Analyzer2DColor::printLine( ofstream& cfgFile,
                                 const vector<Histogram *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_AN2D_COLOR << " ";
  if ( ( *it )->getShowColor() )
    cfgFile << OLDCFG_VAL_TRUE2;
  else
    cfgFile << OLDCFG_VAL_FALSE2;
  cfgFile << endl;
}

bool Analyzer2DZoom::parseLine( KernelConnection *whichKernel, istringstream& line,
                                Trace *whichTrace,
                                vector<Window *>& windows,
                                vector<Histogram *>& histograms )
{
  string strBool;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histograms[ histograms.size() - 1 ] == NULL )
    return false;

  getline( line, strBool, ' ' );

  if ( strBool.compare( OLDCFG_VAL_ENABLED ) == 0 )
    histograms[ histograms.size() - 1 ]->setZoom( true );
  else if ( strBool.compare( OLDCFG_VAL_DISABLED ) == 0 )
    histograms[ histograms.size() - 1 ]->setZoom( false );

  return true;
}

void Analyzer2DZoom::printLine( ofstream& cfgFile,
                                 const vector<Histogram *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_AN2D_COLOR << " ";
  if ( ( *it )->getZoom() )
    cfgFile << OLDCFG_VAL_ENABLED;
  else
    cfgFile << OLDCFG_VAL_DISABLED;
  cfgFile << endl;
}

bool Analyzer2DAccumulator:: parseLine( KernelConnection *whichKernel, istringstream& line,
                                        Trace *whichTrace,
                                        vector<Window *>& windows,
                                        vector<Histogram *>& histograms )
{
  string strAccumulator;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histograms[ histograms.size() - 1 ] == NULL )
    return false;

  return true;
}

void Analyzer2DAccumulator::printLine( ofstream& cfgFile,
                                       const vector<Histogram *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_AN2D_ACCUMULATOR << " ";
  if ( ( *it )->itsCommunicationStat( ( *it )->getCurrentStat() ) )
    cfgFile << levelToStringHisto( ( *it )->getControlWindow()->getLevel() );
  else
    cfgFile << OLDCFG_VAL_AN2D_ACCUM_SEMANTIC;
  cfgFile << endl;
}

bool Analyzer2DAccumulateByControlWindow::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<Window *>& windows,
    vector<Histogram *>& histograms )
{
  string strBool;
#ifndef WIN32
#warning Analyzer2DAccumulateByControlWindow::parseLine
#endif
  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histograms[ histograms.size() - 1 ] == NULL )
    return false;

  getline( line, strBool, ' ' );

  if ( strBool.compare( OLDCFG_VAL_FALSE2 ) == 0 )
    {}
  else if ( strBool.compare( OLDCFG_VAL_TRUE2 ) == 0 )
    {}
  else
    return false;

  return true;
}

void Analyzer2DAccumulateByControlWindow::printLine( ofstream& cfgFile,
    const vector<Histogram *>::const_iterator it )
{
#ifndef WIN32
#warning Analyzer2DAccumulateByControlWindow::printLine
#endif
}

bool Analyzer2DSortCols::parseLine( KernelConnection *whichKernel, istringstream& line,
                                    Trace *whichTrace,
                                    vector<Window *>& windows,
                                    vector<Histogram *>& histograms )
{
  string strBool;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histograms[ histograms.size() - 1 ] == NULL )
    return false;

  getline( line, strBool, ' ' );

  if ( strBool.compare( OLDCFG_VAL_TRUE2 ) == 0 )
  {
    histograms[ histograms.size() - 1 ]->setSortColumns( true );
  }
  else if ( strBool.compare( OLDCFG_VAL_FALSE2 ) == 0 )
  {
    histograms[ histograms.size() - 1 ]->setSortColumns( false );
  }
  else
    return false;

  return true;
}

void Analyzer2DSortCols::printLine( ofstream& cfgFile,
                                    const vector<Histogram *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_AN2D_SORTCOLS << " ";
  if ( ( *it )->getSortColumns() )
    cfgFile << OLDCFG_VAL_TRUE2;
  else
    cfgFile << OLDCFG_VAL_FALSE2;
  cfgFile << endl;
}

bool Analyzer2DSortCriteria::parseLine( KernelConnection *whichKernel, istringstream& line,
                                        Trace *whichTrace,
                                        vector<Window *>& windows,
                                        vector<Histogram *>& histograms )
{
  string strSortCriteria;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histograms[ histograms.size() - 1 ] == NULL )
    return false;

  getline( line, strSortCriteria );

  if ( strSortCriteria.compare( OLDCFG_VAL_SORT_AVERAGE ) == 0 )
    histograms[ histograms.size() - 1 ]->setSortCriteria( AVERAGE );
  else if ( strSortCriteria.compare( OLDCFG_VAL_SORT_TOTAL ) == 0 )
    histograms[ histograms.size() - 1 ]->setSortCriteria( TOTAL );
  else if ( strSortCriteria.compare( OLDCFG_VAL_SORT_MAXIMUM ) == 0 )
    histograms[ histograms.size() - 1 ]->setSortCriteria( MAXIMUM );
  else if ( strSortCriteria.compare( OLDCFG_VAL_SORT_MINIMUM ) == 0 )
    histograms[ histograms.size() - 1 ]->setSortCriteria( MINIMUM );
  else if ( strSortCriteria.compare( OLDCFG_VAL_SORT_STDEV ) == 0 )
    histograms[ histograms.size() - 1 ]->setSortCriteria( STDEV );
  else if ( strSortCriteria.compare( OLDCFG_VAL_SORT_AVGDIVMAX ) == 0 )
    histograms[ histograms.size() - 1 ]->setSortCriteria( AVGDIVMAX );
  else
    return false;

  return true;
}

void Analyzer2DSortCriteria::printLine( ofstream& cfgFile,
                                        const vector<Histogram *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_AN2D_SORTCRITERIA << " ";
  switch ( ( *it )->getSortCriteria() )
  {
    case AVERAGE:
      cfgFile << OLDCFG_VAL_SORT_AVERAGE;
      break;
    case TOTAL:
      cfgFile << OLDCFG_VAL_SORT_TOTAL;
      break;
    case MAXIMUM:
      cfgFile << OLDCFG_VAL_SORT_MAXIMUM;
      break;
    case MINIMUM:
      cfgFile << OLDCFG_VAL_SORT_MINIMUM;
      break;
    case STDEV:
      cfgFile << OLDCFG_VAL_SORT_STDEV;
      break;
    case AVGDIVMAX:
      cfgFile << OLDCFG_VAL_SORT_AVGDIVMAX;
      break;
    default:
      break;
  }
  cfgFile << endl;
}

/*
 Number_of_parameters Parameter1 ... ParameterN
*/
bool Analyzer2DParameters::parseLine( KernelConnection *whichKernel, istringstream& line,
                                      Trace *whichTrace,
                                      vector<Window *>& windows,
                                      vector<Histogram *>& histograms )
{
  string strNumParams, strValue;
  UINT16 numParams;
  double dataValue;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histograms[ histograms.size() - 1 ] == NULL )
    return false;

  getline( line, strNumParams, ' ' ); // Number of following parameters.
  istringstream tmpNumParams( strNumParams );

  if ( !( tmpNumParams >> numParams ) )
    return false;

  for ( UINT16 ii = 0; ii < numParams; ii++ )
  {
    getline( line, strValue, ' ' );
    istringstream tmpValue( strValue );
    if ( !( tmpValue >> dataValue ) )
      return false;
    if ( ii == 0 )
    {
      histograms[ histograms.size() - 1 ]->setDataMin( dataValue );
      //histograms[ histograms.size() - 1 ]->setCommSizeMin( ( TCommSize ) dataValue );
    }
    else if ( ii == 1 )
    {
      histograms[ histograms.size() - 1 ]->setDataMax( dataValue );
      //histograms[ histograms.size() - 1 ]->setCommSizeMax( ( TCommSize ) dataValue );
    }
    else if ( ii == 2 )
    {
      histograms[ histograms.size() - 1 ]->setBurstMin( dataValue );
      //histograms[ histograms.size() - 1 ]->setCommTagMin( ( TCommTag ) dataValue );
    }
    else if ( ii == 3 )
    {
      histograms[ histograms.size() - 1 ]->setBurstMax( dataValue );
      //histograms[ histograms.size() - 1 ]->setCommTagMax( ( TCommTag ) dataValue );
    }
  }

  return true;
}

void Analyzer2DParameters::printLine( ofstream& cfgFile,
                                      const vector<Histogram *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_AN2D_PARAMETERS << " 4 ";
  cfgFile << ( *it )->getDataMin() << " ";
  cfgFile << ( *it )->getDataMax() << " ";
  cfgFile << ( *it )->getBurstMin() << " ";
  cfgFile << ( *it )->getBurstMax();
  cfgFile << endl;
}

bool Analyzer2DAnalysisLimits::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<Window *>& windows,
    vector<Histogram *>& histograms )
{
  string strLimit;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histograms[ histograms.size() - 1 ] == NULL )
    return false;

  getline( line, strLimit, ' ' );

  if ( strLimit.compare( OLDCFG_VAL_LIMIT_ALLTRACE ) == 0 )
  {
    histograms[ histograms.size() - 1 ]->setWindowBeginTime( 0.0 );
    histograms[ histograms.size() - 1 ]->setWindowEndTime( whichTrace->getEndTime() );
  }
  else if ( strLimit.compare( OLDCFG_VAL_LIMIT_ALLWINDOW ) == 0 )
  {
    histograms[ histograms.size() - 1 ]->setWindowBeginTime( histograms[ histograms.size() - 1 ]->getControlWindow()->getWindowBeginTime() );
    histograms[ histograms.size() - 1 ]->setWindowEndTime( histograms[ histograms.size() - 1 ]->getControlWindow()->getWindowEndTime() );
  }
  else if ( strLimit.compare( OLDCFG_VAL_LIMIT_REGION ) == 0 )
  {
    // Not implemented yet
    histograms[ histograms.size() - 1 ]->setWindowBeginTime( 0.0 );
    histograms[ histograms.size() - 1 ]->setWindowEndTime( whichTrace->getEndTime() );
  }
  else
    return false;

  return true;
}

void Analyzer2DAnalysisLimits::printLine( ofstream& cfgFile,
    const SaveOptions& options,
    const vector<Histogram *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_AN2D_ANALYSISLIMITS << " ";
  if ( options.histoAllTrace )
    cfgFile << OLDCFG_VAL_LIMIT_ALLTRACE;
  else
    cfgFile << OLDCFG_VAL_LIMIT_ALLWINDOW;
  cfgFile << endl;
}

bool Analyzer2DRelativeTime::parseLine( KernelConnection *whichKernel, istringstream& line,
                                        Trace *whichTrace,
                                        vector<Window *>& windows,
                                        vector<Histogram *>& histograms )
{
  string strBool;
#ifndef WIN32
#warning Analyzer2DRelativeTime::parseLine
#endif
  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histograms[ histograms.size() - 1 ] == NULL )
    return false;

  getline( line, strBool, ' ' );

  if ( strBool.compare( OLDCFG_VAL_FALSE2 ) == 0 )
    {}
  else if ( strBool.compare( OLDCFG_VAL_TRUE2 ) == 0 )
    {}
  else
    return false;

  return true;
}

void Analyzer2DRelativeTime::printLine( ofstream& cfgFile,
                                        const vector<Histogram *>::const_iterator it )
{
#ifndef WIN32
#warning Analyzer2DRelativeTime::printLine
#endif
}

bool Analyzer2DComputeYScale::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<Window *>& windows,
    vector<Histogram *>& histograms )
{
  string strBool;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histograms[ histograms.size() - 1 ] == NULL )
    return false;

  getline( line, strBool, ' ' );

  if ( strBool.compare( OLDCFG_VAL_FALSE2 ) == 0 )
    histograms[ histograms.size() - 1 ]->setComputeScale( false );
  else if ( strBool.compare( OLDCFG_VAL_TRUE2 ) == 0 )
    histograms[ histograms.size() - 1 ]->setComputeScale( true );
  else
    return false;

  return true;
}

void Analyzer2DComputeYScale::printLine( ofstream& cfgFile,
    const SaveOptions& options,
    const vector<Histogram *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_AN2D_COMPUTEYSCALE << " ";
  if ( options.histoComputeYScale )
    cfgFile << OLDCFG_VAL_TRUE2;
  else
    cfgFile << OLDCFG_VAL_FALSE2;
  cfgFile << endl;
}

bool Analyzer2DMinimum::parseLine( KernelConnection *whichKernel, istringstream& line,
                                   Trace *whichTrace,
                                   vector<Window *>& windows,
                                   vector<Histogram *>& histograms )
{
  string strMinimum;
  THistogramLimit dataMinimum;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histograms[ histograms.size() - 1 ] == NULL )
    return false;

  getline( line, strMinimum );
  istringstream tmpValue( strMinimum );
  if ( !( tmpValue >> dataMinimum ) )
    return false;
  histograms[ histograms.size() - 1 ]->setControlMin( dataMinimum );

  return true;
}

void Analyzer2DMinimum::printLine( ofstream& cfgFile,
                                   const vector<Histogram *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_AN2D_MINIMUM << " " << ( *it )->getControlMin() << endl;
}

bool Analyzer2DMaximum::parseLine( KernelConnection *whichKernel, istringstream& line,
                                   Trace *whichTrace,
                                   vector<Window *>& windows,
                                   vector<Histogram *>& histograms )
{
  string strMaximum;
  THistogramLimit dataMaximum;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histograms[ histograms.size() - 1 ] == NULL )
    return false;

  getline( line, strMaximum );
  istringstream tmpValue( strMaximum );
  if ( !( tmpValue >> dataMaximum ) )
    return false;
  histograms[ histograms.size() - 1 ]->setControlMax( dataMaximum );

  return true;
}

void Analyzer2DMaximum::printLine( ofstream& cfgFile,
                                   const vector<Histogram *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_AN2D_MAXIMUM << " " << ( *it )->getControlMax() << endl;
}

bool Analyzer2DDelta::parseLine( KernelConnection *whichKernel, istringstream& line,
                                 Trace *whichTrace,
                                 vector<Window *>& windows,
                                 vector<Histogram *>& histograms )
{
  string strDelta;
  THistogramLimit dataDelta;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histograms[ histograms.size() - 1 ] == NULL )
    return false;

  getline( line, strDelta );
  istringstream tmpValue( strDelta );
  if ( !( tmpValue >> dataDelta ) )
    return false;
  histograms[ histograms.size() - 1 ]->setControlDelta( dataDelta );

  return true;
}

void Analyzer2DDelta::printLine( ofstream& cfgFile,
                                 const vector<Histogram *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_AN2D_DELTA << " " << ( *it )->getControlDelta() << endl;
}

bool Analyzer2DComputeGradient::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<Window *>& windows,
    vector<Histogram *>& histograms )
{
  string strBool;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histograms[ histograms.size() - 1 ] == NULL )
    return false;

  getline( line, strBool, ' ' );

  if ( strBool.compare( OLDCFG_VAL_FALSE2 ) == 0 )
    histograms[ histograms.size() - 1 ]->setComputeGradient( false );
  else if ( strBool.compare( OLDCFG_VAL_TRUE2 ) == 0 )
    histograms[ histograms.size() - 1 ]->setComputeGradient( true );
  else
    return false;

  return true;
}

void Analyzer2DComputeGradient::printLine( ofstream& cfgFile,
    const SaveOptions& options,
    const vector<Histogram *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_AN2D_COMPUTEGRADIENT << " ";
  if ( options.histoComputeGradient )
    cfgFile << OLDCFG_VAL_TRUE2;
  else
    cfgFile << OLDCFG_VAL_FALSE2;
  cfgFile << endl;
}

bool Analyzer2DMinimumGradient::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<Window *>& windows,
    vector<Histogram *>& histograms )
{
  string strMinimumGradient;
  THistogramLimit dataMinimumGradient;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histograms[ histograms.size() - 1 ] == NULL )
    return false;

  getline( line, strMinimumGradient );
  istringstream tmpValue( strMinimumGradient );
  if ( !( tmpValue >> dataMinimumGradient ) )
    return false;
  histograms[ histograms.size() - 1 ]->setMinGradient( dataMinimumGradient );

  return true;
}

void Analyzer2DMinimumGradient::printLine( ofstream& cfgFile,
    const vector<Histogram *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_AN2D_MINIMUMGRADIENT << " " << ( *it )->getMinGradient() << endl;
}

bool Analyzer2DMaximumGradient::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<Window *>& windows,
    vector<Histogram *>& histograms )
{
  string strMaximumGradient;
  THistogramLimit dataMaximumGradient;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histograms[ histograms.size() - 1 ] == NULL )
    return false;

  getline( line, strMaximumGradient );
  istringstream tmpValue( strMaximumGradient );
  if ( !( tmpValue >> dataMaximumGradient ) )
    return false;
  histograms[ histograms.size() - 1 ]->setMaxGradient( dataMaximumGradient );

  return true;
}

void Analyzer2DMaximumGradient::printLine( ofstream& cfgFile,
    const vector<Histogram *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_AN2D_MAXIMUMGRADIENT << " " << ( *it )->getMaxGradient() << endl;
}

bool Analyzer3DControlWindow::parseLine( KernelConnection *whichKernel, istringstream& line,
    Trace *whichTrace,
    vector<Window *>& windows,
    vector<Histogram *>& histograms )
{
  string str3DControlWindow;
  UINT32 controlWindow;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histograms[ histograms.size() - 1 ] == NULL )
    return false;

  getline( line, str3DControlWindow );
  istringstream tmpValue( str3DControlWindow );
  if ( !( tmpValue >> controlWindow ) )
    return false;

  if ( controlWindow > windows.size() ||
       windows[ controlWindow - 1 ] == NULL )
    return false;

  histograms[ histograms.size() - 1 ]->setExtraControlWindow( windows[ controlWindow - 1 ] );

  return true;
}

void Analyzer3DControlWindow::printLine( ofstream& cfgFile,
    const vector<Window *>& allWindows,
    const vector<Histogram *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_AN3D_CONTROLWINDOW << " ";
  cfgFile << CFGLoader::findWindow( ( *it )->getExtraControlWindow(), allWindows ) + 1;
  cfgFile << endl;
}

bool Analyzer3DMinimum::parseLine( KernelConnection *whichKernel, istringstream& line,
                                   Trace *whichTrace,
                                   vector<Window *>& windows,
                                   vector<Histogram *>& histograms )
{
  string str3DMinimum;
  THistogramLimit data3DMinimum;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histograms[ histograms.size() - 1 ] == NULL )
    return false;

  getline( line, str3DMinimum );
  istringstream tmpValue( str3DMinimum );
  if ( !( tmpValue >> data3DMinimum ) )
    return false;
  histograms[ histograms.size() - 1 ]->setExtraControlMin( data3DMinimum );

  return true;
}

void Analyzer3DMinimum::printLine( ofstream& cfgFile,
                                   const vector<Histogram *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_AN3D_MINIMUM << " " << ( *it )->getExtraControlMin() << endl;
}

bool Analyzer3DMaximum::parseLine( KernelConnection *whichKernel, istringstream& line,
                                   Trace *whichTrace,
                                   vector<Window *>& windows,
                                   vector<Histogram *>& histograms )
{
  string str3DMaximum;
  THistogramLimit data3DMaximum;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histograms[ histograms.size() - 1 ] == NULL )
    return false;

  getline( line, str3DMaximum );
  istringstream tmpValue( str3DMaximum );
  if ( !( tmpValue >> data3DMaximum ) )
    return false;
  histograms[ histograms.size() - 1 ]->setExtraControlMax( data3DMaximum );

  return true;
}

void Analyzer3DMaximum::printLine( ofstream& cfgFile,
                                   const vector<Histogram *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_AN3D_MAXIMUM << " " << ( *it )->getExtraControlMax() << endl;
}

bool Analyzer3DDelta::parseLine( KernelConnection *whichKernel, istringstream& line,
                                 Trace *whichTrace,
                                 vector<Window *>& windows,
                                 vector<Histogram *>& histograms )
{
  string str3DDelta;
  THistogramLimit data3DDelta;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histograms[ histograms.size() - 1 ] == NULL )
    return false;

  getline( line, str3DDelta );
  istringstream tmpValue( str3DDelta );
  if ( !( tmpValue >> data3DDelta ) )
    return false;
  histograms[ histograms.size() - 1 ]->setExtraControlDelta( data3DDelta );

  return true;
}

void Analyzer3DDelta::printLine( ofstream& cfgFile,
                                 const vector<Histogram *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_AN3D_DELTA << " " << ( *it )->getExtraControlDelta() << endl;
}

bool Analyzer3DFixedValue::parseLine( KernelConnection *whichKernel, istringstream& line,
                                      Trace *whichTrace,
                                      vector<Window *>& windows,
                                      vector<Histogram *>& histograms )
{
  string str3DFixedValue;
  double data3DFixedValue;

  if ( windows[ windows.size() - 1 ] == NULL )
    return false;
  if ( histograms[ histograms.size() - 1 ] == NULL )
    return false;

  getline( line, str3DFixedValue );
  istringstream tmpValue( str3DFixedValue );
  if ( !( tmpValue >> data3DFixedValue ) )
    return false;
  histograms[ histograms.size() - 1 ]->setPlaneMinValue( data3DFixedValue );

  return true;
}

void Analyzer3DFixedValue::printLine( ofstream& cfgFile,
                                      const vector<Histogram *>::const_iterator it )
{
  cfgFile << OLDCFG_TAG_AN3D_FIXEDVALUE << " " << ( *it )->getPlaneMinValue();
}

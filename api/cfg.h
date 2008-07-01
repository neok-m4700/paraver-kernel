#ifndef CFG_H_INCLUDED
#define CFG_H_INCLUDED

#include <map>
#include <vector>
#include <sstream>
#include "paraverkerneltypes.h"

class KernelConnection;
class Window;
class Trace;
class Histogram;

using namespace std;

TWindowLevel stringToLevel( const string& strLevel );

class TagFunction
{
  public:
    TagFunction()
    {}
    virtual ~TagFunction()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms ) = 0;
};

class CFGLoader
{
  private:
    static map<string, TagFunction *> cfgTagFunctions;
    static void loadMap();
    static void unLoadMap();

  public:
    static bool isCFGFile( const string& filename );
    static bool loadCFG( KernelConnection *whichKernel, const string& filename,
                         Trace *whichTrace,
                         vector<Window *>& windows,
                         vector<Histogram *>& histograms );
};


class WindowType: public TagFunction
{
  public:
    WindowType()
    {}
    virtual ~WindowType()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class WindowFactors: public TagFunction
{
  public:
    WindowFactors()
    {}
    virtual ~WindowFactors()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class WindowUnits: public TagFunction
{
  public:
    WindowUnits()
    {}
    virtual ~WindowUnits()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class WindowOperation: public TagFunction
{
  public:
    WindowOperation()
    {}
    virtual ~WindowOperation()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class WindowMaximumY: public TagFunction
{
  public:
    WindowMaximumY()
    {}
    virtual ~WindowMaximumY()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class WindowMinimumY: public TagFunction
{
  public:
    WindowMinimumY()
    {}
    virtual ~WindowMinimumY()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class WindowComputeYMax: public TagFunction
{
  public:
    WindowComputeYMax()
    {}
    virtual ~WindowComputeYMax()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class WindowLevel: public TagFunction
{
  public:
    WindowLevel()
    {}
    virtual ~WindowLevel()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class WindowScaleRelative: public TagFunction
{
  public:
    WindowScaleRelative()
    {}
    virtual ~WindowScaleRelative()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class WindowObject: public TagFunction
{
  public:
    WindowObject()
    {}
    virtual ~WindowObject()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class WindowBeginTime: public TagFunction
{
  public:
    WindowBeginTime()
    {}
    virtual ~WindowBeginTime()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class WindowEndTime: public TagFunction
{
  public:
    WindowEndTime()
    {}
    virtual ~WindowEndTime()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class WindowStopTime: public TagFunction
{
  public:
    WindowStopTime()
    {}
    virtual ~WindowStopTime()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class WindowBeginTimeRelative: public TagFunction
{
  public:
    WindowBeginTimeRelative()
    {}
    virtual ~WindowBeginTimeRelative()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class WindowNumberOfRow: public TagFunction
{
  public:
    WindowNumberOfRow()
    {}
    virtual ~WindowNumberOfRow()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};

class WindowSelectedFunctions: public TagFunction
{
  public:
    WindowSelectedFunctions()
    {}
    virtual ~WindowSelectedFunctions()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class WindowSemanticModule: public TagFunction
{
  public:
    WindowSemanticModule()
    {}
    virtual ~WindowSemanticModule()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class WindowComposeFunctions: public TagFunction
{
  public:
    WindowComposeFunctions()
    {}
    virtual ~WindowComposeFunctions()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class WindowFilterModule: public TagFunction
{
  public:
    WindowFilterModule()
    {}
    virtual ~WindowFilterModule()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class WindowFilterLogical: public TagFunction
{
  public:
    WindowFilterLogical()
    {}
    virtual ~WindowFilterLogical()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class WindowFilterPhysical: public TagFunction
{
  public:
    WindowFilterPhysical()
    {}
    virtual ~WindowFilterPhysical()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class WindowFilterBoolOpFromTo: public TagFunction
{
  public:
    WindowFilterBoolOpFromTo()
    {}
    virtual ~WindowFilterBoolOpFromTo()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class WindowFilterBoolOpTagSize: public TagFunction
{
  public:
    WindowFilterBoolOpTagSize()
    {}
    virtual ~WindowFilterBoolOpTagSize()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class WindowFilterBoolOpTypeVal: public TagFunction
{
  public:
    WindowFilterBoolOpTypeVal()
    {}
    virtual ~WindowFilterBoolOpTypeVal()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class WindowIdentifiers: public TagFunction
{
  public:
    WindowIdentifiers()
    {}
    virtual ~WindowIdentifiers()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class Analyzer2DCreate: public TagFunction
{
  public:
    Analyzer2DCreate()
    {}
    virtual ~Analyzer2DCreate()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class Analyzer2DControlWindow: public TagFunction
{
  public:
    Analyzer2DControlWindow()
    {}
    virtual ~Analyzer2DControlWindow()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class Analyzer2DDataWindow: public TagFunction
{
  public:
    Analyzer2DDataWindow()
    {}
    virtual ~Analyzer2DDataWindow()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class Analyzer2DStatistic: public TagFunction
{
  public:
    Analyzer2DStatistic()
    {}
    virtual ~Analyzer2DStatistic()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class Analyzer2DCalculateAll: public TagFunction
{
  public:
    Analyzer2DCalculateAll()
    {}
    virtual ~Analyzer2DCalculateAll()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class Analyzer2DNumColumns: public TagFunction
{
  public:
    Analyzer2DNumColumns()
    {}
    virtual ~Analyzer2DNumColumns()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class Analyzer2DHideColumns: public TagFunction
{
  public:
    Analyzer2DHideColumns()
    {}
    virtual ~Analyzer2DHideColumns()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class Analyzer2DScientificNotation: public TagFunction
{
  public:
    Analyzer2DScientificNotation()
    {}
    virtual ~Analyzer2DScientificNotation()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class Analyzer2DNumDecimals: public TagFunction
{
  public:
    Analyzer2DNumDecimals()
    {}
    virtual ~Analyzer2DNumDecimals()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class Analyzer2DThousandSeparator: public TagFunction
{
  public:
    Analyzer2DThousandSeparator()
    {}
    virtual ~Analyzer2DThousandSeparator()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class Analyzer2DUnits: public TagFunction
{
  public:
    Analyzer2DUnits()
    {}
    virtual ~Analyzer2DUnits()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class Analyzer2DAccumulator: public TagFunction
{
  public:
    Analyzer2DAccumulator()
    {}
    virtual ~Analyzer2DAccumulator()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class Analyzer2DHorizontal: public TagFunction
{
  public:
    Analyzer2DHorizontal()
    {}
    virtual ~Analyzer2DHorizontal()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class Analyzer2DAccumulateByControlWindow: public TagFunction
{
  public:
    Analyzer2DAccumulateByControlWindow()
    {}
    virtual ~Analyzer2DAccumulateByControlWindow()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class Analyzer2DSortCols: public TagFunction
{
  public:
    Analyzer2DSortCols()
    {}
    virtual ~Analyzer2DSortCols()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class Analyzer2DSortCriteria: public TagFunction
{
  public:
    Analyzer2DSortCriteria()
    {}
    virtual ~Analyzer2DSortCriteria()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class Analyzer2DParameters: public TagFunction
{
  public:
    Analyzer2DParameters()
    {}
    virtual ~Analyzer2DParameters()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class Analyzer2DAnalysisLimits: public TagFunction
{
  public:
    Analyzer2DAnalysisLimits()
    {}
    virtual ~Analyzer2DAnalysisLimits()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class Analyzer2DRelativeTime: public TagFunction
{
  public:
    Analyzer2DRelativeTime()
    {}
    virtual ~Analyzer2DRelativeTime()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class Analyzer2DComputeYScale: public TagFunction
{
  public:
    Analyzer2DComputeYScale()
    {}
    virtual ~Analyzer2DComputeYScale()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class Analyzer2DMinimum: public TagFunction
{
  public:
    Analyzer2DMinimum()
    {}
    virtual ~Analyzer2DMinimum()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class Analyzer2DMaximum: public TagFunction
{
  public:
    Analyzer2DMaximum()
    {}
    virtual ~Analyzer2DMaximum()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class Analyzer2DDelta: public TagFunction
{
  public:
    Analyzer2DDelta()
    {}
    virtual ~Analyzer2DDelta()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class Analyzer2DComputeGradient: public TagFunction
{
  public:
    Analyzer2DComputeGradient()
    {}
    virtual ~Analyzer2DComputeGradient()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};

class Analyzer2DMinimumGradient: public TagFunction
{
  public:
    Analyzer2DMinimumGradient()
    {}
    virtual ~Analyzer2DMinimumGradient()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class Analyzer2DMaximumGradient: public TagFunction
{
  public:
    Analyzer2DMaximumGradient()
    {}
    virtual ~Analyzer2DMaximumGradient()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class Analyzer3DControlWindow: public TagFunction
{
  public:
    Analyzer3DControlWindow()
    {}
    virtual ~Analyzer3DControlWindow()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class Analyzer3DMinimum: public TagFunction
{
  public:
    Analyzer3DMinimum()
    {}
    virtual ~Analyzer3DMinimum()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class Analyzer3DMaximum: public TagFunction
{
  public:
    Analyzer3DMaximum()
    {}
    virtual ~Analyzer3DMaximum()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class Analyzer3DDelta: public TagFunction
{
  public:
    Analyzer3DDelta()
    {}
    virtual ~Analyzer3DDelta()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


class Analyzer3DFixedValue: public TagFunction
{
  public:
    Analyzer3DFixedValue()
    {}
    virtual ~Analyzer3DFixedValue()
    {}
    virtual bool parseLine( KernelConnection *whichKernel, istringstream& line,
                            Trace *whichTrace,
                            vector<Window *>& windows,
                            vector<Histogram *>& histograms );
};


#endif // CFG_H_INCLUDED

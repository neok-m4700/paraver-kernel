#include "kernelconnection.h"
#include "histogram.h"
#include "window.h"

Histogram *Histogram::create( KernelConnection *whichKernel )
{
  return new HistogramProxy();
}


Histogram::Histogram( KernelConnection *whichKernel ) : myKernel( whichKernel )
{}

HistogramProxy::HistogramProxy( KernelConnection *whichKernel ):
    Histogram( whichKernel )
{
  myHisto = myKernel->newKHistogram();
}

HistogramProxy::~HistogramProxy()
{
  delete myHisto;
}

bool HistogramProxy::getThreeDimensions() const
{
  return myHisto->getThreeDimensions();
}

TRecordTime HistogramProxy::getBeginTime() const
{
  return myHisto->getBeginTime();
}

TRecordTime HistogramProxy::getEndTime() const
{
  return myHisto->getEndTime();
}

Window *HistogramProxy::getControlWindow() const
{
  return myHisto->getControlWindow();
}

Window *HistogramProxy::getDataWindow() const
{
  return myHisto->getDataWindow();
}

Window *HistogramProxy::getExtraControlWindow() const
{
  return myHisto->getExtraControlWindow();
}

void HistogramProxy::setControlWindow( Window *whichWindow )
{
  myHisto->setControlWindow( whichWindow );
}

void HistogramProxy::setDataWindow( Window *whichWindow )
{
  myHisto->setDataWindow( whichWindow );
}

void HistogramProxy::setExtraControlWindow( Window *whichWindow )
{
  myHisto->setExtraControlWindow( whichWindow );
}

void HistogramProxy::clearControlWindow()
{
  myHisto->clearControlWindow();
}

void HistogramProxy::clearDataWindow()
{
  myHisto->clearDataWindow();
}

void HistogramProxy::clearExtraControlWindow()
{
  myHisto->clearExtraControlWindow();
}

void HistogramProxy::setControlMin( THistogramLimit whichMin )
{
  myHisto->setControlMin( whichMin );
}

void HistogramProxy::setControlMax( THistogramLimit whichMax )
{
  myHisto->setControlMax( whichMax );
}

void HistogramProxy::setControlDelta( THistogramLimit whichDelta )
{
  myHisto->setControlDelta( whichDelta );
}

void HistogramProxy::setExtraControlMin( THistogramLimit whichMin )
{
  myHisto->setExtraControlMin( whichMin );
}

void HistogramProxy::setExtraControlMax( THistogramLimit whichMax )
{
  myHisto->setExtraControlMax( whichMax );
}

void HistogramProxy::setExtraControlDelta( THistogramLimit whichDelta )
{
  myHisto->setExtraControlDelta( whichDelta );
}

void HistogramProxy::setDataMin( THistogramLimit whichMin )
{
  myHisto->setDataMin( whichMin );
}

void HistogramProxy::setDataMax( THistogramLimit whichMax )
{
  myHisto->setDataMax( whichMax );
}

THistogramLimit HistogramProxy::getControlMin() const
{
  return myHisto->getControlMin();
}

THistogramLimit HistogramProxy::getControlMax() const
{
  return myHisto->getControlMax();
}

THistogramLimit HistogramProxy::getControlDelta() const
{
  return myHisto->getControlDelta();
}

THistogramLimit HistogramProxy::getExtraControlMin() const
{
  return myHisto->getExtraControlMin();
}

THistogramLimit HistogramProxy::getExtraControlMax() const
{
  return myHisto->getExtraControlMax();
}

THistogramLimit HistogramProxy::getExtraControlDelta() const
{
  return myHisto->getExtraControlDelta();
}

THistogramLimit HistogramProxy::getDataMin() const
{
  return myHisto->getDataMin();
}

THistogramLimit HistogramProxy::getDataMax() const
{
  return myHisto->getDataMax();
}

void HistogramProxy::setInclusive( bool newValue )
{
  myHisto->setInclusive( newValue );
}

THistogramColumn HistogramProxy::getNumPlanes() const
{
  return myHisto->getNumPlanes();
}

THistogramColumn HistogramProxy::getNumColumns() const
{
  return myHisto->getNumColumns();
}

TObjectOrder HistogramProxy::getNumRows() const
{
  return myHisto->getNumRows();
}


TSemanticValue HistogramProxy::getCurrentValue( UINT32 col,
    UINT16 idStat,
    UINT32 plane ) const
{
  return myHisto->getCurrentValue( col, idStat, plane );
}

UINT32 HistogramProxy::getCurrentRow( UINT32 col, UINT32 plane ) const
{
  return myHisto->getCurrentRow( col, plane );
}

void HistogramProxy::setNextCell( UINT32 col, UINT32 plane )
{
  myHisto->setNextCell( col, plane );
}

void HistogramProxy::setFirstCell( UINT32 col, UINT32 plane )
{
  myHisto->setFirstCell( col, plane );
}

bool HistogramProxy::endCell( UINT32 col, UINT32 plane )
{
  return myHisto->endCell( col, plane );
}

bool HistogramProxy::planeWithValues( UINT32 plane ) const
{
  return myHisto->planeWithValues( plane );
}

TSemanticValue HistogramProxy::getCommCurrentValue( UINT32 col,
    UINT16 idStat,
    UINT32 plane ) const
{
  return myHisto->getCommCurrentValue( col, idStat, plane );
}

UINT32 HistogramProxy::getCommCurrentRow( UINT32 col, UINT32 plane ) const
{
  return myHisto->getCommCurrentRow( col, plane );
}

void HistogramProxy::setCommNextCell( UINT32 col, UINT32 plane )
{
  myHisto->setCommNextCell( col, plane );
}

void HistogramProxy::setCommFirstCell( UINT32 col, UINT32 plane )
{
  myHisto->setCommFirstCell( col, plane );
}

bool HistogramProxy::endCommCell( UINT32 col, UINT32 plane )
{
  return myHisto->endCommCell( col, plane );
}

bool HistogramProxy::planeCommWithValues( UINT32 plane ) const
{
  return myHisto->planeCommWithValues( plane );
}

KHistogramTotals *HistogramProxy::getColumnTotals() const
{
  return myHisto->getColumnTotals();
}

KHistogramTotals *HistogramProxy::getCommColumnTotals() const
{
  return myHisto->getCommColumnTotals();
}

KHistogramTotals *HistogramProxy::getRowTotals() const
{
  return myHisto->getRowTotals();
}

KHistogramTotals *HistogramProxy::getCommRowTotals() const
{
  return myHisto->getCommRowTotals();
}

void HistogramProxy::clearStatistics()
{
  myHisto->clearStatistics();
}

void HistogramProxy::pushbackStatistic( HistogramStatistic *whichStatistic )
{
  myHisto->pushbackStatistic( whichStatistic );
}

void HistogramProxy::execute( TRecordTime whichBeginTime, TRecordTime whichEndTime )
{
  myHisto->execute( whichBeginTime, whichEndTime );
}

void HistogramProxy::setHorizontal( bool horiz )
{
  horizontal = horiz;
}

bool HistogramProxy::getHorizontal() const
{
  return horizontal;
}

void HistogramProxy::setHideColumns( bool hide )
{
  hideColumns = hide;
}

bool HistogramProxy::getHideColumns() const
{
  return hideColumns;
}



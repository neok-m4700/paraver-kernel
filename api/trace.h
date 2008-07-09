#ifndef TRACE_H_INCLUDED
#define TRACE_H_INCLUDED

#include "paraverkerneltypes.h"

class KernelConnection;

class Trace
{
  public:
    static Trace *create( KernelConnection *whichKernel, const string& whichFile );

    Trace() {}
    Trace( KernelConnection *whichKernel );
    virtual ~Trace() {}

    virtual string getFileName() const = 0;
    virtual void dumpFile( const string& whichFile ) const = 0;

    virtual TApplOrder totalApplications() const = 0;
    virtual TTaskOrder totalTasks() const = 0;
    virtual TTaskOrder getGlobalTask( const TApplOrder& inAppl,
                                      const TTaskOrder& inTask ) const = 0;
    virtual void getTaskLocation( TTaskOrder globalTask,
                                  TApplOrder& inAppl,
                                  TTaskOrder& inTask ) const = 0;
    virtual TTaskOrder getFirstTask( TApplOrder inAppl ) const = 0;
    virtual TTaskOrder getLastTask( TApplOrder inAppl ) const = 0;

    virtual TThreadOrder totalThreads() const = 0;
    virtual TThreadOrder getGlobalThread( const TApplOrder& inAppl,
                                          const TTaskOrder& inTask,
                                          const TThreadOrder& inThread ) const = 0;
    virtual void getThreadLocation( TThreadOrder globalThread,
                                    TApplOrder& inAppl,
                                    TTaskOrder& inTask,
                                    TThreadOrder& inThread ) const = 0;
    virtual TThreadOrder getFirstThread( TApplOrder inAppl, TTaskOrder inTask ) const = 0;
    virtual TThreadOrder getLastThread( TApplOrder inAppl, TTaskOrder inTask )const = 0;

    virtual bool existResourceInfo() const = 0;
    virtual TNodeOrder totalNodes() const = 0;
    virtual TCPUOrder totalCPUs() const = 0;
    virtual TCPUOrder getGlobalCPU( const TNodeOrder& inNode,
                                    const TCPUOrder& inCPU ) const = 0;
    virtual void getCPULocation( TCPUOrder globalCPU,
                                 TNodeOrder& inNode,
                                 TCPUOrder& inCPU ) const = 0;
    virtual TCPUOrder getFirstCPU( TNodeOrder inNode ) const = 0;
    virtual TCPUOrder getLastCPU( TNodeOrder inNode ) const = 0;

    virtual TObjectOrder getFirst( TObjectOrder globalOrder,
                                   TWindowLevel fromLevel,
                                   TWindowLevel toLevel ) const = 0;
    virtual TObjectOrder getLast( TObjectOrder globalOrder,
                                  TWindowLevel fromLevel,
                                  TWindowLevel toLevel ) const = 0;

    // Communication info getters
    virtual TThreadOrder getSenderThread( TCommID whichComm ) const = 0;
    virtual TCPUOrder getSenderCPU( TCommID whichComm ) const = 0;
    virtual TThreadOrder getReceiverThread( TCommID whichComm ) const = 0;
    virtual TCPUOrder getReceiverCPU( TCommID whichComm ) const = 0;
    virtual TCommTag getCommTag( TCommID whichComm ) const = 0;
    virtual TCommSize getCommSize( TCommID whichComm ) const = 0;
    virtual TRecordTime getLogicalSend( TCommID whichComm ) const = 0;
    virtual TRecordTime getLogicalReceive( TCommID whichComm ) const = 0;
    virtual TRecordTime getPhysicalSend( TCommID whichComm ) const = 0;
    virtual TRecordTime getPhysicalReceive( TCommID whichComm ) const = 0;

    virtual TTime getEndTime() const = 0;
    virtual TTimeUnit getTimeUnit() const = 0;

    virtual Trace *getConcrete() const
    {
      return NULL;
    }

  protected:
    KernelConnection *myKernel;

};

class TraceProxy: public Trace
{
  public:
    virtual ~TraceProxy();

    virtual string getFileName() const;

    virtual void dumpFile( const string& whichFile ) const;

    virtual TApplOrder totalApplications() const;
    virtual TTaskOrder totalTasks() const;
    virtual TTaskOrder getGlobalTask( const TApplOrder& inAppl,
                                      const TTaskOrder& inTask ) const;
    virtual void getTaskLocation( TTaskOrder globalTask,
                                  TApplOrder& inAppl,
                                  TTaskOrder& inTask ) const;
    virtual TTaskOrder getFirstTask( TApplOrder inAppl ) const;
    virtual TTaskOrder getLastTask( TApplOrder inAppl ) const;

    virtual TThreadOrder totalThreads() const;
    virtual TThreadOrder getGlobalThread( const TApplOrder& inAppl,
                                          const TTaskOrder& inTask,
                                          const TThreadOrder& inThread ) const;
    virtual void getThreadLocation( TThreadOrder globalThread,
                                    TApplOrder& inAppl,
                                    TTaskOrder& inTask,
                                    TThreadOrder& inThread ) const;
    virtual TThreadOrder getFirstThread( TApplOrder inAppl, TTaskOrder inTask ) const;
    virtual TThreadOrder getLastThread( TApplOrder inAppl, TTaskOrder inTask )const;

    virtual bool existResourceInfo() const;
    virtual TNodeOrder totalNodes() const;
    virtual TCPUOrder totalCPUs() const;
    virtual TCPUOrder getGlobalCPU( const TNodeOrder& inNode,
                                    const TCPUOrder& inCPU ) const;
    virtual void getCPULocation( TCPUOrder globalCPU,
                                 TNodeOrder& inNode,
                                 TCPUOrder& inCPU ) const;
    virtual TCPUOrder getFirstCPU( TNodeOrder inNode ) const;
    virtual TCPUOrder getLastCPU( TNodeOrder inNode ) const;

    virtual TObjectOrder getFirst( TObjectOrder globalOrder,
                                   TWindowLevel fromLevel,
                                   TWindowLevel toLevel ) const;
    virtual TObjectOrder getLast( TObjectOrder globalOrder,
                                  TWindowLevel fromLevel,
                                  TWindowLevel toLevel ) const;

    // Communication info getters
    virtual TThreadOrder getSenderThread( TCommID whichComm ) const;
    virtual TCPUOrder getSenderCPU( TCommID whichComm ) const;
    virtual TThreadOrder getReceiverThread( TCommID whichComm ) const;
    virtual TCPUOrder getReceiverCPU( TCommID whichComm ) const;
    virtual TCommTag getCommTag( TCommID whichComm ) const;
    virtual TCommSize getCommSize( TCommID whichComm ) const;
    virtual TRecordTime getLogicalSend( TCommID whichComm ) const;
    virtual TRecordTime getLogicalReceive( TCommID whichComm ) const;
    virtual TRecordTime getPhysicalSend( TCommID whichComm ) const;
    virtual TRecordTime getPhysicalReceive( TCommID whichComm ) const;

    virtual TTime getEndTime() const;
    virtual TTimeUnit getTimeUnit() const;

    virtual Trace *getConcrete() const;

  private:
    Trace *myTrace;

    TraceProxy( KernelConnection *whichKernel, const string& whichFile );

    friend Trace *Trace::create( KernelConnection *, const string& );
};

#endif // TRACE_H_INCLUDED
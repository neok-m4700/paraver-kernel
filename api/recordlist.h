#ifndef RECORDLIST_H_INCLUDED
#define RECORDLIST_H_INCLUDED

#include <set>
#include "paraverkerneltypes.h"

class KernelConnection;

using std::set;

struct RLEvent
{
  TEventType type;
  TEventValue value;
};

struct RLComm
{
  TCommID id;
};

struct RLRecord
{
public:
  TRecordType getType() const
  {
    return type;
  }
  TRecordTime getTime() const
  {
    return time;
  }
  TObjectOrder getOrder() const
  {
    return order;
  }
  TEventType getEventType() const
  {
    return UInfo.event.type;
  }
  TEventValue getEventValue() const
  {
    return UInfo.event.value;
  }
  TCommID getCommIndex() const
  {
    return UInfo.comm.id;
  }
  void setType( TRecordType whichType )
  {
    type = whichType;
  }
  void setTime( TRecordTime whichTime )
  {
    time = whichTime;
  }
  void setOrder( TObjectOrder whichOrder )
  {
    order = whichOrder;
  }
  void setEventType( TEventType whichType )
  {
    UInfo.event.type = whichType;
  }
  void setEventValue( TEventValue whichValue )
  {
    UInfo.event.value = whichValue;
  }
  void setCommIndex( TCommID whichID )
  {
    UInfo.comm.id = whichID;
  }
private:
  TRecordType type;
  TRecordTime time;
  TObjectOrder order;
  union
  {
    RLEvent event;
    RLComm comm;
  } UInfo;
};


class RecordList
{
  public:
    typedef set<RLRecord>::iterator iterator;

    static RecordList *create( RecordList *whichList );

    virtual ~RecordList() {}

    virtual void clear() = 0;
    virtual void erase( iterator first, iterator last ) = 0;
    virtual iterator begin() const = 0;
    virtual iterator end() const = 0;
    virtual bool newRecords() const = 0;
};

class RecordListProxy: public RecordList
{
  public:
    virtual ~RecordListProxy() {};

    virtual void clear();
    virtual void erase( iterator first, iterator last );
    virtual iterator begin() const;
    virtual iterator end() const;
    virtual bool newRecords() const;

  private:
    RecordListProxy( RecordList *whichList );

    RecordList *myRecordList;

    friend RecordList *RecordList::create( RecordList * );
};

#endif // RECORDLIST_H_INCLUDED
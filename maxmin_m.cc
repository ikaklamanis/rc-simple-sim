//
// Generated file, do not edit! Created by opp_msgtool 6.0 from maxmin.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "maxmin_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

Register_Class(RCMessage)

RCMessage::RCMessage(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

RCMessage::RCMessage(const RCMessage& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

RCMessage::~RCMessage()
{
}

RCMessage& RCMessage::operator=(const RCMessage& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void RCMessage::copy(const RCMessage& other)
{
    this->msgId = other.msgId;
    this->source = other.source;
    this->destination = other.destination;
}

void RCMessage::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->msgId);
    doParsimPacking(b,this->source);
    doParsimPacking(b,this->destination);
}

void RCMessage::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->msgId);
    doParsimUnpacking(b,this->source);
    doParsimUnpacking(b,this->destination);
}

int RCMessage::getMsgId() const
{
    return this->msgId;
}

void RCMessage::setMsgId(int msgId)
{
    this->msgId = msgId;
}

int RCMessage::getSource() const
{
    return this->source;
}

void RCMessage::setSource(int source)
{
    this->source = source;
}

int RCMessage::getDestination() const
{
    return this->destination;
}

void RCMessage::setDestination(int destination)
{
    this->destination = destination;
}

class RCMessageDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_msgId,
        FIELD_source,
        FIELD_destination,
    };
  public:
    RCMessageDescriptor();
    virtual ~RCMessageDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(RCMessageDescriptor)

RCMessageDescriptor::RCMessageDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(RCMessage)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

RCMessageDescriptor::~RCMessageDescriptor()
{
    delete[] propertyNames;
}

bool RCMessageDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<RCMessage *>(obj)!=nullptr;
}

const char **RCMessageDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *RCMessageDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int RCMessageDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 3+base->getFieldCount() : 3;
}

unsigned int RCMessageDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_msgId
        FD_ISEDITABLE,    // FIELD_source
        FD_ISEDITABLE,    // FIELD_destination
    };
    return (field >= 0 && field < 3) ? fieldTypeFlags[field] : 0;
}

const char *RCMessageDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "msgId",
        "source",
        "destination",
    };
    return (field >= 0 && field < 3) ? fieldNames[field] : nullptr;
}

int RCMessageDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "msgId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "source") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "destination") == 0) return baseIndex + 2;
    return base ? base->findField(fieldName) : -1;
}

const char *RCMessageDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_msgId
        "int",    // FIELD_source
        "int",    // FIELD_destination
    };
    return (field >= 0 && field < 3) ? fieldTypeStrings[field] : nullptr;
}

const char **RCMessageDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *RCMessageDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int RCMessageDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    RCMessage *pp = omnetpp::fromAnyPtr<RCMessage>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void RCMessageDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    RCMessage *pp = omnetpp::fromAnyPtr<RCMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'RCMessage'", field);
    }
}

const char *RCMessageDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    RCMessage *pp = omnetpp::fromAnyPtr<RCMessage>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string RCMessageDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    RCMessage *pp = omnetpp::fromAnyPtr<RCMessage>(object); (void)pp;
    switch (field) {
        case FIELD_msgId: return long2string(pp->getMsgId());
        case FIELD_source: return long2string(pp->getSource());
        case FIELD_destination: return long2string(pp->getDestination());
        default: return "";
    }
}

void RCMessageDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    RCMessage *pp = omnetpp::fromAnyPtr<RCMessage>(object); (void)pp;
    switch (field) {
        case FIELD_msgId: pp->setMsgId(string2long(value)); break;
        case FIELD_source: pp->setSource(string2long(value)); break;
        case FIELD_destination: pp->setDestination(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'RCMessage'", field);
    }
}

omnetpp::cValue RCMessageDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    RCMessage *pp = omnetpp::fromAnyPtr<RCMessage>(object); (void)pp;
    switch (field) {
        case FIELD_msgId: return pp->getMsgId();
        case FIELD_source: return pp->getSource();
        case FIELD_destination: return pp->getDestination();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'RCMessage' as cValue -- field index out of range?", field);
    }
}

void RCMessageDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    RCMessage *pp = omnetpp::fromAnyPtr<RCMessage>(object); (void)pp;
    switch (field) {
        case FIELD_msgId: pp->setMsgId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_source: pp->setSource(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_destination: pp->setDestination(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'RCMessage'", field);
    }
}

const char *RCMessageDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr RCMessageDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    RCMessage *pp = omnetpp::fromAnyPtr<RCMessage>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void RCMessageDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    RCMessage *pp = omnetpp::fromAnyPtr<RCMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'RCMessage'", field);
    }
}

Register_Class(MaxMinMsg)

MaxMinMsg::MaxMinMsg(const char *name, short kind) : ::RCMessage(name, kind)
{
}

MaxMinMsg::MaxMinMsg(const MaxMinMsg& other) : ::RCMessage(other)
{
    copy(other);
}

MaxMinMsg::~MaxMinMsg()
{
}

MaxMinMsg& MaxMinMsg::operator=(const MaxMinMsg& other)
{
    if (this == &other) return *this;
    ::RCMessage::operator=(other);
    copy(other);
    return *this;
}

void MaxMinMsg::copy(const MaxMinMsg& other)
{
}

void MaxMinMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::RCMessage::parsimPack(b);
}

void MaxMinMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::RCMessage::parsimUnpack(b);
}

class MaxMinMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
    };
  public:
    MaxMinMsgDescriptor();
    virtual ~MaxMinMsgDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(MaxMinMsgDescriptor)

MaxMinMsgDescriptor::MaxMinMsgDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(MaxMinMsg)), "RCMessage")
{
    propertyNames = nullptr;
}

MaxMinMsgDescriptor::~MaxMinMsgDescriptor()
{
    delete[] propertyNames;
}

bool MaxMinMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<MaxMinMsg *>(obj)!=nullptr;
}

const char **MaxMinMsgDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *MaxMinMsgDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int MaxMinMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 0+base->getFieldCount() : 0;
}

unsigned int MaxMinMsgDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    return 0;
}

const char *MaxMinMsgDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    return nullptr;
}

int MaxMinMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->findField(fieldName) : -1;
}

const char *MaxMinMsgDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    return nullptr;
}

const char **MaxMinMsgDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *MaxMinMsgDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int MaxMinMsgDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    MaxMinMsg *pp = omnetpp::fromAnyPtr<MaxMinMsg>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void MaxMinMsgDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    MaxMinMsg *pp = omnetpp::fromAnyPtr<MaxMinMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'MaxMinMsg'", field);
    }
}

const char *MaxMinMsgDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    MaxMinMsg *pp = omnetpp::fromAnyPtr<MaxMinMsg>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string MaxMinMsgDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    MaxMinMsg *pp = omnetpp::fromAnyPtr<MaxMinMsg>(object); (void)pp;
    switch (field) {
        default: return "";
    }
}

void MaxMinMsgDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    MaxMinMsg *pp = omnetpp::fromAnyPtr<MaxMinMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'MaxMinMsg'", field);
    }
}

omnetpp::cValue MaxMinMsgDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    MaxMinMsg *pp = omnetpp::fromAnyPtr<MaxMinMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'MaxMinMsg' as cValue -- field index out of range?", field);
    }
}

void MaxMinMsgDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    MaxMinMsg *pp = omnetpp::fromAnyPtr<MaxMinMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'MaxMinMsg'", field);
    }
}

const char *MaxMinMsgDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    return nullptr;
}

omnetpp::any_ptr MaxMinMsgDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    MaxMinMsg *pp = omnetpp::fromAnyPtr<MaxMinMsg>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void MaxMinMsgDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    MaxMinMsg *pp = omnetpp::fromAnyPtr<MaxMinMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'MaxMinMsg'", field);
    }
}

Register_Class(MaxMinACK)

MaxMinACK::MaxMinACK(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

MaxMinACK::MaxMinACK(const MaxMinACK& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

MaxMinACK::~MaxMinACK()
{
}

MaxMinACK& MaxMinACK::operator=(const MaxMinACK& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void MaxMinACK::copy(const MaxMinACK& other)
{
    this->msgId = other.msgId;
    this->source = other.source;
    this->destination = other.destination;
}

void MaxMinACK::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->msgId);
    doParsimPacking(b,this->source);
    doParsimPacking(b,this->destination);
}

void MaxMinACK::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->msgId);
    doParsimUnpacking(b,this->source);
    doParsimUnpacking(b,this->destination);
}

int MaxMinACK::getMsgId() const
{
    return this->msgId;
}

void MaxMinACK::setMsgId(int msgId)
{
    this->msgId = msgId;
}

int MaxMinACK::getSource() const
{
    return this->source;
}

void MaxMinACK::setSource(int source)
{
    this->source = source;
}

int MaxMinACK::getDestination() const
{
    return this->destination;
}

void MaxMinACK::setDestination(int destination)
{
    this->destination = destination;
}

class MaxMinACKDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_msgId,
        FIELD_source,
        FIELD_destination,
    };
  public:
    MaxMinACKDescriptor();
    virtual ~MaxMinACKDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(MaxMinACKDescriptor)

MaxMinACKDescriptor::MaxMinACKDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(MaxMinACK)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

MaxMinACKDescriptor::~MaxMinACKDescriptor()
{
    delete[] propertyNames;
}

bool MaxMinACKDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<MaxMinACK *>(obj)!=nullptr;
}

const char **MaxMinACKDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *MaxMinACKDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int MaxMinACKDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 3+base->getFieldCount() : 3;
}

unsigned int MaxMinACKDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_msgId
        FD_ISEDITABLE,    // FIELD_source
        FD_ISEDITABLE,    // FIELD_destination
    };
    return (field >= 0 && field < 3) ? fieldTypeFlags[field] : 0;
}

const char *MaxMinACKDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "msgId",
        "source",
        "destination",
    };
    return (field >= 0 && field < 3) ? fieldNames[field] : nullptr;
}

int MaxMinACKDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "msgId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "source") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "destination") == 0) return baseIndex + 2;
    return base ? base->findField(fieldName) : -1;
}

const char *MaxMinACKDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_msgId
        "int",    // FIELD_source
        "int",    // FIELD_destination
    };
    return (field >= 0 && field < 3) ? fieldTypeStrings[field] : nullptr;
}

const char **MaxMinACKDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *MaxMinACKDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int MaxMinACKDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    MaxMinACK *pp = omnetpp::fromAnyPtr<MaxMinACK>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void MaxMinACKDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    MaxMinACK *pp = omnetpp::fromAnyPtr<MaxMinACK>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'MaxMinACK'", field);
    }
}

const char *MaxMinACKDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    MaxMinACK *pp = omnetpp::fromAnyPtr<MaxMinACK>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string MaxMinACKDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    MaxMinACK *pp = omnetpp::fromAnyPtr<MaxMinACK>(object); (void)pp;
    switch (field) {
        case FIELD_msgId: return long2string(pp->getMsgId());
        case FIELD_source: return long2string(pp->getSource());
        case FIELD_destination: return long2string(pp->getDestination());
        default: return "";
    }
}

void MaxMinACKDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    MaxMinACK *pp = omnetpp::fromAnyPtr<MaxMinACK>(object); (void)pp;
    switch (field) {
        case FIELD_msgId: pp->setMsgId(string2long(value)); break;
        case FIELD_source: pp->setSource(string2long(value)); break;
        case FIELD_destination: pp->setDestination(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'MaxMinACK'", field);
    }
}

omnetpp::cValue MaxMinACKDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    MaxMinACK *pp = omnetpp::fromAnyPtr<MaxMinACK>(object); (void)pp;
    switch (field) {
        case FIELD_msgId: return pp->getMsgId();
        case FIELD_source: return pp->getSource();
        case FIELD_destination: return pp->getDestination();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'MaxMinACK' as cValue -- field index out of range?", field);
    }
}

void MaxMinACKDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    MaxMinACK *pp = omnetpp::fromAnyPtr<MaxMinACK>(object); (void)pp;
    switch (field) {
        case FIELD_msgId: pp->setMsgId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_source: pp->setSource(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_destination: pp->setDestination(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'MaxMinACK'", field);
    }
}

const char *MaxMinACKDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr MaxMinACKDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    MaxMinACK *pp = omnetpp::fromAnyPtr<MaxMinACK>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void MaxMinACKDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    MaxMinACK *pp = omnetpp::fromAnyPtr<MaxMinACK>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'MaxMinACK'", field);
    }
}

Register_Class(RCSelfMessage)

RCSelfMessage::RCSelfMessage(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

RCSelfMessage::RCSelfMessage(const RCSelfMessage& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

RCSelfMessage::~RCSelfMessage()
{
}

RCSelfMessage& RCSelfMessage::operator=(const RCSelfMessage& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void RCSelfMessage::copy(const RCSelfMessage& other)
{
    this->msgId = other.msgId;
}

void RCSelfMessage::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->msgId);
}

void RCSelfMessage::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->msgId);
}

float RCSelfMessage::getMsgId() const
{
    return this->msgId;
}

void RCSelfMessage::setMsgId(float msgId)
{
    this->msgId = msgId;
}

class RCSelfMessageDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_msgId,
    };
  public:
    RCSelfMessageDescriptor();
    virtual ~RCSelfMessageDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(RCSelfMessageDescriptor)

RCSelfMessageDescriptor::RCSelfMessageDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(RCSelfMessage)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

RCSelfMessageDescriptor::~RCSelfMessageDescriptor()
{
    delete[] propertyNames;
}

bool RCSelfMessageDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<RCSelfMessage *>(obj)!=nullptr;
}

const char **RCSelfMessageDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *RCSelfMessageDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int RCSelfMessageDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 1+base->getFieldCount() : 1;
}

unsigned int RCSelfMessageDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_msgId
    };
    return (field >= 0 && field < 1) ? fieldTypeFlags[field] : 0;
}

const char *RCSelfMessageDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "msgId",
    };
    return (field >= 0 && field < 1) ? fieldNames[field] : nullptr;
}

int RCSelfMessageDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "msgId") == 0) return baseIndex + 0;
    return base ? base->findField(fieldName) : -1;
}

const char *RCSelfMessageDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "float",    // FIELD_msgId
    };
    return (field >= 0 && field < 1) ? fieldTypeStrings[field] : nullptr;
}

const char **RCSelfMessageDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *RCSelfMessageDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int RCSelfMessageDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    RCSelfMessage *pp = omnetpp::fromAnyPtr<RCSelfMessage>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void RCSelfMessageDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    RCSelfMessage *pp = omnetpp::fromAnyPtr<RCSelfMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'RCSelfMessage'", field);
    }
}

const char *RCSelfMessageDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    RCSelfMessage *pp = omnetpp::fromAnyPtr<RCSelfMessage>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string RCSelfMessageDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    RCSelfMessage *pp = omnetpp::fromAnyPtr<RCSelfMessage>(object); (void)pp;
    switch (field) {
        case FIELD_msgId: return double2string(pp->getMsgId());
        default: return "";
    }
}

void RCSelfMessageDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    RCSelfMessage *pp = omnetpp::fromAnyPtr<RCSelfMessage>(object); (void)pp;
    switch (field) {
        case FIELD_msgId: pp->setMsgId(string2double(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'RCSelfMessage'", field);
    }
}

omnetpp::cValue RCSelfMessageDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    RCSelfMessage *pp = omnetpp::fromAnyPtr<RCSelfMessage>(object); (void)pp;
    switch (field) {
        case FIELD_msgId: return (double)(pp->getMsgId());
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'RCSelfMessage' as cValue -- field index out of range?", field);
    }
}

void RCSelfMessageDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    RCSelfMessage *pp = omnetpp::fromAnyPtr<RCSelfMessage>(object); (void)pp;
    switch (field) {
        case FIELD_msgId: pp->setMsgId(static_cast<float>(value.doubleValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'RCSelfMessage'", field);
    }
}

const char *RCSelfMessageDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr RCSelfMessageDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    RCSelfMessage *pp = omnetpp::fromAnyPtr<RCSelfMessage>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void RCSelfMessageDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    RCSelfMessage *pp = omnetpp::fromAnyPtr<RCSelfMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'RCSelfMessage'", field);
    }
}

Register_Class(SelfTimer)

SelfTimer::SelfTimer(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

SelfTimer::SelfTimer(const SelfTimer& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

SelfTimer::~SelfTimer()
{
}

SelfTimer& SelfTimer::operator=(const SelfTimer& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void SelfTimer::copy(const SelfTimer& other)
{
    this->msgTimer = other.msgTimer;
}

void SelfTimer::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->msgTimer);
}

void SelfTimer::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->msgTimer);
}

float SelfTimer::getMsgTimer() const
{
    return this->msgTimer;
}

void SelfTimer::setMsgTimer(float msgTimer)
{
    this->msgTimer = msgTimer;
}

class SelfTimerDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_msgTimer,
    };
  public:
    SelfTimerDescriptor();
    virtual ~SelfTimerDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(SelfTimerDescriptor)

SelfTimerDescriptor::SelfTimerDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(SelfTimer)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

SelfTimerDescriptor::~SelfTimerDescriptor()
{
    delete[] propertyNames;
}

bool SelfTimerDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<SelfTimer *>(obj)!=nullptr;
}

const char **SelfTimerDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *SelfTimerDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int SelfTimerDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 1+base->getFieldCount() : 1;
}

unsigned int SelfTimerDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_msgTimer
    };
    return (field >= 0 && field < 1) ? fieldTypeFlags[field] : 0;
}

const char *SelfTimerDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "msgTimer",
    };
    return (field >= 0 && field < 1) ? fieldNames[field] : nullptr;
}

int SelfTimerDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "msgTimer") == 0) return baseIndex + 0;
    return base ? base->findField(fieldName) : -1;
}

const char *SelfTimerDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "float",    // FIELD_msgTimer
    };
    return (field >= 0 && field < 1) ? fieldTypeStrings[field] : nullptr;
}

const char **SelfTimerDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *SelfTimerDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int SelfTimerDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    SelfTimer *pp = omnetpp::fromAnyPtr<SelfTimer>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void SelfTimerDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    SelfTimer *pp = omnetpp::fromAnyPtr<SelfTimer>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'SelfTimer'", field);
    }
}

const char *SelfTimerDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    SelfTimer *pp = omnetpp::fromAnyPtr<SelfTimer>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string SelfTimerDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    SelfTimer *pp = omnetpp::fromAnyPtr<SelfTimer>(object); (void)pp;
    switch (field) {
        case FIELD_msgTimer: return double2string(pp->getMsgTimer());
        default: return "";
    }
}

void SelfTimerDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    SelfTimer *pp = omnetpp::fromAnyPtr<SelfTimer>(object); (void)pp;
    switch (field) {
        case FIELD_msgTimer: pp->setMsgTimer(string2double(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'SelfTimer'", field);
    }
}

omnetpp::cValue SelfTimerDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    SelfTimer *pp = omnetpp::fromAnyPtr<SelfTimer>(object); (void)pp;
    switch (field) {
        case FIELD_msgTimer: return (double)(pp->getMsgTimer());
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'SelfTimer' as cValue -- field index out of range?", field);
    }
}

void SelfTimerDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    SelfTimer *pp = omnetpp::fromAnyPtr<SelfTimer>(object); (void)pp;
    switch (field) {
        case FIELD_msgTimer: pp->setMsgTimer(static_cast<float>(value.doubleValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'SelfTimer'", field);
    }
}

const char *SelfTimerDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr SelfTimerDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    SelfTimer *pp = omnetpp::fromAnyPtr<SelfTimer>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void SelfTimerDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    SelfTimer *pp = omnetpp::fromAnyPtr<SelfTimer>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'SelfTimer'", field);
    }
}

Register_Class(InMsgTimer)

InMsgTimer::InMsgTimer(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

InMsgTimer::InMsgTimer(const InMsgTimer& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

InMsgTimer::~InMsgTimer()
{
}

InMsgTimer& InMsgTimer::operator=(const InMsgTimer& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void InMsgTimer::copy(const InMsgTimer& other)
{
    this->msgTimer = other.msgTimer;
    this->msgId = other.msgId;
}

void InMsgTimer::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->msgTimer);
    doParsimPacking(b,this->msgId);
}

void InMsgTimer::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->msgTimer);
    doParsimUnpacking(b,this->msgId);
}

float InMsgTimer::getMsgTimer() const
{
    return this->msgTimer;
}

void InMsgTimer::setMsgTimer(float msgTimer)
{
    this->msgTimer = msgTimer;
}

int InMsgTimer::getMsgId() const
{
    return this->msgId;
}

void InMsgTimer::setMsgId(int msgId)
{
    this->msgId = msgId;
}

class InMsgTimerDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_msgTimer,
        FIELD_msgId,
    };
  public:
    InMsgTimerDescriptor();
    virtual ~InMsgTimerDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(InMsgTimerDescriptor)

InMsgTimerDescriptor::InMsgTimerDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(InMsgTimer)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

InMsgTimerDescriptor::~InMsgTimerDescriptor()
{
    delete[] propertyNames;
}

bool InMsgTimerDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<InMsgTimer *>(obj)!=nullptr;
}

const char **InMsgTimerDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *InMsgTimerDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int InMsgTimerDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 2+base->getFieldCount() : 2;
}

unsigned int InMsgTimerDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_msgTimer
        FD_ISEDITABLE,    // FIELD_msgId
    };
    return (field >= 0 && field < 2) ? fieldTypeFlags[field] : 0;
}

const char *InMsgTimerDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "msgTimer",
        "msgId",
    };
    return (field >= 0 && field < 2) ? fieldNames[field] : nullptr;
}

int InMsgTimerDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "msgTimer") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "msgId") == 0) return baseIndex + 1;
    return base ? base->findField(fieldName) : -1;
}

const char *InMsgTimerDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "float",    // FIELD_msgTimer
        "int",    // FIELD_msgId
    };
    return (field >= 0 && field < 2) ? fieldTypeStrings[field] : nullptr;
}

const char **InMsgTimerDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *InMsgTimerDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int InMsgTimerDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    InMsgTimer *pp = omnetpp::fromAnyPtr<InMsgTimer>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void InMsgTimerDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    InMsgTimer *pp = omnetpp::fromAnyPtr<InMsgTimer>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'InMsgTimer'", field);
    }
}

const char *InMsgTimerDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    InMsgTimer *pp = omnetpp::fromAnyPtr<InMsgTimer>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string InMsgTimerDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    InMsgTimer *pp = omnetpp::fromAnyPtr<InMsgTimer>(object); (void)pp;
    switch (field) {
        case FIELD_msgTimer: return double2string(pp->getMsgTimer());
        case FIELD_msgId: return long2string(pp->getMsgId());
        default: return "";
    }
}

void InMsgTimerDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    InMsgTimer *pp = omnetpp::fromAnyPtr<InMsgTimer>(object); (void)pp;
    switch (field) {
        case FIELD_msgTimer: pp->setMsgTimer(string2double(value)); break;
        case FIELD_msgId: pp->setMsgId(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'InMsgTimer'", field);
    }
}

omnetpp::cValue InMsgTimerDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    InMsgTimer *pp = omnetpp::fromAnyPtr<InMsgTimer>(object); (void)pp;
    switch (field) {
        case FIELD_msgTimer: return (double)(pp->getMsgTimer());
        case FIELD_msgId: return pp->getMsgId();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'InMsgTimer' as cValue -- field index out of range?", field);
    }
}

void InMsgTimerDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    InMsgTimer *pp = omnetpp::fromAnyPtr<InMsgTimer>(object); (void)pp;
    switch (field) {
        case FIELD_msgTimer: pp->setMsgTimer(static_cast<float>(value.doubleValue())); break;
        case FIELD_msgId: pp->setMsgId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'InMsgTimer'", field);
    }
}

const char *InMsgTimerDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr InMsgTimerDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    InMsgTimer *pp = omnetpp::fromAnyPtr<InMsgTimer>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void InMsgTimerDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    InMsgTimer *pp = omnetpp::fromAnyPtr<InMsgTimer>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'InMsgTimer'", field);
    }
}

Register_Class(OutMsgTimer)

OutMsgTimer::OutMsgTimer(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

OutMsgTimer::OutMsgTimer(const OutMsgTimer& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

OutMsgTimer::~OutMsgTimer()
{
}

OutMsgTimer& OutMsgTimer::operator=(const OutMsgTimer& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void OutMsgTimer::copy(const OutMsgTimer& other)
{
    this->msgTimer = other.msgTimer;
    this->msgId = other.msgId;
    this->destination = other.destination;
}

void OutMsgTimer::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->msgTimer);
    doParsimPacking(b,this->msgId);
    doParsimPacking(b,this->destination);
}

void OutMsgTimer::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->msgTimer);
    doParsimUnpacking(b,this->msgId);
    doParsimUnpacking(b,this->destination);
}

float OutMsgTimer::getMsgTimer() const
{
    return this->msgTimer;
}

void OutMsgTimer::setMsgTimer(float msgTimer)
{
    this->msgTimer = msgTimer;
}

int OutMsgTimer::getMsgId() const
{
    return this->msgId;
}

void OutMsgTimer::setMsgId(int msgId)
{
    this->msgId = msgId;
}

int OutMsgTimer::getDestination() const
{
    return this->destination;
}

void OutMsgTimer::setDestination(int destination)
{
    this->destination = destination;
}

class OutMsgTimerDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_msgTimer,
        FIELD_msgId,
        FIELD_destination,
    };
  public:
    OutMsgTimerDescriptor();
    virtual ~OutMsgTimerDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(OutMsgTimerDescriptor)

OutMsgTimerDescriptor::OutMsgTimerDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(OutMsgTimer)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

OutMsgTimerDescriptor::~OutMsgTimerDescriptor()
{
    delete[] propertyNames;
}

bool OutMsgTimerDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<OutMsgTimer *>(obj)!=nullptr;
}

const char **OutMsgTimerDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *OutMsgTimerDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int OutMsgTimerDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 3+base->getFieldCount() : 3;
}

unsigned int OutMsgTimerDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_msgTimer
        FD_ISEDITABLE,    // FIELD_msgId
        FD_ISEDITABLE,    // FIELD_destination
    };
    return (field >= 0 && field < 3) ? fieldTypeFlags[field] : 0;
}

const char *OutMsgTimerDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "msgTimer",
        "msgId",
        "destination",
    };
    return (field >= 0 && field < 3) ? fieldNames[field] : nullptr;
}

int OutMsgTimerDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "msgTimer") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "msgId") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "destination") == 0) return baseIndex + 2;
    return base ? base->findField(fieldName) : -1;
}

const char *OutMsgTimerDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "float",    // FIELD_msgTimer
        "int",    // FIELD_msgId
        "int",    // FIELD_destination
    };
    return (field >= 0 && field < 3) ? fieldTypeStrings[field] : nullptr;
}

const char **OutMsgTimerDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *OutMsgTimerDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int OutMsgTimerDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    OutMsgTimer *pp = omnetpp::fromAnyPtr<OutMsgTimer>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void OutMsgTimerDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    OutMsgTimer *pp = omnetpp::fromAnyPtr<OutMsgTimer>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'OutMsgTimer'", field);
    }
}

const char *OutMsgTimerDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    OutMsgTimer *pp = omnetpp::fromAnyPtr<OutMsgTimer>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string OutMsgTimerDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    OutMsgTimer *pp = omnetpp::fromAnyPtr<OutMsgTimer>(object); (void)pp;
    switch (field) {
        case FIELD_msgTimer: return double2string(pp->getMsgTimer());
        case FIELD_msgId: return long2string(pp->getMsgId());
        case FIELD_destination: return long2string(pp->getDestination());
        default: return "";
    }
}

void OutMsgTimerDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    OutMsgTimer *pp = omnetpp::fromAnyPtr<OutMsgTimer>(object); (void)pp;
    switch (field) {
        case FIELD_msgTimer: pp->setMsgTimer(string2double(value)); break;
        case FIELD_msgId: pp->setMsgId(string2long(value)); break;
        case FIELD_destination: pp->setDestination(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'OutMsgTimer'", field);
    }
}

omnetpp::cValue OutMsgTimerDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    OutMsgTimer *pp = omnetpp::fromAnyPtr<OutMsgTimer>(object); (void)pp;
    switch (field) {
        case FIELD_msgTimer: return (double)(pp->getMsgTimer());
        case FIELD_msgId: return pp->getMsgId();
        case FIELD_destination: return pp->getDestination();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'OutMsgTimer' as cValue -- field index out of range?", field);
    }
}

void OutMsgTimerDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    OutMsgTimer *pp = omnetpp::fromAnyPtr<OutMsgTimer>(object); (void)pp;
    switch (field) {
        case FIELD_msgTimer: pp->setMsgTimer(static_cast<float>(value.doubleValue())); break;
        case FIELD_msgId: pp->setMsgId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_destination: pp->setDestination(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'OutMsgTimer'", field);
    }
}

const char *OutMsgTimerDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr OutMsgTimerDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    OutMsgTimer *pp = omnetpp::fromAnyPtr<OutMsgTimer>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void OutMsgTimerDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    OutMsgTimer *pp = omnetpp::fromAnyPtr<OutMsgTimer>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'OutMsgTimer'", field);
    }
}

Register_Class(AckTimeOut)

AckTimeOut::AckTimeOut(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

AckTimeOut::AckTimeOut(const AckTimeOut& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

AckTimeOut::~AckTimeOut()
{
}

AckTimeOut& AckTimeOut::operator=(const AckTimeOut& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void AckTimeOut::copy(const AckTimeOut& other)
{
    this->msgId = other.msgId;
}

void AckTimeOut::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->msgId);
}

void AckTimeOut::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->msgId);
}

int AckTimeOut::getMsgId() const
{
    return this->msgId;
}

void AckTimeOut::setMsgId(int msgId)
{
    this->msgId = msgId;
}

class AckTimeOutDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_msgId,
    };
  public:
    AckTimeOutDescriptor();
    virtual ~AckTimeOutDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(AckTimeOutDescriptor)

AckTimeOutDescriptor::AckTimeOutDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(AckTimeOut)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

AckTimeOutDescriptor::~AckTimeOutDescriptor()
{
    delete[] propertyNames;
}

bool AckTimeOutDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<AckTimeOut *>(obj)!=nullptr;
}

const char **AckTimeOutDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *AckTimeOutDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int AckTimeOutDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 1+base->getFieldCount() : 1;
}

unsigned int AckTimeOutDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_msgId
    };
    return (field >= 0 && field < 1) ? fieldTypeFlags[field] : 0;
}

const char *AckTimeOutDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "msgId",
    };
    return (field >= 0 && field < 1) ? fieldNames[field] : nullptr;
}

int AckTimeOutDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "msgId") == 0) return baseIndex + 0;
    return base ? base->findField(fieldName) : -1;
}

const char *AckTimeOutDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_msgId
    };
    return (field >= 0 && field < 1) ? fieldTypeStrings[field] : nullptr;
}

const char **AckTimeOutDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *AckTimeOutDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int AckTimeOutDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    AckTimeOut *pp = omnetpp::fromAnyPtr<AckTimeOut>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void AckTimeOutDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    AckTimeOut *pp = omnetpp::fromAnyPtr<AckTimeOut>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'AckTimeOut'", field);
    }
}

const char *AckTimeOutDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    AckTimeOut *pp = omnetpp::fromAnyPtr<AckTimeOut>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string AckTimeOutDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    AckTimeOut *pp = omnetpp::fromAnyPtr<AckTimeOut>(object); (void)pp;
    switch (field) {
        case FIELD_msgId: return long2string(pp->getMsgId());
        default: return "";
    }
}

void AckTimeOutDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    AckTimeOut *pp = omnetpp::fromAnyPtr<AckTimeOut>(object); (void)pp;
    switch (field) {
        case FIELD_msgId: pp->setMsgId(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'AckTimeOut'", field);
    }
}

omnetpp::cValue AckTimeOutDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    AckTimeOut *pp = omnetpp::fromAnyPtr<AckTimeOut>(object); (void)pp;
    switch (field) {
        case FIELD_msgId: return pp->getMsgId();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'AckTimeOut' as cValue -- field index out of range?", field);
    }
}

void AckTimeOutDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    AckTimeOut *pp = omnetpp::fromAnyPtr<AckTimeOut>(object); (void)pp;
    switch (field) {
        case FIELD_msgId: pp->setMsgId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'AckTimeOut'", field);
    }
}

const char *AckTimeOutDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr AckTimeOutDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    AckTimeOut *pp = omnetpp::fromAnyPtr<AckTimeOut>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void AckTimeOutDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    AckTimeOut *pp = omnetpp::fromAnyPtr<AckTimeOut>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'AckTimeOut'", field);
    }
}

Register_Class(AIMDTimer)

AIMDTimer::AIMDTimer(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

AIMDTimer::AIMDTimer(const AIMDTimer& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

AIMDTimer::~AIMDTimer()
{
}

AIMDTimer& AIMDTimer::operator=(const AIMDTimer& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void AIMDTimer::copy(const AIMDTimer& other)
{
    this->msgTimer = other.msgTimer;
    this->msgId = other.msgId;
    this->peerIdx = other.peerIdx;
}

void AIMDTimer::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->msgTimer);
    doParsimPacking(b,this->msgId);
    doParsimPacking(b,this->peerIdx);
}

void AIMDTimer::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->msgTimer);
    doParsimUnpacking(b,this->msgId);
    doParsimUnpacking(b,this->peerIdx);
}

float AIMDTimer::getMsgTimer() const
{
    return this->msgTimer;
}

void AIMDTimer::setMsgTimer(float msgTimer)
{
    this->msgTimer = msgTimer;
}

int AIMDTimer::getMsgId() const
{
    return this->msgId;
}

void AIMDTimer::setMsgId(int msgId)
{
    this->msgId = msgId;
}

int AIMDTimer::getPeerIdx() const
{
    return this->peerIdx;
}

void AIMDTimer::setPeerIdx(int peerIdx)
{
    this->peerIdx = peerIdx;
}

class AIMDTimerDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_msgTimer,
        FIELD_msgId,
        FIELD_peerIdx,
    };
  public:
    AIMDTimerDescriptor();
    virtual ~AIMDTimerDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(AIMDTimerDescriptor)

AIMDTimerDescriptor::AIMDTimerDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(AIMDTimer)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

AIMDTimerDescriptor::~AIMDTimerDescriptor()
{
    delete[] propertyNames;
}

bool AIMDTimerDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<AIMDTimer *>(obj)!=nullptr;
}

const char **AIMDTimerDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *AIMDTimerDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int AIMDTimerDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 3+base->getFieldCount() : 3;
}

unsigned int AIMDTimerDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_msgTimer
        FD_ISEDITABLE,    // FIELD_msgId
        FD_ISEDITABLE,    // FIELD_peerIdx
    };
    return (field >= 0 && field < 3) ? fieldTypeFlags[field] : 0;
}

const char *AIMDTimerDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "msgTimer",
        "msgId",
        "peerIdx",
    };
    return (field >= 0 && field < 3) ? fieldNames[field] : nullptr;
}

int AIMDTimerDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "msgTimer") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "msgId") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "peerIdx") == 0) return baseIndex + 2;
    return base ? base->findField(fieldName) : -1;
}

const char *AIMDTimerDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "float",    // FIELD_msgTimer
        "int",    // FIELD_msgId
        "int",    // FIELD_peerIdx
    };
    return (field >= 0 && field < 3) ? fieldTypeStrings[field] : nullptr;
}

const char **AIMDTimerDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *AIMDTimerDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int AIMDTimerDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    AIMDTimer *pp = omnetpp::fromAnyPtr<AIMDTimer>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void AIMDTimerDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    AIMDTimer *pp = omnetpp::fromAnyPtr<AIMDTimer>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'AIMDTimer'", field);
    }
}

const char *AIMDTimerDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    AIMDTimer *pp = omnetpp::fromAnyPtr<AIMDTimer>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string AIMDTimerDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    AIMDTimer *pp = omnetpp::fromAnyPtr<AIMDTimer>(object); (void)pp;
    switch (field) {
        case FIELD_msgTimer: return double2string(pp->getMsgTimer());
        case FIELD_msgId: return long2string(pp->getMsgId());
        case FIELD_peerIdx: return long2string(pp->getPeerIdx());
        default: return "";
    }
}

void AIMDTimerDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    AIMDTimer *pp = omnetpp::fromAnyPtr<AIMDTimer>(object); (void)pp;
    switch (field) {
        case FIELD_msgTimer: pp->setMsgTimer(string2double(value)); break;
        case FIELD_msgId: pp->setMsgId(string2long(value)); break;
        case FIELD_peerIdx: pp->setPeerIdx(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'AIMDTimer'", field);
    }
}

omnetpp::cValue AIMDTimerDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    AIMDTimer *pp = omnetpp::fromAnyPtr<AIMDTimer>(object); (void)pp;
    switch (field) {
        case FIELD_msgTimer: return (double)(pp->getMsgTimer());
        case FIELD_msgId: return pp->getMsgId();
        case FIELD_peerIdx: return pp->getPeerIdx();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'AIMDTimer' as cValue -- field index out of range?", field);
    }
}

void AIMDTimerDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    AIMDTimer *pp = omnetpp::fromAnyPtr<AIMDTimer>(object); (void)pp;
    switch (field) {
        case FIELD_msgTimer: pp->setMsgTimer(static_cast<float>(value.doubleValue())); break;
        case FIELD_msgId: pp->setMsgId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_peerIdx: pp->setPeerIdx(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'AIMDTimer'", field);
    }
}

const char *AIMDTimerDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr AIMDTimerDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    AIMDTimer *pp = omnetpp::fromAnyPtr<AIMDTimer>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void AIMDTimerDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    AIMDTimer *pp = omnetpp::fromAnyPtr<AIMDTimer>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'AIMDTimer'", field);
    }
}

Register_Class(RxRateTimer)

RxRateTimer::RxRateTimer(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

RxRateTimer::RxRateTimer(const RxRateTimer& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

RxRateTimer::~RxRateTimer()
{
}

RxRateTimer& RxRateTimer::operator=(const RxRateTimer& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void RxRateTimer::copy(const RxRateTimer& other)
{
    this->msgTimer = other.msgTimer;
}

void RxRateTimer::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->msgTimer);
}

void RxRateTimer::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->msgTimer);
}

float RxRateTimer::getMsgTimer() const
{
    return this->msgTimer;
}

void RxRateTimer::setMsgTimer(float msgTimer)
{
    this->msgTimer = msgTimer;
}

class RxRateTimerDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_msgTimer,
    };
  public:
    RxRateTimerDescriptor();
    virtual ~RxRateTimerDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(RxRateTimerDescriptor)

RxRateTimerDescriptor::RxRateTimerDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(RxRateTimer)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

RxRateTimerDescriptor::~RxRateTimerDescriptor()
{
    delete[] propertyNames;
}

bool RxRateTimerDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<RxRateTimer *>(obj)!=nullptr;
}

const char **RxRateTimerDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *RxRateTimerDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int RxRateTimerDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 1+base->getFieldCount() : 1;
}

unsigned int RxRateTimerDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_msgTimer
    };
    return (field >= 0 && field < 1) ? fieldTypeFlags[field] : 0;
}

const char *RxRateTimerDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "msgTimer",
    };
    return (field >= 0 && field < 1) ? fieldNames[field] : nullptr;
}

int RxRateTimerDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "msgTimer") == 0) return baseIndex + 0;
    return base ? base->findField(fieldName) : -1;
}

const char *RxRateTimerDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "float",    // FIELD_msgTimer
    };
    return (field >= 0 && field < 1) ? fieldTypeStrings[field] : nullptr;
}

const char **RxRateTimerDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *RxRateTimerDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int RxRateTimerDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    RxRateTimer *pp = omnetpp::fromAnyPtr<RxRateTimer>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void RxRateTimerDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    RxRateTimer *pp = omnetpp::fromAnyPtr<RxRateTimer>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'RxRateTimer'", field);
    }
}

const char *RxRateTimerDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    RxRateTimer *pp = omnetpp::fromAnyPtr<RxRateTimer>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string RxRateTimerDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    RxRateTimer *pp = omnetpp::fromAnyPtr<RxRateTimer>(object); (void)pp;
    switch (field) {
        case FIELD_msgTimer: return double2string(pp->getMsgTimer());
        default: return "";
    }
}

void RxRateTimerDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    RxRateTimer *pp = omnetpp::fromAnyPtr<RxRateTimer>(object); (void)pp;
    switch (field) {
        case FIELD_msgTimer: pp->setMsgTimer(string2double(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'RxRateTimer'", field);
    }
}

omnetpp::cValue RxRateTimerDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    RxRateTimer *pp = omnetpp::fromAnyPtr<RxRateTimer>(object); (void)pp;
    switch (field) {
        case FIELD_msgTimer: return (double)(pp->getMsgTimer());
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'RxRateTimer' as cValue -- field index out of range?", field);
    }
}

void RxRateTimerDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    RxRateTimer *pp = omnetpp::fromAnyPtr<RxRateTimer>(object); (void)pp;
    switch (field) {
        case FIELD_msgTimer: pp->setMsgTimer(static_cast<float>(value.doubleValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'RxRateTimer'", field);
    }
}

const char *RxRateTimerDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr RxRateTimerDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    RxRateTimer *pp = omnetpp::fromAnyPtr<RxRateTimer>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void RxRateTimerDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    RxRateTimer *pp = omnetpp::fromAnyPtr<RxRateTimer>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'RxRateTimer'", field);
    }
}

Register_Class(RateChangeTimer)

RateChangeTimer::RateChangeTimer(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

RateChangeTimer::RateChangeTimer(const RateChangeTimer& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

RateChangeTimer::~RateChangeTimer()
{
}

RateChangeTimer& RateChangeTimer::operator=(const RateChangeTimer& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void RateChangeTimer::copy(const RateChangeTimer& other)
{
    this->msgTimer = other.msgTimer;
}

void RateChangeTimer::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->msgTimer);
}

void RateChangeTimer::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->msgTimer);
}

float RateChangeTimer::getMsgTimer() const
{
    return this->msgTimer;
}

void RateChangeTimer::setMsgTimer(float msgTimer)
{
    this->msgTimer = msgTimer;
}

class RateChangeTimerDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_msgTimer,
    };
  public:
    RateChangeTimerDescriptor();
    virtual ~RateChangeTimerDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(RateChangeTimerDescriptor)

RateChangeTimerDescriptor::RateChangeTimerDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(RateChangeTimer)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

RateChangeTimerDescriptor::~RateChangeTimerDescriptor()
{
    delete[] propertyNames;
}

bool RateChangeTimerDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<RateChangeTimer *>(obj)!=nullptr;
}

const char **RateChangeTimerDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *RateChangeTimerDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int RateChangeTimerDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 1+base->getFieldCount() : 1;
}

unsigned int RateChangeTimerDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_msgTimer
    };
    return (field >= 0 && field < 1) ? fieldTypeFlags[field] : 0;
}

const char *RateChangeTimerDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "msgTimer",
    };
    return (field >= 0 && field < 1) ? fieldNames[field] : nullptr;
}

int RateChangeTimerDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "msgTimer") == 0) return baseIndex + 0;
    return base ? base->findField(fieldName) : -1;
}

const char *RateChangeTimerDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "float",    // FIELD_msgTimer
    };
    return (field >= 0 && field < 1) ? fieldTypeStrings[field] : nullptr;
}

const char **RateChangeTimerDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *RateChangeTimerDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int RateChangeTimerDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    RateChangeTimer *pp = omnetpp::fromAnyPtr<RateChangeTimer>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void RateChangeTimerDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    RateChangeTimer *pp = omnetpp::fromAnyPtr<RateChangeTimer>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'RateChangeTimer'", field);
    }
}

const char *RateChangeTimerDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    RateChangeTimer *pp = omnetpp::fromAnyPtr<RateChangeTimer>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string RateChangeTimerDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    RateChangeTimer *pp = omnetpp::fromAnyPtr<RateChangeTimer>(object); (void)pp;
    switch (field) {
        case FIELD_msgTimer: return double2string(pp->getMsgTimer());
        default: return "";
    }
}

void RateChangeTimerDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    RateChangeTimer *pp = omnetpp::fromAnyPtr<RateChangeTimer>(object); (void)pp;
    switch (field) {
        case FIELD_msgTimer: pp->setMsgTimer(string2double(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'RateChangeTimer'", field);
    }
}

omnetpp::cValue RateChangeTimerDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    RateChangeTimer *pp = omnetpp::fromAnyPtr<RateChangeTimer>(object); (void)pp;
    switch (field) {
        case FIELD_msgTimer: return (double)(pp->getMsgTimer());
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'RateChangeTimer' as cValue -- field index out of range?", field);
    }
}

void RateChangeTimerDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    RateChangeTimer *pp = omnetpp::fromAnyPtr<RateChangeTimer>(object); (void)pp;
    switch (field) {
        case FIELD_msgTimer: pp->setMsgTimer(static_cast<float>(value.doubleValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'RateChangeTimer'", field);
    }
}

const char *RateChangeTimerDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr RateChangeTimerDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    RateChangeTimer *pp = omnetpp::fromAnyPtr<RateChangeTimer>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void RateChangeTimerDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    RateChangeTimer *pp = omnetpp::fromAnyPtr<RateChangeTimer>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'RateChangeTimer'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp


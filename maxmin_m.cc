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

Register_Class(MaxMinMsg)

MaxMinMsg::MaxMinMsg(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

MaxMinMsg::MaxMinMsg(const MaxMinMsg& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

MaxMinMsg::~MaxMinMsg()
{
}

MaxMinMsg& MaxMinMsg::operator=(const MaxMinMsg& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void MaxMinMsg::copy(const MaxMinMsg& other)
{
    this->msgId = other.msgId;
    this->source = other.source;
    this->destination = other.destination;
}

void MaxMinMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->msgId);
    doParsimPacking(b,this->source);
    doParsimPacking(b,this->destination);
}

void MaxMinMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->msgId);
    doParsimUnpacking(b,this->source);
    doParsimUnpacking(b,this->destination);
}

int MaxMinMsg::getMsgId() const
{
    return this->msgId;
}

void MaxMinMsg::setMsgId(int msgId)
{
    this->msgId = msgId;
}

int MaxMinMsg::getSource() const
{
    return this->source;
}

void MaxMinMsg::setSource(int source)
{
    this->source = source;
}

int MaxMinMsg::getDestination() const
{
    return this->destination;
}

void MaxMinMsg::setDestination(int destination)
{
    this->destination = destination;
}

class MaxMinMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_msgId,
        FIELD_source,
        FIELD_destination,
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

MaxMinMsgDescriptor::MaxMinMsgDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(MaxMinMsg)), "omnetpp::cMessage")
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
    return base ? 3+base->getFieldCount() : 3;
}

unsigned int MaxMinMsgDescriptor::getFieldTypeFlags(int field) const
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

const char *MaxMinMsgDescriptor::getFieldName(int field) const
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

int MaxMinMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "msgId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "source") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "destination") == 0) return baseIndex + 2;
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
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_msgId
        "int",    // FIELD_source
        "int",    // FIELD_destination
    };
    return (field >= 0 && field < 3) ? fieldTypeStrings[field] : nullptr;
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
        case FIELD_msgId: return long2string(pp->getMsgId());
        case FIELD_source: return long2string(pp->getSource());
        case FIELD_destination: return long2string(pp->getDestination());
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
        case FIELD_msgId: pp->setMsgId(string2long(value)); break;
        case FIELD_source: pp->setSource(string2long(value)); break;
        case FIELD_destination: pp->setDestination(string2long(value)); break;
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
        case FIELD_msgId: return pp->getMsgId();
        case FIELD_source: return pp->getSource();
        case FIELD_destination: return pp->getDestination();
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
        case FIELD_msgId: pp->setMsgId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_source: pp->setSource(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_destination: pp->setDestination(omnetpp::checked_int_cast<int>(value.intValue())); break;
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
    switch (field) {
        default: return nullptr;
    };
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

Register_Class(ACKMsg)

ACKMsg::ACKMsg(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

ACKMsg::ACKMsg(const ACKMsg& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

ACKMsg::~ACKMsg()
{
}

ACKMsg& ACKMsg::operator=(const ACKMsg& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void ACKMsg::copy(const ACKMsg& other)
{
    this->msgId = other.msgId;
    this->source = other.source;
    this->destination = other.destination;
}

void ACKMsg::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->msgId);
    doParsimPacking(b,this->source);
    doParsimPacking(b,this->destination);
}

void ACKMsg::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->msgId);
    doParsimUnpacking(b,this->source);
    doParsimUnpacking(b,this->destination);
}

int ACKMsg::getMsgId() const
{
    return this->msgId;
}

void ACKMsg::setMsgId(int msgId)
{
    this->msgId = msgId;
}

int ACKMsg::getSource() const
{
    return this->source;
}

void ACKMsg::setSource(int source)
{
    this->source = source;
}

int ACKMsg::getDestination() const
{
    return this->destination;
}

void ACKMsg::setDestination(int destination)
{
    this->destination = destination;
}

class ACKMsgDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_msgId,
        FIELD_source,
        FIELD_destination,
    };
  public:
    ACKMsgDescriptor();
    virtual ~ACKMsgDescriptor();

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

Register_ClassDescriptor(ACKMsgDescriptor)

ACKMsgDescriptor::ACKMsgDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(ACKMsg)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

ACKMsgDescriptor::~ACKMsgDescriptor()
{
    delete[] propertyNames;
}

bool ACKMsgDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<ACKMsg *>(obj)!=nullptr;
}

const char **ACKMsgDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *ACKMsgDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int ACKMsgDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 3+base->getFieldCount() : 3;
}

unsigned int ACKMsgDescriptor::getFieldTypeFlags(int field) const
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

const char *ACKMsgDescriptor::getFieldName(int field) const
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

int ACKMsgDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "msgId") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "source") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "destination") == 0) return baseIndex + 2;
    return base ? base->findField(fieldName) : -1;
}

const char *ACKMsgDescriptor::getFieldTypeString(int field) const
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

const char **ACKMsgDescriptor::getFieldPropertyNames(int field) const
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

const char *ACKMsgDescriptor::getFieldProperty(int field, const char *propertyName) const
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

int ACKMsgDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    ACKMsg *pp = omnetpp::fromAnyPtr<ACKMsg>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void ACKMsgDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    ACKMsg *pp = omnetpp::fromAnyPtr<ACKMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'ACKMsg'", field);
    }
}

const char *ACKMsgDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    ACKMsg *pp = omnetpp::fromAnyPtr<ACKMsg>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string ACKMsgDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    ACKMsg *pp = omnetpp::fromAnyPtr<ACKMsg>(object); (void)pp;
    switch (field) {
        case FIELD_msgId: return long2string(pp->getMsgId());
        case FIELD_source: return long2string(pp->getSource());
        case FIELD_destination: return long2string(pp->getDestination());
        default: return "";
    }
}

void ACKMsgDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ACKMsg *pp = omnetpp::fromAnyPtr<ACKMsg>(object); (void)pp;
    switch (field) {
        case FIELD_msgId: pp->setMsgId(string2long(value)); break;
        case FIELD_source: pp->setSource(string2long(value)); break;
        case FIELD_destination: pp->setDestination(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ACKMsg'", field);
    }
}

omnetpp::cValue ACKMsgDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    ACKMsg *pp = omnetpp::fromAnyPtr<ACKMsg>(object); (void)pp;
    switch (field) {
        case FIELD_msgId: return pp->getMsgId();
        case FIELD_source: return pp->getSource();
        case FIELD_destination: return pp->getDestination();
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'ACKMsg' as cValue -- field index out of range?", field);
    }
}

void ACKMsgDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ACKMsg *pp = omnetpp::fromAnyPtr<ACKMsg>(object); (void)pp;
    switch (field) {
        case FIELD_msgId: pp->setMsgId(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_source: pp->setSource(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_destination: pp->setDestination(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ACKMsg'", field);
    }
}

const char *ACKMsgDescriptor::getFieldStructName(int field) const
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

omnetpp::any_ptr ACKMsgDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    ACKMsg *pp = omnetpp::fromAnyPtr<ACKMsg>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void ACKMsgDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    ACKMsg *pp = omnetpp::fromAnyPtr<ACKMsg>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ACKMsg'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp


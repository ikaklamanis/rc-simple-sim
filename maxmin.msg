//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2003 Ahmet Sekercioglu
// Copyright (C) 2003-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

message RCMessage
{
    int msgId;
    int source;
    int destination;
}

message MaxMinMsg extends RCMessage 
{
}

message MaxMinACK
{
    int msgId;
    int source;
    int destination; 
}

// only used as self messages

message RCSelfMessage
{
    float msgId;
}

message SelfTimer
{
    float msgTimer;
}

message InMsgTimer
{
    float msgTimer;
    int msgId;
}

message OutMsgTimer
{
    float msgTimer;
    int msgId;
    int destination;
}

message AckTimeOut
{
    int msgId;
}

message AIMDTimer
{
    float msgTimer;
    int msgId;
    int peerIdx;
}

message RxRateTimer
{
	float msgTimer;
}

message RateChangeTimer
{
 	float msgTimer;   
}

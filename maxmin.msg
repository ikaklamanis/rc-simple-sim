//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2003 Ahmet Sekercioglu
// Copyright (C) 2003-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

message MaxMinMsg
{
    int msgId;
    int source;
    int destination;
}

message ACKMsg
{
    int msgId;
    int source;
    int destination;  
}
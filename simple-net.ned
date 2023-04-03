//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

simple RcNode
{
    // add signals, statistics
    parameters:
        @display("i=block/routing");

        @signal[numRxSignal](type="int");
        @signal[numRxAsIntermSignal](type="int");

        @signal[uScore1Signal](type="float");
        @signal[uScore2Signal](type="float");
        @signal[uScore3Signal](type="float");
        
        @signal[intermNodeSignal](type="int");
        
        @signal[numDrInQSignal](type="int");
        @signal[numDrOutQSignal](type="int");
        @signal[inQSizeSignal](type="int");
        @signal[outQSizeSignal](type="int");
        
        @signal[rate1Signal](type="float");
        @signal[rate2Signal](type="float");


        @statistic[numReceived](title="num msgs rx"; source=sum(numRxSignal); record=stats,vector; );
        @statistic[numReceivedAsInterm](title="num msgs rx as interm node"; source=sum(numRxAsIntermSignal); record=stats,vector);

        @statistic[uScore1](title="utility score of node 1"; source="uScore1Signal"; record=stats,vector; );
        @statistic[uScore2](title="utility score of node 2"; source="uScore2Signal"; record=stats,vector; );
        @statistic[uScore3](title="utility score of node 3"; source="uScore3Signal"; record=stats,vector; );
        
        @statistic[intermNode](title="interm node idx"; source=intermNodeSignal; record=stats,vector; );
        
        @statistic[numDrIn](title="inQueue num msgs dropped"; source=sum(numDrInQSignal); record=stats,vector; );
        @statistic[numDrOut](title="outQueue num msgs dropped"; source=sum(numDrOutQSignal); record=stats,vector; );
        @statistic[inQSize](title="inQueue size"; source=inQSizeSignal; record=stats,vector; );
        @statistic[outQSize](title="outQueue size"; source=outQSizeSignal; record=stats,vector; );
        
        @statistic[rate1](title="rate to peer 1"; source="rate1Signal"; record=stats,vector; );
        @statistic[rate2](title="rate to peer 2"; source="rate2Signal"; record=stats,vector; );


    gates:
        inout gate[];
}

network MaxMinRC
{
    types:
        channel Channel extends ned.DelayChannel
        {
            delay = 10ms;
        }

    submodules:
        node[4]: RcNode;
    connections:
        //        peer[i] <--> node.peer++ for i=0..sizeof(peer)-1;

        for i=0..sizeof(node)-1, for j=i..sizeof(node)-1, if i!=j {
            node[i].gate++ <--> Channel <--> node[j].gate++;
        }

}

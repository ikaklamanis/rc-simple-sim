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

		@signal[bdInRateSignal](type="float");  
		@signal[bdOutRateSignal](type="float");  
		@signal[roptSignal](type="float");
		@signal[minRxRateSignal](type="float");

        @signal[numRxSignal](type="int");
        @signal[rxRateSignal](type="float");      
        @signal[numRxAsIntermSignal](type="int");
        @signal[rxRateAsIntermSignal](type="float");       
        @signal[intermNodeSignal](type="int");      
        @signal[numDrInQSignal](type="int");
        @signal[numDrOutQSignal](type="int");
        @signal[inQSizeSignal](type="int");
        @signal[outQSizeSignal](type="int");
              
        @signal[utilScore*](type="float");
        @signal[peerRate*](type="float");

		@statistic[bdInRate](title="bandwidth ingress rate"; source=bdInRateSignal; record=stats,vector; );
		@statistic[bdOutRate](title="bandwidth egress rate"; source=bdOutRateSignal; record=stats,vector; );
		@statistic[optRate](title="theory optimal rate"; source=roptSignal; record=stats,vector; );
		@statistic[minRxRate](title="minimum receiving rate"; source=rxRateSignal; record=stats,vector; );
		

        @statistic[numReceived](title="num msgs rx"; source=sum(numRxSignal); record=stats,vector; );
        @statistic[rxRate](title="msgs rx rate"; source=rxRateSignal; record=stats,vector; );
        @statistic[numReceivedAsInterm](title="num msgs rx as interm node"; source=sum(numRxAsIntermSignal); record=stats,vector);
        @statistic[rxRateAsInterm](title="msgs rx rate as interm"; source=rxRateAsIntermSignal; record=stats,vector; );      
        @statistic[intermNode](title="interm node idx"; source=intermNodeSignal; record=stats,vector; );
        @statistic[numDrIn](title="inQueue num msgs dropped"; source=sum(numDrInQSignal); record=stats,vector; );
        @statistic[numDrOut](title="outQueue num msgs dropped"; source=sum(numDrOutQSignal); record=stats,vector; );
        @statistic[inQSize](title="inQueue size"; source=inQSizeSignal; record=stats,vector; );
        @statistic[outQSize](title="outQueue size"; source=outQSizeSignal; record=stats,vector; );
        
        @statisticTemplate[uScore](record=stats,vector; );        
        @statisticTemplate[pRate](record=stats,vector; );  
       

    gates:
        inout gate[];
}

network MaxMinRC
{
    parameters:
        int numNodes = default(4);
    
    types:
        channel Channel extends ned.DelayChannel
        {
            delay = 10ms;
        }

    submodules:
        node[numNodes]: RcNode;
    connections:
        //        peer[i] <--> node.peer++ for i=0..sizeof(peer)-1;

        for i=0..sizeof(node)-1, for j=i..sizeof(node)-1, if i!=j {
            node[i].gate++ <--> Channel <--> node[j].gate++;
        }

}

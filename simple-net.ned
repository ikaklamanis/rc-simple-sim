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
    gates:
        inout gate[];
}

network MaxMinRC
{
    types:
        channel Channel extends ned.DelayChannel {
            delay = 100ms;
        }

    submodules:
        node[4]: RcNode;
    connections:
        node[0].gate++ <--> Channel <--> node[1].gate++;
        node[0].gate++ <--> Channel <--> node[2].gate++;
        node[0].gate++ <--> Channel <--> node[3].gate++;
        node[1].gate++ <--> Channel <--> node[2].gate++;
        node[1].gate++ <--> Channel <--> node[3].gate++;
        node[2].gate++ <--> Channel <--> node[3].gate++;
}

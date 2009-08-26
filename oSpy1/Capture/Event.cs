﻿//
// Copyright (c) 2009 Ole André Vadla Ravnås <oleavr@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

using System;
using System.Net;

namespace oSpy.Capture
{
    [Serializable]
    public class Event
    {
        [Serializable]
        public struct ExecutionOrigin
        {
            public string ProcessName;
            public uint ProcessId;
            public uint ThreadId;

            public ExecutionOrigin(string processName, uint processId, uint threadId)
            {
                this.ProcessName = processName;
                this.ProcessId = processId;
                this.ThreadId = threadId;
            }
        }

        [Serializable]
        public struct InvocationOrigin
        {
            public string FunctionName;
            public string Backtrace;
            public uint ResourceId;

            public InvocationOrigin(string functionName, string backtrace, uint resourceId)
            {
                this.FunctionName = functionName;
                this.Backtrace = backtrace;
                this.ResourceId = resourceId;
            }
        }

        private TimeSpan timestamp;
        private ExecutionOrigin executionOrigin;
        private InvocationOrigin invocationOrigin;

        private PacketDirection direction = PacketDirection.PACKET_DIRECTION_INVALID;
        private IPEndPoint localEndpoint;
        private IPEndPoint peerEndpoint;
        private byte[] data;

        public TimeSpan Timestamp
        {
            get
            {
                return timestamp;
            }
        }

        public string ProcessName
        {
            get
            {
                return executionOrigin.ProcessName;
            }
        }

        public uint ProcessId
        {
            get
            {
                return executionOrigin.ProcessId;
            }
        }

        public uint ThreadId
        {
            get
            {
                return executionOrigin.ThreadId;
            }
        }

        public string FunctionName
        {
            get
            {
                return invocationOrigin.FunctionName;
            }
        }

        public string Backtrace
        {
            get
            {
                return invocationOrigin.Backtrace;
            }
        }

        public uint ResourceId
        {
            get
            {
                return invocationOrigin.ResourceId;
            }
        }

        public PacketDirection Direction
        {
            get
            {
                return direction;
            }

            set
            {
                direction = value;
            }
        }

        public IPEndPoint LocalEndpoint
        {
            get
            {
                return localEndpoint;
            }

            set
            {
                localEndpoint = value;
            }
        }

        public IPEndPoint PeerEndpoint
        {
            get
            {
                return peerEndpoint;
            }

            set
            {
                peerEndpoint = value;
            }
        }

        public byte[] Data
        {
            get
            {
                return data;
            }

            set
            {
                data = value;
            }
        }

        protected Event(EventFactory factory, InvocationOrigin invocationOrigin)
        {
            this.timestamp = factory.DurationNow();
            this.executionOrigin = factory.ExecutionOriginHere();
            this.invocationOrigin = invocationOrigin;
        }
    }
}

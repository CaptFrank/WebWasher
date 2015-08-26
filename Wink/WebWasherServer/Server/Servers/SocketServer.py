"""

    :SocketServer.py:
    ==========

    :This is the base module that receives the following
    types of messages:

        - Heartbeat signals
        - Echo signals

    Both those signals are aggregated together and simply
    processed

    :

    :copyright: (c) 8/19/2015 by fpapinea.
    :license: BSD, see LICENSE for more details.

    Author:         fpapinea
    Version:        :version: #TODO
    Date:           8/19/2015
"""

"""
=============================================
Imports
=============================================
"""

import socketserver
from WebWasherServer.config import *
from WebWasherServer.Server.Server import Server
from WebWasherServer.Storage.Redis import RedisStorage


"""
=============================================
Source
=============================================
"""

class TCPSocketServerHandler(
    socketserver.BaseRequestHandler,
    Server
):
    """
    This is the handler class for our socket server.

    This is the handler that will parse out the heartbeat signals
    from the sensors that are alive and running in the home.
    """

    # ===============
    # Override Params
    # ===============

    # Sensor list that are active
    _sensors        = dict()

    # The request queues
    _queue          = RedisStorage()

    # Handler type
    _type           = "RAW"

    def handle(self):
        """
        The default handling method that is invoked when received
        a new message on the interface.

        :return:
        """

        # Receive data
        request = self.request.recv(1024)

        # Add the request to the queue
        self._queue.append(self._type, request)
        print("[+] Adding an entry in the db: {%s : %s}"
              % (self._type, request))

        # If debug is set
        if DEBUG:
            print("[+] Received: %s" % request)
        return


class TCPSocketServer(
    socketserver.ThreadingTCPServer
):
    """
    This is the high level TCP socket server that is threaded.
    It handles all the data that is sent via raw sockets.

    i.e. Heartbeat signals and echos.
    """

    # Daemonize the threads
    daemon_threads          = True

    # Much faster rebind
    allow_reuse_address     = True

    def __init__(self, server_address, cls):
        """
        This is the default constrcutor for the socket server
        class object.

        :param server_address:      The server address
        :param cls:                 The handler class
        :return:
        """

        # Override the class
        socketserver.ThreadingTCPServer.__init__(self, server_address, cls)
        print("[+} Created a new TCPSocketServer class.")
        return
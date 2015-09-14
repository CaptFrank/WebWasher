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
from queue import Queue
from threading import Thread
from WebWasherServer.config import *
from WebWasherServer.Server.Server import Server

"""
=============================================
Constants
=============================================
"""

RAW_TYPE                        = "RAW"

"""
=============================================
Source
=============================================
"""

class TCPSocketServerHandler(
    socketserver.BaseRequestHandler
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
    queue          = None

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
        self.queue.put(request)
        print("[+] Received: %s" % request)
        return


class TCPSocketServer(
    socketserver.ThreadingTCPServer,
    Server
):
    """
    This is the high level TCP socket server that is threaded.
    It handles all the data that is sent via raw sockets.

    i.e. Heartbeat signals and echos.
    """

    # Process alive
    _alive                  = True

    # Daemonize the threads
    daemon_threads          = True

    # Much faster rebind
    allow_reuse_address     = True

    # Application queue reference
    queue                   = Queue()

    # Server Thread
    thread                  = None

    # Process data queue
    data                    = None

    def __init__(self, server_address):
        """
        This is the default constrcutor for the socket server
        class object.

        :param manager:             The data manager handle
        :param server_address:      The server address
        :return:
        """

        # Override the class
        socketserver.ThreadingTCPServer.__init__(
            self, server_address, TCPSocketServerHandler
        )
        print("[+} Created a new TCPSocketServer class.")

        # Setting the storage attribute in the handler
        self.RequestHandlerClass.queue = self.queue

        # Override the server class
        Server.__init__(self, RAW_TYPE)

        # Setup the list for the data
        self.data = self.get_q("RX")
        return

    def run(self):
        """
        Runs the server as a daemon.

        :return:
        """

        # Make the server a thread
        self.thread = Thread(target=self.serve_forever)
        self.thread.daemon = True
        self.thread.start()

        # We run until killed
        while self._alive:

            # Publish the requests to the manager
            item = self.queue.get(block=True)
            if item:
                self.data.set(item)
                print("[+] Sent request to manager.")
        self.kill()
        return

    def kill(self):
        """
        Kills the app.

        :return:
        """

        # Kill server
        self._alive = False
        self.thread.join()
        self.join()
        return
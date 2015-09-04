"""

    :Publisher:
    ==========

    :This is the publishing task. It takes
    the commands inputted through the command
    socket and sends them through to the broker
    based on the topic and the request.:

    :copyright: (c) 8/31/2015 by fpapinea.
    :license: BSD, see LICENSE for more details.

    Author:         fpapinea
    Version:        :version: #TODO
    Date:           8/31/2015
"""

"""
=============================================
Imports
=============================================
"""

import zmq
from ..config import *
from threading import Thread

"""
=============================================
Source
=============================================
"""

class Publisher(Thread):
    """
    This class is an object that sets up a server on the
    defined port in the configuration. Once the server
    is started, it listens to subscribers. (i.e. applications,
    clients, cli, ect.)
    """

    # Context object for the zmq library
    __context       = None

    # Server Configs
    __configs       = {
        "address"       : COMMAND_SERVER_ADDRESS,
        "port"          : COMMAND_SERVER_PORT,
        "protocol"      : COMMAND_SERVER_PROTOCOL
    }

    # Needed command queues
    __queues        = COMMAND_QUEUES

    # Needed IO Threads
    __threads       = COMMAND_QUEUE_THREADS

    # The socekt handle
    __socket        = None

    def __init__(self):
        """
        This is the default constructor to the class. It
        sets up the server attributes and spins up the server, based
        on the topic identified within the configs.

        :return:
        """

        # We create a context object
        self.__context = zmq.Context(
            io_threads=self.__threads
        )

        # We create a socket to connect to
        #TODO
        self.__socket = self.__context.socket(zmq.SUB)

        # Override the thread class
        Thread.__init__(self)
        return

    def setup(self):
        """
        Sets up the subscriber queues.

        :return:
        """

        # Socket ok?
        if not self.__socket:
            return False

        # We setup the queues

        return True
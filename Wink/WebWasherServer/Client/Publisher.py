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

    # The socket handle
    __sockets       = list()

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

        # Override the thread class
        Thread.__init__(self)
        return

    def setup(self):
        """
        Sets up the subscriber queues and connects to the subscribed brokers.

        :return:
        """

        # Socket ok?
        if not self.__sockets:
            return False

        # If the sockets are good we can create
        # threads in a dict of threads
        for item in self.__queues:

            # We create a pub socket
            item['pub'] = self.__context.socket(zmq.PUB)

            # Bind to the address
            item['pub'].connect(
                "{protocol}{address}:{port}".format(
                    **self.__configs
                )
            )
            print(
                '[+] Connected {name} queue: {protocol}{address}:{port}'.format(
                    name=item['name'],
                    protocol=self.__configs['protocol'],
                    address=self.__configs['address'],
                    port=self.__configs['port']
                )
            )

            # Create a subscriber to subscribe to the external applications
            # We create a pub socket
            item['sub'] = self.__context.socket(zmq.SUB)
            item['sub'].setsockopt(zmq.SUBSCRIBE, item['name'])

            # Bind to the address
            item['sub'].connect(
                "{protocol}localhost:{port}".format(
                    **item
                )
            )
            print(
                '[+] Subscriber listening to: {protocol}localhost:{port}'.format(
                    **item
                )
            )
        return True

    def run(self):
        """
        This method looks at the following queues to
        determine the coarse of action of the application.

            - Client Command Line
                - Contains the following types of messages

            - System Command Queue
            - Application Command Queue

        This method also spawns the number of threads needed for each
        queue.

            - reboot        + service / driver
                - REBOOT_SYS_SOFT   = 0
                - REBOOT_SYS_HARD   = 1
                - REBOOT_COMS       = 2
                - REBOOT_MQTT       = 3
                - REBOOT_WIFI       = 4
                - REBOOT_OS         = 5
                - REBOOT_SCHEDULER  = 6
                - REBOOT_TASK       = 7

            - restart       + true / false
            - suspend       + thread id
            - disconnect    + interface
                - INTERFACE_WIFI    = 0
                - INTERFACE_MQTT    = 1
                - INTERFACE_BOTH    = 2

            - status        + true / false
            - selftest      + true / false
            - get           + data type
                - MSG_HEARTBEAT     = 0
                - MSG_STATUS        = 1
                - MSG_TEMP_DATA     = 2
                - MSG_ACC_DATA      = 3
                - MSG_OTHER         = 4
            - echo
                - MSG

        :return:
        """

        # We spawn the threads and set the queues

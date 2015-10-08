"""

    :Server:
    ==========

    :This is the base class for a server type that is
    created in the wink framework:

    :copyright: (c) 8/26/2015 by fpapinea.
    :license: BSD, see LICENSE for more details.

    Author:         fpapinea
    Version:        :version: #TODO
    Date:           8/26/2015
"""

"""
=============================================
Imports
=============================================
"""

from multiprocessing import Process
from WebWasherServer.Base.Manager import DataClient

"""
=============================================
Constants
=============================================
"""

QUEUES      = {
    "TX"    : {
        "MQTT"  : "get_MqttTxBuffer",
        "RAW"   : "get_RawTxBuffer",
    },
    "RX"    : {
        "MQTT"  : "get_MqttRxBuffer",
        "RAW"   : "get_RawRxBuffer",
    }
}

"""
=============================================
Source
=============================================
"""

class Server(Process):
    """
    This is the base class for a Server Object type.
    We use this class as an interface to the Server object type.
    """

    # The Queue
    queue              = None

    # The server type
    _type               = ""

    # The Storage option
    _storage            = None

    # The Manager handle
    _manager            = DataClient()

    def __init__(self, type):
        """
        This constructs a new Server object with the server
        type attribute and the server storage attribute.

        :param type:            The server type
        :return:
        """

        # Assign the attributes locally
        self._type = type
        Process.__init__(self, name=type)
        return

    def get_q(self, dir):
        """
        Gets the queue that corresponds to the object type,
        and the direction.

        :param dir:             The object direction
        :return:
        """
        return QUEUES[self._type][dir]




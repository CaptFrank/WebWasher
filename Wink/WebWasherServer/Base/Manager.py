"""

    :Manager:
    ==========

    :This is the manager object that handles
    all the data that is passed between the processes.
    We use this object as a prosy between 2 processes.
    :

    :copyright: (c) 9/10/2015 by fpapinea.
    :license: BSD, see LICENSE for more details.

    Author:         fpapinea
    Version:        :version: #TODO
    Date:           9/10/2015
"""

"""
=============================================
Imports
=============================================
"""

from WebWasherServer.config import *
from WebWasherServer.Base.Queue import Buffer
from multiprocessing.managers import SyncManager
from WebWasherServer.Base.Singleton import Singleton

"""
=============================================
Source
=============================================
"""

class DataManager(
    SyncManager,
    Singleton
):
    """
    This is the base class object for the data manager
    that handles the data interactions between the subprocess
    that are running within this application.
    """

    # ============
    # Attributes
    # ============

    __address               = None

    # ============
    # Buffers
    # ============

    __MqttRxBuffer          = Buffer(
        name="MqttRxBuffer", type="MQTT"
    )
    __MqttTxBuffer          = Buffer(
        name="MqttTxBuffer", type="MQTT"
    )
    __RawTxBuffer           = Buffer(
        name="RawTxBuffer", type="RAW"
    )
    __RawRxBuffer           = Buffer(
        name="RawRxBuffer", type="RAW"
    )

    def __init__(self, address=MANAGER_ADDRESS, pwd=MANAGER_PASSWORD):
        """
        This is the constructor for the class object that takes
        both the managers address and password.

        :param address:     The address of the manager server
        :param pwd:         The managers password access
        :return:
        """

        # Override the base class
        SyncManager.__init__(address, pwd)
        Singleton.__init__(self)

        # Register the classes needed
        self.register('get_MqttRxBuffer',   callable=lambda: self.__MqttRxBuffer)
        self.register('get_MqttTxBuffer',   callable=lambda: self.__MqttTxBuffer)
        self.register('get_RawRxBuffer',    callable=lambda: self.__RawTxBuffer)
        self.register('get_RawTxBuffer',    callable=lambda: self.__RawRxBuffer)
        return

    def run(self):
        """
        Starts the manager process

        :return:
        """

        self.start()
        print("[+] Manager process startedS:{address}"
              .format(address=self.address))
        return

class DataClient(
    SyncManager
):
    """
    This is the base class object for the data manager client
    that handles the data interactions between the subprocess
    that are running within this application.
    """

    def __int__(self, address=MANAGER_ADDRESS, authkey=MANAGER_PASSWORD):
        """
        This is the default constructor for the
        class that registers the valid methods that
        can be called in the manager context.

        :param address:         The address as a tuple (address, port)
        :param auth:            The authentication for the server
        :return:
        """

        self.register("get_MqttRxBuffer")
        self.register("get_MqttTxBuffer")
        self.register("get_RawRxBuffer")
        self.register("get_RawTxBuffer")

        # Override the base class
        SyncManager.__init__(address, authkey)
        return

    def run(self):
        """
        This is the default running method that takes the address
        of the server and the password of the manager class.

        :return:
        """

        # Connect
        self.connect()
        print("[+] Connected to the manager server: {address}"
              .format(address=self.address))

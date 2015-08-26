"""

    :MqttServer:
    ==========

    :This is the MQTT server interface that gets the
    write requests from the sensors and validates them
    to then put them into the database.

    This class takes in the write requests sanitizes them
    and validates them to then input them into the db.

    The write requests are listed below:

        - Accelerometer values
        - Temperature values
        - Status values
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

from WebWasherServer.config import *
from WebWasherServer.Storage.Redis import RedisStorage
from WebWasherServer.Server.Server import Server
from paho.mqtt.client import Client

"""
=============================================
Source
=============================================
"""

class MqttServer(
    Client,
    Server
):
    """
    This is the base class for the MQTT client. We use this class object
    as the MQTT client that will serve us the validated and sanitized write
    requests from the broker.

    extends: MQTT::Client
    """

    # Connection configurations
    _configs  = dict()

    def __init__(self, name=None, config=None):
        """
        This creates a new client object that extends the Client class.
        We pass in the name of the client that will connect to the broker.

        The configs are structured as follows:

            {
                com     :   {
                    'host'      : <name>,
                    'port'      : <port>,
                    'alive'     : <int>
                },
                sub     :   [
                    'sensor/+/data/temp',
                    'sensor/+/data/acc',
                    'sensor/+/status'
                ],
            }

        :param name:            The client name
        :param config:          The connection configs
        :return:                A constructed object
        """

        # Set configs
        if config:
            self._configs = config

        # Override the base classes
        Client.__init__(self, name)
        Server.__init__(self, "MQTT", RedisStorage())
        print("[+] Created a new MQTTServer object.")
        return

    def setup(self):
        """
        This method sets up the callbacks and the connection agents as well
        as subscribes to the topics that are important to the system.

        We connect to the broker and subscribe
        :return:
        """

        # Connect
        print("[+] Connecting to the broker.")
        self.connect(
            self._configs['com']['host'],
            self._configs['com']['port'],
            self._configs['com']['alive']
        )
        print("[+] Connected...")

        # Subscribe
        for item in self._configs['sub']:
            self.subscribe(item)
            print("[+] Subscribing to %s." %item)
        return

    def run(self):
        """
        This method connects the client to the specified broker and
        runs the loop thread.
        :return:
        """

        # Start the handler thread
        #self.loop_start()
        return

    def stop(self):
        """
        This method kills the handler thread and kills the client.
        :return:
        """

        # Kill the thread
        self.loop_stop(force=True)

        # Disconnect the client
        self.disconnect()
        return

    # Callbacks
    def on_connect(self):

        return

    def on_disconnect(self):

        return

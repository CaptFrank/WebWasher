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
from paho.mqtt.client import Client

from WebWasherServer.Server.Server import Server


"""
=============================================
Constants
=============================================
"""

MQTT_TYPE                       = "MQTT"

# MQTT server states
MQTT_SERVER_STATE_INIT          = 1
MQTT_SERVER_STATE_SETUP         = 2
MQTT_SERVER_STATE_RUN           = 3
MQTT_SERVER_STATE_DEAD          = 4

# MQTT connection states
MQTT_SERVER_CONNECTION_ALIVE    = 1
MQTT_SERVER_CONNECTION_DEAD     = 2

# Shortcuts
ALIVE                           = MQTT_SERVER_CONNECTION_ALIVE
DEAD                            = MQTT_SERVER_CONNECTION_DEAD

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
    _configs    = dict()

    # State
    _state      = None

    # Connection
    _conn       = None

    # Subscriptions
    _subs       = []

    # Thread alive
    _alive      = True

    # Queue reference
    queue       = None

    # The data queue reference
    data        = None

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
        Server.__init__(self, MQTT_TYPE)

        self._set_state(MQTT_SERVER_STATE_INIT)
        self.data = self.get_q("RX")

        print("[+] Created a new MQTTServer object.")
        return

    def setup(self):
        """
        This method sets up the callbacks and the connection agents as well
        as subscribes to the topics that are important to the system.

        We connect to the broker and subscribe
        :return:
        """

        # Check the state machine
        if not self._check_state(
                        MQTT_SERVER_STATE_SETUP - 1):
            return False

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
            self._subs.append(item)
            print("[+] Subscribing to %s." %item)

        print("[+] Installing the callback hooks.")
        self.on_connect     = self._on_connect
        self.on_disconnect  = self._on_disconnect
        self.on_message     = self._on_message
        self.on_publish     = self._on_publish
        self.on_subscribe   = self._on_subscribe
        self.on_unsubscribe = self._on_unsubscribe

        if DEBUG:
            print("[+] Installing the log mechanism.")
            self.on_log     = self._on_log

        self._set_state(MQTT_SERVER_STATE_SETUP)
        return True

    def run(self):
        """
        This method connects the client to the specified broker and
        runs the loop thread.
        :return:
        """

        # Check the state machine
        if not self._check_state(
                        MQTT_SERVER_STATE_RUN -1):
            return False
        elif not self._check_cxn_status():
            return False

        # Start the handler thread
        self._set_state(MQTT_SERVER_STATE_RUN)
        self.loop_start()

        # Start the collector thread
        while self._check_state(ALIVE):

            """
            We need to take the entries in the queue and
            put them into the Redis Queue.
            """

            global __queue
            if __queue:

                # Get the data piece
                item = __queue.get(block=True)
                if item:

                    # Put the entry in the redis Queue
                    self.data.set(item['message'])

            return

    def stop(self):
        """
        This method kills the handler thread and kills the client.
        :return:
        """

        # Check the state machine
        if not self._check_state(
                        MQTT_SERVER_STATE_DEAD -1):
            return False

        # Kill the thread
        self.loop_stop(force=True)

        # Disconnect the client
        self.disconnect()

        if self._check_cxn_status():
            return False

        self._set_state(MQTT_SERVER_STATE_DEAD)
        return True

    # ==============
    # Callbacks
    # ==============

    def _on_connect(self):
        """
        This method is called when there is a new connection
        that is spawned.
        """

        self._set_cxn_status(ALIVE)
        print("[+] Connection ACK received.")
        return

    def _on_disconnect(self):
        """
        This method is called when there is a disconnect request
        broadcasted.
        """

        self._set_cxn_status(DEAD)
        print("[+] Disconnection ACK received.")
        return

    @staticmethod
    def _on_message(client, userdata, message, cls):
        """
        This method is invoked when there is a new message in the
        receive buffer.
        """

        # We add the message to the message queue.
        queue = cls.get_q()
        if queue:
            queue.put(
                {
                    "type"      : MQTT_TYPE,
                    "data"      : {
                        "client"    : client,
                        "data"      : userdata,
                        "message"   : message
                    }
                },
                block=True
            )
        return

    @staticmethod
    def _on_publish(client, userdata, mid):
        """
        This method is invoked when there is a message publish
        request.
        """

        print(
            "[+] Client: %s published a message with mid: %s"
            % (str(client), str(mid))
        )
        print(
            "[+] Message: %s"
            % str(userdata)
        )
        return

    @staticmethod
    def _on_subscribe(client, userdata, mid, qos):
        """
        This method is invoked when there is a subscribe request
        that is broadcasted.
        """

        print(
            "[+] Subscribed to: %s\r"
            "With qos: %s\r"
            "From client: %s\r"
            "Message Id: %s"
            % (str(userdata), str(qos), str(client), str(mid))
        )
        return

    @staticmethod
    def _on_unsubscribe(client, userdata, mid):
        """
        This is method is invoked when there is an unsubscribe
        request that is broadcasted.
        """

        print(
            "[+] Unsubscribed to: %s\r"
            "From client: %s\r"
            "Message Id: %s"
            % (str(userdata), str(client), str(mid))
        )
        return

    @staticmethod
    def _on_log(client, userdata, level, buf):
        """
        This method is only invoked when there is debug requests and
        when the DEBUG config is enabled.
        """

        print(
            "[+] Debug Log from client: %s:\r"
            "\t - data: %s\r"
            "\t - level: %s\r"
            "\t - buffer: %s\r"
            % (client, userdata, level, buf)
        )
        return

    # ==============
    # State Machine
    # ==============

    """
    The following Methods correspond to the internal state property
    of the class. We use these properties as a control to the MQTT server.
    It serves as a blocking mechanism when there is a set process required before
    another.
    """

    def _set_state(self, state):
        self._state = state
        return

    def _get_state(self):
        return self._state

    def _check_state(self, check):
        if self._get_state() == check:
            return True
        else:
            return False

    # The private access property
    __state = property(_get_state, _set_state)

    # ==============
    # Connection
    # ==============

    """
    The following Methods correspond to the internal connection state property
    of the class. We use these properties as a control to the MQTT server.
    It serves as a blocking mechanism when there is a set process required before
    another.
    """

    def _set_cxn_status(self, state):
        self._state = state
        return

    def _get_cxn_status(self):
        return self._state

    def _check_cxn_status(self):
        if self._get_state() == MQTT_SERVER_CONNECTION_ALIVE:
            return True
        else:
            return False

    # The private access property
    __cxn = property(_get_cxn_status, _set_cxn_status)

    # ==============
    # Queue
    # ==============

    """
    The following Methods correspond to the internal queue property
    of the class. We use these properties as a control to the MQTT server.
    It serves as a blocking mechanism when there is a set process required before
    another.
    """

    def _set_q(self, q):
        self.queue = q
        return

    def _get_q(self):
        return self.queue

    # The private access property
    __queue = property(_get_q, _set_q)

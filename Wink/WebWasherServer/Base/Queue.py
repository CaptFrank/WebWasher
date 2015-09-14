"""

    :Queue:
    ==========

    :The queue class object that contains the
    queues attributes and their respective
    operating methods.:

    :copyright: (c) 8/18/2015 by fpapinea.
    :license: BSD, see LICENSE for more details.

    Author:         fpapinea
    Version:        :1.0:
    Date:           8/18/2015
"""

"""
=============================================
Constants
=============================================
"""

from queue import Queue

"""
=============================================
Source
=============================================
"""

class Buffer(Queue):
    """
    This is the command queue class object that contains the
    methods needed to queue up commands for system, command
    line and application commands.
    """

    # Queue Type
    __type              = None

    # Queue name
    __name              = None

    def __init__(self, name, type):
        """
        This is the default constructor for the class.

        :param name:        The name of the queue
        :param type:        The type of the queue
        :return:
        """

        # Override the base class
        Queue.__init__(self)

        # Set internals
        self.__name = name
        self.__type = type
        return

    def set(self, kwargs):
        """
        Sets the queues attributes.

        :param args:        The arguments
        :return:
        """

        self.put(kwargs, block=True)
        return

    def retrieve(self):
        """
        Get the queue value

        :return:            The param popped off.
        """
        return self.get()

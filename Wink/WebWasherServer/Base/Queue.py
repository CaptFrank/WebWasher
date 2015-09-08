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


"""
=============================================
Source
=============================================
"""

class CommandQueue:
    """
    This is the command queue class object that contains the
    methods needed to queue up commands for system, command
    line and application commands.
    """

    # Queue Type
    __type              = None

    # Queue Size
    __size              = None

    # Queue Fetch Timeout
    __timeout           = None

    def __init__(self):
        """
        This is the default constructor for the class.
        :return:
        """
        return

    def set(self, *args):
        """
        Sets the queues attributes.

        :param args:        The arguments
        :return:
        """

        return
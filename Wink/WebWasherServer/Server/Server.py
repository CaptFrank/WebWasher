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

"""
=============================================
Source
=============================================
"""

class Server:
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

    def __init__(self, type, storage):
        """
        This constructs a new Server object with the server
        type attribute and the server storage attribute.

        :param type:            The server type
        :param storage:         The server storage class
        :return:
        """

        # Assign the attributes locally
        self.queue = storage
        self._type = type
        return

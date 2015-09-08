"""

    :Redis:
    ==========

    :This is the wrapper to the base mongodb library.:

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

import os
import uuid
from pymongo import MongoClient

from .Base.Singleton import Singleton

"""
=============================================
Source
=============================================
"""

class MongoStorage(Singleton):
    """
    This is the wrapper class object that wraps the
    base mongo interface to a more comprehensive interface.
    This is also a Singleton design pattern.
    """

    # The connection
    _mongo      = None

    # The count
    _count      = 0

    def __init__(self):

        # Init the Redis
        os.system("mongod &")

        # Connect to the server
        self._mongo = MongoClient()

        # Override the singleton base class
        Singleton.__init__(self)
        return

    def append(self, type, request):
        """
        This is the append method that will insert a new row.

        :param type:            The request type
        :param request:         The request itself
        :return: count          The item count
        """

        # Generate a tag
        tag = ":" + str(uuid.uuid4())

        # Add it
        self._mongo[type].insert_one(
            {
                'tag'       :   tag,
                'request'   :  request
            }
        )

        # Get the new count to validate
        return self._mongo[type].count()

    def consume(self):
        """
        This gets an entry from the redis database.
        :return:                The request
        """

        # Get an entry to process
        obj = self._redis.dequeue()

        # Update the count
        self._count = self._redis.count

        # Return the object
        return obj

    def count(self):
        """
        This is the count generator

        :return:
        """
        return self._count

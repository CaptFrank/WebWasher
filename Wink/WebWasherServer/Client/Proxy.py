"""

    :name:
    ==========

    :This is the applciations proxy to the
    publisher class. This class instantiates a
    proxy object that will map the request queue
    to a request worker.

    Each worker will then process the request and
    send the command to the borker. Once the borker
    receives the data, the mqtt server/client interface
    will input the result into the REDIS database, where we
    can access it.
    :

    :copyright: (c) 9/8/2015 by fpapinea.
    :license: BSD, see LICENSE for more details.

    Author:         fpapinea
    Version:        :version: #TODO
    Date:           9/8/2015
"""

"""
=============================================
Imports
=============================================
"""

"""
=============================================
Constants
=============================================
"""

# Program Attributes
__author__ = 'fpapinea'
__version__ = ""  #TODO
__date__ = "9/8/2015"

"""
=============================================
Variables
=============================================
"""

"""
=============================================
Source
=============================================
"""
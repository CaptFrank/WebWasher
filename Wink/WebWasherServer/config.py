"""

    :name:
    ==========

    :description:

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

"""
=============================================
Constants
=============================================
"""

# Program Attributes
__author__                  = 'fpapinea'
__version__                 = '1.0'
__date__                    = '8/19/2015'

# Debug the system?
DEBUG                       = True

# The command server attributes
COMMAND_SERVER_ADDRESS      = ''
COMMAND_SERVER_PORT         = 5555
COMMAND_SERVER_PROTOCOL     = 'tcp://'

COMMAND_QUEUE_THREADS       = 2
COMMAND_QUEUES              = [

    # Name,     Size,  Timeout
    (
        "CLI",  255,    None,
    ),
    (
        "SYS",  255,    None,
    ),
    (
        "APP",  255,    None,
    )
]

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
BASE_PORT                   = 4000

# Queue types
QUEUE_TYPE_PUB              = 1
QUEUE_TYPE_SUB              = 2
QUEUE_TYPE_PUBSUB           = QUEUE_TYPE_PUB + QUEUE_TYPE_SUB

# The command server attributes
COMMAND_SERVER_ADDRESS      = 'broker.haligonia.home.com'
COMMAND_SERVER_PORT         = 5555
COMMAND_SERVER_PROTOCOL     = 'tcp://'

COMMAND_QUEUE_THREADS       = 2
COMMAND_QUEUES              = [
    {
        'name'        :   "CLI",
        'size'        :   255,
        'port'        :   BASE_PORT,
        'protocol'    :   'tcp://',
        'timeout'     :   None,
    },
    {
        'name'        :   "SYS",
        'size'        :   255,
        'port'        :   BASE_PORT + 1,
        'protocol'    :   'tcp://',
        'timeout'     :   None,
    },
    {
        'name'        :   "APP",
        'size'        :   255,
        'port'        :   BASE_PORT + 2,
        'protocol'    :   'tcp://',
        'timeout'     :   None,
    },
]

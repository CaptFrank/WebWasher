"""

    :Push:
    ==========

    :This is the push task that will take the data from
    the application queue and insert it into the Redis
    queue.:

    :copyright: (c) 8/31/2015 by fpapinea.
    :license: BSD, see LICENSE for more details.

    Author:         fpapinea
    Version:        :version: #TODO
    Date:           8/31/2015
"""

"""
=============================================
Imports
=============================================
"""

from threading import Thread

from queue import Queue

"""
=============================================
Source
=============================================
"""

class Push(Thread):
    """
    This class object extends the Thread model.
    It is ran concurrently with the main tasks and
    the server tasks.
    """

    # The Queue handle
    __queue         = None

    # The Redis Queue handle
    __redis         = None

    # Alive
    __alive         = True

    def __init__(self, appQueue, redisQueue):
        """
        This sets up the push task with the handles needed
        to queue up data.

        :param appQueue:            The application blocking queue
        :param redisQueue:          The redis queue
        :return:
        """

        self.__redis = redisQueue
        self.__queue = appQueue

        # Override the thread
        Thread.__init__(self)
        return

    def run(self):
        """
        The default run method that runs the tasks needed
        to take the data and add it to the redis queue.

        :return:
        """



        # While the thread is alive
        while self.__alive:

            # Get the data
            data = self.__queue.get(block=True)

            # Put it into the redis queue.
            self.__redis.append(
                data["type"],
                data["data"]
            )
        return

    def kill(self):
        """
        Kills the thread
        :return:
        """

        self.__alive = False
        return

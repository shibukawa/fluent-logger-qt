fluent logger for Qt
====================

Fluent logger for Qt. It will work on Windows too.

Now this is an alpha stage. In future, it will move to more flexible packaging system.

Usage
-----

.. code-block:: sh

   $ cd <your cool application folder>
   $ git submodule add git@github.com:shibukawa/fluent-logger-qt.git
   # to install msgpack-c
   $ git submodule update --init --recursive --depth=1

Add ``msgpack-c.pri`` and ``fluent-logger-qt.pri`` to you ".pro" project file.

Thanks
------

QLogger code is based on `todayman's fluent-logger-cpp <https://github.com/todayman/fluent-logger-cpp>`_.

License
-------

Apatch-2

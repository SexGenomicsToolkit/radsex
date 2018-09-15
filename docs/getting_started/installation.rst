Installation
===============

Requirements
------------

* A C++11 compliant compiler (GCC >= 4.8.1, Clang >= 3.3)
* The zlib library (which should be installed on linux by default)


Installation
------------

RADSex can be installed from one of the release packages [TODO: link], or the latest stable development version can be installed directly from the GitHub repository.

.. _install-release:

Install the latest release
~~~~~~~~~~~~~~~~~~~~~~~~~~

TODO

Install from GitHub
~~~~~~~~~~~~~~~~~~~

To install the latest stable version of RADSex from the GitHub repository, run the following commands:

::

    git clone https://github.com/RomainFeron/RadSex.git
    cd RadSex
    make

The compiled **radsex** binary will be located in **RadSex/bin/**.


Update RADSex
-------------

To update RADSex, you can download the latest stable release and install it as described in :ref:`install-release`.

If you installed RADSex from Github, run the following commands in the RadSex directory:

::

    git pull
    make rebuild

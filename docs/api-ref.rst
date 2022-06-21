.. _api-ref:

API Reference
=============

The Arduino library API is implemented via the PASCO2Ino class.

XENSIV™ PAS CO2 Arduino API
---------------------------

.. doxygenclass:: PASCO2Ino
   :members:

Types
""""" 

Return Error Codes
^^^^^^^^^^^^^^^^^^

.. doxygentypedef:: Error_t 

.. doxygendefine:: XENSIV_PASCO2_OK
.. doxygendefine:: XENSIV_PASCO2_ERR_COMM
.. doxygendefine:: XENSIV_PASCO2_ERR_WRITE_TOO_LARGE
.. doxygendefine:: XENSIV_PASCO2_ERR_NOT_READY
.. doxygendefine:: XENSIV_PASCO2_ICCERR
.. doxygendefine:: XENSIV_PASCO2_ORVS
.. doxygendefine:: XENSIV_PASCO2_ORTMP
.. doxygendefine:: XENSIV_PASCO2_READ_NRDY

Dignosis 
^^^^^^^^

.. doxygentypedef:: Diag_t

.. doxygenunion:: xensiv_pasco2_status_t

Baseline Offset Compensation
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. doxygentypedef:: ABOC_t 

.. doxygenenum:: xensiv_pasco2_boc_cfg_t

XENSIV™ PAS CO2 C Reference API
-------------------------------

The Arduino library is wrapping the platform abstracted C library from `this project <https://github.com/Infineon/sensor-xensiv-pasco2>`_. 
Find out the complete C core library documentation `here <https://infineon.github.io/sensor-xensiv-pasco2/html/index.html>`_.
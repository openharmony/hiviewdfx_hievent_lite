# hievent Module<a name="EN-US_TOPIC_0000001078674782"></a>

-   [Overview](#section469617221261)
-   [Architecture](#section15884114210197)

## Overview<a name="section469617221261"></a>

This module provides APIs for printing faults, user behavior, and power consumption statistics of service components. It also supports event serialization.

## Architecture<a name="section15884114210197"></a>

1.  An event is created.
2.  Data is added for the event.
3.  The event is reported.
4.  The event component checks the parameter validity and performs binary serialization to convert the event into structured data.
5.  The event component calls the output function to write the event into a file. Each time a new file is written, the common header information is added to it.
6.  The upload component passively or proactively reports the event information to smartphones at a scheduled time.


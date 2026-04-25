.. _cluster_attributes:


Cluster Attributes
==================


.. option:: name [str]

    Name of the cluster. This attribute is used to identify the cluster and can be used for referencing the cluster in other attributes or commands.

.. option:: type [str]

    Type of the cluster. This attribute defines the functional role of the cluster, such as "logic", "memory", "DSP", etc. It can be used to categorize clusters and apply specific optimizations or constraints based on their type.

.. option:: input_pin_utils [float]

    Input pin utilization of the cluster. This attribute represents the ratio of used input pins to the total available input pins in the cluster. It can be used to assess the congestion and routing complexity of the cluster.

.. option:: output_pin_utils [float]

    Output pin utilization of the cluster. This attribute represents the ratio of used output pins to the total available output pins in the cluster. Similar to input pin utilization, it can be used to evaluate congestion and routing complexity.

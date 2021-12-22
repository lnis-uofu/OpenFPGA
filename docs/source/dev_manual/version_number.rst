.. dev_manual_version_number::

Version Number
==============

Convention
----------

OpenFPGA follows the `semantic versioning <www.semver.org>`_, where the version number is in the form of

.. code-block::

  [Major].[Minor].[Patch]

For example, version ``1.2.300`` denotes 

- One major milestone is achieved. 
- Two minor milestone is achieved after the major revision ``1.0.0``
- ``300`` patches has been applied after the minor revision ``1.2.0``

Version Update Rules
--------------------

.. warning:: Please discuss with maintainers before modifying major and minor numbers.

.. warning:: Please do not modify patch number manually.

To update the version number, please follow the rules:

- Major and minor version number are defined by maintainers
- Patch number is automatically updated through github actions. See detailed in the `workflow file <https://github.com/lnis-uofu/OpenFPGA/blob/master/.github/workflows/patch_updater.yml>`_

Version updates are made in the following scenario

- When a minor milestone is achieved, the minor revision number can be increased by 1. The following condition is considered as a minor milestone:
  - a new feature has been developed.
  - a critical patch has been applied.
  - a sufficient number of small patches has been applied in the past quarter. In other words, the minor revision will be updated by the end of each quarter as long as there are patches.

- When several minor milestones are achieved, the major revision number can be increased by 1. The following condition is considered as a major milestone:
  - significant improvements on Quality-of-Results (QoR).
  - significant changes on user interface.
  - a techical feature is developed and validated by the community, which can impact the complete design flow.



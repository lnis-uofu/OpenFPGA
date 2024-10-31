.. _developer_contributor_guidelines_general_rules:

General Rules
=============

Motivation
----------
Github projects involve many parties with different interests.
It is necessary to establish rules to

- guarantee the quality of each pull request by establishing a standard
- code review for each pull request is straightforward
- contributors have confidence when submitting changes

Create Pull requests
--------------------

- Contributors should state clearly their motivation and the principles of code changes in each pull request
- Contributors should be active in resolving conflicts with other contributors as well as maintainers. In principle, all the maintainers want every pull request in and are looking for reasons to approve it.
- Each pull request should pass all the existing tests in CI (See :ref:`developer_contributor_guidelines_checkin_system` for details). Otherwise, it should not be merged unless you get a waiver from all the maintainers.
- Contributors should not modify any codes/tests which are unrelated to the scope of their pull requests.
- The size of each pull request should be small. Large pull request takes weeks to be merged. The recommend size of pull request is up to 500 lines of codes changes. If you have one large file, this can be waived. However, the number of files to be changed should be as small as possible.

  .. note:: For large pull requests, it is strongly recommended that contributors should talk to maintainers first or create an issue on the Github. Contributors should clearly define the motivation, detailed technical plan as well as deliverables. Through discussions, the technical plan may be requested to change. Please do not start code changes blindly before the technical plan is approved.

- For any new feature/functionality to be added, there should be

  - Dedicated test cases in CI which validates its correctness
  - An update on the documentation, if it changes user interface
  - Provide sufficient code comments to ease the maintenance

.. _developer_contributor_guidelines_checkin_system:

Check-in System
---------------

.. seealso:: The check-in system is based on continous integration (CI). See details in :ref:`developer_ci` 

The check-in system aims to offer a standardized way to 

- ensure quailty of each contribution
- resolve conflicts between teams

It is designed for efficient communication between teams.

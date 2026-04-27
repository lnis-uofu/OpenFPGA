Query Format
============

Query a collection of objects to create a new subset while preserving the original in a runtime-efficient way. Most commands support a -query option during reporting. Conditional expressions combine relations with AND/OR operators and support parentheses. Relations compare an attribute name to a value using relational operators.


Relationship Operators
----------------------

.. code-block:: text

    ==   Equal
    !=   Not equal
    >    Greater than
    <    Less than
    >=   Greater than or equal to
    <=   Less than or equal to
    =~   Matches pattern
    !~   Does not match pattern

Query Operation Rules
----------------------

- String attributes support all operators
- Numeric attributes do not support pattern matching (=~, !~)
- Boolean attributes support only == and != with values true or false
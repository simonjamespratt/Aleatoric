\mainpage This is the main page.

Some more info.

\namespace actlib
\brief The main namespace for the Aleatoric library

Some more information about actlib

\namespace Numbers
\brief A set of classes for the production of random numbers.

The namespace is further sub-divided into Steps and Granular. Steps provides classes that output integers in a range. Granular provides classes that output fractions (doubles) within a range.

\namespace Steps
\brief A set of classes that outputs integers from a provided range.

Outputs distinct steps within a range which allows you certainty that the returned number will be one of these steps (integers). This is in contrast to the Granular set of classes which does not give that guarantee, but rather focuses on providing values at a level of granularity within the provided range.

\namespace Granular
\brief A set of classes that outputs fractions (doubles) from a provided range.

Outputs values at a more granular level from within a range. If you require an output of distinct steps (integers) from a range, use the Steps set of classes.

# Unit Commitment Model

This code implements a MILP model for a simple Unit Commitment Problem, using Cplex C++ API.

The MILP model implemented corresponds to a min-up/min-down Unit Commitment Problem with or without ramp constraints :

 * Minimization of the total production cost (including proportional and fixed production cost, and start-up costs), under :

  * demand constraints at each time step
  * production limits Pmin/Pmax for every unit which is up
  * min-up and min down time constraint for each production unit
  * ramp up and down constraints (optional). Note that for the moment, ramp values are not given in the instance but computed as a ratio of the difference Pmax-Pmin for each units. In practice, during one time period, units can ramp up their power of at most $\frac{Pmax - Pmin}{3}$ and ramp down at most $\frac{Pmax - Pmin}{2}$.

 Note that the model implemented is the one from Section 5 of the following article : https://hal.science/hal-02945921v1/file/mpb_minorrev.pdf


# Launch code

* Update the path to Cplex in cplex_dir.mk

* Create build dir: ```mkdir build```

* Compile : ```make```

* Execute with example parameters from main.cc

  ```./mf``` 

* Execute with argument parameters:

  ```./mf $ramp $data_folder $n $T 1 3 $sym 0 0 $id``` 

  * ramp (bool) indicates whether or not to use ramp constraints
  * data_folder is the path of the directory containing the instance file 
  * n is the number of units
  * T is the number of time steps
  * sym is the symmetry level of the instance
  * id is the identifier of the instance

Example:


  ```./mf 1 data/smaller/ 20 24 1 3 3 0 0 1``` 

Instance format is the one from : https://github.com/CecileRottner/UCPInstanceGenerator

Example data is given in directory data, but new data can also be generated with the given generator.
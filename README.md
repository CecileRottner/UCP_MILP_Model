# Unit Commitment Model

The MILP model implemented corresponds to a min-up/min-down Unit Commitment Problem with or without ramp constraints :

 * Minimization of the total production cost (including proportional and fixed production cost, and start-up costs)

 * demand constraints at each time step
 * production limits Pmin/Pmax for every unit which is up
 * min-up and min down time constraint for each production unit
 * ramp up and down constraints (optional)

Instance format is the one from : https://github.com/CecileRottner/UCPInstanceGenerator

# Launch code

* Update the path to Cplex in cplex_dir.mk

* Create build dir: ```mkdir build```

* Compile : ```make```

* Execute with example parameters from main.cc

  ```./mf``` 

* Execute with argument parameters:

  ```./mf $ramp $data_folder $n $T 1 3 $sym 0 0 $id``` 

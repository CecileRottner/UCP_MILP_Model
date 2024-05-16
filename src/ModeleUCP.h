#ifndef MODELEUCP
#define MODELEUCP

#include <ilcplex/ilocplex.h>

#include "InstanceUCP.h"
#include "Process.h"


class ModeleUCP {
public:
    IloEnv env;
    Methode met ;
    InstanceUCP* pb ;
    IloBoolVarArray x ;
    IloBoolVarArray u ;


    ModeleUCP(IloEnv env, InstanceUCP* pb, Methode & m, IloBoolVarArray x, IloBoolVarArray u);
    IloModel defineModel(int uNum) ;
    IloModel defineModel_y() ;
    IloModel defineModel_numberOfOnes() ;
    IloModel defineModel_sum() ;
    IloModel AggregatedModel() ;

    void AddRSUIneq(IloModel & model) ;
    void AddRSDIneqForRamps(IloModel & model);

};














#endif /* MODELEUCP_INCLUDED */

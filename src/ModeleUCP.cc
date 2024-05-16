#include "ModeleUCP.h"
#include "InstanceUCP.h"

#include <ilcplex/ilocplex.h>

using namespace std ;


ModeleUCP::ModeleUCP(IloEnv e, InstanceUCP* i, Methode & m, IloBoolVarArray xx, IloBoolVarArray uu) : env(e), pb(i), met(m), x(xx), u(uu) {

}


IloModel ModeleUCP::defineModel(int uNum) {

    cout << "Classical Min-up/Min-down UCP model "<< endl ;

    IloModel model = IloModel(env);

    int n = pb->getn(); // number of units
    int T = pb->getT() ; // number of time steps

    IloInt t ;
    IloInt i ;
    IloInt k ;


    IloNumVarArray pp(env, n*T, 0.0, 1000); // power variables

    // Objective Function: Minimize Cost
    IloExpr cost(env) ;
    for (t=0 ; t < T ; t++) {
        for (i=0; i<n; i++) {
            cost += x[i*T + t]*pb->getcf(i) + pb->getc0(i)*u[i*T + t] + (pp[i*T + t]+pb->getPmin(i)*x[i*T + t])*(pb->getcp(i)) ;
        }
    }

    model.add(IloMinimize(env, cost));

    cost.end() ;


    // Initial conditions
    for (i=0; i<n; i++) {
        model.add(u[i*T] >= x[i*T] - pb->getInit(i) ) ;
    }

    for (i=0; i<n; i++) {
        IloExpr sum(env) ;
        for (k= 0; k < pb->getl(i) ; k++) {
            sum += u[i*T + k] ;
        }
        model.add(sum <= 1 - pb->getInit(i) ) ;
        sum.end() ;
    }


// Rajan & Takriti (2005) min-up/min-down constraints

    // Min up constraints
    for (i=0; i<n; i++) {
        for (t=pb->getL(i) -1 ; t < T ; t++) {
            IloExpr sum(env) ;
            for (k= t - pb->getL(i) + 1; k <= t ; k++) {
                sum += u[i*T + k] ;
            }
            model.add(sum <= x[i*T + t]) ;
            sum.end() ;
        }
    }


    // Min down constraints
    for (i=0; i<n; i++) {
        for (t=pb->getl(i) ; t < T ; t++) {
            IloExpr sum(env) ;
            for (k= t - pb->getl(i) + 1; k <= t ; k++) {
                sum += u[i*T + k] ;
            }
            model.add(sum <= 1 - x[i*T + t - pb->getl(i)]) ;
            sum.end() ;
        }
    }

    //Logical relationship between u et x
    for (i=0; i<n; i++) {
        for (t=1 ; t < T ; t++) {
            model.add(x[i*T + t] - x[i*T + t-1] <= u[i*T + t]);
        }
    }


    //Production limits
    for (i=0; i<n; i++) {
        for (t=0 ; t < T ; t++) {
            model.add(pp[i*T + t] <= (pb->getPmax(i)-pb->getPmin(i))*x[i*T + t]);
            model.add(pp[i*T + t] >= 0);
        }
    }


    //Demande
    for (t=0; t < T ; t++) {
        IloExpr Prod(env) ;
        for (i=0; i<n; i++) {
            Prod += pp[i*T + t] + pb->getPmin(i)*x[i*T + t];
        }
        model.add(pb->getD(t) <= Prod);
        Prod.end() ;
    }

    if (uNum) { // if unum=1, we just solve the linear relaxation of this model
        cout << "conversion to float for x and u" << endl ;
        model.add(IloConversion(env, u, IloNumVar::Float) ) ;
        model.add(IloConversion(env, x, IloNumVar::Float) ) ;
    }



    //if ramping constraints are active
    if (met.Ramping()) {
        cout << "gradients"<< endl;
        for (i = 0 ; i <n ; i++) {
            for (t = 1 ; t < T ; t++) {
                model.add(pp[i*T + t] - pp[i*T + t-1] <= (pb->getGradUp(i))*x[i*T + t-1] );
                model.add(pp[i*T + t-1] - pp[i*T + t] <= (pb->getGradDown(i))*x[i*T + t] );
            }
        }
    }

    return model ;

}


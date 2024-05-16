#include <ilcplex/ilocplex.h>
#include <ilcplex/ilocplexi.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <math.h>
#include <ctime>
#include <list>

#include "InstanceUCP.h"
#include "ModeleUCP.h"
#include "Process.h"

using namespace std ;

ILOSTLBEGIN


int process(InstanceProcessed I, ofstream & fichier, double & time, Methode met, IloEnv env) {

    string nom = I.fileName() ;
    const char* file = strdup(nom.c_str()) ;

    int id=I.id ;

    InstanceUCP* inst = new InstanceUCP(env, file) ;

    int n = inst->n ;
    int T = inst->T ;

    IloBoolVarArray x(env,n*T);// on/off status variables 
    IloBoolVarArray u(env,n*T);//startup variables

    ModeleUCP FrontalModel = ModeleUCP(env, inst,met, x,u) ;

    IloModel model ;

    model = FrontalModel.defineModel(0) ;

    IloCplex cplex(model) ;


    //Paramètres
    cplex.setParam(IloCplex::Param::ClockType, 1); //1 : CPU TIME
    cplex.setParam(IloCplex::Param::Threads, 1);
    cplex.setParam(IloCplex::EpGap, 0.0000001) ;
    cplex.setParam(IloCplex::Param::TimeLimit, 10) ;
    //cplex.setParam(IloCplex::LongParam::NodeLim, 1) ;


    //Résolution et affichage de la solution

    cplex.solve();

    double t = cplex.getCplexTime();
    if (cplex.isPrimalFeasible()) {
       

        ///Affichage solution optimale


        //    IloNumArray x_frac(env,0);
        //    cplex.getValues(x_frac, x) ;
        //    for (int i=0; i<n; i++) {
        //        cout << "unité " << i << " : " ;
        //        for (int t=0 ; t < T ; t++) {
        //            cout << x_frac[i*T +t] << " " ;
        //        }
        //        cout << endl ;
        //    }
        //    cout << endl ;

        //    IloNumArray u_frac(env,0);
        //    cplex.getValues(u_frac, u) ;
        //    for (int i=0; i<n; i++) {
        //        cout << "start up unité " << i << " : " ;
        //        for (int t=0 ; t < T ; t++) {
        //            cout << u_frac[i*T +t] << " " ;
        //        }
        //        cout << endl ;
        //    }
        //    cout << endl ;

        


        double opt = cplex.getObjValue() ;

        fichier << met.getNum() <<  " ; " << n << " ; " << T  << " ; " << id ;
        fichier <<  " ; " << cplex.getObjValue()  ; //Optimal value
        fichier <<  " ; " << cplex.getBestObjValue()  ; 
        fichier <<  " ; "       << cplex.getMIPRelativeGap() << " \\% " ; //approx gap
        fichier <<  " ; " << cplex.getNnodes() ;
        fichier <<  " ; " << t - time ;
        fichier <<" \\\\ " << endl ;


    }

    time = t ;
    //Destructeurs
    // delete inst ;
    // delete dataNode ;
    // env.end() ;

    return 1;
}


int
main(int argc,char**argv)
{
    srand(time(NULL));

    ///// Model without ramp constraints
    Methode DefaultCplex ;

    ///// Model with ramp constraints
    Methode RampDefaultCplex;
    RampDefaultCplex.UseRampConstraints();



    /////////////////////// ARGUMENTS to choose instance and parameters //////////////////////
    if (argc>1) {
        ofstream fichier("result.txt", std::ofstream::out | std::ofstream::app);

        int met= atoi(argv[1]);
        string localisation = argv[2] ;

        int n = atoi(argv[3]);
        int T = atoi(argv[4]);
        int bloc = atoi(argv[5]);
        int demande = atoi(argv[6]);
        int sym = atoi(argv[7]);

        int cat01 = atoi(argv[8]);
        int intra = atoi(argv[9]);

        int id = atoi(argv[10]);

        InstanceProcessed Instance = InstanceProcessed(n, T, bloc, demande, sym, cat01, intra, id, localisation) ;

        double time = 0 ;
        IloEnv env ;

        if (met==1) {
            env=IloEnv() ;
            process(Instance, fichier, time, DefaultCplex , env) ;
            env.end() ;
        }

        if (met==2) {
            env=IloEnv() ;
            process(Instance, fichier, time, RampDefaultCplex , env) ;
            env.end() ;
        }

    
    }
    ////////////////////////// no arguments case ////////////////////
    if (argc==1) {
        ofstream fichier("result.txt");

        fichier << "Methode ; n ; T ; id ; BestVal ; LowerBound ; Nodes ; NbFixs ; TimeFix ; CPU \\\\ " << endl;

        double time = 0 ; // total computational time (incremented by )

        //Instance parameters

        int T = 24;
        int n = 20 ;
        int sym = 3 ;
        int demande = 3;
        int cat01 = 0;
        int bloc = 1; // parameter always to 1 in this setting

        int intra =0 ;

        string localisation = "data/smaller/" ; // folder containing data sets
        InstanceProcessed Instance = InstanceProcessed(n, T, bloc, demande, sym, cat01, intra, 0, localisation) ;

        fichier << localisation << endl ;
        Instance.localisation = localisation ;

        n=30;
        T=48;
        Instance.n=n;
        Instance.T=T ;
        IloEnv env ;

        Instance.symetrie = 3 ;
        Instance.T=T ;
        Instance.id = 1 ;

        //PROCESS INSTANCE: write model and solve with Cplex
        env=IloEnv() ;
        process(Instance, fichier, time,RampDefaultCplex, env) ;
        env.end() ;
        fichier << endl ;
    }
    return 0 ;
}

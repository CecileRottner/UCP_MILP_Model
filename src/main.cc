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
    cplex.setParam(IloCplex::Param::TimeLimit, 20) ;
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

        //fichier << "Instance & n & T & Sym & nG & max & mean & OptVal & RootRelax & ApproxGap &  USCuts & IUSCuts & SepTime & Prof & Nodes & CplexCuts & CPU \\\\ " << endl;
        fichier << "Methode & n & T & OptVal & Nodes & NbFixs & TimeFix & CPU \\\\ " << endl;

        double time = 0 ;

        //Paramètres de l'instance

        int T = 24;
        int n = 20 ;
        int sym = 3 ;
        int demande = 3;
        int cat01 = 0;
        int bloc = 1;

        int intra =0 ;

        string localisation = "data/smaller/" ;
        InstanceProcessed Instance = InstanceProcessed(n, T, bloc, demande, sym, cat01, intra, 0, localisation) ;

        fichier << localisation << endl ;
        Instance.localisation = localisation ;

        n=30;
        T=48;
        Instance.n=n;
        Instance.T=T ;
        IloEnv env ;

        for (sym= 3; sym >=3 ; sym--) {
            Instance.symetrie = sym ;

            for (T=48 ; T <=48; T*=2) {

                Instance.T=T ;


                for (int id=1; id <=1; id++) {
                    Instance.id = id ;


                    env=IloEnv() ;
                    process(Instance, fichier, time,RampDefaultCplex, env) ;
                    env.end() ;
//                    env=IloEnv() ;
//                    process(Instance, fichier, time, AggregModel, env) ;
//                    env.end() ;

//                    env=IloEnv() ;
//                    process(Instance, fichier, time, RampIneqRSU_RSDRamps, env) ;
//                    env.end() ;

//                    env=IloEnv() ;
//                    process(Instance, fichier, time, ModeleIntervalle, env) ;
//                    env.end() ;

                    fichier << endl ;
                }

                fichier << endl ;
                fichier << endl ;
            }
        }
    }







    return 0 ;
}

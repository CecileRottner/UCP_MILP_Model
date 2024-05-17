#ifndef PROCESSS
#define PROCESSS

#include <ilcplex/ilocplex.h>
#include <ilcplex/ilocplexi.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <math.h>
#include <ctime>
#include <list>

using namespace std ;

class Methode {
private:

    int num ; // numéro de référence de la méthode
    // méthodes de résolution
    int doCplex ;
    int doIneqSum ;
    int doRSUOnly ;
    int doRSDforRamps;
    int doIneqVarY ;
    int doModeleFlot ;
    int doNumberOfOnes ;
    int doAggregatedModel ;
    int doIntervalModel ;
    int doRHS_RSU_u ;

    //contraintes
    int CteRamping ;
    int emptyCB;


public:

    //Constructeur (par défault c'est Cplex Default)
    Methode() {// crée Cplex default
        doCplex=1 ; // use cplex only
        num=0 ; //dentifier of the case studied (0 without ramp constraints, 1 with ramp constraints)
        CteRamping = 0 ; // 1 if ramp constraints are used
    }

    void setNum(int nb) { num=nb ; }


    void UseRampConstraints() {
        doCplex=1;
        CteRamping=1 ;
        num=1 ;
    }

    //access to parameters
    int getNum() {return num ;}
    int CplexOnly() {return doCplex;}
    int Ramping() {return CteRamping;}

};

class InstanceProcessed {
public :

    //donnees
    int n ;
    int T ;
    int bloc ;
    int demande ;
    int symetrie ;
    int cat ;
    int intra ;
    int id ;

    //Methode
    string localisation ;

    InstanceProcessed(int n_, int T_, int bloc_, int demande_, int symetrie_, int cat_, int intra_, int id_, string loc_)
    {
        n=n_ ;
        T=T_ ;
        bloc = bloc_ ;
        demande = demande_ ;
        symetrie = symetrie_ ;
        cat = cat_ ;
        intra = intra_ ;
        id = id_ ;
        localisation = loc_ ;
    }

    string createName() const ;

    string fileName() {

        string nom = createName() ;
        string fileI = localisation + nom;
        string fileS = fileI + ".txt" ;
        cout << fileS << endl ;
        return fileS ;
    }

};

#endif 

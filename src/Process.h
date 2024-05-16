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
        doCplex=1 ;
        num=-1 ;

        doIneqSum = 0;
        doRSUOnly = 0 ;
        doRSDforRamps= 0 ;
        doIneqVarY = 0;
        doModeleFlot =0 ;
        doNumberOfOnes = 0 ;
        doAggregatedModel = 0 ;
        doIntervalModel = 0 ;
        CteRamping = 0 ;
        doRHS_RSU_u = 0 ;
        emptyCB=0;

    }


    // modification des paramètres
    void setNum(int nb) { num=nb ; }
    void AddIneqRSU() {num=-3; doRSUOnly=1;}


    //Méthodes de résolution
    //paramètres utilisés = le minimum nécessaire


    void UseIneqSum() { // on peut choisir ou non d'utiliser un lazy callback dans ce cas (même si fixing static en plus)
        num=-3;
        doCplex=0 ;
        doIneqSum=1 ;
    }

    void UseNumberOfOnes() {
        num=-6 ;
        doCplex=0 ;
        doNumberOfOnes=1 ;
    }
    void UseIneqVarY() { // on peut choisir ou non d'utiliser un lazy callback dans ce cas (même si fixing static en plus)
        num=-2 ;
        doCplex=0 ;
        doIneqVarY=1 ;
    }

    void UseAggregatedModel() { // on peut choisir ou non d'utiliser un lazy callback dans ce cas (même si fixing static en plus)
        num=-4 ;
        doCplex=0 ;
        doAggregatedModel=1 ;
    }

    void UseModeleFlot() { // on peut choisir ou non d'utiliser un lazy callback dans ce cas (même si fixing static en plus)
        num=-7 ;
        doCplex=0 ;
        doModeleFlot=1 ;
    }

    void UseModeleInterval() {
        num=-7 ;
        doCplex=0 ;
        doIntervalModel = 1 ;
    }

    void AddIneqSum() { // on peut choisir ou non d'utiliser un lazy callback dans ce cas (même si fixing static en plus)
        doCplex=0 ;
        doIneqSum=1 ;
    }


    void UseRampConstraints() {
        //num=-3 ;
        doCplex=0 ;
        CteRamping=1 ;
        num*=10 ;
    }

    void Use_RHS_RSU_u()  {
        doRHS_RSU_u = 1 ;
    }

    void UseRSDforRamps()  {
        doRSDforRamps = 1 ;
    }

    void printParam()  {
        cout << "method nb: " << num << endl ;
        cout << "Cplex only: " << doCplex << endl ;
        cout << "Use sub symmetry ineq: " << doIneqSum << endl ;
        cout << "Use RSU sub symmetry ineq only: " << doRSUOnly << endl ;
        cout << "Use variables y: " << doIneqVarY<< endl ;
        cout << "Use modèle flot: " << doModeleFlot << endl ;
        cout << "Agregated model: " << doAggregatedModel << endl ;
        cout << "Ramping constraints: " << CteRamping << endl ;
    }

    void EmptyCallback() {
        emptyCB=1;
    }

    //accès aux paramètres
    int getNum() {return num ;}
    int CplexOnly() {return doCplex;}
    int IneqSum() {return doIneqSum;}
    int NumberOfOnes() {return doNumberOfOnes;}
    int IneqVarY() {return doIneqVarY;}
    int AggregatedModel() {return doAggregatedModel;}
    int ModeleFlot() {return doModeleFlot;}
    int ModeleIntervalle() {return doIntervalModel ;}
    int Ramping() {return CteRamping;}
    int RSUonly() {return doRSUOnly;}
    int RHS_RSU_u()  {return doRHS_RSU_u;}
    int RSDforRamps() {return doRSDforRamps;}
    int useCB() {return emptyCB;}

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

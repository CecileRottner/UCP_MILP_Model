#include "InstanceUCP.h"

#include <ctime>
#include <stdlib.h> // use rand
#include <fstream> // pour lire le fichier de données

using namespace std ;


InstanceUCP::InstanceUCP(IloEnv envir, const char* file) {
    env = envir ;
    instance_joco=0;
    Lecture(file) ;
    Initialise() ;
}




void InstanceUCP::Initialise() {

    //calculs symétries

    nbG=0 ;

    for (int i = 0 ; i <n ; i++) {
        if (First_[i]) {
            nbG++;
        }
    }


    FirstG_ = IloIntArray(env, nbG) ;
    FirstG = IloIntArray(env, nbG) ;
    LastG_ = IloIntArray(env, nbG) ;
    LastG = IloIntArray(env, nbG) ;



    Group = IloIntArray(env, n) ;

    int nb =0 ;
    for (int i = 0 ; i <n ; i++) {
        if (First_[i]) {
            FirstG_[nb] = i ;

        }
        if (Last_[i]) {
            LastG_[nb] = i ;
            nb++ ;
        }
    }




    //Tri des unités et des pas de temps
    C_ = IloIntArray(env, nbG);
    IloNumArray ordre = IloNumArray(env, nbG);

    for (IloInt j = 0 ; j <nbG ; j++ ) {
        int elem = FirstG_[j];
        C_[j] = j ;
        ordre[j] = 1/Pmax_[elem] ;
    }


    IloNumArray invDemande = IloNumArray(env, T);

    for (IloInt j = 0 ; j < T ; j++ ) {
        ordreT[j] = j ;
        invDemande[j] = 1/D[j] ;
    }

    quickSort(invDemande, ordreT, 0, T) ; // ordreT trie les t par demande décroissante
    //cout << "ordreT : " << ordreT << endl ;

   // quickSort(ordre,C_, 0, nbG);  //On trie les unités par Pmax décroissantes



    int first =0 ;
    int last = 0;
    for (IloInt kg = 0 ; kg < nbG ; kg++ ) {
        int g = C_[kg] ;
        int sizeOfGroup = LastG_[g] - FirstG_[g] + 1;
        last = first + sizeOfGroup - 1 ;
        FirstG[kg] = first ;
        LastG[kg] = last ;


        int fElem = FirstG_[g] ; // indice du premier élément de g dans l'ancien système

        for (int k=first ; k <= last ; k++) {
            P[k] = P_[fElem] ;
            Pmax[k] = Pmax_[fElem] ;
            L[k] = L_[fElem] ;
            l[k] = l_[fElem] ;
            c0[k] = c0_[fElem] ;
            cf[k] = cf_[fElem] ;
            cp[k] = cp_[fElem] ;
            Init[k] = Init_[fElem] ;
            nk[k] = nk_[fElem] ;
            nS[k] = nS_[fElem] ;
        }

        first = last+1 ;
    }

    First = IloIntArray(env, n) ;
    Last = IloIntArray(env, n) ;

    for (int g= 0 ; g < nbG ; g++) {
        First[FirstG[g]] = 1;
        Last[LastG[g]] =  1 ;
    }




    SommePmax = 0 ;
    for (int j = 0 ; j <n ; j++) {
        SommePmax += Pmax[j] ;
    }


    //Calcul des indicateurs de symétries

    SizeG = IloIntArray(env, nbG) ;


    repartition_tailles = IloIntArray(env, n+1) ;
    for (int i=0 ; i <n ; i++) {
        repartition_tailles[i] = 0 ;
    }

    MaxSize = 0 ;
    MeanSize = 0 ;
    nbG2 = 0 ;


    int groupSize = 0 ;

    int group_ind=0 ;

    for (int i = 0 ; i <n ; i++) {

        if (!Last[i]) {
            groupSize ++ ;
            if (First[i]) {
                nbG2++ ;
            }
        }
        else {

            groupSize++ ;
            repartition_tailles[groupSize]++ ;
            SizeG[group_ind] = groupSize ;


            if (!First[i]) {
                MeanSize+=groupSize ;
                if (MaxSize < groupSize) {
                    MaxSize = groupSize ;
                }
            }
            groupSize = 0 ;
            group_ind++;
        }
    }

    cout << "First : " << First << endl ;
    cout << "Size : " << SizeG << endl ;
    MeanSize = MeanSize / nbG2;

    nb=0 ;

    for (int i = 0 ; i <n ; i++) {
        Group[i] = nb ;
        if (Last[i]) {
            nb++ ;
        }
    }



    //dérivée de la demande
    isIncreasing = IloIntArray(env, T) ;

    for (int t=0 ; t < T ; t++) {
        int mean_next_k = 0;
        int k=5 ;
        for (int s=fmin(T-1,t+1) ; s <= fmin(T-1,t+k) ; s++) {
            mean_next_k += D[s] ;
        }
        mean_next_k /= k ;
        if (mean_next_k > D[t]) {
            isIncreasing[t] = 1 ;
        }
        else {
            isIncreasing[t] = 0 ;
        }
    }


    ////// Indicateurs pour le branchement maison

    ordre_ratio = IloIntArray(env, n) ;

    //demande

    /*Dmin = SommePmax ;
    int Dmax1 = 0 ;
    int Dmax2 = 0 ;

    int t=0 ;
    while (D[t] > Dmax1) {
        tmax1=t ;
        Dmax1 = D[t] ;
        t++ ;
    }

    cout << "tmax1 : " << tmax1 << endl ;

    cout << "t : " << t << endl ;
    cout << "D[t], Dmin" << D[t] << ", " << Dmin << endl ;
    while (D[t] < Dmin) {
        tmin=t ;
        Dmin = D[t] ;
        t++ ;
    }

    cout << "tmin : " << tmin << endl ;
    while (D[t] > Dmax2) {
        tmax2=t ;
        Dmax2 = D[t] ;
        t++ ;
    }

    Dmax = fmax(Dmax1, Dmax2) ;

    cout << "Dmin, Dmax : " << Dmin << ", " << Dmax << endl;


    //Tri des unités selon le ratio

    IloNumArray ratio = IloNumArray(env, n);

    for (IloInt j = 0 ; j <n ; j++ ) {
        ordre_ratio[j] = j ;
        ratio[j] = (Pmax[j]*cp[j] + cf[j])/Pmax[j] ;
    }

    quickSort(ratio,ordre_ratio, 0, n);


    cout << "ici" << endl ;
    //unités de base et de pic

    //base
    nb=0 ;
    int D = Dmin ;
    while (D > 0 && nb < n) {
        D -= Pmax[nb] ;
        nb++ ;
    }

    nb-- ;

    //pic

    np=nb+1 ;
    D = Dmax - Dmin ;
    while (D > 0 && np < n) {
        D += Pmax[np] ;
        np++ ;
    }*/

}

void InstanceUCP::Lecture(const char* file) {
    //Lecture de n et de T
    ifstream fichier(file, ios::in);

    string nom = "";
    fichier >> nom;

    while(nom!="="){
        fichier >> nom;
    }
    nom = "";
    fichier >> n;

    while(nom!="="){
        fichier >> nom;
    }
    nom = "";
    fichier >> T;

    //Initialisation des vecteurs de taille n et T
    Init_ = IloBoolArray(env, n);
    L_ = IloIntArray(env, n);
    l_ = IloIntArray(env, n);
    P_ = IloNumArray(env, n);
    Pmax_ = IloNumArray(env, n);
    cf_ = IloNumArray(env, n);
    c0_ = IloNumArray(env, n);
    cp_ = IloNumArray(env, n);
    nk_ = IloIntArray(env, n);
    nS_ = IloIntArray(env, n);


    First_ = IloIntArray(env,n) ;
    Last_ = IloIntArray(env,n) ;

    ordreT = IloIntArray(env, T) ;

    Init = IloBoolArray(env, n);
    L = IloIntArray(env, n);
    l = IloIntArray(env, n);
    P = IloNumArray(env, n);
    Pmax = IloNumArray(env, n);
    cf = IloNumArray(env, n);
    c0 = IloNumArray(env, n);
    cp = IloNumArray(env, n);
    nk = IloIntArray(env, n);
    nS = IloIntArray(env, n);
    D = IloNumArray(env, T);


    //Lecture des données
    //Init
    while(nom!="["){
        fichier >> nom;
    }
    nom = "";
    for(IloInt j=0; j<n; j++){
        Init_[j] = 1;
    }

    //L
    while(nom!="["){
        fichier >> nom;
    }
    nom = "";
    for(IloInt j=0; j<n; j++){
        fichier >> L_[j];
    }

    //l
    while(nom!="["){
        fichier >> nom;
    }
    nom = "";
    for(IloInt j=0; j<n; j++){
        fichier >> l_[j];
    }

    //P
    while(nom!="["){
        fichier >> nom;
    }
    nom = "";
    for(IloInt j=0; j<n; j++){
        fichier >> P_[j];
    }

    //Pmax
    while(nom!="["){
        fichier >> nom;
    }
    nom = "";
    for(IloInt j=0; j<n; j++){
        fichier >> Pmax_[j];
    }

    //cf
    while(nom!="["){
        fichier >> nom;
    }
    nom = "";
    for(IloInt j=0; j<n; j++){
        fichier >> cf_[j];
    }

    //c0
    while(nom!="["){
        fichier >> nom;
    }
    nom = "";
    for(IloInt j=0; j<n; j++){
        fichier >> c0_[j];
    }

    //cp
    while(nom!="["){
        fichier >> nom;
    }
    nom = "";
    for(IloInt j=0; j<n; j++){
        fichier >> cp_[j];
    }

    //D
    while(nom!="["){
        fichier >> nom;
    }
    nom = "";
    for(IloInt j=0; j<T; j++){
        fichier >> D[j];
    }




    if (!instance_joco)  {
    //Symétries
    while(nom!="="){
        fichier >> nom;
    }
    nom = "";
    fichier >> K;

    cout << K << endl ;

    while(nom!="["){
        fichier >> nom;
    }
    nom = "";
    for(IloInt j=0; j<n; j++){
        fichier >> nk_[j];
    }

    //First
    while(nom!="["){
        fichier >> nom;
    }
    nom = "";
    for(IloInt j=0; j<n; j++){
        fichier >> First_[j];
    }

    //Last
    while(nom!="["){
        fichier >> nom;
    }
    nom = "";
    for(IloInt j=0; j<n; j++){
        fichier >> Last_[j];
    }



    //intra-site
    while(nom!="="){
        fichier >> nom;
    }
    nom = "";
    fichier >> S;

    while(nom!="["){
        fichier >> nom;
    }
    nom = "";
    }

    else {
        K=n;
        for (int i=0 ; i <n ; i++) {
            nk_[i] = i ;
            First_[i] = 1 ;
            Last_[i] = 1;
            S=n ;

        }
    }


    //fin lecture de fichier
}

/*InstanceUCP::~InstanceUCP() {

    Init.end() ;
    L.end() ;
    l.end() ;
    D.end() ;
    P.end() ;
    Pmax.end();
    c0.end() ;
    cf.end() ;


    Init_.end() ;
    L_.end() ;
    l_.end() ;
    P_.end() ;
    Pmax_.end();
    c0_.end() ;
    cf_.end() ;
    C_.end() ;


}*/



double moyenne(IloNumArray V) {
    IloInt n = V.getSize() ;
    double moyP = 0 ;
    for (IloInt j =0 ; j < n ; j++) {
        moyP += V[j];
    }
    moyP = moyP/n ;
    return moyP ;
}

double variance(IloNumArray X, IloNumArray Y) {
    IloInt n = X.getSize() ;
    double sigma = 0 ;
    double moyX = moyenne(X);
    double moyY = moyenne(Y) ;
    for (IloInt j = 0 ; j < n ; j++) {
        sigma += (X[j] - moyX)*(Y[j] - moyY);
    }
    sigma = sigma/n ;

    return sigma ;
}


// Accès aux données

IloInt InstanceUCP::getn() {
    return n ;
}

IloInt InstanceUCP::getT() {
    return T ;
}

IloBool InstanceUCP::getInit(IloInt i) {
    if ((i >= n)||(i < 0)) {
        cout << "Attention, Init[" << i << "] n'existe pas." << endl ;
    }
    return Init[i] ;
}

IloInt InstanceUCP::getL(IloInt i) {
    if ((i >= n)||(i < 0)) {
        cout << "Attention, L[" << i << "] n'existe pas." << endl ;
    }
    return L[i] ;
}

IloInt InstanceUCP::getl(IloInt i) {
    if ((i >= n)||(i < 0)) {
        cout << "Attention, l[" << i << "] n'existe pas." << endl ;
    }
    return l[i] ;
}

IloNum InstanceUCP::getD(IloInt i) {
    if ((i >= T)||(i < 0)) {
        cout << "Attention, D[" << i << "] n'existe pas." << endl ;
    }
    return D[i] ;
}

//deprecated: use getPmin instead
IloNum InstanceUCP::getP(IloInt i) {
    if ((i >= n)||(i < 0)) {
        cout << "Attention, P[" << i << "] n'existe pas." << endl ;
    }
    return P[i] ;
}
IloNum InstanceUCP::getPmin(IloInt i) {
    if ((i >= n)||(i < 0)) {
        cout << "Attention, P[" << i << "] n'existe pas." << endl ;
    }
    return P[i] ;
}

IloNum InstanceUCP::getPmax(IloInt i) {
    if ((i >= n)||(i < 0)) {
        cout << "Attention, Pmax[" << i << "] n'existe pas." << endl ;
    }
    return Pmax[i] ;
}


IloNum InstanceUCP::getGradUp(IloInt i) {
    return ((getPmax(i)-getP(i))/3) ;
}

IloNum InstanceUCP::getGradDown(IloInt i) {
    return ((getPmax(i)-getP(i))/2) ;
}

IloNum InstanceUCP::getcf(IloInt i) {
    if ((i >= n)||(i < 0)) {
        cout << "Attention, cf[" << i << "] n'existe pas." << endl ;
    }
    return cf[i] ;
}

IloNum InstanceUCP::getc0(IloInt i) {
    if ((i >= n)||(i < 0)) {
        cout << "Attention, c0[" << i << "] n'existe pas." << endl ;
    }
    return c0[i] ;
}

IloNum InstanceUCP::getcp(IloInt i) {
    if ((i >= n)||(i < 0)) {
        cout << "Attention, cp[" << i << "] n'existe pas." << endl ;
    }
    return cp[i] ;
}

IloNum InstanceUCP::getnbG() {
    return nbG ;
}

IloNum InstanceUCP::getFirst(IloInt i) {
    if ((i >= n)||(i < 0)) {
        cout << "Attention, First[" << i << "] n'existe pas." << endl ;
    }
    return First[i] ;
}

IloNum InstanceUCP::getLast(IloInt i) {
    if ((i >= n)||(i < 0)) {
        cout << "Attention, Last[" << i << "] n'existe pas." << endl ;
    }
    return Last[i] ;
}
IloNum InstanceUCP::getGroup(IloInt i) {
    if ((i >= n)||(i < 0)) {
        cout << "Attention, Group[" << i << "] n'existe pas." << endl ;
    }
    return Group[i] ;
}

IloNum InstanceUCP::getFirstG(IloInt i) {
    if ((i >= n)||(i < 0)) {
        cout << "Attention, FirstG[" << i << "] n'existe pas." << endl ;
    }
    return FirstG[i] ;
}
IloNum InstanceUCP::getLastG(IloInt i) {
    if ((i >= n)||(i < 0)) {
        cout << "Attention, LastG[" << i << "] n'existe pas." << endl ;
    }
    return LastG[i] ;
}

IloNum InstanceUCP::getSizeG(IloInt g) {
    if ((g >= nbG)||(g < 0)) {
        cout << "Attention, SizeG[" << g << "] n'existe pas." << endl ;
    }
    return SizeG[g] ;
}

IloNum InstanceUCP::getordreT(IloInt t) {
    if ((t >= T)||(t < 0)) {
        cout << "Attention, ordreT[" << t << "] n'existe pas." << endl ;
    }
    return ordreT[t] ;
}

IloInt InstanceUCP::getSommePmax() {
    return SommePmax ;
}


// Tri (Quicksort)

void InstanceUCP::quickSort(IloNumArray const & ordre, IloIntArray & indices, IloInt p, IloInt q) {
    IloInt r;
    if(p<q)
    {
        r=partition(ordre, indices, p,q);
        quickSort(ordre, indices,p,r);
        quickSort(ordre, indices,r+1,q);
    }
}

IloInt InstanceUCP::partition(IloNumArray const & ordre, IloIntArray & indices, IloInt p, IloInt q)
{
    IloInt x= indices[p];
    IloInt i=p;
    IloInt j;

    for(j=p+1; j<q; j++)
    {
        if(ordre[indices[j]]<= ordre[x])
        {
            i=i+1;
            swap(indices[i],indices[j]);
        }

    }

    swap(indices[i],indices[p]);
    return i;
}

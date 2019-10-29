#ifndef ARBREABSTRAIT_H
#define ARBREABSTRAIT_H

// Contient toutes les déclarations de classes nécessaires
//  pour représenter l'arbre abstrait

#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;

#include "Symbole.h"
#include "Exceptions.h"

////////////////////////////////////////////////////////////////////////////////
class Noeud {
// Classe abstraite dont dériveront toutes les classes servant à représenter l'arbre abstrait
// Remarque : la classe ne contient aucun constructeur
  public:
    virtual int  executer() =0 ; // Méthode pure (non implémentée) qui rend la classe abstraite
    virtual void ajoute(Noeud* instruction) { throw OperationInterditeException(); }
    virtual ~Noeud() {} // Présence d'un destructeur virtuel conseillée dans les classes abstraites
    virtual void traduitEnCPP(ostream & cout,unsigned int indentation)const;
};

////////////////////////////////////////////////////////////////////////////////
class NoeudSeqInst : public Noeud {
// Classe pour représenter un noeud "sequence d'instruction"
//  qui a autant de fils que d'instructions dans la séquence
  public:
     NoeudSeqInst();         // Construit une séquence d'instruction vide
    ~NoeudSeqInst() {}       // A cause du destructeur virtuel de la classe Noeud
    int executer() override; // Exécute chaque instruction de la séquence
    void ajoute(Noeud* instruction) override;  // Ajoute une instruction à la séquence
    void traduitEnCPP(ostream & cout,unsigned int indentation)const override;
  private:
    vector<Noeud *> m_instructions; // pour stocker les instructions de la séquence
};

////////////////////////////////////////////////////////////////////////////////
class NoeudAffectation : public Noeud {
// Classe pour représenter un noeud "affectation"
//  composé de 2 fils : la variable et l'expression qu'on lui affecte
  public:
     NoeudAffectation(Noeud* variable, Noeud* expression); // construit une affectation
    ~NoeudAffectation() {}   // A cause du destructeur virtuel de la classe Noeud
    int executer() override; // Exécute (évalue) l'expression et affecte sa valeur à la variable
    void traduitEnCPP(ostream & cout,unsigned int indentation)const override;
  private:
    Noeud* m_variable;
    Noeud* m_expression;
};

////////////////////////////////////////////////////////////////////////////////
class NoeudOperateurBinaire : public Noeud {
// Classe pour représenter un noeud "opération binaire" composé d'un opérateur
//  et de 2 fils : l'opérande gauche et l'opérande droit
  public:
    NoeudOperateurBinaire(Symbole operateur, Noeud* operandeGauche, Noeud* operandeDroit);
    // Construit une opération binaire : operandeGauche operateur OperandeDroit
   ~NoeudOperateurBinaire() {} // A cause du destructeur virtuel de la classe Noeud
    int executer() override;   // Exécute (évalue) l'opération binaire)
    void traduitEnCPP(ostream & cout,unsigned int indentation)const override;
  private:
    Symbole m_operateur;
    Noeud*  m_operandeGauche;
    Noeud*  m_operandeDroit;
};

////////////////////////////////////////////////////////////////////////////////
class NoeudInstSi : public Noeud {
// Classe pour représenter un noeud "instruction si"
//  et ses 2 fils : la condition du si et la séquence d'instruction associée
  public:
    NoeudInstSi(Noeud* condition, Noeud* sequence);
     // Construit une "instruction si" avec sa condition et sa séquence d'instruction
   ~NoeudInstSi() {}         // A cause du destructeur virtuel de la classe Noeud
    int executer() override; // Exécute l'instruction si : si condition vraie on exécute la séquence
    void traduitEnCPP(ostream & cout,unsigned int indentation)const override;
  private:
    Noeud*  m_condition;
    Noeud*  m_sequence;
};

////////////////////////////////////////////////////////////////////////////////
class NoeudInstSiRiche : public Noeud {
  public:
    NoeudInstSiRiche(NoeudInstSi* si);
     // Construit une "instruction si" avec sa condition et sa séquence d'instruction
   ~NoeudInstSiRiche() {}         // A cause du destructeur virtuel de la classe Noeud
    int executer() override; // Exécute l'instruction si : si condition vraie on exécute la séquence
    void ajoute(NoeudInstSi* sinonsi);
    void ajouteSinon(Noeud* sinon);
    void traduitEnCPP(ostream & cout,unsigned int indentation)const override;
  private:
    vector<NoeudInstSi*>  m_si_sinonsi;
    Noeud*  m_sinon;
};

////////////////////////////////////////////////////////////////////////////////
class NoeudInstTantQue : public Noeud{
// Classe pour représenter un noeud "instruction tantque"
//  et ses 2 fils : la condition du tantque et la séquence d'instruction associée
  public:
    NoeudInstTantQue(Noeud* condition, Noeud* sequence);
     // Construit une "instruction tantque" avec sa condition et sa séquence d'instruction
   ~NoeudInstTantQue() {}         // A cause du destructeur virtuel de la classe Noeud
    int executer() override; // Exécute l'instruction tantque : tantque condition vraie on exécute la séquence
    void traduitEnCPP(ostream & cout,unsigned int indentation)const override;
  private: 
    Noeud*  m_condition;
    Noeud*  m_sequence;
};

////////////////////////////////////////////////////////////////////////////////
class NoeudInstRepeter : public NoeudInstTantQue{
// Classe pour représenter un noeud "instruction repeter"
//  et ses 2 fils : la condition du repeter et la séquence d'instruction associée
  public:
    NoeudInstRepeter(Noeud* condition, Noeud* sequence);
     // Construit une "instruction repeter" avec sa condition et sa séquence d'instruction
   ~NoeudInstRepeter() {}         // A cause du destructeur virtuel de la classe Noeud
    int executer() override; // Exécute l'instruction repeter : on exécute la séquence jusqu'a condition vraie 
    void traduitEnCPP(ostream & cout,unsigned int indentation)const override;
  private: 
    Noeud*  m_condition;
    Noeud*  m_sequence;
};

////////////////////////////////////////////////////////////////////////////////
class NoeudInstPour : public NoeudInstTantQue{
// Classe pour représenter un noeud "instruction repeter"
//  et ses 2 fils : la condition du repeter et la séquence d'instruction associée
  public:
    NoeudInstPour(Noeud* condition, Noeud* sequence, Noeud* affectation1, Noeud* affectation2);
     // Construit une "instruction repeter" avec sa condition et sa séquence d'instruction
   ~NoeudInstPour() {}         // A cause du destructeur virtuel de la classe Noeud
    int executer() override; // Exécute l'instruction repeter : on exécute la séquence jusqu'a condition vraie 
    void traduitEnCPP(ostream & cout,unsigned int indentation)const override;
  private: 
    Noeud*  m_condition;
    Noeud*  m_sequence;
    Noeud*  m_affectation1;
    Noeud*  m_affectation2;
};


////////////////////////////////////////////////////////////////////////////////
class NoeudInstEcrire : public Noeud{
// Classe pour représenter un noeud "instruction repeter"
//  et ses 2 fils : la condition du repeter et la séquence d'instruction associée
  public:
    NoeudInstEcrire();
     // Construit une "instruction repeter" avec sa condition et sa séquence d'instruction
   ~NoeudInstEcrire() {}         // A cause du destructeur virtuel de la classe Noeud
    int executer() override; // Exécute l'instruction repeter : on exécute la séquence jusqu'a condition vraie 
    void ajoute(Noeud* aEcrire) override;  // Ajoute une instruction à la sequence d'ecriture
    void traduitEnCPP(ostream & cout,unsigned int indentation)const override;
  protected: 
    vector<Noeud*> m_seq;
};


////////////////////////////////////////////////////////////////////////////////
class NoeudInstLire : public NoeudInstEcrire{
// Classe pour représenter un noeud "instruction lire"
//  et ses 2 fils : la condition du repeter et la séquence d'instruction associée
  public:
    NoeudInstLire();
     // Construit une "instruction repeter" avec sa condition et sa séquence d'instruction
   ~NoeudInstLire() {}         // A cause du destructeur virtuel de la classe Noeud
    int executer() override; // Exécute l'instruction repeter : on exécute la séquence jusqu'a condition vraie
    void traduitEnCPP(ostream & cout,unsigned int indentation)const override;
};

#endif /* ARBREABSTRAIT_H */

#include <stdlib.h>
#include "ArbreAbstrait.h"
#include "Symbole.h"
#include "SymboleValue.h"
#include "Exceptions.h"
#include <iterator>

////////////////////////////////////////////////////////////////////////////////
// NoeudSeqInst
////////////////////////////////////////////////////////////////////////////////

NoeudSeqInst::NoeudSeqInst() : m_instructions() {
}

int NoeudSeqInst::executer() {
  for (unsigned int i = 0; i < m_instructions.size(); i++)
    m_instructions[i]->executer(); // on exécute chaque instruction de la séquence
  return 0; // La valeur renvoyée ne représente rien !
}

void NoeudSeqInst::ajoute(Noeud* instruction) {
  if (instruction!=nullptr) m_instructions.push_back(instruction);
}

////////////////////////////////////////////////////////////////////////////////
// NoeudAffectation
////////////////////////////////////////////////////////////////////////////////

NoeudAffectation::NoeudAffectation(Noeud* variable, Noeud* expression)
: m_variable(variable), m_expression(expression) {
}

int NoeudAffectation::executer() {
  int valeur = m_expression->executer(); // On exécute (évalue) l'expression
  ((SymboleValue*) m_variable)->setValeur(valeur); // On affecte la variable
  return 0; // La valeur renvoyée ne représente rien !
}

////////////////////////////////////////////////////////////////////////////////
// NoeudOperateurBinaire
////////////////////////////////////////////////////////////////////////////////

NoeudOperateurBinaire::NoeudOperateurBinaire(Symbole operateur, Noeud* operandeGauche, Noeud* operandeDroit)
: m_operateur(operateur), m_operandeGauche(operandeGauche), m_operandeDroit(operandeDroit) {
}

int NoeudOperateurBinaire::executer() {
  int og, od, valeur;
  if (m_operandeGauche != nullptr) og = m_operandeGauche->executer(); // On évalue l'opérande gauche
  if (m_operandeDroit != nullptr) od = m_operandeDroit->executer(); // On évalue l'opérande droit
  // Et on combine les deux opérandes en fonctions de l'opérateur
  if (this->m_operateur == "+") valeur = (og + od);
  else if (this->m_operateur == "-") valeur = (og - od);
  else if (this->m_operateur == "*") valeur = (og * od);
  else if (this->m_operateur == "==") valeur = (og == od);
  else if (this->m_operateur == "!=") valeur = (og != od);
  else if (this->m_operateur == "<") valeur = (og < od);
  else if (this->m_operateur == ">") valeur = (og > od);
  else if (this->m_operateur == "<=") valeur = (og <= od);
  else if (this->m_operateur == ">=") valeur = (og >= od);
  else if (this->m_operateur == "et") valeur = (og && od);
  else if (this->m_operateur == "ou") valeur = (og || od);
  else if (this->m_operateur == "non") valeur = (!og);
  else if (this->m_operateur == "/") {
    if (od == 0) throw DivParZeroException();
    valeur = og / od;
  }
  return valeur; // On retourne la valeur calculée
}

////////////////////////////////////////////////////////////////////////////////
// NoeudInstSi
////////////////////////////////////////////////////////////////////////////////

NoeudInstSi::NoeudInstSi(Noeud* condition, Noeud* sequence)
: m_condition(condition), m_sequence(sequence) {
}

int NoeudInstSi::executer() {
  if (m_condition->executer()) m_sequence->executer();
  return m_condition->executer(); // La valeur renvoyée ne représente rien !
}

////////////////////////////////////////////////////////////////////////////////
// NoeudInstSiRiche
////////////////////////////////////////////////////////////////////////////////

NoeudInstSiRiche::NoeudInstSiRiche(NoeudInstSi* si)
: m_si_sinonsi() {
    ajoute(si);
}

int NoeudInstSiRiche::executer() {
    vector<NoeudInstSi*>::iterator it = m_si_sinonsi.begin();
    bool uneExecution = false;
    while(it < m_si_sinonsi.end() && !uneExecution){
        uneExecution = ((NoeudInstSi*) (*it))->executer();
        it++;
    }
    if(!uneExecution){
        m_sinon->executer();
    }
    return 0; // La valeur renvoyée ne représente rien !
}

void NoeudInstSiRiche::ajoute(NoeudInstSi* sinonsi){
    m_si_sinonsi.push_back(sinonsi);
}
void NoeudInstSiRiche::ajouteSinon(Noeud* sinon){
    m_sinon = sinon;
}



////////////////////////////////////////////////////////////////////////////////
// NoeudInstTantQue
////////////////////////////////////////////////////////////////////////////////

NoeudInstTantQue::NoeudInstTantQue(Noeud* condition, Noeud* sequence)
: m_condition(condition), m_sequence(sequence) {
}

int NoeudInstTantQue::executer() {
  while (m_condition->executer()) m_sequence->executer();
  return 0; // La valeur renvoyée ne représente rien !
}

////////////////////////////////////////////////////////////////////////////////
// NoeudInstRepeter
////////////////////////////////////////////////////////////////////////////////

NoeudInstRepeter::NoeudInstRepeter(Noeud* condition, Noeud* sequence)
: NoeudInstTantQue(condition, sequence) {
}

int NoeudInstRepeter::executer() {
  while (!m_condition->executer()) m_sequence->executer();
  return 0; // La valeur renvoyée ne représente rien !
}

////////////////////////////////////////////////////////////////////////////////
// NoeudInstPour
////////////////////////////////////////////////////////////////////////////////

NoeudInstPour::NoeudInstPour(Noeud* condition, Noeud* sequence, Noeud* affectation1, Noeud* affectation2)
: NoeudInstTantQue(condition, sequence), m_affectation1(affectation1), m_affectation2(affectation2) {
}

int NoeudInstPour::executer() {
  if(m_affectation1 != nullptr) m_affectation1->executer();
  while(m_condition->executer()){
      m_sequence->executer();
      if(m_affectation2 != nullptr) m_affectation2->executer();
  }
  return 0; // La valeur renvoyée ne représente rien !
}

void NoeudInstPour::traduitEnCPP(ostream & cout ,unsigned int indentation)const {
    cout << setw(4 * indentation) << "for(";
    if(m_affectation1 != nullptr) m_affectation1->traduitEnCPP(cout, 0);
    m_condition->traduitEnCPP(cout, indentation);
    if(m_affectation2 != nullptr) m_affectation2->traduitEnCPP(cout, 0);
    cout << " {" << std::endl;
    m_sequence->traduitEnCPP(cout, indentation+1);
    cout << setw(4 * indentation) << "}";
}



////////////////////////////////////////////////////////////////////////////////
// NoeudInstEcrire
////////////////////////////////////////////////////////////////////////////////

NoeudInstEcrire::NoeudInstEcrire() 
: NoeudSeqInst() {}

int NoeudInstEcrire::executer() {
    for(Noeud* p : m_instructions){
        if (typeid(*p)==typeid(SymboleValue) &&  *((SymboleValue*)p)== "<CHAINE>" ){
            string chaine = ((SymboleValue*) p)->getChaine();
            cout << chaine.substr(1, chaine.size()-2);
        }else{
            cout << p->executer();
        }
    }
    cout << std::endl;
  return 0; // La valeur renvoyée ne représente rien !
}


void NoeudInstEcrire::traduitEnCPP(ostream & cout ,unsigned int indentation)const{
    cout << setw(4 * indentation) << "cout " ;
    for(Noeud* p : m_instructions){
        cout << " << ";
        string chaine = ((SymboleValue*) p)->getChaine();
        if (typeid(*p)==typeid(SymboleValue) &&  *((SymboleValue*)p)== "<CHAINE>" ){
            cout << chaine;
        }else {
            cout << chaine.substr(1, chaine.size() - 2);
        }
    }
    cout << ";";
}

////////////////////////////////////////////////////////////////////////////////
// NoeudInstLire
////////////////////////////////////////////////////////////////////////////////

NoeudInstLire::NoeudInstLire() 
: NoeudSeqInst() {}

int NoeudInstLire::executer() {
    for(Noeud* p : m_instructions){
        int valeur;
        cin >> valeur;
        ((SymboleValue*) p)->setValeur(valeur); // On affecte la variable
    }
  return 0; // La valeur renvoyée ne représente rien !
}

void NoeudInstLire::traduitEnCPP(ostream & cout ,unsigned int indentation)const{
    cout << setw(4 * indentation) << "cin ";
    for(Noeud* p : m_instructions){
        cout << " << " << ((SymboleValue*) p)->getChaine();
    }
    cout << ";";
}
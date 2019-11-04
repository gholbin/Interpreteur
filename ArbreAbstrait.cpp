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

void NoeudSeqInst::traduitEnCPP(ostream & cout ,unsigned int indentation)const {
    for (unsigned int i = 0; i < m_instructions.size(); i++){
        m_instructions[i]->traduitEnCPP(cout, indentation);
        cout << std::endl;
    }

}
////////////////////////////////////////////////////////////////////////////////
// NoeudAffectation
////////////////////////////////////////////////////////////////////////////////

NoeudAffectation::NoeudAffectation(Noeud* variable, Noeud* expression)
: m_variable(variable), m_expression(expression) {}

int NoeudAffectation::executer() {
    int valeur = m_expression->executer(); // On exécute (évalue) l'expression
    ((SymboleValue*) m_variable)->setValeur(valeur); // On affecte la variable
    return 0; // La valeur renvoyée ne représente rien !
}

void NoeudAffectation::traduitEnCPP(ostream & cout ,unsigned int indentation)const {
    //if (indentation != 0){}
    cout << setw(4 * indentation) << "";
    m_variable->traduitEnCPP(cout, 0);
    cout << " = ";
    m_expression->traduitEnCPP(cout, 0);
    cout << ";";
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
    else if (this->m_operateur == "et") valeur = (og and od);
    else if (this->m_operateur == "ou") valeur = (og or od);
    else if (this->m_operateur == "non") valeur = (not og);
    else if (this->m_operateur == "/") {
    if (od == 0) throw DivParZeroException();
    valeur = og / od;
    }
    return valeur; // On retourne la valeur calculée
}

void NoeudOperateurBinaire::traduitEnCPP(ostream & cout ,unsigned int indentation)const {
    m_operandeGauche->traduitEnCPP(cout, 0);

    cout << " ";
    if(m_operateur == "et") cout << "and";
    else if(m_operateur == "ou") cout << "or";
    else if(m_operateur == "non") cout << "not";
    else cout << m_operateur.getChaine();
    cout << " ";
    m_operandeDroit->traduitEnCPP(cout, 0);
}

////////////////////////////////////////////////////////////////////////////////
// NoeudInstSi
////////////////////////////////////////////////////////////////////////////////

NoeudInstSi::NoeudInstSi(Noeud* condition, Noeud* sequence)
: m_condition(condition), m_sequence(sequence) {
}

int NoeudInstSi::executer() {
    if (m_condition->executer()) m_sequence->executer();
    return m_condition->executer(); // La valeur renvoyée sert pour savoir si la séquence à été executée
}

void NoeudInstSi::traduitEnCPP(ostream & cout ,unsigned int indentation)const {
    cout  << "if(";
    m_condition->traduitEnCPP(cout, 0);
    cout << "){" << std::endl;
    m_sequence->traduitEnCPP(cout, indentation + 1);
    cout << setw(4 * indentation) << "" << "}";
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
    while(it != m_si_sinonsi.end() and not uneExecution){
        uneExecution = ((NoeudInstSi*) (*it))->executer();
        it++;
    }
    if(!uneExecution && m_sinon != nullptr){
        m_sinon->executer();
    }
    return 0; // La valeur renvoyée ne représente rien !
}

void NoeudInstSiRiche::ajoute(NoeudInstSi* sinonSi){
    m_si_sinonsi.push_back(sinonSi);
}
void NoeudInstSiRiche::ajouteSinon(Noeud* sinon){
    m_sinon = sinon;
}

void NoeudInstSiRiche::traduitEnCPP(ostream & cout ,unsigned int indentation)const {
    vector<NoeudInstSi *>::const_iterator it;
    for (it = m_si_sinonsi.begin(); it != m_si_sinonsi.end(); it++) {
        if (it != m_si_sinonsi.begin()) {
            cout <<  "else ";
        }else{
            cout << setw(4 * indentation) << "";
        }
        (*it)->traduitEnCPP(cout, indentation);
    }
    if (m_sinon != nullptr) {
        cout << "else{" << std::endl;
        m_sinon->traduitEnCPP(cout, indentation + 1);
    }
    cout << setw(4 * indentation) << "" << "}";
}
////////////////////////////////////////////////////////////////////////////////
// NoeudInstTantQue
////////////////////////////////////////////////////////////////////////////////

NoeudInstTantQue::NoeudInstTantQue(Noeud* condition, Noeud* sequence)
: m_condition(condition), m_sequence(sequence) {}

int NoeudInstTantQue::executer() {
    while (m_condition->executer()) m_sequence->executer();
    return 0; // La valeur renvoyée ne représente rien !
}
void NoeudInstTantQue::traduitEnCPP(ostream & cout ,unsigned int indentation)const {
    cout << setw(4 * indentation) << "" << "while(";
    m_condition->traduitEnCPP(cout, 0);
    cout << "){" << std::endl;
    m_sequence->traduitEnCPP(cout, indentation+1);
    cout << setw(4 * indentation) << "" << "}";
}


////////////////////////////////////////////////////////////////////////////////
// NoeudInstRepeter
////////////////////////////////////////////////////////////////////////////////

NoeudInstRepeter::NoeudInstRepeter(Noeud* condition, Noeud* sequence)
: m_condition(condition), m_sequence(sequence) {}

int NoeudInstRepeter::executer() {
    do{
        m_sequence->executer();
    } while(!m_condition->executer());
    return 0; // La valeur renvoyée ne représente rien !
}

void NoeudInstRepeter::traduitEnCPP(ostream & cout ,unsigned int indentation)const {
    cout << setw(4 * indentation) << "" << "do{" << std::endl;
    m_sequence->traduitEnCPP(cout, indentation+1);
    cout << setw(4 * indentation) << "" << "}while(!(";
    m_condition->traduitEnCPP(cout, 0);
    cout << "));" ;
};

////////////////////////////////////////////////////////////////////////////////
// NoeudInstPour
////////////////////////////////////////////////////////////////////////////////

NoeudInstPour::NoeudInstPour(Noeud* condition, Noeud* sequence, Noeud* affectation1, Noeud* affectation2)
: m_condition(condition), m_sequence(sequence), m_affectation1(affectation1), m_affectation2(affectation2) {
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
    if(m_affectation1 != nullptr){
        m_affectation1->traduitEnCPP(cout, indentation);
        cout << std::endl;
    }
    cout << setw(4 * indentation) << "" << "while(";
    m_condition->traduitEnCPP(cout, 0);
    cout << ") {" << std::endl;
    m_sequence->traduitEnCPP(cout, indentation+1);
    if(m_affectation2 != nullptr) m_affectation2->traduitEnCPP(cout, indentation+1);
    cout << std::endl;
    cout << setw(4 * indentation) << "" << "}";
}



////////////////////////////////////////////////////////////////////////////////
// NoeudInstEcrire
////////////////////////////////////////////////////////////////////////////////

NoeudInstEcrire::NoeudInstEcrire() 
: NoeudSeqInst() {}

int NoeudInstEcrire::executer() {
    for(Noeud* p : m_instructions){
        if (typeid(*p)==typeid(SymboleValue) &&    *((SymboleValue*)p)== "<CHAINE>" ){
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
    cout << setw(4 * indentation) << "" << "std::cout " ;
    for(Noeud* p : m_instructions){
        cout << " << ";
        cout << ((SymboleValue*) p)->getChaine();
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
    cout << setw(4 * indentation) << "" << "std::cin ";
    for(Noeud* p : m_instructions){
        cout << " >> ";
        ((SymboleValue*)p)->traduitEnCPP(cout, 0);
    }
    cout << ";";
}
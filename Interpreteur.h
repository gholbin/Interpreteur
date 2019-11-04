#ifndef INTERPRETEUR_H
#define INTERPRETEUR_H

#include "Symbole.h"
#include "Lecteur.h"
#include "Exceptions.h"
#include "TableSymboles.h"
#include "ArbreAbstrait.h"



class Interpreteur {
public:
	Interpreteur(ifstream & fichier);     // Construit un interpréteur pour interpreter
	                                    //    le programme dans    fichier

	void analyse();                     // Si le contenu du fichier est conforme à la grammaire,
	                                    //     cette méthode se termine normalement et affiche un message "Syntaxe correcte".
                                        //     la table des symboles (ts) et l'arbre abstrait (arbre) auront été construits
	                                    // Sinon, une exception sera levée

	inline const TableSymboles & getTable () const    { return m_table;    } // accesseur
	inline Noeud* getArbre () const { return m_arbre; }                    // accesseur
	void traduitEnCPP(ostream & cout,unsigned int indentation)const;

private:
    Lecteur        m_lecteur;    // Le lecteur de symboles utilisé pour analyser le fichier
    TableSymboles    m_table;    // La table des symboles valués
    Noeud*         m_arbre;    // L'arbre abstrait
    bool             m_erreur;        // La detection d'erreur
    // Implémentation de la grammaire
    Noeud*    programme();      // <programme> ::= procedure principale() <seqInst> finproc FIN_FICHIER
    Noeud*    seqInst();        // <seqInst> ::= <inst> { <inst> }
    Noeud*    inst();	        // <inst> ::= <affectation> ; | <instSi>
    Noeud*    affectation();    // <affectation> ::= <variable> = <expression>
    Noeud*    expression();     // <expression> ::= <facteur> { <opBinaire> <facteur> }
    Noeud*    facteur();        // <facteur> ::= <entier> | <variable> | - <facteur> | non <facteur> | ( <expression> )
                                // <opBinaire> ::= + | - | *    | / | < | > | <= | >= | == | != | et | ou
    Noeud*    instSiRiche();    // <instSiRiche> ::=si(<expression>) <seqInst> {sinonsi(<expression>) <seqInst> }[sinon <seqInst>]finsi
    Noeud*    instTantQue();    // <instTantQue> ::=tantque( <expression> ) <seqInst> fintantque
    Noeud*    instRepeter();    // <instRepeter> ::=repeter <seqInst> jusqua( <expression> )
    Noeud*    instPour();       // <instPour>    ::=pour( [ <affectation> ] ; <expression> ;[ <affectation> ]) <seqInst> finpour
    Noeud*    instEcrire();     // <instEcrire>    ::=ecrire( <expression> | <chaine> {, <expression> | <chaine> })
    Noeud*    instLire();       // <instLire>    ::=lire( <variable> {, <variable> })



    //Decomposition de l'expression pour la priorité
    //Noeud*    expression();   //<expression> ::= <expEt> {ou <expEt> }
    Noeud*    expEt();          // <expEt> ::= <expComp> {et <expComp> }
    Noeud*    expComp();        // <expComp> ::= <expAdd> {==|!=|<|<=|>|>= <expAdd> }
    Noeud*    expAdd();         // <expAdd> ::= <expMult> {+|-<expMult> }
    Noeud*    expMult();        // <expMult>::= <facteur> {*|/<facteur> }

    static const std::vector<std::vector<const char const*>> exp;






    //| <instRepeter> ; | <instPour> | <instEcrire> ; | <instLire> ;
    // outils pour simplifier l'analyse syntaxique
    void tester (const string & symboleAttendu) const;     // Si symbole courant != symboleAttendu, on lève une exception
    void testerEtAvancer(const string & symboleAttendu); // Si symbole courant != symboleAttendu, on lève une exception, sinon on avance
    void erreur (const string & mess) const;             // Lève une exception "contenant" le message mess
};

#endif /* INTERPRETEUR_H */

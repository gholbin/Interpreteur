#include "TantQueTest.h"
#include "../ArbreAbstrait.h"
#include "../Interpreteur.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TantQueTest);

TantQueTest::TantQueTest() {
}

TantQueTest::~TantQueTest() {
}

void TantQueTest::setUp() {
}

void TantQueTest::tearDown() {
}

void TantQueTest::testExecution(){
    ifstream file("testUnitaire/TestTantQue_SUCCESS.txt");
    try {
        if (file.fail()) throw FichierException();
        Interpreteur interpreteur(file);

        CPPUNIT_ASSERT_NO_THROW_MESSAGE("Analyse du programme de test", interpreteur.analyse());

        if (interpreteur.getArbre() != nullptr) interpreteur.getArbre()->executer();
        int k = 0;
        while(k < interpreteur.getTable().getTaille() && interpreteur.getTable()[k].getChaine() != "i") k++;
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Verification de la valeur finale de i", 20, interpreteur.getTable()[k].getValeur());
    } catch (InterpreteurException & e) {
        std::cout << e.what() << std::endl;
    }
}

void TantQueTest::testFailedExecution(){
    ifstream file("testUnitaire/TestTantQue_FAILED.txt");
    try {
        if (file.fail()) throw FichierException();
        Interpreteur interpreteur(file);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Verification que l'arbre n'existe pas", (Noeud*) nullptr, interpreteur.getArbre());        
    } catch (InterpreteurException & e) {
        std::cout << e.what() << std::endl;
    }
}

void TantQueTest::testTranslation(){
    ifstream file("testUnitaire/TestTantQue_SUCCESS.txt");
    try {
        if (file.fail()) throw FichierException();
        Interpreteur interpreteur(file);

        interpreteur.analyse();

        if (interpreteur.getArbre() != nullptr) {
            ofstream cout;
            cout.open("prog.cpp", ios::out);
            interpreteur.traduitEnCPP(cout, 0);
            cout.close();
            string command = "g++ prog.cpp -o prog";
            CPPUNIT_ASSERT_NO_THROW_MESSAGE("Compilation du programme", system(command.c_str()));
        }

    } catch (InterpreteurException & e) {
        std::cout << e.what() << std::endl;
    }
}


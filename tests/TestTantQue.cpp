#include "TestTantQue.h"
#include <iterator>

CPPUNIT_TEST_SUITE_REGISTRATION(TestTantQue);

void TestTantQue::testExecution(){
    ifstream file("TestTantQue_Failed.txt");
    try {
        if (file.fail()) throw FichierException();
        Interpreteur interpreteur(file);

        CPPUNIT_ASSERT_NO_THROW_MESSAGE("Analyse du programme de test", interpreteur.analyse());

        if (interpreteur.getArbre() != nullptr) interpreteur.getArbre()->executer();
        int i = 0;
        while(i < interpreteur.getTable().getTaille() && interpreteur.getTable()[i]) i++;
        CPPUNIT_ASSERT_EQUAL_MESSAGE("Verification de la valeur finale de i", 20, (interpreteur.getTable())[i].getValeur());

    } catch (InterpreteurException & e) {
        std::cout << "Fichier : " << fileName << std::endl;
        std::cout << e.what() << std::endl;
    }
}

void TestTantQue::testFailedExecution(){
    ifstream file("TestTantQue_Failed.txt");
    try {
        if (file.fail()) throw FichierException();
        Interpreteur interpreteur(file);

        CPPUNIT_ASSERT_THROW_MESSAGE("Analyse du programme de test", interpreteur.analyse(), SyntaxeException);

    } catch (InterpreteurException & e) {
        std::cout << "Fichier : " << fileName << std::endl;
        std::cout << e.what() << std::endl;
    }
}

void testTranslation(){
    ifstream file("TestTantQue_SUCCESS.txt");
    try {
        if (file.fail()) throw FichierException();
        Interpreteur interpreteur(file);

        CPPUNIT_ASSERT_NO_THROW_MESSAGE("Analyse du programme de test", interpreteur.analyse());

        if (interpreteur.getArbre() != nullptr) {
            string translation = "int main(){\n"
                    + setW(4) + "int i;\n"
                    + setW(4) + "while(i < 4){\n"
                    + setW(8) + "i = i + 1;\n"
                    + setW(4) + "return 0;\n"
                    + setW(4) + "}\n" ;

            CPPUNIT_ASSERT_EQUAL_MESSAGE("Verification de la Traduction du programme de test en CPP",
                    )
            interpreteur.getArbre()->traduitEnCPP();
        }

    } catch (InterpreteurException & e) {
        std::cout << "Fichier : " << fileName << std::endl;
        std::cout << e.what() << std::endl;
    }
}
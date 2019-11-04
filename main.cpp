#include <iostream>
#include "Interpreteur.h"
#include "Exceptions.h"

int main(int argc, char* argv[]) {
    string nomFich;
    if (argc != 2) {
        std::cout << "Usage : " << argv[0] << " nom_fichier_source" << std::endl << std::endl;
        std::cout << "Entrez le nom du fichier que voulez-vous interpréter : ";
        getline(std::cin, nomFich);
    } else
        nomFich = argv[1];
    ifstream fichier(nomFich.c_str());
    try {
        if (fichier.fail()) throw FichierException();
        Interpreteur interpreteur(fichier);
        interpreteur.analyse();
        // Si pas d'exception levée, l'analyse syntaxique a réussi
        std::cout << std::endl << "================ Syntaxe Correcte" << std::endl;
        // On affiche le contenu de la table des symboles avant d'exécuter le programme
        std::cout << std::endl << "================ Table des symboles avant exécution : " << interpreteur.getTable();

        // On exécute le programme si l'arbre n'est pas vide
        if (interpreteur.getArbre() != nullptr) {
            std::cout << std::endl << "================ Execution de l'arbre" << std::endl;
            interpreteur.getArbre()->executer();

            std::cout << std::endl << "================ Réécriture du fichier en C++" << std::endl;
            const string cppFileName = nomFich.substr(0, nomFich.size() - 4);
            ofstream cout;
            cout.open(cppFileName + ".cpp", ios::out);
            interpreteur.getArbre()->traduitEnCPP(cout, 0);

            std::cout << std::endl << "================ Compilation du fichier" << std::endl;
            string command = "g++ ";
            command += cppFileName.c_str();
            command += ".cpp -o ";
            command += cppFileName;
            system(command.c_str());

            // Et on vérifie qu'il a fonctionné en regardant comment il a modifié la table des symboles
            std::cout << std::endl << "================ Table des symboles apres exécution : "
                      << interpreteur.getTable();
        }
    } catch (InterpreteurException & e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}

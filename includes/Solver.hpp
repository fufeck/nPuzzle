#include <iostream>
#include <vector>
#include <map>
 
#include <SFML/System.hpp>
 
typedef unsigned char uchar;
 
/************************/
 
const uchar nombre_de_lignes    = 3;
const uchar nombre_de_colonnes  = 3;
 
const uchar lignes[nombre_de_lignes][nombre_de_colonnes] = {
    { 6, 3, 1 },
    { 8, 0, 2 },
    { 7, 4, 5 }
};
 
const uchar numero_case_vide    = 8;
 
/************************/
 
typedef unsigned int uint;
typedef std::vector< std::vector<uchar> > Jeu;
 
struct Noeud {
    uint F;
    uint G;
    uint H;
    Jeu parent;
};
 
typedef std::map<Jeu, Noeud> Liste;
 
inline void afficherJeu(Jeu &jeu) {
    std::cout << std::endl;
    for (uchar i = 0; i < nombre_de_lignes; ++i) {
        for (uchar j = 0; j < nombre_de_colonnes; ++j) {
            std::cout << (uint)jeu[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
 
// distance de manathan entre deux points
typedef std::<uchar, uchar> Point;

inline uint distanceManhattan(const Point &p1, const Point &p2) {
    return (p1.first - p2.first) * (p1.first - p2.first) + (p1.second - p2.second) * (p1.second - p2.second);
}
 
inline Point localiserPositionFinale(uchar valeur, const Jeu &etatFini) {
    for ( uchar i = 0; i < nombre_de_lignes; ++i) {
        for ( uchar j = 0; j < nombre_de_colonnes; ++j ) {
            if ( etatFini[i][j] == valeur ) {
                return Point(i, j);
            }
        }
    }
}
 
// une case mal placée = +1 d'heuristique
inline uint heuristiqueSimple(const Jeu &jeu, const Jeu &etatFini) {
    uint k = 0;
    for ( uchar i = 0; i < nombre_de_lignes; ++i) {
        for ( uchar j = 0; j < nombre_de_colonnes; ++j ) {
            if ( jeu[i][j] != etatFini[i][j] ) {
                ++k;
            }
        }
    }
    return k;
}
 
// une case mal placée = +distance de manathan à la destination d'heuristique
inline uint heuristiqueManathan(const Jeu &jeu, const Jeu &etatFini) {
    uint k = 0;
    for ( uchar i = 0; i < nombre_de_lignes; ++i) {
        for ( uchar j = 0; j < nombre_de_colonnes; ++j ) {
            k += distanceManhattan( Point(i, j), localiserPositionFinale(jeu[i][j], etatFini) );
        }
    }
    return k;
}
 
 
class Solveur : public sf::Thread {
 
    public :
        Solveur() {    
            // heuristique choisie
            heuristique = &heuristiqueManathan;
 
            // initialiser le jeu
            for (uchar i = 0; i < nombre_de_lignes; ++i) {
                m_jeu.push_back( std::vector<uchar>(nombre_de_colonnes) );
                for (uchar j = 0; j < nombre_de_colonnes; ++j) {
                    m_jeu[i][j] = lignes[i][j];
                }
            }
 
            // trouver quel est le jeu dans son etat fini, pour ne pas le rechercher a chaque fois
            for (uchar i=0; i < nombre_de_lignes; ++i) {
                m_etatFini .push_back( std::vector<uchar>(nombre_de_colonnes) );
                for (uchar j=0; j < nombre_de_colonnes; ++j) {
                    m_etatFini[i][j] = nombre_de_lignes*i+j;
                }
            }
 
            // on met le premier noeud dans l'open list
            Noeud noeud;
            noeud.F = noeud.G = 0;
            noeud.H = (*heuristique)(m_jeu, m_etatFini);
            noeud.parent = m_jeu;
            openList[m_jeu] = noeud;
 
        }
 
        void Run() {
 
            Noeud noeud;
            Jeu jeu;
 
            bool okPourCeTour;
            uint nombre_tentative = 0;
 
            while ( ! openList.empty() ) {
 
                mutex.Lock();
 
                // recuperation du meilleur noeud
                meilleurJeuListeOuverte(jeu);
                noeud = openList[jeu];
 
                // passage dans la liste fermee
                closedList[jeu] = noeud;
 
                // suppression de la liste ouverte
                openList.erase(jeu);
 
                mutex.Unlock();
 
                if ( jeu == m_etatFini ) {
                    break;
                }
 
                // regarder les permutations possibles
                okPourCeTour = false;
                for (uchar i=0; i < nombre_de_lignes && ! okPourCeTour; ++i) {
                    for (uchar j=0; j < nombre_de_colonnes && ! okPourCeTour; ++j ) {
 
                        // on cherche la case vide
                        if ( jeu[i][j] != numero_case_vide ) {
                            continue;
                        }
 
                        // on a notre case vide
                        okPourCeTour = true;
                        ++nombre_tentative;
 
                        if ( j > 0 ) {
                            Jeu nJeu = jeu;
                            nJeu[i][j]   = jeu[i][j-1];
                            nJeu[i][j-1] = jeu[i][j];
                            Noeud nNoeud;
                            nNoeud.G = noeud.G + 1;
                            nNoeud.H = (*heuristique)(nJeu, m_etatFini);
                            nNoeud.F = nNoeud.G + nNoeud.H;
                            nNoeud.parent = jeu;
                            ajouterDansOpenList(nJeu, nNoeud, noeud);
                        }
 
                        if ( j < nombre_de_colonnes - 1 ) {
                            Jeu nJeu = jeu;
                            nJeu[i][j]   = jeu[i][j+1];
                            nJeu[i][j+1] = jeu[i][j];
                            Noeud nNoeud;
                            nNoeud.G = noeud.G + 1;
                            nNoeud.H = (*heuristique)(nJeu, m_etatFini);
                            nNoeud.F = nNoeud.G + nNoeud.H;
                            nNoeud.parent = jeu;
                            ajouterDansOpenList(nJeu, nNoeud, noeud);
                        }
 
                        if ( i > 0 ) {
                            Jeu nJeu = jeu;
                            nJeu[i][j]   = jeu[i-1][j];
                            nJeu[i-1][j] = jeu[i][j];
                            Noeud nNoeud;
                            nNoeud.G = noeud.G + 1;
                            nNoeud.H = (*heuristique)(nJeu, m_etatFini);
                            nNoeud.F = nNoeud.G + nNoeud.H;
                            nNoeud.parent = jeu;
                            ajouterDansOpenList(nJeu, nNoeud, noeud);
                        }
 
                        if ( i < nombre_de_lignes - 1 ) {
                            Jeu nJeu = jeu;
                            nJeu[i][j]   = jeu[i+1][j];
                            nJeu[i+1][j] = jeu[i][j];
                            Noeud nNoeud;
                            nNoeud.G = noeud.G + 1;
                            nNoeud.H = (*heuristique)(nJeu, m_etatFini);
                            nNoeud.F = nNoeud.G + nNoeud.H;
                            nNoeud.parent = jeu;
                            ajouterDansOpenList(nJeu, nNoeud, noeud);
                        }
 
                    }
                }
            }
 
            if ( jeu != m_etatFini ) {
                std::cout << "pas de solution" << std::endl;
                return;
            }
            std::cout << "solution trouvée en " << nombre_tentative << " tentatives" << std::endl;
 
            std::vector<Jeu> vj;
            vj.push_back(jeu);
            while ( noeud.parent != m_jeu ) {
                jeu = noeud.parent;
                vj.push_back( jeu );
                noeud = closedList[jeu];
            }
            std::cout << "solution en " << vj.size() << " coups :" << std::endl;
 
            afficherJeu(m_jeu);
            for ( std::vector<Jeu>::iterator it = --vj.end(); it != vj.begin(); --it) {
                afficherJeu(*it);
            }
            afficherJeu(m_etatFini);
        }
 
    protected :
 
        inline void ajouterDansOpenList(const Jeu &jeu, const Noeud &nouveauNoeud, const Noeud &noeudRemplace) {
            mutex.Lock();
            if ( openList.find(jeu) == openList.end() ) { // pas encore dans la liste ouverte, donc on l'ajoute
                openList[jeu] = nouveauNoeud;
            } else { // déjà dans la liste ouverte, avons nous trouvé un meilleur chemin ?
                if ( nouveauNoeud.F < noeudRemplace.F ) {
                   openList[jeu] = nouveauNoeud;
                }
            }
            mutex.Unlock();
        }
 
        inline void meilleurJeuListeOuverte(Jeu &retour) {
            retour = openList.begin()->first;
            uint plusBasCout = openList.begin()->second.F;
 
            for ( Liste::iterator it = ++openList.begin(); it != openList.end(); ++it) {
                if ( it->second.F < plusBasCout ) {
                    retour = it->first;
                    plusBasCout = it->second.F;
                }
            }
        }
 
        // attributs
        Liste openList;
        Liste closedList;
 
        Jeu m_jeu;
        Jeu m_etatFini;
 
        uint (*heuristique)(const Jeu&, const Jeu&);
 
        sf::Mutex mutex;
};

int main(void) {
    Solveur solveur;
    solveur.Launch();
 
    solveur.Wait();
    return 0;
}
 
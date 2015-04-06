// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Npuzzle.cpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ftaffore <ftaffore@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/04/01 15:36:17 by ftaffore          #+#    #+#             //
//   Updated: 2015/04/01 15:36:18 by ftaffore         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <unistd.h>
#include "Npuzzle.hpp"

Npuzzle::Npuzzle(std::string const &fileName) : _n(0) {
	// initialiser le jeu a sont etat de depart
	this->_createMap(fileName);

	// trouver quel est le jeu dans son etat fini, pour ne pas le rechercher a chaque fois
	for (int y = 0; y < this->_n; ++y) {
		this->_mapFinish.push_back(std::vector<int>(this->_n));
		for (int x = 0; x < this->_n; ++x) {
			this->_mapFinish[y][x] = static_cast<int>(this->_n * y + x);
		}
	}
	// on met le premier noeud dans l'open list
  
	for (int y = 0; y < this->_n; ++y) {
		for (int x = 0; x < this->_n; ++x) {
			std::cout <<  this->_mapFinish[y][x] << " ";
		}
		std::cout << std::endl;
	}
	std::cout 	<< std::endl;
	for (int y = 0; y < this->_n; ++y) {
		for (int x = 0; x < this->_n; ++x) {
			std::cout <<  this->_mapStart[y][x] << " ";
		}
		std::cout << std::endl;
	}
	std::cout 	<< std::endl;

	Noeud noeud;

	noeud.F = 0;
	noeud.G = 0;
	noeud.H = this->_getHeuristiqueManathan(this->_mapStart);
	noeud.parent = this->_mapStart;
	this->_openList[this->_mapStart] = noeud;

}

Npuzzle::~Npuzzle() {

}


/************************/
/*******HEURISTIQUE******/
/************************/

void				Npuzzle::_createMap(std::string const &filename) {
	//std::istringstream 	nb;
	std::ifstream   	file;
	std::string 		line;
	int 				nbLine = 0;

	file.open(filename.c_str());
	if (!file.is_open()) {
		std::cerr << "file can't be open" << std::endl;
		throw std::exception();
	}

	while (file.tellg() >= 0) {
		std::stringstream 	ss;
		std::string 		command;
		std::getline(file, line);
	
		ss.str(line);
		ss >> command;

		if (command != "" && command[0] != '#') {
			std::stringstream 	iss(line);
			if (nbLine == 0) {
				iss >> this->_n;
			} else {
				this->_mapStart.push_back(std::vector<int>(this->_n));
				for (int i = 0; i < this->_n; ++i) {
					iss >> this->_mapStart[nbLine - 1][i];
				}
			}
			nbLine++;
		}
	}
	file.close();
}

int 				Npuzzle::_getDistanceManhattan(const Point &p1, const Point &p2) {

	return (p1.y - p2.y) * (p1.y - p2.y) + (p1.x - p2.x) * (p1.x - p2.x);
}
 
Point 				Npuzzle::_getPositionFinale(int valeur) {
	Point 			p;
	for (int y = 0; y < this->_n; ++y) {
		for (int x = 0; x < this->_n; ++x) {
			if (this->_mapFinish[y][x] == valeur) {
				p.x = x;
				p.y = y;
				return p;
			}
		}
	}
	std::cerr << "ERROR : _getPositionFinale Fail." << std::endl;
	throw std::exception();
	return p;
}
 
int 				Npuzzle::_getHeuristiqueManathan(const Map &map) {
	int 			k = 0;

	for (int y = 0; y < this->_n; ++y) {
		for (int x = 0; x < this->_n; ++x) {
			Point p;
			p.x = x;
			p.y = y;
			k += this->_getDistanceManhattan(p, this->_getPositionFinale(map[y][x]));
		}
	}
	return k;
}


/************************/
/******MANAGE*NOEUD******/
/************************/


void				Npuzzle::_bestMapOpened(Map &ret) {
	ret = this->_openList.begin()->first;
	int 			min = this->_openList.begin()->second.F;

	int i = 0;
	for (Liste::iterator it = ++this->_openList.begin(); it != this->_openList.end(); ++it) {
		std::cout << it->second.F << std::endl;
		std::cout << it->second.G << std::endl;
		std::cout << it->second.H << std::endl << std::endl;
		if (it->second.F < min) {
			std::cout << "i = " << i  << std::endl; 
			ret = it->first;
			min = it->second.F;
		}
		i++;
	}
}

Point				Npuzzle::_getEmptyCase(Map const &map) {
	Point 			p;
	p.x = 0;
	p.y = 0;
	// on cherche la case vide
   for (int y = 0; y < this->_n; ++y) {
		for (int x = 0; x < this->_n; ++x) {
			if (map[y][x] == EMPTY_CASE) {
				//std::cout << "RET : " << x << ", " << y << std::endl;
				p.x = x;
				p.y = y;
				//std::cout << "RET EMPTY_CASE : " << p.x << ", " << p.y << std::endl;
				//sleep(1);
				return p;
			}
		}
	}
	std::cerr << "ERROR : _getEmptyCase Fail." << std::endl;
	throw std::exception();
	return p;
}

Noeud				Npuzzle::_createNoeud(Noeud &noeud, Map &nMap, Map &map) {
	Noeud 			nNoeud;

	std::cout << "CREATE NO" << std::endl;
	nNoeud.G = noeud.G + 1;
	nNoeud.H = this->_getHeuristiqueManathan(nMap);
	nNoeud.F = nNoeud.G + nNoeud.H;
	nNoeud.parent = map;
	return noeud;
}

void 				Npuzzle::_addInOpenList(const Map &map, const Noeud &newNoeud, const Noeud &oldNoeud) {
	//mutex.Lock();
	if (this->_openList.find(map) == this->_openList.end()) { // pas encore dans la liste ouverte, donc on l'ajoute
		this->_openList[map] = newNoeud;
	} else { // déjà dans la liste ouverte, avons nous trouvé un meilleur chemin ?
		if (newNoeud.F < oldNoeud.F) {
		   this->_openList[map] = newNoeud;
		}
	}
	//mutex.Unlock();
}

/************************/
/******SOLVE*PUZZLE******/
/************************/


void 				Npuzzle::_addAllOpenList(Noeud &noeud, Map &map, Point &p) {
	//std::cout << "ONE" << std::endl;
	if (p.x > 0) {
		//std::cout << "merde" << std::endl;
		//std::cout << p.y << std::endl;
		//std::cout << p.x << std::endl;
		Map 		nMap = map;
		nMap[p.y][p.x] = map[p.y][p.x - 1];
		nMap[p.y][p.x - 1] = map[p.y][p.x];
		
		//std::cout << "ONE ONE" << std::endl;
		Noeud 		nNoeud = this->_createNoeud(noeud, nMap, map);
		//std::cout << "ONE TWO" << std::endl;
		this->_addInOpenList(nMap, nNoeud, noeud);
	}

	if (p.x < this->_n - 1) {
		Map 		nMap = map;
		nMap[p.y][p.x] = map[p.y][p.x + 1];
		nMap[p.y][p.x + 1] = map[p.y][p.x];
		Noeud 		nNoeud = this->_createNoeud(noeud, nMap, map);
		this->_addInOpenList(nMap, nNoeud, noeud);
	}

	if (p.y > 0) {
		Map 		nMap = map;
		nMap[p.y][p.x] = map[p.y - 1][p.x];
		nMap[p.y - 1][p.x] = map[p.y][p.x];
		Noeud 		nNoeud = this->_createNoeud(noeud, nMap, map);
		this->_addInOpenList(nMap, nNoeud, noeud);
	}

	if (p.y < this->_n - 1) {
		Map 		nMap = map;
		nMap[p.y][p.x] = map[p.y + 1][p.x];
		nMap[p.y + 1][p.x] = map[p.y][p.x];
		Noeud 		nNoeud = this->_createNoeud(noeud, nMap, map);
		this->_addInOpenList(nMap, nNoeud, noeud);
	}
}

void 				Npuzzle::solveNpuzzle(void) {

	int 			nombre_tentative = 0;
	Noeud 			noeud;
	Point			emptyCase;
	Map 			map;


	while (!this->_openList.empty()) {

		//mutex.Lock();

		// recuperation du meilleur noeud
		//std::cout << "START" << std::endl;
		this->_bestMapOpened(map);
		noeud = this->_openList[map];

		// passage dans la liste fermee
		this->_closedList[map] = noeud;

		// suppression de la liste ouverte
		this->_openList.erase(map);

		//mutex.Unlock();

		// check end
		if (map == this->_mapFinish) {
			break ;
		}

		//std::cout << "GO" << std::endl;
		emptyCase = this->_getEmptyCase(map);
		//std::cout << "STACH" << std::endl;
		++nombre_tentative;
		this->_addAllOpenList(noeud, map, emptyCase);
		//std::cout << "END" << std::endl;
		std::cout << "============================" << std::endl;
		sleep(1);
	}

	if (map != this->_mapFinish) {
		std::cout << "pas de solution" << std::endl;
		return;
	}
	std::cout << "solution trouvée en " << nombre_tentative << " tentatives" << std::endl;
/*
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
	afficherJeu(m_etatFini);*/
}

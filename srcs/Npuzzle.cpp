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
	this->_createFinishMap();

	Noeud noeud;

	noeud.G = 0;
	noeud.H = this->_getDistanceManathan(this->_mapStart);
	noeud.F = 0;
	noeud.parent = this->_mapStart;
	this->_openList[this->_mapStart] = noeud;

}

Npuzzle::~Npuzzle() {

}

bool				Npuzzle::_checkSolvable(void) {
	int a = 0;
	int b = 0;
	for (int y = 0; y < this->_n; ++y) {
		for (int x = 0; x < this->_n; ++x) {
			int c = this->_mapStart[y][x];
			Point p = this->_pointFinish[c];
			if (this->_mapStart[y][x] == EMPTY_CASE) {
				a = ((y - p.y >= 0) ? (y - p.y) : (-(y - p.y))) + ((x - p.x >= 0) ? (x - p.x) : (-(x - p.x)));
			} else {
				b += ((y - p.y >= 0) ? (y - p.y) : (-(y - p.y))) + ((x - p.x >= 0) ? (x - p.x) : (-(x - p.x)));
			}
		}
	}
	std::cout << a << " == " << b << std::endl;
	if (a % 2 != b % 2) {
		std::cerr << "file can't be open" << std::endl;
		return false;
	}
	return true;
}

void				Npuzzle::_displayMap(Map const &map) const {
	for (int y = 0; y < this->_n; ++y) {
		for (int x = 0; x < this->_n; ++x) {
			if (this->_n > 3 && map[y][x] < 10) {
				std::cout << " ";
			}
			std::cout << map[y][x] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void				Npuzzle::_createMap(std::string const &filename) {
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

void				Npuzzle::_createFinishMap(void) {
	for (int y = 0; y < this->_n; ++y) {
		this->_mapFinish.push_back(std::vector<int>(this->_n));
		for (int x = 0; x < this->_n; ++x) {
			int 	v = static_cast<int>(this->_n * y + x);
			this->_mapFinish[y][x] = v;
			Point	p;
			p.x = x;
			p.y = y;
			this->_pointFinish[v] = p;
		}
	}
}


/************************/
/******MANAGE*NOEUD******/
/************************/

int 				Npuzzle::_getDistanceManathan(const Map &map) {
	int 			k = 0;
	int 			a = 0;
	int 			b = 0;

	for (int y = 0; y < this->_n; ++y) {
		for (int x = 0; x < this->_n; ++x) {
			Point p = this->_pointFinish[map[y][x]];
			if (this->_mapStart[y][x] == EMPTY_CASE) {
				a = ((y - p.y >= 0) ? (y - p.y) : (-(y - p.y))) + ((x - p.x >= 0) ? (x - p.x) : (-(x - p.x)));
			} else {
				b += ((y - p.y >= 0) ? (y - p.y) : (-(y - p.y))) + ((x - p.x >= 0) ? (x - p.x) : (-(x - p.x)));
			}
//			k += ((y - p.y >= 0) ? (y - p.y) : (-(y - p.y))) + ((x - p.x >= 0) ? (x - p.x) : (-(x - p.x)));
		}
	}
	if (a % 2 != b % 2) {
		return -1;
	}
	k = a + b;
	return k;
}


void				Npuzzle::_bestMapOpened(Map &ret) {
	ret = this->_openList.begin()->first;
	int 			min = this->_openList.begin()->second.F;

	for (Liste::iterator it = ++this->_openList.begin(); it != this->_openList.end(); ++it) {
		if (it->second.F < min) {
			ret = it->first;
			min = it->second.F;
		}
	}
}

//METTRE EMPTY CASE DANS NOEUD
Point				Npuzzle::_getEmptyCase(Map const &map) {
	Point 			p;
	p.x = 0;
	p.y = 0;
   for (int y = 0; y < this->_n; ++y) {
		for (int x = 0; x < this->_n; ++x) {
			if (map[y][x] == EMPTY_CASE) {
				p.x = x;
				p.y = y;
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

	nNoeud.G = noeud.G + 1;
	nNoeud.H = this->_getDistanceManathan(nMap);
	nNoeud.F = nNoeud.G + nNoeud.H;
	nNoeud.parent = map;
	if (nNoeud.H < 0)
		this->_closedList[nMap] = nNoeud;
	return nNoeud;
}

void 				Npuzzle::_addInOpenList(const Map &map, const Noeud &newNoeud, const Noeud &oldNoeud) {
	//mutex.Lock();
	if (this->_openList.find(map) == this->_openList.end() && \
		this->_closedList.find(map) == this->_closedList.end()) {
		this->_openList[map] = newNoeud;
	} else {
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
	if (p.x > 0) {
		Map 		nMap = map;
		nMap[p.y][p.x] = map[p.y][p.x - 1];
		nMap[p.y][p.x - 1] = map[p.y][p.x];
		Noeud 		nNoeud = this->_createNoeud(noeud, nMap, map);
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

//	if (this->_checkSolvable() == false)
//		return ;
	while (!this->_openList.empty()) {

		//mutex.Lock();

		this->_bestMapOpened(map);
		noeud = this->_openList[map];
		this->_closedList[map] = noeud;
		this->_openList.erase(map);

		//mutex.Unlock();

		if (map == this->_mapFinish) {
			break ;
		}

		emptyCase = this->_getEmptyCase(map);
		++nombre_tentative;
		this->_addAllOpenList(noeud, map, emptyCase);
		if (nombre_tentative % 1000 == 0) {
			std::cout << nombre_tentative << std::endl;
		}
		//std::cout << "===========================" << std::endl;
		//sleep(1);
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

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
	Point		emptyCase = this->_createMap(fileName);
	std::cout << this->_n << std::endl;
	this->_displayMap(this->_mapStart);
	this->_createFinishMap();

	Noeud noeud;

	noeud.G = 0;
	noeud.H = this->_getHeuristique(this->_mapStart);
	noeud.F = 0;
	noeud.parent = this->_mapStart;
	noeud.emptyCase = emptyCase;
	this->_openList[this->_mapStart] = noeud;

}

Npuzzle::~Npuzzle() {

}

/************************/
/******USEFULL*FUNC******/
/************************/

void					Npuzzle::_displayMap(Map const &map) const {
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

Point					Npuzzle::_createMap(std::string const &filename) {
	Point 				emptyCase;
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
					if (this->_mapStart[nbLine - 1][i] ==  EMPTY_CASE) {
						emptyCase.y = nbLine - 1;
						emptyCase.x = i;
					}
				}
			}
			nbLine++;
		}
	}
	file.close();
	return emptyCase;
}

void					Npuzzle::_createFinishMap(void) {
	int 				nb = 1;
	int 				max = this->_n * this->_n;
	int 				x = 0;
	int 				y = 0;
	int 				nBoucle = 0;

	for (int y = 0; y < this->_n; ++y) {	
		this->_mapFinish.push_back(std::vector<int>(this->_n));
	}
	while (nb < max) {
		while (x < this->_n - nBoucle && nb < max) {
			this->_mapFinish[y][x] = nb;
			Point	p;
			p.x = x;
			p.y = y;
			this->_pointFinish[nb] = p;
			nb++;
			x++;	
		}
		if (nb > max)
			break ;
		y++;
		x--;
		while (y < this->_n - nBoucle && nb < max) {
			this->_mapFinish[y][x] = nb;
			Point	p;
			p.x = x;
			p.y = y;
			this->_pointFinish[nb] = p;
			nb++;
			y++;
		}
		if (nb > max)
			break ;
		x--;
		y--;
		while (x >= nBoucle && nb < max) {
			this->_mapFinish[y][x] = nb;
			Point	p;
			p.x = x;
			p.y = y;
			this->_pointFinish[nb] = p;
			nb++;
			x--;
		}
		if (nb > max)
			break ;
		x++;
		y--;
		nBoucle++;
		while (y > nBoucle && nb < max) {
			this->_mapFinish[y][x] = nb;
			Point	p;
			p.x = x;
			p.y = y;
			this->_pointFinish[nb] = p;
			nb++;
			y--;
		}
	}
	this->_displayMap(this->_mapFinish);
}

/************************/
/******HEURISTIQUES******/
/************************/

int 					Npuzzle::_heuristiqueManathan(const Map &map) {
	int 				k = 0;

	for (int y = 0; y < this->_n; ++y) {
		for (int x = 0; x < this->_n; ++x) {
			Point 		p = this->_pointFinish[map[y][x]];
			k += ((y - p.y >= 0) ? (y - p.y) : (-(y - p.y))) + ((x - p.x >= 0) ? (x - p.x) : (-(x - p.x)));
		}
	}
	return k;
}

int 					Npuzzle::_heuristiqueSimple(const Map &map) {
	int 				k = 0;

	for (int y = 0; y < this->_n; ++y) {
		for (int x = 0; x < this->_n; ++x) {
			if (map[y][x] != this->_mapFinish[y][x])
				++k;
		}
	}
	return k;
}

int 					Npuzzle::_getHeuristique(const Map &map) {

	return this->_heuristiqueManathan(map);
}

/************************/
/******MANAGE*NOEUD******/
/************************/

void					Npuzzle::_bestMapOpened(Map &ret) {
	ret = this->_openList.begin()->first;
	int 			min = this->_openList.begin()->second.F;

	for (Liste::iterator it = ++this->_openList.begin(); it != this->_openList.end(); ++it) {
		if (it->second.F < min) {
			ret = it->first;
			min = it->second.F;
		}
	}
}

Noeud					Npuzzle::_createNoeud(Noeud const &noeud, Map const &nMap, Map const &map, Point const &nEmptyCase) {
	Noeud 			nNoeud;

	nNoeud.G = noeud.G + 1;
	nNoeud.H = this->_getHeuristique(nMap);
	nNoeud.F = nNoeud.G + nNoeud.H;
	nNoeud.parent = map;
	nNoeud.emptyCase = nEmptyCase;
	if (nNoeud.H < 0)
		this->_closedList[nMap] = nNoeud;
	return nNoeud;
}

void 					Npuzzle::_addInOpenList(const Map &map, Noeud const &newNoeud, Noeud const &oldNoeud) {
	if (this->_openList.find(map) == this->_openList.end() && \
		this->_closedList.find(map) == this->_closedList.end()) {
		this->_openList[map] = newNoeud;
	} else {
		if (newNoeud.F < oldNoeud.F) {
			this->_openList[map] = newNoeud;
		}
	}
}

void 					Npuzzle::_addAllOpenList(Noeud const &noeud, Map const &map) {
	Point				p = noeud.emptyCase;

	if (p.x > 0) {
		Map 		nMap = map;
		nMap[p.y][p.x] = map[p.y][p.x - 1];
		nMap[p.y][p.x - 1] = map[p.y][p.x];
		Point		nEmptyCase;
		nEmptyCase.y = p.y;
		nEmptyCase.x = p.x - 1;
		this->_addInOpenList(nMap, this->_createNoeud(noeud, nMap, map, nEmptyCase), noeud);
	}

	if (p.x < this->_n - 1) {
		Map 		nMap = map;
		nMap[p.y][p.x] = map[p.y][p.x + 1];
		nMap[p.y][p.x + 1] = map[p.y][p.x];
		Point		nEmptyCase;
		nEmptyCase.y = p.y;
		nEmptyCase.x = p.x + 1;
		this->_addInOpenList(nMap, this->_createNoeud(noeud, nMap, map, nEmptyCase), noeud);
	}

	if (p.y > 0) {
		Map 		nMap = map;
		nMap[p.y][p.x] = map[p.y - 1][p.x];
		nMap[p.y - 1][p.x] = map[p.y][p.x];
		Point		nEmptyCase;
		nEmptyCase.y = p.y - 1;
		nEmptyCase.x = p.x;
		this->_addInOpenList(nMap, this->_createNoeud(noeud, nMap, map, nEmptyCase), noeud);
	}

	if (p.y < this->_n - 1) {
		Map 		nMap = map;
		nMap[p.y][p.x] = map[p.y + 1][p.x];
		nMap[p.y + 1][p.x] = map[p.y][p.x];
		Point		nEmptyCase;
		nEmptyCase.y = p.y + 1;
		nEmptyCase.x = p.x;
		this->_addInOpenList(nMap, this->_createNoeud(noeud, nMap, map, nEmptyCase), noeud);
	}
}

/************************/
/******SOLVE*PUZZLE******/
/************************/

void 					Npuzzle::run(void) {
	int 				nombre_tentative = 0;
	Noeud 				noeud;
	Map 				map;

	while (!this->_openList.empty()) {
		this->_bestMapOpened(map);
		noeud = this->_openList[map];
		this->_closedList[map] = noeud;
		this->_openList.erase(map);
		++nombre_tentative;
		if (map == this->_mapFinish) {
			break ;
		}
		this->_addAllOpenList(noeud, map);
	}
	if (map != this->_mapFinish) {
		std::cout << "pas de solution" << std::endl;
		return;
	}
	std::cout << "solution trouvée en " << nombre_tentative << " tentatives" << std::endl;
}

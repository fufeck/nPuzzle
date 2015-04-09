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
#include <time.h>
#include "Npuzzle.hpp"


Npuzzle::Npuzzle(std::string const &fileName) : _n(0) {
	Point		emptyCase = this->_createMap(fileName);
	//std::cout << this->_n << std::endl;
	//this->_displayMap(this->_mapStart);
	this->_createFinishMap();

	Noeud noeud;

	//noeud.H = this->_getHeuristique(this->_mapStart);
	//noeud.F = 0;
	noeud.F = this->_getHeuristiqueMap(this->_mapStart);
	noeud.G = 0;
	noeud.parent = NO;
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
	int 				max = this->_n * this->_n + 1;
	int 				x = 0;
	int 				y = 0;
	int 				nBoucle = 0;

	for (int i = 0; i < this->_n; i++) {
		this->_mapFinish.push_back( std::vector<int>(this->_n));
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
	Point	p = this->_pointFinish[this->_n * this->_n];
	this->_pointFinish[EMPTY_CASE] = p;
	this->_pointFinish.erase(this->_n * this->_n);
	this->_mapFinish[p.y][p.x] = EMPTY_CASE;
	//std::cout << std::endl;
	//this->_displayMap(this->_mapFinish);
}

/************************/
/******HEURISTIQUES******/
/************************/

int 					Npuzzle::_heuristiqueManathanMap(const Map &map) {
	int 				k = 0;

	for (int y = 0; y < this->_n; ++y) {
		for (int x = 0; x < this->_n; ++x) {
			Point 		p = this->_pointFinish[map[y][x]];
			k += ((y - p.y >= 0) ? (y - p.y) : (-(y - p.y))) + ((x - p.x >= 0) ? (x - p.x) : (-(x - p.x)));
		}
	}
	return k;
}

int 					Npuzzle::_heuristiqueSimpleMap(const Map &map) {
	int 				k = 0;


	for (int y = 0; y < this->_n; ++y) {
		for (int x = 0; x < this->_n; ++x) {
			if (map[y][x] != this->_mapFinish[y][x])
				++k;
		}
	}
	
	return k;
}

int 					Npuzzle::_getHeuristiqueMap(const Map &map) {
	return this->_heuristiqueManathanMap(map);
}


int 					Npuzzle::_heuristiqueManathanLoop(Map const &map, Point const &p1, Point const &p2) {
	Point 				a = this->_pointFinish[map[p1.y][p1.x]];
	Point				b = this->_pointFinish[map[p2.y][p2.x]];
	int 				k = 0;

	k -= ((a.y - p1.y >= 0) ? (a.y - p1.y) : (-(a.y - p1.y))) + ((a.x - p1.x >= 0) ? (a.x - p1.x) : (-(a.x - p1.x)));
	k -= ((b.y - p2.y >= 0) ? (b.y - p2.y) : (-(b.y - p2.y))) + ((b.x - p2.x >= 0) ? (b.x - p2.x) : (-(b.x - p2.x)));
	k += ((a.y - p2.y >= 0) ? (a.y - p2.y) : (-(a.y - p2.y))) + ((a.x - p2.x >= 0) ? (a.x - p2.x) : (-(a.x - p2.x)));
	k += ((b.y - p1.y >= 0) ? (b.y - p1.y) : (-(b.y - p1.y))) + ((b.x - p1.x >= 0) ? (b.x - p1.x) : (-(b.x - p1.x)));
	return k;
}
/*
int 					Npuzzle::_heuristiqueSimpleLoop(Map const &map, Point const &p1, Point const &p2) {
	int 				k = 0;

	k -= (this->_pointFinish[p1.y][p1.x] == map[p1.y][p1.x]) ? (1) : (0);
	k -= (this->_pointFinish[p2.y][p2.x] == map[p2.y][p2.x]) ? (1) : (0);
	k += (this->_pointFinish[p1.y][p1.x] == map[p2.y][p2.x]) ? (1) : (0);
	k += (this->_pointFinish[p2.y][p2.x] == map[p1.y][p1.x]) ? (1) : (0);
	return (p1 != p2) ? (1) : (0);
}
*/
int 					Npuzzle::_getHeuristiqueLoop(Map const &map, Point const &p1, Point const &p2) {
	return this->_heuristiqueManathanLoop(map, p1, p2);
}

/************************/
/******MANAGE*NOEUD******/
/************************/

void 					Npuzzle::_updateOpened(void) {
	if (this->_openList.empty() && !this->_saveList.empty()) {
		int max = 1000;
		for (Liste::iterator it = ++this->_saveList.begin(); it != this->_saveList.end(); ++it) {
			if (max > 0) {
				this->_openList[it->first] = it->second;
				this->_saveList.erase(it->first);
			} else {
				break ;
			}
			max--;
		}
	}
}

void 					Npuzzle::_saveOpened(int moyenn) {
	int 	size = this->_openList.size();

	if (size > 1000) {
		moyenn = moyenn / size;
		for (Liste::iterator it = ++this->_openList.begin(); it != this->_openList.end(); ++it) {
			if (it->second.F > moyenn) {
				this->_saveList[it->first] = it->second;
				this->_openList.erase(it->first);
			}
		}
	}
}

void					Npuzzle::_bestMapOpened(Map &ret) {
	ret = this->_openList.begin()->first;
	int 				min = this->_openList.begin()->second.F;
	int 				moyenn = min;

	this->_updateOpened();

	for (Liste::iterator it = ++this->_openList.begin(); it != this->_openList.end(); ++it) {
		if (it->second.F < min) {
			ret = it->first;
			min = it->second.F;
		}
		moyenn += it->second.F;
	}

	this->_saveOpened(moyenn);
}

Noeud					Npuzzle::_createNoeud(int const move, Point const emptyCase, int const f) {
	
/*	Noeud 			nNoeud;

	nNoeud.G = noeud.G + 1;
	nNoeud.H = noeud.h + k;
	nNoeud.F = nNoeud.G + nNoeud.H;
	nNoeud.parent = map;
	
	nNoeud.emptyCase = nEmptyCase;
	return nNoeud;*/
	Noeud 			noeud;

	noeud.F = f + 1;
	noeud.parent = move;
	noeud.emptyCase = emptyCase;
	return noeud;
}

void 					Npuzzle::_addInOpenList(const Map &map, Noeud newNoeud, Noeud const &oldNoeud) {
	if ((newNoeud.G = oldNoeud.G + 1) > 100)
		return ;
	if (this->_openList.find(map) == this->_openList.end() && \
		this->_saveList.find(map) == this->_saveList.end() && \
		this->_closedList.find(map) == this->_closedList.end()) {
		this->_openList[map] = newNoeud;
	} else {
		if (newNoeud.F < oldNoeud.F) {
			this->_openList[map] = newNoeud;
		}
	}
}



void 					Npuzzle::_moveLeft(Noeud const &noeud, Map const &map, Point const &p) {
	if (p.x > 0 && noeud.parent != RIGHT) {
		Point		n;
		n.y = p.y;
		n.x = p.x - 1;
		
		Map 		nMap = map;
		nMap[p.y][p.x] = map[n.y][n.x];
		nMap[n.y][n.x] = map[p.y][p.x];

		int 			f = this->_getHeuristiqueLoop(map, p, n) + noeud.F;
		this->_addInOpenList(nMap, this->_createNoeud(UP, n, f), noeud);
	}
}

void 					Npuzzle::_moveRight(Noeud const &noeud, Map const &map, Point const &p) {
	if (p.x < this->_n - 1 && noeud.parent != LEFT) {
		Point		n;
		n.y = p.y;
		n.x = p.x + 1;
		
		Map 		nMap = map;
		nMap[p.y][p.x] = map[n.y][n.x];
		nMap[n.y][n.x] = map[p.y][p.x];

		int 			f = this->_getHeuristiqueLoop(map, p, n) + noeud.F;
		this->_addInOpenList(nMap, this->_createNoeud(DOWN, n, f), noeud);
	}
}

void 					Npuzzle::_moveUp(Noeud const &noeud, Map const &map, Point const &p) {
	if (p.y > 0 && noeud.parent != DOWN) {
		Point		n;
		n.y = p.y - 1;
		n.x = p.x;
		
		Map 		nMap = map;
		nMap[p.y][p.x] = map[n.y][n.x];
		nMap[n.y][n.x] = map[p.y][p.x];

		int 			f = this->_getHeuristiqueLoop(map, p, n) + noeud.F;
		this->_addInOpenList(nMap, this->_createNoeud(DOWN, n, f), noeud);
	}
}

void 					Npuzzle::_moveDown(Noeud const &noeud, Map const &map, Point const &p) {
	if (p.y < this->_n - 1 && noeud.parent != UP) {
		Point		n;
		n.y = p.y + 1;
		n.x = p.x;
		
		Map 		nMap = map;
		nMap[p.y][p.x] = map[n.y][n.x];
		nMap[n.y][n.x] = map[p.y][p.x];

		int 			f = this->_getHeuristiqueLoop(map, p, n) + noeud.F;
		this->_addInOpenList(nMap, this->_createNoeud(DOWN, n, f), noeud);
	}
}

void 					Npuzzle::_addAllOpenList(Noeud const &noeud, Map const &map) {
	Point				p = noeud.emptyCase;

	this->_moveLeft(noeud, map, p);
	this->_moveRight(noeud, map, p);
	this->_moveUp(noeud, map, p);
	this->_moveDown(noeud, map, p);

}

/************************/
/******SOLVE*PUZZLE******/
/************************/

void 					Npuzzle::run(void) {
	int 				nombre_tentative = 0;
	Noeud 				noeud;
	Map 				map;

	//std::cout << "=======================" << std::endl;
	clock_t t = clock();
	while (!this->_openList.empty() && !this->_openList.empty()) {
		this->_bestMapOpened(map);
		noeud = this->_openList[map];
		this->_closedList[map] = noeud;
		this->_openList.erase(map);
		if (map == this->_mapFinish) {
			break ;
		}
		++nombre_tentative;
		if (nombre_tentative % 10000 == 0) {
			std::cout << nombre_tentative << std::endl;
		}
		this->_addAllOpenList(noeud, map);
		
		//std::cout << "=======================" << std::endl;
		//sleep(1);
	}
	std::cout << clock() - t << std::endl;
	if (map != this->_mapFinish) {
		std::cout << "pas de solution" << std::endl;
		return;
	}
	std::cout << "solution trouvée en " << nombre_tentative << " tentatives" << std::endl;
}

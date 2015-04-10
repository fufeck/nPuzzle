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


Npuzzle::Npuzzle(int ac, char **av) : _n(0) {
	std::string	fileName(av[1]);
	Point		emptyCase = this->_createMap(fileName);
	std::cout << this->_n << std::endl;
	this->_displayMap(this->_mapStart);
	this->_createFinishMap();
	this->_setHeuristique(ac, av);
	this->_displayMap(this->_mapFinish);
	std::cout << "=====================" << std::endl;
	
	Noeud noeud;

	noeud.F = (this->*_heuristique)(this->_mapStart);
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

void					Npuzzle::_setHeuristique(int ac, char **av) {
	this->_heuristique = &Npuzzle::_heuristiqueManathan;
	if (ac == 3) {
		std::string 		h(av[2]);
		if (h == "SIMPLE") {
			this->_heuristique = &Npuzzle::_heuristiqueSimple;
		} else if (h == "LINEAR") {
			this->_heuristique = &Npuzzle::_heuristiqueLinear;
		} else if (h != "MANATHAN") {
			std::cerr << "ERROR : Heuristique " << h << " doesnt exist." << std::endl;
			throw std::exception();
		}
	}
}

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
		std::cerr << "ERROR : File " << filename << " can't be open." << std::endl;
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
}

/************************/
/******HEURISTIQUES******/
/************************/

int 					Npuzzle::_addLinearConflict(const Map &map, int xx, int yy) {
	int 				k = 0;

	for (int y = 0; y < this->_n; ++y) {
		for (int x = 0; x < this->_n; ++x) {
			if ((y == yy && x != xx) || (x == xx && y != yy)) {
				Point 		p = this->_pointFinish[map[y][x]];
				if (p.x == xx && p.y == yy) {
					k++;
				}
			}
		}
	}
	return (k);
}

int 					Npuzzle::_heuristiqueSimple(const Map &map) {
	int 				k = 0;

	for (int y = 0; y < this->_n; ++y) {
		for (int x = 0; x < this->_n; ++x) {
			if (map[y][x] != this->_mapFinish[y][x])
				k += 2;
		}
	}
	
	return k;
}

int 					Npuzzle::_heuristiqueManathan(const Map &map) {
	int 				k = 0;

	for (int y = 0; y < this->_n; ++y) {
		for (int x = 0; x < this->_n; ++x) {
			Point 		p = this->_pointFinish[map[y][x]];
			k += ((y - p.y >= 0) ? (y - p.y) : (-(y - p.y))) + ((x - p.x >= 0) ? (x - p.x) : (-(x - p.x)));
			k += this->_addLinearConflict(map, x, y);
		}
	}
	return k;
}

int 					Npuzzle::_heuristiqueLinear(const Map &map) {
	int 				k = 0;

	for (int y = 0; y < this->_n; ++y) {
		for (int x = 0; x < this->_n; ++x) {
			Point 		p = this->_pointFinish[map[y][x]];
			k += ((y - p.y >= 0) ? (y - p.y) : (-(y - p.y))) + ((x - p.x >= 0) ? (x - p.x) : (-(x - p.x)));
			k += this->_addLinearConflict(map, x, y);
		}
	}
	return k;
}

/************************/
/******MANAGE*NOEUD******/
/************************/

void 					Npuzzle::_updateOpened(void) {
	if ((this->_openList.empty() || this->_openList.size() < 2) && !this->_saveList.empty()) {
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

	if (size > 1500) {
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

Noeud					Npuzzle::_createNoeud(int const move, Point const emptyCase, int const f, Noeud const & noeud) {
	Noeud 			nNoeud;

	nNoeud.G = noeud.G + 1;
	nNoeud.F = f + noeud.F + 1;
	nNoeud.parent = move;
	nNoeud.emptyCase = emptyCase;
	return nNoeud;
}

void 					Npuzzle::_addInOpenList(const Map &map, Noeud newNoeud, Noeud const &oldNoeud) {
	
	if (newNoeud.G > 10000)
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

		int 			f = (this->*_heuristique)(map);
		this->_addInOpenList(nMap, this->_createNoeud(LEFT, n, f, noeud), noeud);
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

		int 			f = (this->*_heuristique)(map);
		this->_addInOpenList(nMap, this->_createNoeud(RIGHT, n, f, noeud), noeud);
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

		int 			f = (this->*_heuristique)(map);
		this->_addInOpenList(nMap, this->_createNoeud(UP, n, f, noeud), noeud);
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

		int 			f = (this->*_heuristique)(map);
		this->_addInOpenList(nMap, this->_createNoeud(DOWN, n, f, noeud), noeud);
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
		
	}
	std::cout << "En " << noeud.G << " coups." << std::endl;
	std::cout << static_cast<double>(clock() - t) / 1000000.0 << " secondes" << std::endl;
	if (map != this->_mapFinish) {
		std::cout << "pas de solution" << std::endl;
		return;
	}
	std::cout << "solution trouvée en " << nombre_tentative << " tentatives" << std::endl;
}

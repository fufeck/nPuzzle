// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Npuzzle.hpp                                        :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ftaffore <ftaffore@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/04/01 15:36:25 by ftaffore          #+#    #+#             //
//   Updated: 2015/04/01 15:36:25 by ftaffore         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef NPUZZLE_HPP
# define NPUZZLE_HPP

#include <thread>
#include <mutex>
#include <iostream>
#include <exception>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>

enum eMove { NO, LEFT, RIGHT, UP, DOWN };
enum eHeuristique { SIMPLE, MANATHAN, LINEAR };

# define EMPTY_CASE	0
# define NUM_THREADS 4

typedef std::vector< std::vector<int> > Map;

struct 		Point {
	int		x;
	int		y;
};

struct 		Noeud {
    //int 	F;
    int 	G;
    int 	F;
    int 	parent;
    //Map 	parent;
    Point 	emptyCase;
};

typedef std::map<int, Point> MapInv;

typedef std::map<Map, Noeud> Liste;


class Npuzzle {

public:
	typedef				int (Npuzzle::*hMap)(const Map &map);

	int 				_heuristiqueSimple(const Map &map);
	int 				_heuristiqueManathan(const Map &map);
	int 				_heuristiqueLinear(const Map &map);

protected:

	//std::mutex 			_mutex;
	int					_n;
	hMap				_heuristique;

	Map 				_mapStart;
	Map					_mapFinish;

	MapInv				_pointFinish;

	Liste				_openList;
	Liste				_saveList;
	Liste				_closedList;


	void				_setHeuristique(int ac, char **av);
	void				_displayMap(Map const &map) const;
	Point				_createMap(std::string const &filename);
	void				_createFinishMap(void);

	int 				_addLinearConflict(const Map &map, int xx, int yy);
	void 				_updateOpened(void);
	void 				_saveOpened(int moyenn);
	void				_bestMapOpened(Map &ret);

	Noeud				_createNoeud(int const move, Point const emptyCase, int const f, Noeud const & noeud);
	void 				_addInOpenList(const Map &map, Noeud nouveauNoeud, const Noeud &noeudRemplace);

	void 				_moveLeft(Noeud const &noeud, Map const &map, Point const &p);
	void 				_moveRight(Noeud const &noeud, Map const &map, Point const &p);
	void 				_moveUp(Noeud const &noeud, Map const &map, Point const &p);
	void 				_moveDown(Noeud const &noeud, Map const &map, Point const &p);

	void 				_addAllOpenList(Noeud const &noeud, Map const &map);

	Npuzzle();
	Npuzzle(Npuzzle const &n);
	Npuzzle const &		operator=(Npuzzle const &);

public:
	Npuzzle(int ac, char **av);
	~Npuzzle();

	void				run(void);

};

#endif
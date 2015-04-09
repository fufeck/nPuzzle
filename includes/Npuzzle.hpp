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
protected:

	int					_n;

	Map 				_mapStart;
	Map					_mapFinish;

	MapInv				_pointFinish;

	Liste				_openList;
	Liste				_saveList;
	Liste				_closedList;


	void				_displayMap(Map const &map) const;
	Point				_createMap(std::string const &filename);
	void				_createFinishMap(void);


	int 				_heuristiqueManathanMap(const Map &map);
	int 				_heuristiqueSimpleMap(const Map &map);
	int 				_getHeuristiqueMap(const Map &map);
	int 				_heuristiqueManathanLoop(Map const &map, Point const &p1, Point const &p2);
	int 				_heuristiqueSimpleLoop(Map const &map, Point const &p1, Point const &p2);
	int 				_getHeuristiqueLoop(Map const &map, Point const &p1, Point const &p2);


	void 				_updateOpened(void);
	void 				_saveOpened(int moyenn);
	void				_bestMapOpened(Map &ret);

	Noeud				_createNoeud(int const move, Point const emptyCase, int const f);
	void 				_addInOpenList(const Map &map, Noeud nouveauNoeud, const Noeud &noeudRemplace);

	void 				_moveLeft(Noeud const &noeud, Map const &map, Point const &p);
	void 				_moveRight(Noeud const &noeud, Map const &map, Point const &p);
	void 				_moveUp(Noeud const &noeud, Map const &map, Point const &p);
	void 				_moveDown(Noeud const &noeud, Map const &map, Point const &p);

	void 				_addAllOpenList(Noeud const &noeud, Map const &map);

public:
	Npuzzle(std::string const &fileName);
	~Npuzzle();

	void				run(void);

};

#endif
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
#include <iostream>
#include <exception>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>

# define EMPTY_CASE	0

typedef std::vector< std::vector<int> > Map;

struct 		Point {
	int		x;
	int		y;
};

struct 		Noeud {
    int 	F;
    int 	G;
    int 	H;
    Map 	parent;
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
	Liste				_closedList;


	void				_displayMap(Map const &map) const;
	Point				_createMap(std::string const &filename);
	void				_createFinishMap(void);

	int 				_heuristiqueManathan(const Map &map);
	int 				_heuristiqueSimple(const Map &map);
	int 				_getHeuristique(const Map &map);

	void				_bestMapOpened(Map &ret);
	Noeud				_createNoeud(Noeud const &noeud, Map const &nMap, Map const &map, Point const &nEmptyCase);
	void 				_addInOpenList(const Map &map, const Noeud &nouveauNoeud, const Noeud &noeudRemplace);

	void 				_addAllOpenList(Noeud const &noeud, Map const &map);

public:
	Npuzzle(std::string const &fileName);
	~Npuzzle();

	void				run(void);

};

#endif
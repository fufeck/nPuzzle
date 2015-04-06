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

struct Point {

	int	x;
	int	y;
};

struct Noeud {
    int F;
    int G;
    int H;
    Map parent;
};
 
typedef std::map<Map, Noeud> Liste;

class Npuzzle
{
protected:
	int				_n;

	Map 				_mapStart;
	Map					_mapFinish;

	Liste				_openList;
	Liste				_closedList;

	void				_createMap(std::string const &filename);

	int 				_getDistanceManhattan(const Point &p1, const Point &p2);
	Point 				_getPositionFinale(int valeur);
	int 				_getHeuristiqueManathan(const Map &map);

	void				_bestMapOpened(Map &ret);
	Point				_getEmptyCase(Map const &map);
	Noeud				_createNoeud(Noeud &noeud, Map &nMap, Map &map);
	void 				_addInOpenList(const Map &map, const Noeud &nouveauNoeud, const Noeud &noeudRemplace);

	void 				_addAllOpenList(Noeud &noeud, Map &map, Point &p);

public:
	Npuzzle(std::string const &fileName);
	~Npuzzle();

	void				solveNpuzzle(void);

};

#endif
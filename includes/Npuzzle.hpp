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

struct 		Point {
	int		x;
	int		y;
};

struct 		Noeud {
    int 	F;
    int 	G;
    int 	H;
    Map 	parent;
    //Point 	emptyCase;
};

typedef std::map<int, Point> MapInv;

typedef std::map<Map, Noeud> Liste;

class Npuzzle
{
protected:
	int					_n;

	Map 				_mapStart;
	Map					_mapFinish;

	MapInv				_pointFinish;

	Liste				_openList;
	Liste				_closedList;

	bool				_checkSolvable(void);
	void				_displayMap(Map const &map) const;

	void				_createMap(std::string const &filename);
	void				_createFinishMap(void);

	int 				_getDistanceManathan(const Map &map);

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
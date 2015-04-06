// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   main.cpp                                           :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ftaffore <ftaffore@student.42.fr>          +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2015/04/01 15:34:56 by ftaffore          #+#    #+#             //
//   Updated: 2015/04/01 15:34:57 by ftaffore         ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include <string>
#include "Npuzzle.hpp"

int 			main(int ac, char **av) {

	if (ac == 2) {
		std::string	fileName(av[1]);
		
		try {
			Npuzzle	puzzle(fileName);
			puzzle.solveNpuzzle();
		} catch (std::exception & e) {
			return 1;
		}
	} else {
		std::cerr << "Usage : ./puzzle + fileMap" << std::endl;
	}
	return 0;
}
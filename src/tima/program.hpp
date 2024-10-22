/* tima
 * Copyright (C) 2024 Killian Valverde.
 *
 * This file is part of tima.
 *
 * tima is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * tima is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with tima. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file        tima/program.hpp
 * @brief       program class header.
 * @author      Killian Valverde
 * @date        2024/07/17
 */
 
#ifndef TIMA_PROGRAM_HPP
#define TIMA_PROGRAM_HPP

#include <speed/speed.hpp>

#include "exception.hpp"
#include "program_args.hpp"


/**
 * @brief       Contians all tima resources.
 */
namespace tima {


class program
{
public:
    /**
     * @brief       Constructor with parameters.
     * @param       prog_args : The program arguments.
     */
    explicit program(program_args&& prog_args);
    
    /**
     * @brief       Execute the program.
     * @return      The value that represents if the program succeed.
     */
    int execute();

private:

private:
    /** The program arguments. */
    program_args prog_args_;
};


}


#endif

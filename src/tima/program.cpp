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
 * @file        tima/program.cpp
 * @brief       program class implementation.
 * @author      Killian Valverde
 * @date        2024/07/17
 */

#include "program.hpp"


namespace tima {


program::program(program_args&& prog_args) 
        : prog_args_(std::move(prog_args))
{
}


int program::execute()
{
    std::cout << "hello, world" << std::endl;
    
    return 0;
}


}

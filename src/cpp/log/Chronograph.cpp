//
//  Copyright (c) 2014-2017 Philipp Paulweber
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                Emmanuel Pescosta
//                https://github.com/ppaulweber/libstdhl
//
//  This file is part of libstdhl.
//
//  libstdhl is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  libstdhl is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with libstdhl. If not, see <http://www.gnu.org/licenses/>.
//

#include "Chronograph.h"

using namespace libstdhl;
using namespace Log;

//
// Chronograph
//

Chronograph::Chronograph( u1 autostart )
: Item( Item::ID::CHRONOGRAPH )
, m_running( autostart )
, m_start()
, m_stop()
{
    m_start = std::chrono::high_resolution_clock::now();
    m_stop = m_start;

    if( autostart )
    {
        start();
    }
}

u1 Chronograph::running( void ) const
{
    return m_running;
}

void Chronograph::start( void )
{
    if( not m_running )
    {
        m_running = true;
        m_start = std::chrono::high_resolution_clock::now();
    }
}

void Chronograph::stop( void )
{
    if( m_running )
    {
        m_stop = std::chrono::high_resolution_clock::now();
        m_running = false;
    }
}

void Chronograph::cont( void )
{
    if( not m_running )
    {
        m_running = true;
    }
}

std::string Chronograph::accept( Formatter& formatter )
{
    return formatter.visit( *this );
}

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

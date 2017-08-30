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
//  Additional permission under GNU GPL version 3 section 7
//
//  libstdhl is distributed under the terms of the GNU General Public License
//  with the following clarification and special exception: Linking libstdhl
//  statically or dynamically with other modules is making a combined work
//  based on libstdhl. Thus, the terms and conditions of the GNU General
//  Public License cover the whole combination. As a special exception,
//  the copyright holders of libstdhl give you permission to link libstdhl
//  with independent modules to produce an executable, regardless of the
//  license terms of these independent modules, and to copy and distribute
//  the resulting executable under terms of your choice, provided that you
//  also meet, for each linked independent module, the terms and conditions
//  of the license of that module. An independent module is a module which
//  is not derived from or based on libstdhl. If you modify libstdhl, you
//  may extend this exception to your version of the library, but you are
//  not obliged to do so. If you do not wish to do so, delete this exception
//  statement from your version.
//

#include "Packet.h"

#include "../../String.h"

using namespace libstdhl;
using namespace Network;
using namespace LSP;

LSP::Packet::Packet( const std::string& data )
: m_header{ 0 }
, m_payload{}
, m_data( data )
{
    std::vector< std::string > parts;
    String::split( data, "\r\n", parts );

    u64 length = 0;

    for( auto p : parts )
    {
        // check for content length field
        if( strncmp( p.c_str(), CL.c_str(), CL.size() ) == 0 )
        {
            length = std::stoull( p.substr( CL.size() + 1 ) );
            m_header = Protocol( length );
        }
        // check for content type field
        else if( strncmp( p.c_str(), CT.c_str(), CT.size() ) == 0 )
        {
            const auto type = String::trim( p.substr( CL.size() + 1 ) );
            if( type.compare( TYPE ) != 0 )
            {
                throw std::domain_error(
                    "LSP: invalid content type '" + type + "'" );
            }
        }
        // check for json rpc content data
        else if( strncmp( p.c_str(), "{", 1 ) == 0 )
        {
            m_payload = Message( Json::Object::parse( p ) );
        }
        else
        {
            throw std::domain_error(
                "LSP: invalid packet content '" + p + "'" );
        }
    }
}

LSP::Packet::Packet( const Message& payload )
: m_header( payload.dump().size() )
, m_payload( payload )
, m_data( m_header.data() + NL + payload.dump() )
{
}

const LSP::Protocol& LSP::Packet::header( void ) const
{
    return m_header;
}

const Message& LSP::Packet::payload( void ) const
{
    return m_payload;
}

const u8* LSP::Packet::buffer( void ) const
{
    return m_data.buffer();
}

std::size_t LSP::Packet::size( void ) const
{
    return m_data.size();
}

std::string LSP::Packet::dump( const u1 formatted ) const
{
    if( not formatted )
    {
        return m_data.data();
    }
    else
    {
        return m_header.data() + NL + payload().dump( 2 );
    }
}

void LSP::Packet::process( ServerInterface& interface ) const
{
    payload().process( interface );
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

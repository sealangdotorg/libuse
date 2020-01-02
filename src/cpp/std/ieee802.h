//
//  Copyright (C) 2014-2020 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                Emmanuel Pescosta
//                Christian Lascsak
//                <https://github.com/casm-lang/libstdhl>
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

#pragma once
#ifndef _LIBSTDHL_CPP_STANDARD_IEEE802_H_
#define _LIBSTDHL_CPP_STANDARD_IEEE802_H_

#include <libstdhl/Type>

#include <array>

/**
   @brief    TBD

   TBD
*/

namespace libstdhl
{
    namespace Standard
    {
        /**
           @extends Standard
        */
        namespace IEEE802
        {
            static constexpr std::size_t ADDR = 6;
            static constexpr std::size_t TYPE = 2;

            using Address = std::array< u8, ADDR >;
            using Type = std::array< u8, TYPE >;

            //
            // https://www.iana.org/assignments/ieee-802-numbers/ieee-802-numbers.xhtml

            // 0000-05DC       IEEE802.3 Length Field
            namespace _3
            {
                static constexpr Type NIL = { { 0x00, 0x00 } };
                static constexpr Type MIN = { { 0x00, 0x30 } };  // runt frame
                static constexpr Type MAX = { { 0x05, 0xDC } };
            }

            // 0101-01FF       Experimental

            // 0200            XEROX PUP
            // 0201            PUP Addr Trans
            // 0400            Nixdorf
            // 0600            XEROX NS IDP
            // 0660            DLOG
            // 0661            DLOG
            // 0800            Internet Protocol version 4 (IPv4)
            static constexpr Type IPv4 = { { 0x08, 0x00 } };

            // 0801            X.75 Internet
            // 0802            NBS Internet
            // 0803            ECMA Internet
            // 0804            Chaosnet
            // 0805            X.25 Level 3

            // 0806            Address Resolution Protocol (ARP)
            static constexpr Type ARP = { { 0x08, 0x06 } };

            // 0807            XNS Compatability

            // 0808            Frame Relay ARP
            static constexpr Type FrameRelayARP = { { 0x08, 0x08 } };

            // 081C            Symbolics Private
            // 0888-088A       Xyplex
            // 0900            Ungermann-Bass net debugr
            // 0A00            Xerox IEEE802.3 PUP
            // 0A01            PUP Addr Trans
            // 0BAD            Banyan VINES
            // 0BAE            VINES Loopback
            // 0BAF            VINES Echo
            // 1000            Berkeley Trailer nego
            // 1001-100F       Berkeley Trailer encap/IP
            // 1600            Valid Systems
            // 22F3            TRILL
            // 22F4            L2-IS-IS
            // 4242            PCS Basic Block Protocol
            // 5208            BBN Simnet
            // 6000            DEC Unassigned (Exp.)
            // 6001            DEC MOP Dump/Load
            // 6002            DEC MOP Remote Console
            // 6003            DEC DECNET Phase IV Route
            // 6004            DEC LAT
            // 6005            DEC Diagnostic Protocol
            // 6006            DEC Customer Protocol
            // 6007            DEC LAVC, SCA
            // 6008-6009       DEC Unassigned
            // 6010-6014       3Com Corporation
            // 6558            Trans Ether Bridging
            // 6559            Raw Frame Relay
            // 7000            Ungermann-Bass download
            // 7002            Ungermann-Bass dia/loop
            // 7020-7029       LRT
            // 7030            Proteon
            // 7034            Cabletron
            // 8003            Cronus VLN
            // 8004            Cronus Direct
            // 8005            HP Probe
            // 8006            Nestar
            // 8008            AT&T
            // 8010            Excelan
            // 8013            SGI diagnostics
            // 8014            SGI network games
            // 8015            SGI reserved
            // 8016            SGI bounce server
            // 8019            Apollo Domain
            // 802E            Tymshare
            // 802F            Tigan, Inc.

            // 8035            Reverse Address Resolution Protocol (RARP)
            static constexpr Type RARP = { { 0x80, 0x35 } };

            // 8036            Aeonic Systems
            // 8038            DEC LANBridge
            // 8039-803C       DEC Unassigned
            // 803D            DEC Ethernet Encryption
            // 803E            DEC Unassigned
            // 803F            DEC LAN Traffic Monitor
            // 8040-8042       DEC Unassigned
            // 8044            Planning Research Corp.
            // 8046            AT&T
            // 8047            AT&T
            // 8049            ExperData
            // 805B            Stanford V Kernel exp.
            // 805C            Stanford V Kernel prod.
            // 805D            Evans & Sutherland
            // 8060            Little Machines
            // 8062            Counterpoint Computers
            // 8065            Univ. of Mass. @ Amherst
            // 8066            Univ. of Mass. @ Amherst
            // 8067            Veeco Integrated Auto.
            // 8068            General Dynamics
            // 8069            AT&T
            // 806A            Autophon
            // 806C            ComDesign
            // 806D            Computgraphic Corp.
            // 806E-8077       Landmark Graphics Corp.
            // 807A            Matra
            // 807B            Dansk Data Elektronik
            // 807C            Merit Internodal
            // 807D-807F       Vitalink Communications
            // 8080            Vitalink TransLAN III
            // 8081-8083       Counterpoint Computers
            // 809B            Appletalk
            // 809C-809E       Datability
            // 809F            Spider Systems Ltd.
            // 80A3            Nixdorf Computers
            // 80A4-80B3       Siemens Gammasonics Inc.
            // 80C0-80C3       DCA Data Exchange Cluster
            // 80C4            Banyan Systems
            // 80C5            Banyan Systems
            // 80C6            Pacer Software
            // 80C7            Applitek Corporation
            // 80C8-80CC       Intergraph Corporation
            // 80CD-80CE       Harris Corporation
            // 80CF-80D2       Taylor Instrument
            // 80D3-80D4       Rosemount Corporation
            // 80D5            IBM SNA Service on Ether
            // 80DD            Varian Associates
            // 80DE-80DF       Integrated Solutions TRFS
            // 80E0-80E3       Allen-Bradley
            // 80E4-80F0       Datability
            // 80F2            Retix
            // 80F3            AppleTalk AARP (Kinetics)
            // 80F4-80F5       Kinetics
            // 80F7            Apollo Computer
            // 80FF            Wellfleet Communications Customer VLAN Tag Type
            // 8100            C-Tag (aka. Q-Tag, Wellfleet)
            // 8101-8103       Wellfleet Communications
            // 8107-8109       Symbolics Private
            // 8130            Hayes Microcomputers
            // 8131            VG Laboratory Systems
            // 8132-8136       Bridge Communications
            // 8137-8138       Novell, Inc.
            // 8139-813D       KTI
            // 8148            Logicraft
            // 8149            Network Computing Devices
            // 814A            Alpha Micro
            // 814C            SNMP
            // 814D            BIIN
            // 814E            BIIN
            // 814F            Technically Elite Concept
            // 8150            Rational Corp
            // 8151-8153       Qualcomm
            // 815C-815E       Computer Protocol Pty Ltd
            // 8164-8166       Charles River Data System
            // 817D            XTP
            // 817E            SGI/Time Warner prop.
            // 8180            HIPPI-FP encapsulation
            // 8181            STP, HIPPI-ST
            // 8182            Reserved for HIPPI-6400
            // 8183            Reserved for HIPPI-6400
            // 8184-818C       Silicon Graphics prop.
            // 818D            Motorola Computer
            // 819A-81A3       Qualcomm
            // 81A4            ARAI Bunkichi
            // 81A5-81AE       RAD Network Devices
            // 81B7-81B9       Xyplex
            // 81CC-81D5       Apricot Computers
            // 81D6-81DD       Artisoft
            // 81E6-81EF       Polygon
            // 81F0-81F2       Comsat Labs
            // 81F3-81F5       SAIC
            // 81F6-81F8       VG Analytical
            // 8203-8205       Quantum Software
            // 8221-8222       Ascom Banking Systems
            // 823E-8240       Advanced Encryption Syste
            // 827F-8282       Athena Programming
            // 8263-826A       Charles River Data System
            // 829A-829B       Inst Ind Info Tech
            // 829C-82AB       Taurus Controls
            // 82AC-8693       Walker Richer & Quinn
            // 8694-869D       Idea Courier
            // 869E-86A1       Computer Network Tech
            // 86A3-86AC       Gateway Communications
            // 86DB            SECTRA
            // 86DE            Delta Controls

            // 86DD            Internet Protocol version 6 (IPv6)
            static constexpr Type IPv6 = { { 0x86, 0xdd } };

            // 86DF            ATOMIC
            // 86E0-86EF       Landis & Gyr Powers
            // 8700-8710       Motorola
            // 876B            TCP/IP Compression
            // 876C            IP Autonomous Systems
            // 876D            Secure Data 802.3 - Ethernet
            // 8808            Passive Optical Network (EPON)
            // 880B            Point-to-Point Protocol (PPP)
            // 880C            General Switch Management Protocol (GSMP)
            // 8847            MPLS
            // 8848            MPLS with upstream-assigned label
            // 8861            Multicast Channel Allocation Protocol (MCAP)
            // 8863            PPP over Ethernet (PPPoE) Discovery Stage
            // 8864            PPP over Ethernet (PPPoE) Session Stage
            // 888E            802.1X - Port-based network access control
            // 88A8            802.1Q - Service VLAN tag identifier (S-Tag)
            // 8A96-8A97       Invisible Software
            // 88B5            802 - Local Experimental Ethertype
            // 88B6            802 - Local Experimental Ethertype
            // 88B7            802 - OUI Extended Ethertype
            // 88C7            802.11 - Pre-Authentication (802.11i)
            // 88E5            802.1AE - Media Access Control Security
            // 88E7            Provider Backbone Bridging Instance tag
            // 890D            Roaming Remote Request (802.11r) 802.21
            // 8917            Independent Handover Protocol
            // 8929            802.1Qbe - Multiple I-SID Registration
            // 893B            TRILL Fine Grained Labeling (FGL) (802.1Qbg)
            // 8940            Protocol (also used in 802.1BR)
            // 8946            TRILL RBridge Channel
            // 8947            GeoNetworking as defined in ETSI EN 302 636-4-1
            // 9000            Loopback
            // 9001            3Com(Bridge) XNS Sys Mgmt
            // 9002            3Com(Bridge) TCP-IP Sys
            // 9003            3Com(Bridge) loop detect
            // A0ED            LoWPAN encapsulation
            // B7EA            GRE packets
            // FF00            BBN VITAL-LanBridge cache
            // FF00-FF0F       ISC Bunker Ramo
            // FFFF            Reserved
        }
    }
}

#endif  // _LIBSTDHL_CPP_STANDARD_IEEE802_H_

//
//  Local variables:
//  mode: c++
//  indent-tabs-mode: nil
//  c-basic-offset: 4
//  tab-width: 4
//  End:
//  vim:noexpandtab:sw=4:ts=4:
//

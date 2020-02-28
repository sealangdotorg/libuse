#
#   Copyright (C) 2018-2020 CASM Organization <https://casm-lang.org>
#   All rights reserved.
#
#   Developed by: Philipp Paulweber
#                 <https://github.com/casm-lang/casm-lang.pkg.docker>
#
#   This file is part of casm-lang.pkg.docker.
#
#   casm-lang.pkg.docker is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   casm-lang.pkg.docker is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with casm-lang.pkg.docker. If not, see <http://www.gnu.org/licenses/>.
#

FROM alpine

COPY .attic/. /opt

RUN tar -vxf /opt/*-linux-x86_64.tar.gz -C /tmp \
&&  cp  -vrf /tmp/*/* /usr/local/ \
&&  rm  -vrf /tmp/*

CMD ["/bin/sh"]

LABEL maintainer ppaulweber

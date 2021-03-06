#==================================================================================================
#
#  Makefile for the dmatdmatkron module of the Blaze test suite
#
#  Copyright (C) 2012-2019 Klaus Iglberger - All Rights Reserved - All Rights Reserved
#
#  This file is part of the Blaze library. You can redistribute it and/or modify it under
#  the terms of the New (Revised) BSD License. Redistribution and use in source and binary
#  forms, with or without modification, are permitted provided that the following conditions
#  are met:
#
#  1. Redistributions of source code must retain the above copyright notice, this list of
#     conditions and the following disclaimer.
#  2. Redistributions in binary form must reproduce the above copyright notice, this list
#     of conditions and the following disclaimer in the documentation and/or other materials
#     provided with the distribution.
#  3. Neither the names of the Blaze development group nor the names of its contributors
#     may be used to endorse or promote products derived from this software without specific
#     prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
#  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
#  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
#  SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
#  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
#  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
#  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
#  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
#  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
#  DAMAGE.
#
#==================================================================================================


# Including the compiler and library settings
ifneq ($(MAKECMDGOALS),reset)
ifneq ($(MAKECMDGOALS),clean)
-include ../../Makeconfig
endif
endif


# Setting the source, object and dependency files
SRC = $(wildcard ./*.cpp)
DEP = $(SRC:.cpp=.d)
OBJ = $(SRC:.cpp=.o)
BIN = $(SRC:.cpp=)


# General rules
default: M3x3aM3x3a M3x3aM3x3b M3x3bM3x3a M3x3bM3x3b \
         MHaMHa MHaMHb MHbMHa MHbMHb \
         MDaMDa MDaMDb MDbMDa MDbMDb \
         MUaMUa MUaMUb MUbMUa MUbMUb \
         SDaSDa SDaSDb SDbSDa SDbSDb \
         HDaHDa HDaHDb HDbHDa HDbHDb \
         LDaLDa LDaLDb LDbLDa LDbLDb \
         UDaUDa UDaUDb UDbUDa UDbUDb \
         DDaDDa DDaDDb DDbDDa DDbDDb
all: $(BIN)
essential: M3x3aM3x3a MHaMHa MDaMDa MUaMUa SDaSDa HDaHDa LDaLDa UDaUDa DDaDDa
single: MDaMDa


# Build rules
D3x3aD3x3a: D3x3aD3x3a.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
D3x3aD3x3b: D3x3aD3x3b.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
D3x3bD3x3a: D3x3bD3x3a.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
D3x3bD3x3b: D3x3bD3x3b.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)

DDaDDa: DDaDDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
DDaDDb: DDaDDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
DDaHDa: DDaHDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
DDaHDb: DDaHDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
DDaLDa: DDaLDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
DDaLDb: DDaLDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
DDaMDa: DDaMDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
DDaMDb: DDaMDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
DDaSDa: DDaSDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
DDaSDb: DDaSDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
DDaUDa: DDaUDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
DDaUDb: DDaUDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
DDbDDa: DDbDDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
DDbDDb: DDbDDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
DDbHDa: DDbHDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
DDbHDb: DDbHDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
DDbLDa: DDbLDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
DDbLDb: DDbLDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
DDbMDa: DDbMDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
DDbMDb: DDbMDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
DDbSDa: DDbSDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
DDbSDb: DDbSDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
DDbUDa: DDbUDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
DDbUDb: DDbUDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)

DHaDHa: DHaDHa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
DHaDHb: DHaDHb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
DHbDHa: DHbDHa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
DHbDHb: DHbDHb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)

H3x3aH3x3a: H3x3aH3x3a.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
H3x3aH3x3b: H3x3aH3x3b.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
H3x3bH3x3a: H3x3bH3x3a.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
H3x3bH3x3b: H3x3bH3x3b.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)

HDaDDa: HDaDDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
HDaDDb: HDaDDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
HDaHDa: HDaHDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
HDaHDb: HDaHDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
HDaLDa: HDaLDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
HDaLDb: HDaLDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
HDaMDa: HDaMDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
HDaMDb: HDaMDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
HDaSDa: HDaSDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
HDaSDb: HDaSDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
HDaUDa: HDaUDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
HDaUDb: HDaUDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
HDbDDa: HDbDDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
HDbDDb: HDbDDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
HDbHDa: HDbHDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
HDbHDb: HDbHDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
HDbLDa: HDbLDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
HDbLDb: HDbLDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
HDbMDa: HDbMDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
HDbMDb: HDbMDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
HDbSDa: HDbSDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
HDbSDb: HDbSDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
HDbUDa: HDbUDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
HDbUDb: HDbUDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)

HHaHHa: HHaHHa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
HHaHHb: HHaHHb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
HHbHHa: HHbHHa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
HHbHHb: HHbHHb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)

L3x3aL3x3a: L3x3aL3x3a.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
L3x3aL3x3b: L3x3aL3x3b.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
L3x3bL3x3a: L3x3bL3x3a.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
L3x3bL3x3b: L3x3bL3x3b.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)

LDaDDa: LDaDDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LDaDDb: LDaDDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LDaHDa: LDaHDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LDaHDb: LDaHDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LDaLDa: LDaLDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LDaLDb: LDaLDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LDaMDa: LDaMDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LDaMDb: LDaMDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LDaSDa: LDaSDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LDaSDb: LDaSDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LDaUDa: LDaUDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LDaUDb: LDaUDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LDbDDa: LDbDDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LDbDDb: LDbDDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LDbHDa: LDbHDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LDbHDb: LDbHDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LDbLDa: LDbLDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LDbLDb: LDbLDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LDbMDa: LDbMDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LDbMDb: LDbMDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LDbSDa: LDbSDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LDbSDb: LDbSDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LDbUDa: LDbUDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LDbUDb: LDbUDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)

LHaLHa: LHaLHa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LHaLHb: LHaLHb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LHbLHa: LHbLHa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
LHbLHb: LHbLHb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)

M3x3aM3x3a: M3x3aM3x3a.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
M3x3aM3x3b: M3x3aM3x3b.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
M3x3aMDa: M3x3aMDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
M3x3aMDb: M3x3aMDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
M3x3bM3x3a: M3x3bM3x3a.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
M3x3bM3x3b: M3x3bM3x3b.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
M3x3bMDa: M3x3bMDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
M3x3bMDb: M3x3bMDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)

MDaDDa: MDaDDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDaDDb: MDaDDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDaHDa: MDaHDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDaHDb: MDaHDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDaLDa: MDaLDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDaLDb: MDaLDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDaM3x3a: MDaM3x3a.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDaM3x3b: MDaM3x3b.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDaMDa: MDaMDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDaMDb: MDaMDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDaMHa: MDaMHa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDaMHb: MDaMHb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDaMUa: MDaMUa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDaMUb: MDaMUb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDaSDa: MDaSDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDaSDb: MDaSDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDaUDa: MDaUDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDaUDb: MDaUDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDbDDa: MDbDDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDbDDb: MDbDDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDbHDa: MDbHDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDbHDb: MDbHDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDbLDa: MDbLDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDbLDb: MDbLDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDbM3x3a: MDbM3x3a.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDbM3x3b: MDbM3x3b.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDbMDa: MDbMDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDbMDb: MDbMDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDbMHa: MDbMHa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDbMHb: MDbMHb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDbMUa: MDbMUa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDbMUb: MDbMUb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDbSDa: MDbSDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDbSDb: MDbSDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDbUDa: MDbUDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MDbUDb: MDbUDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)

MHaM3x3a: MHaM3x3a.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MHaM3x3b: MHaM3x3b.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MHaMDa: MHaMDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MHaMDb: MHaMDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MHaMHa: MHaMHa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MHaMHb: MHaMHb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MHaMUa: MHaMUa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MHaMUb: MHaMUb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MHbM3x3a: MHbM3x3a.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MHbM3x3b: MHbM3x3b.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MHbMDa: MHbMDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MHbMDb: MHbMDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MHbMHa: MHbMHa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MHbMHb: MHbMHb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MHbMUa: MHbMUa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MHbMUb: MHbMUb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)

MUaM3x3a: MUaM3x3a.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MUaM3x3b: MUaM3x3b.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MUaMDa: MUaMDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MUaMDb: MUaMDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MUaMHa: MUaMHa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MUaMHb: MUaMHb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MUaMUa: MUaMUa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MUaMUb: MUaMUb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MUbM3x3a: MUbM3x3a.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MUbM3x3b: MUbM3x3b.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MUbMDa: MUbMDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MUbMDb: MUbMDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MUbMHa: MUbMHa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MUbMHb: MUbMHb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MUbMUa: MUbMUa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
MUbMUb: MUbMUb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)

S3x3aS3x3a: S3x3aS3x3a.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
S3x3aS3x3b: S3x3aS3x3b.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
S3x3bS3x3a: S3x3bS3x3a.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
S3x3bS3x3b: S3x3bS3x3b.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)

SDaDDa: SDaDDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SDaDDb: SDaDDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SDaHDa: SDaHDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SDaHDb: SDaHDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SDaLDa: SDaLDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SDaLDb: SDaLDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SDaMDa: SDaMDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SDaMDb: SDaMDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SDaSDa: SDaSDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SDaSDb: SDaSDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SDaUDa: SDaUDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SDaUDb: SDaUDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SDbDDa: SDbDDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SDbDDb: SDbDDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SDbHDa: SDbHDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SDbHDb: SDbHDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SDbLDa: SDbLDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SDbLDb: SDbLDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SDbMDa: SDbMDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SDbMDb: SDbMDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SDbSDa: SDbSDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SDbSDb: SDbSDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SDbUDa: SDbUDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SDbUDb: SDbUDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)

SHaSHa: SHaSHa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SHaSHb: SHaSHb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SHbSHa: SHbSHa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
SHbSHb: SHbSHb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)

U3x3aU3x3a: U3x3aU3x3a.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
U3x3aU3x3b: U3x3aU3x3b.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
U3x3bU3x3a: U3x3bU3x3a.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
U3x3bU3x3b: U3x3bU3x3b.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)

UDaDDa: UDaDDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UDaDDb: UDaDDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UDaHDa: UDaHDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UDaHDb: UDaHDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UDaLDa: UDaLDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UDaLDb: UDaLDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UDaMDa: UDaMDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UDaMDb: UDaMDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UDaSDa: UDaSDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UDaSDb: UDaSDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UDaUDa: UDaUDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UDaUDb: UDaUDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UDbDDa: UDbDDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UDbDDb: UDbDDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UDbHDa: UDbHDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UDbHDb: UDbHDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UDbLDa: UDbLDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UDbLDb: UDbLDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UDbMDa: UDbMDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UDbMDb: UDbMDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UDbSDa: UDbSDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UDbSDb: UDbSDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UDbUDa: UDbUDa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UDbUDb: UDbUDb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)

UHaUHa: UHaUHa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UHaUHb: UHaUHb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UHbUHa: UHbUHa.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)
UHbUHb: UHbUHb.o
	@$(CXX) $(CXXFLAGS) -o $@ $< $(LIBRARIES)


# Cleanup
reset:
	@$(RM) $(OBJ) $(BIN)
clean:
	@$(RM) $(OBJ) $(BIN) $(DEP)


# Makefile includes
ifneq ($(MAKECMDGOALS),reset)
ifneq ($(MAKECMDGOALS),clean)
-include $(DEP)
endif
endif


# Makefile generation
%.d: %.cpp
	@$(CXX) -MM -MP -MT "$*.o $*.d" -MF $@ $(CXXFLAGS) $<


# Setting the independent commands
.PHONY: default all essential single reset clean

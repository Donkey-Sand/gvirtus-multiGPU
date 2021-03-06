/*
 * gVirtuS -- A GPGPU transparent virtualization component.
 *
 * Copyright (C) 2009-2010  The University of Napoli Parthenope at Naples.
 *
 * This file is part of gVirtuS.
 *
 * gVirtuS is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * gVirtuS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gVirtuS; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Written by: Giuseppe Coviello <giuseppe.coviello@uniparthenope.it>,
 *             Department of Applied Science
 */

/**
 * @file   CudaUtil.cpp
 * @author Giuseppe Coviello <giuseppe.coviello@uniparthenope.it>
 * @date   Sun Oct 11 17:16:48 2009
 *
 * @brief
 *
 *
 */

#include "OpenclUtil.h"

#include <cstdio>
#include <iostream>

//#include <cuda.h>

using namespace std;

OpenclUtil::OpenclUtil() {
}

OpenclUtil::OpenclUtil(const OpenclUtil& orig) {
}

OpenclUtil::~OpenclUtil() {
}

char * OpenclUtil::MarshalHostPointer(const void* ptr) {
    char *marshal = new char[OpenclUtil::MarshaledHostPointerSize];
    MarshalHostPointer(ptr, marshal);
    return marshal;
}

void OpenclUtil::MarshalHostPointer(const void * ptr, char * marshal) {
#ifdef _WIN32
    sprintf_s(marshal, 10, "%p", ptr);
#else
    sprintf(marshal, "%p", ptr);
#endif
}



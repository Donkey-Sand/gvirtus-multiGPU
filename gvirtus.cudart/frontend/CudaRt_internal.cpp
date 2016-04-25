
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

#include <cstdio>

#include "CudaRt.h"
/*
 Routines not found in the cuda's header files.
 KEEP THEM WITH CARE
 */


/*	Sandy test04
//修改时间：2016.04.08
//这两个API应该被发送到同一个GPU上进行CUDA 设备的注册与取消注册的任务，在单GPU上这不会有问题
//但是，在多GPU上运行是，只有第一个API会被发送到第一个GPU上，而第二个API会发送给第二个GPU，由于必须成对使用，因此会在第二个GPU上报错，
//为了解决这个问题，应该把这个API同时发送给两个后端的GPU
 * 参考另外的API
 */
extern "C" __host__ void** __cudaRegisterFatBinary(void *fatCubin) {
    /* Fake host pointer */
    __fatBinC_Wrapper_t *bin = (__fatBinC_Wrapper_t *)fatCubin;
    char* data =(char*) bin->data;

    Buffer * input_buffer = new Buffer();
    input_buffer->AddString(CudaUtil::MarshalHostPointer((void **) bin));	//MarshalHostPointer函数的返回值是参数指针的内容，是64位的地址值
    input_buffer = CudaUtil::MarshalFatCudaBinary(bin, input_buffer);

    CudaRtFrontend::Prepare();
    CudaRtFrontend::Execute_internal("cudaRegisterFatBinary", input_buffer);		//有第二个参数？？？？ 问题就处在这里？？？,所有重写了这个函数
    if (CudaRtFrontend::Success())
        return (void **) fatCubin;
    return NULL;
}

/*
 * Sandy 2016.04.13
 */
extern "C" __host__ void __cudaUnregisterFatBinary(void **fatCubinHandle)
{
	Buffer *input_buffer = new Buffer();
	input_buffer->AddString(CudaUtil::MarshalHostPointer(fatCubinHandle));	//MarshalHostPointer函数的返回值是参数指针的内容，是64位的地址值
	//input_buffer = CudaUtil::MarshalFatCudaBinary(bin, input_buffer);
    CudaRtFrontend::Prepare();
    //CudaRtFrontend::AddStringForArguments(CudaUtil::MarshalHostPointer(fatCubinHandle));	//MarshalHostPointer函数的返回值是参数指针的内容，是64位的地址值
    CudaRtFrontend::Execute_internal("cudaUnregisterFatBinary",input_buffer);	//这个函数被重写了 Sandy 2016.04.12
}

//Sandy 2016.04.15
extern "C" __host__ void __cudaRegisterFunction(void **fatCubinHandle, const char *hostFun,
        char *deviceFun, const char *deviceName, int thread_limit, uint3 *tid,
        uint3 *bid, dim3 *bDim, dim3 *gDim, int *wSize) {
    CudaRtFrontend::Prepare();
    CudaRtFrontend::AddStringForArguments(CudaUtil::MarshalHostPointer(fatCubinHandle));
    
    CudaRtFrontend::AddVariableForArguments((pointer_t)hostFun);
    CudaRtFrontend::AddStringForArguments(deviceFun);
    CudaRtFrontend::AddStringForArguments(deviceName);
    CudaRtFrontend::AddVariableForArguments(thread_limit);
    CudaRtFrontend::AddHostPointerForArguments(tid);
    CudaRtFrontend::AddHostPointerForArguments(bid);
    CudaRtFrontend::AddHostPointerForArguments(bDim);
    CudaRtFrontend::AddHostPointerForArguments(gDim);
    CudaRtFrontend::AddHostPointerForArguments(wSize);

    CudaRtFrontend::Execute("cudaRegisterFunction");

    deviceFun = CudaRtFrontend::GetOutputString();
    tid = CudaRtFrontend::GetOutputHostPointer<uint3 > ();
    bid = CudaRtFrontend::GetOutputHostPointer<uint3 > ();
    bDim = CudaRtFrontend::GetOutputHostPointer<dim3 > ();
    gDim = CudaRtFrontend::GetOutputHostPointer<dim3 > ();
    wSize = CudaRtFrontend::GetOutputHostPointer<int>();
}

extern "C" __host__ void __cudaRegisterVar(void **fatCubinHandle, char *hostVar,
        char *deviceAddress, const char *deviceName, int ext, int size,
        int constant, int global) {
    CudaRtFrontend::Prepare();
    CudaRtFrontend::AddStringForArguments(CudaUtil::MarshalHostPointer(fatCubinHandle));
    CudaRtFrontend::AddStringForArguments(CudaUtil::MarshalHostPointer(hostVar));
    CudaRtFrontend::AddStringForArguments(deviceAddress);
    CudaRtFrontend::AddStringForArguments(deviceName);
    CudaRtFrontend::AddVariableForArguments(ext);
    CudaRtFrontend::AddVariableForArguments(size);
    CudaRtFrontend::AddVariableForArguments(constant);
    CudaRtFrontend::AddVariableForArguments(global);
    CudaRtFrontend::Execute("cudaRegisterVar");
}

extern "C" __host__ void __cudaRegisterShared(void **fatCubinHandle, void **devicePtr) {
    CudaRtFrontend::Prepare();
    CudaRtFrontend::AddStringForArguments(CudaUtil::MarshalHostPointer(fatCubinHandle));
    CudaRtFrontend::AddStringForArguments((char *) devicePtr);
    CudaRtFrontend::Execute("cudaRegisterShared");
}

extern "C" __host__ void __cudaRegisterSharedVar(void **fatCubinHandle, void **devicePtr,
        size_t size, size_t alignment, int storage) {
    CudaRtFrontend::Prepare();
    CudaRtFrontend::AddStringForArguments(CudaUtil::MarshalHostPointer(fatCubinHandle));
    CudaRtFrontend::AddStringForArguments((char *) devicePtr);
    CudaRtFrontend::AddVariableForArguments(size);
    CudaRtFrontend::AddVariableForArguments(alignment);
    CudaRtFrontend::AddVariableForArguments(storage);
    CudaRtFrontend::Execute("cudaRegisterSharedVar");
}

extern "C" __host__ void __cudaRegisterTexture(void **fatCubinHandle,
        const textureReference *hostVar, void **deviceAddress, char *deviceName,
        int dim, int norm, int ext) {
    CudaRtFrontend::Prepare();
    CudaRtFrontend::AddStringForArguments(CudaUtil::MarshalHostPointer(fatCubinHandle));
    CudaRtFrontend::AddStringForArguments(CudaUtil::MarshalHostPointer(hostVar));
    // Achtung: passing the address and the content of the textureReference
    CudaRtFrontend::AddHostPointerForArguments(hostVar);
    CudaRtFrontend::AddStringForArguments((char *) deviceAddress);
    CudaRtFrontend::AddStringForArguments(deviceName);
    CudaRtFrontend::AddVariableForArguments(dim);
    CudaRtFrontend::AddVariableForArguments(norm);
    CudaRtFrontend::AddVariableForArguments(ext);
    CudaRtFrontend::Execute("cudaRegisterTexture");
}


extern "C" __host__ void __cudaRegisterSurface(void **fatCubinHandle,
        const surfaceReference *hostVar, void **deviceAddress, char *deviceName,
        int dim, int ext) {
    CudaRtFrontend::Prepare();
    CudaRtFrontend::AddStringForArguments(CudaUtil::MarshalHostPointer(
        fatCubinHandle));
    CudaRtFrontend::AddStringForArguments(CudaUtil::MarshalHostPointer(
        hostVar));
    // Achtung: passing the address and the content of the textureReference
    CudaRtFrontend::AddHostPointerForArguments(hostVar);
    CudaRtFrontend::AddStringForArguments((char *) deviceAddress);
    CudaRtFrontend::AddStringForArguments(deviceName);
    CudaRtFrontend::AddVariableForArguments(dim);
    CudaRtFrontend::AddVariableForArguments(ext);
    CudaRtFrontend::Execute("cudaRegisterSurface");
}

/* */

extern "C" __host__ int __cudaSynchronizeThreads(void** x, void* y) {
    // FIXME: implement
    std::cerr << "*** Error: __cudaSynchronizeThreads() not yet implemented!"
            << std::endl;
    return 0;
}

extern "C" __host__ void __cudaTextureFetch(const void *tex, void *index, int integer,
        void *val) {
    // FIXME: implement 
    std::cerr << "*** Error: __cudaTextureFetch() not yet implemented!" << std::endl;
}

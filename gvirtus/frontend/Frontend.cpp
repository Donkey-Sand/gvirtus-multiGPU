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
 * @file   Frontend.cpp
 * @author Giuseppe Coviello <giuseppe.coviello@uniparthenope.it>
 * @date   Wed Sep 30 12:57:11 2009
 *
 * @brief
 *
 *
 */

#include "Frontend.h"

#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>

#include "ConfigFile.h"

using namespace std;

static Frontend msFrontend;
map<pthread_t, Frontend*> *Frontend::mpFrontends = NULL;
int Frontend::device_number = 0;//初始化静态数据成员，不收private/public等限制
int Frontend::device_choiced = 0;
static bool initialized = false;

/**
 *
 */
void Frontend::Init(Communicator *c) {
#if 1
    const char *config_file;
#ifdef _CONFIG_FILE
    if((config_file = getenv("CONFIG_FILE")) == NULL)//getenv函数的功能是查找环境变量的内容
        config_file = _CONFIG_FILE;
#else
	config_file = "gvirtus.properties";
#endif
    ConfigFile *cf = new ConfigFile(config_file);
    string communicator;
#ifndef _WIN32
    char *tmp;
	if((tmp = getenv("COMMUNICATOR")) != NULL)
        communicator = string(tmp);
    else
#endif
    /*
    * cf->Get函数返回的是字符串："tcp://219.219.216.177:9988
    * Communicator::Get函数返回一个new TcpCommunicator(communicator)对象
    * 如果在这里多次调用Communicator::Get函数返回多个TcpCommunicator对象然后connect到不同的服务器如何？
    */
    	/*
    communicator = cf->Get("communicator");//就是这里，2016.03.10,Get 函数修改 Sand
    mpCommunicator = Communicator::Get(communicator);

    mpCommunicator->Connect();//这里包括了socket中的connect（）函数
    mpInputBuffer = new Buffer();
    mpOutputBuffer = new Buffer();
    mpLaunchBuffer = new Buffer();
    mExitCode = -1;
    mpInitialized = true;	//注意这个赋值命令
    */
    device_index = 0;	//这是我加上的，用来标识GPU的下标
    device_count = DEVICE_COUNT_MY;	//这也是我加上的，标识一共有多少个GPU
   // device_choiced = device_number;
    //这也是我加上的，标识正在使用的是第几个GPU,通过函数cudaSetDevice来设定这个值

    int i = 0;
    for( i=0; i<device_count; ++i)
    {
        communicator = cf->Get_IPs("communicator",i);//得到的返回值类似于"tcp://219.219.216.177:9988"这种形式
        mpCommunicator[i] = Communicator::Get(communicator);
        mpCommunicator[i]->Connect();//这里包括了socket中的connect（）函数
        mpInputBuffer[i] = new Buffer();
        mpOutputBuffer[i] = new Buffer();
        mpLaunchBuffer[i] = new Buffer();
        mExitCode[i] = -1;
    }
    /*
    communicator = cf->Get_IPs("communicator",device_choiced);//得到的返回值类似于"tcp://219.219.216.177:9988"这种形式
    mpCommunicator[device_choiced] = Communicator::Get(communicator);
    mpCommunicator[device_choiced]->Connect();//这里包括了socket中的connect（）函数
    mpInputBuffer[device_choiced] = new Buffer();
    mpOutputBuffer[device_choiced] = new Buffer();
    mpLaunchBuffer[device_choiced] = new Buffer();
    mExitCode[device_choiced] = -1;
    */

#else//这是针对最上面的 #if 1
    mpCommunicator = c;
#endif

    mpInitialized = true;	//注意这个赋值命令,当这个值为true时候，这个成员函数不会执行，即只会初始化一次

}
/*******************************************************************************************************************************
 * 2016.03.16
 * 这个函数是为了解决cudaSetDevice API 中的初始化时重新选择socket而写，解决调用了cudaSetDevice之后socket没有变化的问题
 * 其实与Init()函数没有任何变化，只是形参为void
 * test04中废弃
 *******************************************************************************************************************************/
void Frontend::InitForSetDevice() {
#if 1
    const char *config_file;
#ifdef _CONFIG_FILE
    if((config_file = getenv("CONFIG_FILE")) == NULL)//getenv函数的功能是查找环境变量的内容
        config_file = _CONFIG_FILE;
#else
	config_file = "gvirtus.properties";
#endif
    ConfigFile *cf = new ConfigFile(config_file);
    string communicator;
#ifndef _WIN32
    char *tmp;
	if((tmp = getenv("COMMUNICATOR")) != NULL)
        communicator = string(tmp);
    else
#endif
    /*
    * cf->Get函数返回的是字符串："tcp://219.219.216.177:9988
    * Communicator::Get函数返回一个new TcpCommunicator(communicator)对象
    * 如果在这里多次调用Communicator::Get函数返回多个TcpCommunicator对象然后connect到不同的服务器如何？
    */
    	/*
    communicator = cf->Get("communicator");//就是这里，2016.03.10,Get 函数修改 Sand
    mpCommunicator = Communicator::Get(communicator);

    mpCommunicator->Connect();//这里包括了socket中的connect（）函数
    mpInputBuffer = new Buffer();
    mpOutputBuffer = new Buffer();
    mpLaunchBuffer = new Buffer();
    mExitCode = -1;
    mpInitialized = true;	//注意这个赋值命令
    */
    device_index = 0;	//这是我加上的，用来标识GPU的下标
    device_count = DEVICE_COUNT_MY;	//这也是我加上的，标识一共有多少个GPU
    device_choiced = device_number;//这也是我加上的，标识正在使用的是第几个GPU,通过函数cudaSetDevice来设定这个值

    communicator = cf->Get_IPs("communicator",device_choiced);//得到的返回值类似于"tcp://219.219.216.177:9988"这种形式
    mpCommunicator[device_choiced] = Communicator::Get(communicator);
    mpCommunicator[device_choiced]->Connect();//这里包括了socket中的connect（）函数
    mpInputBuffer[device_choiced] = new Buffer();
    mpOutputBuffer[device_choiced] = new Buffer();
    mpLaunchBuffer[device_choiced] = new Buffer();
    mExitCode[device_choiced] = -1;

#else//这是针对最上面的 #if 1
    mpCommunicator = c;
#endif

    mpInitialized = true;	//注意这个赋值命令,当这个值为true时候，这个成员函数不会执行，即只会初始化一次

}

Frontend::~Frontend() {
//    if (mpFrontends != NULL) {
//        map<pthread_t, Frontend*>::iterator it;
//        for(it = mpFrontends->begin(); it != mpFrontends->end(); it++) {
//            delete it->second;
//        }
//    }
//    else {
//        mpCommunicator->Close();
//        delete mpCommunicator;
//    }
}

     /**
     * Retrieves the single instance of the Frontend class.
     *
     * @param register_var
     *
     * @return The instance of the Frontend class.
     */
Frontend * Frontend::GetFrontend(Communicator *c) {
    if (mpFrontends == NULL)
        mpFrontends = new map<pthread_t, Frontend*>();
    
    /*
     *  syscall() 执行一个系统调用，根据指定的参数number和所有系统调用的汇编语言接口来确定调用哪个系统调用。
       系统调用所使用的符号常量可以在头文件<sys/syscll.h>里面找到。
       根据tid号来决定使用哪一个Frontend对象
     */
    pid_t tid = syscall(SYS_gettid);
    if (mpFrontends->find(tid) != mpFrontends->end())
           return mpFrontends->find(tid)->second;
    
    else {
        Frontend* f = new Frontend();
        if (!f->initialized()) {
            try {
                f->Init(c);		//注意这个函数，其内部有socket函数与后端进行连接通信
                mpFrontends->insert(make_pair(tid, f));	//注意mpFrontends是static型，属于整个Frontend类
            } catch (const char *e) {
                cerr << "Error: cannot create Frontend ('" << e << "')" << endl;
            }
        }
        return f;
    }    
}
//	Sandy 2016.03.23	用来设置静态数据成员device_number	用于test04
//是静态成员函数
void Frontend::set_device_choiced(const int dev)
{
	 device_number = dev;//左值是一个static型变量，为Frontend类所共享
	 device_choiced = dev;//左值是一个static型变量，为Frontend类所共享
}

//这两个函数是在test04中添加，为了解决cudaGetDeviceProperties API的问题
void Frontend::Get_Device_Properties(const int dev)
{
	 //device_number	保持不变化
	 device_choiced = dev;//左值是一个static型变量，为Frontend类所共享
}
void Frontend::restore_Device_Properties(void)
{
	 //device_number	保持不变化
	 device_choiced = device_number;//恢复原来的值
}
/*
 * 2016.03.10,函数修改 Sandy
 */
void Frontend::Execute(const char* routine, const Buffer* input_buffer) {
    if (input_buffer == NULL)
        input_buffer = mpInputBuffer[device_choiced];

    /* sending job */
    mpCommunicator[device_choiced]->Write(routine, strlen(routine) + 1);
    input_buffer->Dump(mpCommunicator[device_choiced]);
    mpCommunicator[device_choiced]->Sync();

    /* receiving output */
    //std::istream &in = mpCommunicator->GetInputStream();

    mpOutputBuffer[device_choiced]->Reset();

    mpCommunicator[device_choiced]->Read((char *) & mExitCode[device_choiced], sizeof (int));
    size_t out_buffer_size;
    mpCommunicator[device_choiced]->Read((char *) & out_buffer_size, sizeof (size_t));
    if (out_buffer_size > 0)
        mpOutputBuffer[device_choiced]->Read<char>(mpCommunicator[device_choiced], out_buffer_size);
}

/*
 * 2016.04.12,函数修改 Sandy
 * 这个函数是专门为了CudaRtHandler_internal.cpp中的六个函数而改写的的
 * 目的就是为了把所有的参数都同时传送到所有的后端GPU
 */
void Frontend::Execute_internal(const char* routine, const Buffer* input_buffer)
{
	int i = 0;
	for(i=0; i<device_count; ++i)
	{
		if (input_buffer == NULL)
			input_buffer = mpInputBuffer[i];

		/* sending job */
		mpCommunicator[i]->Write(routine, strlen(routine) + 1);
		input_buffer->Dump(mpCommunicator[i]);
		mpCommunicator[i]->Sync();

		/* receiving output */
		//std::istream &in = mpCommunicator->GetInputStream();

		mpOutputBuffer[i]->Reset();

		mpCommunicator[i]->Read((char *) & mExitCode[i], sizeof (int));
		size_t out_buffer_size;
		mpCommunicator[i]->Read((char *) & out_buffer_size, sizeof (size_t));
		if (out_buffer_size > 0)
			mpOutputBuffer[i]->Read<char>(mpCommunicator[i], out_buffer_size);
	}//end for
}
//Sandy 添加
//test04中废除此函数
#if 0
void Frontend::set_device_choiced(int device) {
	if(device < device_count)
	{
        device_number = device;//左值是一个static型变量，为Frontend类所共享
        device_choiced = device_number;
	}

	/*******************************************************************************************************************************
	 * 2016.03.16
	 * 这个函数是为了解决cudaSetDevice API 中的初始化时重新选择socket而写，解决调用了cudaSetDevice之后socket没有变化的问题
	 * 其实下面是Init()函数
	 *
	 *******************************************************************************************************************************/
#if 1
    const char *config_file;
#ifdef _CONFIG_FILE
    if((config_file = getenv("CONFIG_FILE")) == NULL)//getenv函数的功能是查找环境变量的内容
        config_file = _CONFIG_FILE;
#else
	config_file = "gvirtus.properties";
#endif
    ConfigFile *cf = new ConfigFile(config_file);
    string communicator;
#ifndef _WIN32
    char *tmp;
	if((tmp = getenv("COMMUNICATOR")) != NULL)
        communicator = string(tmp);
    else
#endif
    /*
    * cf->Get函数返回的是字符串："tcp://219.219.216.177:9988
    * Communicator::Get函数返回一个new TcpCommunicator(communicator)对象
    * 如果在这里多次调用Communicator::Get函数返回多个TcpCommunicator对象然后connect到不同的服务器如何？
    */
    	/*
    communicator = cf->Get("communicator");//就是这里，2016.03.10,Get 函数修改 Sand
    mpCommunicator = Communicator::Get(communicator);

    mpCommunicator->Connect();//这里包括了socket中的connect（）函数
    mpInputBuffer = new Buffer();
    mpOutputBuffer = new Buffer();
    mpLaunchBuffer = new Buffer();
    mExitCode = -1;
    mpInitialized = true;	//注意这个赋值命令
    */
    device_index = 0;	//这是我加上的，用来标识GPU的下标，目前看来毫无用处
    device_count = DEVICE_COUNT_MY;	//这也是我加上的，标识一共有多少个GPU


    communicator = cf->Get_IPs("communicator",device_choiced);//得到的返回值类似于"tcp://219.219.216.177:9988"这种形式
    mpCommunicator[device_choiced] = Communicator::Get(communicator);
    mpCommunicator[device_choiced]->Connect();//这里包括了socket中的connect（）函数
    mpInputBuffer[device_choiced] = new Buffer();
    mpOutputBuffer[device_choiced] = new Buffer();
    mpLaunchBuffer[device_choiced] = new Buffer();
    mExitCode[device_choiced] = -1;

#else//这是针对最上面的 #if 1
    mpCommunicator = c;
#endif

    mpInitialized = true;	//注意这个赋值命令,当这个值为true时候，这个成员函数不会执行，即只会初始化一次
}
#endif
void Frontend::Prepare() {
    mpInputBuffer[device_choiced]->Reset();//主要是执行置零的工作
}
